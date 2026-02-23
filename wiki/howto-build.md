# How to Build

```bash
cmake --build build/debug/
```

## Running tests

```bash
ctest --preset dev
ctest --test-dir build/debug
```

```bash
(cd /home/wmarini/projects/cpplab/wdb/build/debug/test && ./wdb-tests)
```

## Running cli

```bash
sudo bash -c 'echo 0 > /proc/sys/kernel/yama/ptrace_scope'
CAP_SYS_PTRACE=+eip build/debug/tools/wdbcli
CAP_SYS_PTRACE=+eip build/debug/tools/wdbcli -p <PID>
CAP_SYS_PTRACE=+eip build/debug/tools/wdbcli -p 40831
```

