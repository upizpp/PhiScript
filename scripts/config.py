from os import path
from hashlib import md5
from re import finditer
from cpp_libs import is_standard
from json import load


EXTRA = ""
LINK_EXTRA = ""
STANDARD = ""
INCLUDE = ["./src/phi"]
MACRO = []
IGNORE = []
MAKEFILE = "./MakeFile"
ROOT = "./"
COMPILER = "g++"
OBJECT = path.join(ROOT, "build")


def configure(data: any) -> None:
    try:
        global EXTRA
        global STANDARD
        global LINK_EXTRA
        global INCLUDE
        global MACRO
        global IGNORE
        global MAKEFILE
        global ROOT
        global COMPILER
        global OBJECT
        EXTRA = eval(data.get("extra", '""'))
        STANDARD = eval(data.get("standard", '""'))
        LINK_EXTRA = eval(data.get("link_extra", '""'))
        INCLUDE = eval(data.get("include", '["./src/phi"]'))
        MACRO = eval(data.get("macro", "[]"))
        IGNORE = eval(data.get("ignore", "[]"))
        MAKEFILE = eval(data.get("makefile", '"./MakeFile"'))
        ROOT = eval(data.get("root", '"./"'))
        COMPILER = eval(data.get("compiler", '"g++"'))
        OBJECT = path.join(ROOT, "build")
    except KeyError as e:
        print(f"[ERROR] {e}")
        exit(1)


def is_unit(basedir: str, filename: str) -> str:
    return (
        filename.endswith(".cpp")
        and not path.join(basedir, filename).replace("\\", "/") in IGNORE
    )


def get_clean(units) -> str:
    return f"clean: \n\trm -rf {OBJECT}/* && rm -rf ./main\n"


def get_target(units) -> str:
    return f"main: {' '.join([get_output(unit) for unit in units])}\n\t{COMPILER} {LINK_EXTRA} $^ -o $@\n"


def get_dependence(unit: str):
    yield unit
    reg = '#include\s*(?:(?:<(.*)>)|(?:"(.*)"))'
    with open(unit, "r", encoding="utf-8") as f:
        content = f.read()
        for m in finditer(reg, content):
            lib = m.group(1)
            if not lib or is_standard(lib):
                continue
            prefix = ""
            it = iter(INCLUDE)
            while not path.isfile(prefix + lib):
                try:
                    prefix = next(it) + "/"
                except StopIteration:
                    break
            yield prefix + lib


def get_output(unit: str) -> str:
    return path.join(OBJECT, get_fingerprint(unit)).replace("\\", "/") + ".o"


def get_unit_command(unit: str, dependence) -> str:
    return f"{COMPILER} -std={STANDARD} {('-D' if MACRO else '') + ' -D'.join(MACRO)} {('-I' if INCLUDE else '') + ' -I'.join(INCLUDE)} {EXTRA} -c {unit} -o {get_output(unit)}"


def get_fingerprint(filepath: str) -> str:
    return md5(filepath.encode()).hexdigest()
