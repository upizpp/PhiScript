# PhiScript ![PhiScript](https://img.shields.io/github/stars/upizpp/PhiScript.svg)

![DEVELOPING](https://img.shields.io/badge/REPO-DEVELOPING-blue)

PhiScript is a scripting language that is designed to make the code more pretty and elegantly.  
The more the function is used, the shorter it will be coded.

# Contents

- [build](#build)

## Build

### builder.py

| Type           | Description                                                          |
| -------------- | -------------------------------------------------------------------- |
| release        | The released program.                                                |
| release_static | The static linked(-static) released program.                         |
| debug          | To debug program.                                                    |
| test           | To test program.                                                     |
| lib            | The dynamic linked library(\*\*.dll/\*\*.so).                        |
| lib            | The static linked(-static) dynamic linked library(\*\*.dll/\*\*.so). |
| clean          | To clean all the outputs.                                            |
| all            | All of them.                                                         |

A possible way to build is:

```bash
git clone https://github.com/upizpp/PhiScript.git
cd PhiScript
./builder -t{Your type here}
```

Then you will find the output in the directory `./build`.
