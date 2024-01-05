# φPhiScript

[![PHISCRIPT](https://img.shields.io/badge/φPhiScript-4AAADA)](https://github.com/upizpp/phiscript) [![UPIZPP](https://img.shields.io/badge/UPIZPP-4A2F96)](https://github.com/upizpp)

PhiScript is an interpreted programming language that I have created with the goal of enhancing my programming skills. Due to its experimental nature, it may not be actively maintained for an extended period. However, I plan to address any reported bugs and provide occasional updates.

## Content

- [φPhiScript](#φphiscript)
  - [Content](#content)
  - [Features](#features)
  - [Build](#build)
    - [Dependencies](#dependencies)
    - [Steps](#steps)
    - [Release](#release)
    - [Debug](#debug)
    - [Unit Test](#unit-test)
  - [Changing config.json](#changing-configjson)

## Features

- Dynamic typing
- Support for if-else statements
- Support for for-else statements with tags
- Support for while-else statements with tags
- Ability to use break statements with tags
- Ability to use continue statements with tags
- Import statement functionality
- Built-in modules
- *Functions* (work in progress)

## Build

### Dependencies

**Python:** Version 3.9 or higher  
**Clang:** Version 10 or higher. Please ensure that it supports [C++20](https://en.cppreference.com/w/cpp/20). If you wish to change the compiler, please modify the `config.json` file (refer to the *[Changing config.json](#changing-configjson)* section for detailed information).

### Steps

```cmd
git clone https://github.com/upizpp/PhiScript.git
cd PhiScript
python build.py
```

### Release

```cmd
python build.py
```

### Debug

```cmd
python build.py DEBUG
```

### Unit Test

```cmd
python build.py TEST DEBUG
```

## Changing config.json

The `config.json` file is used to configure the build process. Below is a description of each field:

- `compiler`: The compiler used to compile the source code. The default value is `clang++`.
- **Warning:** Do not attempt to modify the value of `standard`. The `build.py` script can only run in the `C++20` standard.
