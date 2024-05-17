from os import path
from hashlib import md5
from re import finditer
from cpp_libs import is_standard


MAKEFILE = "./MakeFile"
ROOT = "./"
COMPILER = "g++"
OBJECT = path.join(ROOT, "build")


def is_unit(basedir: str, filename: str) -> str:
    return filename.endswith(".cpp")


def get_clean(units) -> str:
    return f"clean: \n\trm -rf {OBJECT}/*\n"


def get_target(units) -> str:
    return f"main: {' '.join([get_output(unit) for unit in units])}\n\t{COMPILER} $^ -o $@\n"


def get_dependence(unit: str):
    reg = '#include\s*(?:(?:<(.*)>)|(?:"(.*)"))'
    with open(unit, "r", encoding="utf-8") as f:
        content = f.read()
        for m in finditer(reg, content):
            lib = m.group(1)
            if is_standard(lib):
                continue
            yield lib


def get_output(unit: str) -> str:
    return path.join(OBJECT, get_fingerprint(unit)).replace("\\", "/") + ".o"


def get_unit_command(unit: str, dependence: list[str]) -> str:
    return f"{COMPILER} -c {unit} -o {get_output(unit)}"


def get_fingerprint(filepath: str) -> str:
    return md5(filepath.encode()).hexdigest()
