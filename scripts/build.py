import argparse
import os
import json
import re
from hashlib import sha256
from shutil import rmtree
from fnmatch import fnmatch

from config import Config, ConfigType
from colors import Colors

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

    units = scan()
    build(units)

    save_file_map()

def build(units: dict) -> None:
    objects = []
    print(Colors([Colors.YELLOW, Colors.BOLD], "开始编译..."))
    print()
    for unit in units:
        print(Colors([Colors.CYAN, Colors.BOLD], unit + ":"))
        output_source = os.path.join(config.cache, unit)
        basename = os.path.splitext(output_source)[0]
        output = basename + ".o"
        output_dir = os.path.split(output)[0]
        if not os.path.isdir(output_dir):
            os.makedirs(output_dir)
        objects.append(output)
        if units[unit]:
            command = get_command(unit, output)
            if os.system(command) != 0:
                print(Colors([Colors.RED, Colors.BOLD], "出现错误，编译终止。"))
                print(Colors([Colors.RED, Colors.BOLD], "编译命令："), Colors(Colors.WHITE, command))
                exit(-1)
    print()
    print(Colors([Colors.YELLOW, Colors.BOLD], Colors(Colors.BOLD, "编译结束...")))
    print(Colors(Colors.WHITE, "-" * 64))
    print(Colors([Colors.YELLOW, Colors.YELLOW], "开始链接..."))
    command = f"{config.compiler} {config.link_extra} {' '.join(objects)} -o {config.output} "
    print(Colors([Colors.RED, Colors.BOLD], "链接命令："), Colors(Colors.WHITE, command))
    if os.system(command) != 0:
        print(Colors([Colors.RED, Colors.BOLD], "链接错误，编译终止。"))
        exit(-1)
    print(Colors(Colors.WHITE, "-" * 64))
    if config.auto_run:
        os.system(config.output)


def get_command(unit: str, output: str) -> str:
    return config.get_command(unit, output)

def clear_cache() -> None:
    rmtree(config.cache, True)

def load_file_map() -> None:
    if not os.path.isdir(config.cache):
        return
    global file_map
    try:
        with open(os.path.join(config.cache, "file_map.json"), encoding="utf-8") as file:
            file_map = json.loads(file.read())
    except:
        file_map = {}

def save_file_map():
    if not os.path.isdir(config.cache):
        os.makedirs(config.cache)

    for root, _, files in os.walk(config.scan):
        for filename in files:
            full_path = os.path.join(root, filename)
            file_map[full_path] = get_id(full_path)
    with open(os.path.join(config.cache, "file_map.json"), "w", encoding="utf-8") as file:
        file.write(json.dumps(file_map, indent=4))

def scan() -> dict:
    units: dict = {}
    for root, _, files in os.walk(config.scan):
        for filename in files:
            full_path = os.path.join(root, filename)

            if is_unit(full_path):
                units[full_path] = has_unit_changed(full_path)
    return units


def has_unit_changed(path: str) -> None:
    if not is_unit(path):
        return has_changed(path)
    return has_changed(path) or any([has_unit_changed(x) for x in get_associated_files(path)])
    

def has_changed(path: str) -> bool:
    if path in file_map:
        return file_map[path] != get_id(path)
    else:
        return True

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
