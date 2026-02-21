# wdb

**wdb** is a native Linux debugger written in C++23, built as a hands-on learning project to explore how debuggers work at a low level — process control, breakpoints, register inspection, DWARF debug info, and more.

It is developed by following the reference repository [sdb](https://github.com/TartanLlama/sdb) by TartanLlama and the book *Building a Debugger* from the same author.

## Project structure

```text
wdb/
├── include/libwdb/   # Public headers for the core debugger library
├── src/              # Core library (libwdb) implementation
├── tools/            # CLI frontend (wdbcli)
├── test/             # Catch2 unit tests
├── wiki/             # Notes and guides (e.g. dependency setup)
├── scripts/          # Helper scripts
├── gdb/              # GDB scripts / experiments
└── examples/         # Usage examples
```

## Dependencies

Managed via [vcpkg](https://vcpkg.io):

- **libedit** — readline-compatible line editing for the CLI
- **Catch2** — unit testing framework

## Building

Requires CMake ≥ 3.30, a C++23-capable compiler, Ninja, and vcpkg.

```bash
# Configure (debug build with vcpkg)
cmake --preset debug

# Build
cmake --build --preset debug
```

To also build tests:

```bash
cmake --preset debug -DBUILD_TESTS=ON
cmake --build --preset debug
ctest --preset debug
```

Available presets: `debug`, `release`, `relwithdebinfo`, and `system-*` variants for system-installed libraries.

## Reference

- [TartanLlama/sdb](https://github.com/TartanLlama/sdb) — the reference debugger this project follows
