import config
from argparse import ArgumentParser
from os import path, walk, makedirs
from itertools import tee
from configparser import ConfigParser


def main() -> None:
    conf = ConfigParser()
    conf.read("./config.ini", encoding="utf-8")

    parser = ArgumentParser()
    parser.add_argument(
        "--project",
        "-p",
        required=True,
        help="the project to build.",
        choices=conf.sections(),
    )
    args = parser.parse_args()

    config.configure(conf[args.project])

    if not path.exists(config.OBJECT):
        makedirs(config.OBJECT)
        with open(path.join(config.OBJECT, ".gitignore"), "w", encoding="utf-8") as f:
            f.write("*")
    units = scan_units(config.ROOT)
    makefile = get_makefile(units)
    with open(config.MAKEFILE, "w", encoding="utf-8") as f:
        f.write(makefile)


def get_makefile(units) -> str:
    units, copied1, copied2 = tee(units, 3)
    result = (
        config.get_target(copied1)
        + "\n"
        + config.get_clean(copied2)
        + "\n.PHONY: clean\n\n"
    )

    for unit in units:
        dependence = config.get_dependence(unit)
        result += f"{config.get_output(unit)}: {' '.join(dependence)}\n\t{config.get_unit_command(unit, dependence)}\n"

    return result


def scan_units(root: str):
    for (
        root,
        _dirs,
        files,
    ) in walk(root):
        for file in files:
            if config.is_unit(root, file):
                yield path.join(root, file).replace("\\", "/")


if __name__ == "__main__":
    main()
