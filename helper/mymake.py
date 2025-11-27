import sys
from argparse import ArgumentParser
from configparser import ConfigParser
from os import popen, getcwd, path


class Config:
    includes = []
    cxxflags = ""
    cxx = "g++"
    output = "./main"
    makefile = "MakeFile"
    linkflags = ""
    ignores = []

    def configure(self, data: any):
        try:
            self.includes = eval(data.get("includes", "[]"))
            self.cxxflags = eval(data.get("cxxflags", '""'))
            self.linkflags = eval(data.get("linkflags", '""'))
            self.cxx = eval(data.get("cxx", '"g++"'))
            self.output = eval(data.get("output", '"./main"'))
            self.ignores = eval(data.get("ignores", "[]"))
            self.makefile = eval(data.get("makefile", '"./MakeFile"'))
        except KeyError as e:
            print(f"[ERROR] {e}")


config = Config()


def main():
    config_parser = load_config()

    parser = ArgumentParser()
    parser.add_argument("project", type=str, choices=config_parser.sections() + ["all"])
    args = parser.parse_args()

    if args.project == "all":
        for section in config_parser.sections():
            make(config_parser[section])
    else:
        make(config_parser[args.project])
    with open("src/path.hpp", "w") as file:
        file.write('#define PATH "' + getcwd().replace("\\", "/") + '"')


def make(data):
    global config
    config.configure(data)

    files = list(get_files())
    objs = list(map(lambda x: "./build/" + x.replace("/", "___") + ".o", files))
    includes = " ".join(map(lambda x: "-I" + x, config.includes))
    with open(config.makefile, "w") as file:
        file.write("CXX = g++\n")
        file.write(f"CXXFLAGS = {config.cxxflags}\n")
        if sys.platform == "win32":
            file.write("SHELL=cmd.exe\n")
        file.write(f"OBJS = {' '.join(objs)}\n\n")
        file.write(".PHONY: all clean\n\n")
        file.write(
            f"all: $(OBJS)\n\t$(CXX) $(CXXFLAGS) {includes} {config.linkflags} $^ -o {config.output}\n\n"
        )
        file.write(
            f"clean:\n\t{delete()} build\n\t{delete()} {config.output}{'.exe' if sys.platform == 'win32' else ''}\n\n\n"
        )
        for f in files:
            dependence = get_includes(f + ".cpp")
            out = f"./build/{f.replace('/', '___')}.o"
            file.write(
                f"{out}: {' '.join(dependence)} {f + '.cpp'}\n\t$(CXX) {f}.cpp {includes} $(CXXFLAGS) -c -o {out}\n\n"
            )


def delete():
    if sys.platform == "win32":
        return "del /f /q"
    elif sys.platform == "linux":
        return "rm -rf"


def load_config():
    parser = ConfigParser()
    parser.read("config.ini", encoding="utf-8")
    return parser


def get_includes(filename: str):
    global config
    with open(filename) as file:
        line = file.readline()
        while line:
            if line.startswith("#include"):
                line = line.removeprefix("#include").lstrip(" ").removesuffix("\n")
                if line.startswith("<") and line.endswith(">"):
                    res = line.removeprefix("<").removesuffix(">")
                    if res.find(".") == -1:
                        line = file.readline()
                        continue
                    for i in config.includes:
                        if path.exists(i + "/" + res):
                            res = i + "/" + res
                elif line.startswith('"') and line.endswith('"'):
                    res = line.removeprefix('"').removesuffix('"')
                    res = path.dirname(filename) + "/" + res
                yield res
            line = file.readline()


def get_files():
    global config
    l = len(getcwd())
    if sys.platform == "win32":
        out = popen("dir /b /s *.cpp")
    elif sys.platform == "linux":
        out = popen('find . -name "*.cpp"')

    return filter(
        lambda x: not x.startswith("helper/") and not x in config.ignores,
        map(
            lambda x: x[l + 1 :].replace("\\", "/").removesuffix(".cpp\n"),
            out.readlines(),
        ),
    )


if __name__ == "__main__":
    main()
