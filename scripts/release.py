from sys import argv
from builder import build_project as build
from builder import clear_cache
import re


def main():
    if len(argv) < 2:
        type = "patch"
    else:
        type = argv[1]

    global jump
    jump = int(argv[2]) if len(argv) >= 3 else 1
    match type:
        case "patch":
            patch()
        case "minor":
            minor()
        case "major":
            major()

    build({"type": "release"})


def patch():
    save(increase(get_current(), 2))


def minor():
    save(increase(get_current(), 1))


def major():
    save(increase(get_current(), 0))


def increase(version: str, level: int):
    split_version = version.split(".")
    split_version[level] = str(int(split_version[level]) + jump)
    for i in range(level + 1, len(split_version)):
        split_version[i] = "0"

    return ".".join(split_version)


def get_current():
    with open("./src/phi/version", "r", encoding="utf-8") as file:
        return re.findall('"(.*)"', file.read())[0]


def save(version: str):
    with open("./src/phi/version", "w", encoding="utf-8") as file:
        file.write('#define PHI_VERSION "' + version + '"')


if __name__ == "__main__":
    main()
