import json

class ConfigType:
    # the compiler to use
    compiler: str = "g++"
    # extra arguments for the compiler
    extra: str = ""
    # extra arguments for the linker
    link_extra: str = ""
    # the path to the cache directory
    cache: str = ""
    # the directory to scan
    scan: str = ""
    # the naming format of the code units
    units: list = ["*.cpp"]
    # the units which are ignored
    ignore: list = [""]
    # the RegEx pattern to search the associated files.
    pattern: str = """^#include\s*(?:(?:"(.*)")|(?:<(.*)>))\s*$"""
    # the output file path
    output: str = "main.exe"
    # the compile command pattern.
    command: str = """{compiler} {includes} {unit} {extra} -c -o {output}"""
    # whether to run the output file.
    auto_run: bool = True
    # the include directories
    includes: list = []

    def __init__(self, config: dict) -> None:
        self._lock(config)
        self.pick("compiler")
        self.pick("extra")
        self.pick("link_extra")
        self.pick("cache")
        self.pick("scan")
        self.pick("units")
        self.pick("ignore")
        self.pick("pattern")
        self.pick("command")
        self.pick("output")
        self.pick("auto_run")
        self.pick("includes")
        self._unlock()
         
    def get_command(self, unit: str, output: str) -> str:
        return self.command.format(
            includes = " ".join([("-I" + x) for x in self.includes ]),
            extra=self.extra, compiler=self.compiler, output=output, unit=unit
        )
    
    def pick(self, key: str) -> None:
        if self.__temp is None:
            raise Exception("Unable to call pick before calling _lock.")
        if key in self.__temp:
            setattr(self, key, self.__temp[key])

    def _lock(self, config: dict) -> None:
        self.__temp = config
    
    def _unlock(self) -> None:
        del self.__temp


class Config:
    def __init__(self, path: str) -> None:
        with open(path, encoding="utf-8") as file:
            data: dict = json.loads(file.read())
            self.data = data
            self.types = data.keys()
    
    def pick(self, type: str) -> ConfigType:
        if not type in self.data:
            raise KeyError("the type is not existed in the config file.")
        return ConfigType(self.data[type])



