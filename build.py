from concurrent.futures import ThreadPoolExecutor
import re
import subprocess
from fnmatch import fnmatch
from hashlib import sha256
from json import dumps, loads
from os import makedirs, path, walk, system
from sys import argv
from colors import Colors

global config
global file_map


def main(args: list[str]) -> None:
    global config
    global file_map
    config = load_config()
    files = scan_files(config.scan)
    load_file_map()
    old_file_map = file_map.copy()
    compile_only = False
    if "COMPILE_ONLY" in args:
        compile_only = True
        args.remove("COMPILE_ONLY")

    if list_hash(args) != file_map.get("::ARGS", ""):
        file_map = {}

    units = get_units(files)
    changed_units = get_changed_units(units)
    commands = [get_command(unit, args) for unit in changed_units]
    if not run_commands(commands, config.max_workers):
        save_file_map(old_file_map, args)
        return

    print(Colors([Colors.BOLD, Colors.BLUE], "Linking..."))
    link_command = get_link_command(units)
    print(Colors(Colors.CYAN, link_command))
    system(link_command)
    print()
    print("=" * 64)
    print()
    if not compile_only:
        system(config.target)

    save_file_map(files, args)


def list_hash(l: list) -> str:
    return sha256(dumps(l).encode()).hexdigest()


def get_link_command(units: list[str]) -> str:
    ofiles = " ".join([get_ofile(unit) for unit in units])
    return f"{config.compiler} {ofiles} -o {config.target}"


def run_commands(commands, max_workers) -> bool:
    succeed = True

    def run_command(command):
        process = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        stdout, stderr = process.communicate()
        print(Colors(Colors.YELLOW, f"Compiling... ({command})"))
        return {
            'stdout': stdout.decode().strip(),
            'stderr': stderr.decode().strip(),
            'command': command
        }

    with ThreadPoolExecutor(max_workers=max_workers) as executor:
        results = list(executor.map(run_command, commands))

    for i, result in enumerate(results):
        flag = False
        if result['stdout'] or result['stderr']:
            print(Colors(Colors.CYAN, result['command']), ":")
            flag = True
        if result['stdout']:
            print(Colors(Colors.BOLD, "Stdout:\n"), result['stdout'])
        if result['stderr']:
            succeed = False
            print(Colors(Colors.RED, "Stderr:\n"), result['stderr'])
        if flag:
            print()
        if not succeed:
            return False

    print(Colors([Colors.YELLOW, Colors.BOLD], "Compile finished."))
    return succeed



def get_command(unit: str, args: list[str]) -> str:
    macro_string = " ".join([f"-D{macro}" for macro in args])
    ofile = get_ofile(unit)
    debug_command = "-fdiagnostics-color=always -g" if "DEBUG" in args else ""
    if not path.exists(path.dirname(ofile)):
        makedirs(path.dirname(ofile))
    return f"{config.compiler} {config.extra_args} {debug_command} -c -I./src {macro_string} -std={config.standard} {unit} -o {ofile}"


def get_ofile(unit: str) -> str:
    return path.join(config.build, replace_extension(unit, ".o"))


def replace_extension(source_path, new_extension):
    directory, filename_ext = path.split(source_path)
    filename, _ = path.splitext(filename_ext)
    new_filename_ext = filename + new_extension
    new_path = path.join(directory, new_filename_ext)
    return new_path


def get_changed_units(units: list[str]) -> list[str]:
    result = []
    for unit in units:
        if has_changed(unit):
            result.append(unit)
            continue
        included_files = get_included_files(unit)
        changed_included_files = list(filter(lambda x: has_changed(x), included_files))
        if changed_included_files:
            result.append(unit)
    return result


def relative_path(abspath: str, rel: str) -> str:
    rel_paths = rel.replace("/", "\\").split('\\')
    result = abspath.replace("/", "\\").split('\\')[0:-1]
    for path in rel_paths:
        if path == "..":
            result.pop()
        else:
            result.append(path)
    return "\\".join(result)


def get_included_files(unit: str) -> list[str]:
    if not path.exists(unit):
        return []
    result = []
    with open(unit, "r", encoding="utf-8") as file:
        pattern = """^#include\s*"(.*)"\s*$"""
        for match in re.finditer(pattern, file.read(), re.MULTILINE):
            filename = match.group(1)
            included_path = None
            if path.isabs(filename):
                included_path = path.join("src", filename)
            else:
                included_path = relative_path(unit, filename)
            result.append(included_path)
            result.extend(get_included_files(included_path))
    return [item.replace("/", "\\") for item in result]


def get_units(files: list[str]) -> list[str]:
    return list(filter(lambda x: is_unit(x), files))


def has_changed(path: str) -> None:
    return file_map.get(path, "") != get_sha256(path)


def scan_files(scan_path: str) -> list[str]:
    result = []
    for current, _, files in walk(scan_path):
        for filename in files:
            filepath = path.join(current, filename)
            if should_ignore(filepath):
                continue
            result.append(filepath[2:])
    return result


def load_file_map() -> None:
    global file_map
    data_path = path.join(config.build, "__hash_data__")
    if not path.exists(data_path):
        file_map = {}
        return
    with open(data_path, "r", encoding="utf-8") as file:
        file_map = loads(file.read())


def save_file_map(files: list[str], args: list[str]) -> None:
    data = {}
    for filepath in files:
        data[filepath] = get_sha256(filepath)
    data["::ARGS"] = list_hash(args)
    with open(path.join(config.build, "__hash_data__"), "w", encoding="utf-8") as file:
        file.write(dumps(data))


def should_ignore(path: str) -> bool:
    return match_patterns(path, config.ignore + ["./build/*"])


def is_unit(path: str) -> bool:
    return match_patterns(path, config.units)


def match_patterns(path: str, patterns: list[str]) -> bool:
    for pattern in patterns:
        if fnmatch(path, pattern):
            return True
    return False


class ConfigData:
    def __init__(self, path: str) -> None:
        self.path = path
        self.load()

    def load(self) -> None:
        with open(self.path, "r", encoding="utf-8") as file:
            data: dict = loads(file.read())
            self.build = data.get("build", ".build")
            self.ignore = ["./" + pattern for pattern in data.get("ignore", [])]
            self.standard = data.get("standard", "c++17")
            self.target = data.get("target", "main.exe")
            self.units = data.get("units", ["*.cpp"])
            self.scan = data.get("scan", ".")
            self.compiler = data.get("compiler", "g++")
            self.max_workers = data.get("max_workers", 8)
            self.extra_args = data.get("extra_args", "")

            if not path.isdir(self.build):
                makedirs(self.build)


def load_config() -> ConfigData:
    return ConfigData("config.json")


def get_sha256(file_path: str) -> str:
    if not path.exists(file_path):
        return ""
    sha256_hash = sha256()
    with open(file_path, "rb") as file:
        for chunk in iter(lambda: file.read(4096), b""):
            sha256_hash.update(chunk)
    return sha256_hash.hexdigest()


if __name__ == "__main__":
    main(argv[1:])
