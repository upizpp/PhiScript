from re import finditer, RegexFlag
from log_handler import *


def main() -> None:
    logs = get_logs(".\\logs")
    log_path = input_data(logs)
    print()
    succeeded, uncaptured, address_used_count  = analyze(log_path)
    color = Colors.GREEN if succeeded else Colors.RED
    print(
        "Analyze",
        Colors([color, Colors.BOLD], "[Succeed]" if succeeded else "[Failed]"),
    )
    print(f"Used {address_used_count} addresses.")
    print(f"{len(uncaptured)} uncaptured" + (":" if uncaptured else "."))
    if uncaptured:
        print(", ".join(uncaptured))


def analyze(filename: str) -> bool:
    with open(filename, "r", encoding="utf-8") as file:
        log_content = file.read()
        log_content = filter_type(log_content, ["TRACE"])
        init_map = []
        aliases = {}
        for mat in finditer(
            "([A-Z]*) ([0-9a-f]*), content: (.*)\.", log_content, RegexFlag.MULTILINE
        ):
            content = mat.group(3)
            address = mat.group(2)
            operation = mat.group(1)
            if operation == "INITIALIZE":
                init_map.append(address)
                aliases[address] = f"Address{Colors([Colors.YELLOW, Colors.BOLD], len(aliases))}"
                print(
                    Colors(Colors.CYAN, "Initialize"),
                    "\t",
                    aliases[address],
                    content,
                    address,
                )
            elif operation == "FREE":
                try:
                    print(
                        Colors(Colors.CYAN, "Free"),
                        "\t\t",
                        aliases[address],
                        content,
                        address,
                    )
                    init_map.remove(address)
                except ValueError as e:
                    print(e)
                    return False, init_map, -1
                except KeyError:
                    ...
                    # return False, init_map, -1
        return len(init_map) == 0, init_map, len(aliases)


if __name__ == "__main__":
    main()
