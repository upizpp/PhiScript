import argparse
import os
import json
import re
from shutil import rmtree
from fnmatch import fnmatch

from config import Config, ConfigType


global_config: ConfigType = None
config: Config = None
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
    scan()
    save_file_map()

def clear_cache():
    rmtree(config.cache, True)


def load_file_map():
    if not os.path.isdir(config.cache):
        return
    global file_map
    with open(os.path.join(config.cache, "file_map.json"), encoding="utf-8") as file:
        file_map = json.loads(file.read())

def save_file_map():
    if not os.path.isdir(config.cache):
        os.makedirs(config.cache)
    with open(os.path.join(config.cache, "file_map.json"), "w", encoding="utf-8") as file:
        file.write(json.dumps(file_map, indent=4))

def scan() -> None:
    changed: list = []
    for root, _, files in os.walk(config.scan):
        for filename in files:
            full_path = os.path.join(root, filename)
            file_id = get_id(full_path)

            handle_file(full_path)

            file_map[full_path] = file_id


def handle_file(path: str) -> None:
    if not is_unit(path):
        return
    print(path)
def get_included_files(unit: str) -> list[str]:
    if not os.path.exists(unit):
        return []
    result = []
    with open(unit, "r", encoding="utf-8") as file:
        pattern = """^#include\s*"(.*)"\s*$"""
        for match in re.finditer(pattern, file.read(), re.MULTILINE):
            filename = match.group(1)
            included_path = None
            if os.path.isabs(filename):
                included_path = os.path.join("src", filename)
            else:
                included_path = relative_path(unit, filename)
            result.append(included_path)
            result.extend(get_included_files(included_path))
    return [item.replace("/", "\\") for item in result]
def is_unit(path: str) -> bool:
    for pat in config.units:
        if fnmatch(path, pat):
            return True
    return False

def get_id(path: str) -> int:
    return int(os.path.getmtime(path))

def load_config():
    global global_config
    global_config = Config("./config.json")


if __name__ == "__main__":
    main()
