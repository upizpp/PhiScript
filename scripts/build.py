import argparse
import os
import json
import re
from hashlib import sha256
from shutil import rmtree
from fnmatch import fnmatch

from config import Config, ConfigType


global_config: Config = None
config: ConfigType = None
file_map: dict = {}

def main() -> None:
    load_config()

    parser = argparse.ArgumentParser(description="Build a project.")
    parser.add_argument("--type", "-t", required=True, choices=list(global_config.types), help="type of project to build.")
    parser.add_argument("--clear", "-c", action="store_true", help="clear the cache")
    args = parser.parse_args()
    global config
    config = global_config.pick(args.type)

    if args.clear:
        clear_cache()
        return

    load_file_map()

    changed = scan()
    build(changed)

    save_file_map()

def build(changed_units: list) -> None:
    for unit in changed_units:
        print()


def get_command(unit: str) -> str:
    return config.get_command(unit)

def clear_cache() -> None:
    rmtree(config.cache, True)


def load_file_map() -> None:
    if not os.path.isdir(config.cache):
        return
    global file_map
    with open(os.path.join(config.cache, "file_map.json"), encoding="utf-8") as file:
        file_map = json.loads(file.read())

def save_file_map():
    if not os.path.isdir(config.cache):
        os.makedirs(config.cache)

    for root, _, files in os.walk(config.scan):
        for filename in files:
            full_path = os.path.join(root, filename)
            file_map[full_path] = get_id(full_path)
    with open(os.path.join(config.cache, "file_map.json"), "w", encoding="utf-8") as file:
        file.write(json.dumps(file_map, indent=4))

def scan() -> list:
    changed_units: list = []
    for root, _, files in os.walk(config.scan):
        for filename in files:
            full_path = os.path.join(root, filename)

            if is_unit(full_path) and has_unit_changed(full_path):
                changed_units.append(full_path)
    return changed_units


def has_unit_changed(path: str) -> None:
    if not is_unit(path):
        return has_changed(path)
    return has_changed(path) or any([has_unit_changed(x) for x in get_associated_files(path)])
    

def has_changed(path: str) -> bool:
    return path in file_map and file_map[path] != get_id(path)

def get_associated_files(unit: str) -> list:
    if not os.path.exists(unit):
        return []
    result = []
    with open(unit, "r", encoding="utf-8") as file:
        pattern = config.pattern
        for match in re.finditer(pattern, file.read(), re.MULTILINE):
            filenames = match.groups()
            filename = ""
            for fn in filenames:
                if not fn is None:
                    filename = fn
            included_path = None
            try:
                if os.path.isabs(filename):
                    included_path = os.path.join("src", filename)
                else:
                    included_path = relative_path(unit, filename)
                result.append(included_path)
                result.extend(get_associated_files(included_path))
            except TypeError:
                ...
    return [item.replace("/", "\\") for item in result]

def relative_path(abspath: str, rel: str) -> str:
    rel_paths = rel.replace("/", "\\").split('\\')
    result = abspath.replace("/", "\\").split('\\')[0:-1]
    for path in rel_paths:
        if path == "..":
            result.pop()
        else:
            result.append(path)
    return "\\".join(result)

def is_unit(path: str) -> bool:
    return filename_match(path, config.units)

def filename_match(path: str, patterns: list) -> bool:
    for pat in patterns:
        if fnmatch(path, pat):
            return True
    return False

def get_id(path: str) -> int:
    if not os.path.exists(path):
        return ""
    sha256_hash = sha256()
    with open(path, "rb") as file:
        for chunk in iter(lambda: file.read(4096), b""):
            sha256_hash.update(chunk)
    return sha256_hash.hexdigest()

def load_config():
    global global_config
    global_config = Config("./config.json")


if __name__ == "__main__":
    main()
