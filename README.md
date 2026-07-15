# shell-cpp

A POSIX-compliant shell implemented in C++, supporting command parsing, builtin
commands (`cd`, `pwd`, `echo`, `type`, `exit`), and execution of external
programs.

## Building

```sh
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake
cmake --build ./build
```

## Running

```sh
./run.sh
```
