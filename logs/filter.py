import os
from sys import path as sys_path

sys_path.append("./")
from colors import Colors


def filter_type(content: str, filters: str | list[str]) -> str:
    if filters is str:
        filters = [filters]
    lines = content.splitlines()
    passed = []
    for line in lines:
        for filter in filters:
            if line.startswith(filter):
                passed.append(line)
    return "\n".join(passed)


def input_data(logs: list[str]) -> str:
    print("Select a log file: ")
    for i in range(len(logs)):
        print(
            "●",
            Colors([Colors.CYAN, Colors.BOLD], i),
            "-",
            Colors(Colors.BOLD, os.path.split(logs[i])[-1]),
        )
    index = int(input("Input the index: "))
    return logs[index]


def get_logs(dir: str) -> list[str]:
    result = []
    for path in os.listdir(dir):
        _, ext = os.path.splitext(path)
        if ext == ".log":
            result.append(os.path.join(dir, path))
    return result
