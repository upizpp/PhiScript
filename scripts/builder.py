import argparse
import os
import json
import re
from sys import platform
from hashlib import sha256
from shutil import rmtree, copyfile
from fnmatch import fnmatch

from config import Config, ConfigType
from colors import Colors

global_config: Config = None
config: ConfigType = None
file_map: dict = {}
def main() -> None:
    parser = argparse.ArgumentParser(description="Build a project.")
    parser.add_argument("--type", "-t", required=True, choices=list(global_config.types) + ["all"], help="type of project to build.")
    parser.add_argument("--clear", "-c", action="store_true", help="clear the cache")
    parser.add_argument("--run", "-r", action="store_true", help="run the project")
    args = parser.parse_args()
    
    global config
    
    if args.clear:
        clear_cache()
        return
    
    if args.type != "all":
        config = global_config.pick(args.type)
    else:
        for type in global_config.types:
            print(Colors([Colors.BOLD, Colors.WHITE], "开始处理："+ type))
            args = global_config.data[type]
            args["type"] = type
            build_project(args, False)
        return

    if args.run and (os.path.isfile(os.path.abspath(config.output)) or os.path.isfile(os.path.abspath(config.output) + ".exe")):
        os.system(f'"{os.path.abspath(config.output)}"')
        return

    load_file_map()

    units = scan()
    build(units)

    save_file_map()
    if config.auto_run:
        os.system(f'"{os.path.abspath(config.output)}"')

def build_project(args: dict, allow_auto_run = True, save = True) -> None:
    global config
    config = global_config.pick(args["type"])

    if args.get("clear", False):
        clear_cache()
        return

    load_file_map()

    units = scan()
    build(units)

    if save:
        save_file_map()
    
    if config.auto_run and allow_auto_run:
        os.system(config.output)


def build_with_config(config_: ConfigType, allow_auto_run = True) -> None:
    global config
    config = config_

    load_file_map()

    units = scan()
    build(units)

    save_file_map()
    
    if config.auto_run and allow_auto_run:
        os.system(config.output)


def build(units: dict) -> None:
    if not any(units.values()):
        return
    objects = []
    print(Colors([Colors.YELLOW, Colors.BOLD], "开始编译..."))
    print()
    for unit in units:
        output_source = os.path.join(config.cache, unit)
        basename = os.path.splitext(output_source)[0]
        output = basename + ".o"
        output_dir = os.path.split(output)[0]
        if not os.path.isdir(output_dir):
            os.makedirs(output_dir)
        objects.append(output)
        if units[unit]:
            command = get_command(unit, output)
            print(command)
            print(Colors(Colors.CYAN, unit + ":"))
            if os.system(command) != 0:
                print(Colors([Colors.RED, Colors.BOLD], "出现错误，编译终止。"))
                print(Colors([Colors.RED, Colors.BOLD], "编译命令："), Colors(Colors.WHITE, command))
                exit(-1)
    print()
    print(Colors([Colors.YELLOW, Colors.BOLD], Colors(Colors.BOLD, "编译结束...")))
    print(Colors(Colors.WHITE, "-" * 64))
    print(Colors([Colors.YELLOW, Colors.YELLOW], "开始链接..."))
    _output = config.output.replace("$LIB$", "dll" if platform == "win32" else "so")
    command = f"{config.compiler} {config.link_extra} {' '.join(objects)} -o {_output} "
    print(Colors([Colors.CYAN, Colors.BOLD], "链接命令："), Colors(Colors.WHITE, command))
    output_dir = os.path.split(config.output)[0]
    if output_dir != "" and not os.path.isdir(output_dir):
        os.makedirs(output_dir)
    if os.system(command) != 0:
        print(Colors([Colors.RED, Colors.BOLD], "链接错误，编译终止。"))
        exit(-1)
    if len(config.dependence) != 0:
        for dependence in config.dependence:
            filename = os.path.split(dependence)[1]
            output = os.path.join(output_dir, filename)
            if not os.path.isfile(dependence):
                print(Colors([Colors.RED, Colors.BOLD], f"依赖文件不存在，编译终止。{dependence}"))
                exit(-1)
            if os.path.isfile(output) and get_id(dependence) == get_id(output):
                continue
            print(Colors(Colors.BOLD, "dependence: "), Colors(Colors.CYAN, dependence))
            if os.path.isfile(output):
                os.remove(output)
            copyfile(dependence, output)
                
    print(Colors(Colors.WHITE, "-" * 64))


def get_command(unit: str, output: str) -> str:
    return config.get_command(unit, output)

def clear_cache() -> None:
    global config
    if config is None:
        for config in global_config.data:
            rmtree(global_config.data[config]["output"].split()[0], True)
            rmtree(global_config.data[config]["cache"], True)
    else:
        rmtree(config.output.split()[0], True)
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

    with open(os.path.join(config.cache, ".gitignore"), "w", encoding="utf-8") as file:
        file.write("*")

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
            if is_ignored(full_path):
                continue

            if is_unit(full_path):
                units[full_path] = has_unit_changed(full_path)
    return units

def is_ignored(path: str) -> bool:
    return filename_match(path, config.ignore)

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
            filenames = filter(lambda x: x is not None, filenames)
            filename = list(filenames)[-1]
            included_path = None
            try:
                included_path = relative_path(unit, filename)
                it = iter(config.includes)
                while not os.path.isfile(included_path):
                    try:
                        include = next(it)
                        included_path = relative_path(include, filename)
                        if not included_path.startswith(include):
                            included_path = os.path.join(include, included_path)
                    except StopIteration:
                        break
                included_path = included_path.replace("\\", "/")
                if os.path.isfile(included_path):
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


load_config()
if __name__ == "__main__":
    main()
