import argparse
import subprocess
import threading
from colors import Colors
from builder import find_files, global_config
from typing import Iterable


MAX_TASKS = 8


def main():
    parser = argparse.ArgumentParser(description="Format a project.")
    parser.add_argument(
        "--type",
        "-t",
        required=True,
        choices=list(global_config.types) + ["all"],
        help="type of project to format.",
    )
    args = parser.parse_args()
    units = find_files(
        lambda path: (
            path.endswith(".cpp") or path.endswith(".h") or path.endswith(".hpp")
        ),
        args.type,
    )
    thread = threading.Thread(target=do_format, args=[units])
    thread.start()
    thread.join()


def do_format(units: Iterable):
    global finished
    finished = 0
    tasks = []
    task_count = 0
    for unit in units:
        tasks.append(unit)
        if len(tasks) == MAX_TASKS:
            thread = threading.Thread(target=format, args=[tasks])
            thread.start()
            tasks = []
        task_count += 1
    if len(tasks) != 0:
        thread = threading.Thread(target=format, args=[tasks])
        thread.start()
    temp = -1
    while finished != task_count:
        if temp != finished:
            temp = finished
            print(
                Colors(
                    Colors.GREEN, f"Formatting... {finished / task_count * 100:2.2f}% "
                )
            )
    print(Colors(Colors.GREEN, "Formatting... 100.00% "))


def format(tasks: list):
    for filename in tasks:
        subprocess.call(["clang-format", "-i", filename, "--style=file"])
    mutex = threading.Lock()
    mutex.acquire()
    global finished
    finished += len(tasks)
    mutex.release()


if __name__ == "__main__":
    main()
