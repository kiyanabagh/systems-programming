# Systems Programming

C and Bash programs from CSCE 3600 (Systems Programming) at the University of North Texas,
covering low-level systems concepts: bitwise manipulation, POSIX threads, UDP networking,
and Unix process and pipe mechanics.

---

## major1-bitwise — Bitwise Operations in C

A menu-driven C program implementing four bitwise operations on 32-bit unsigned integers,
each in its own source file.

| File | Operation |
|---|---|
| `clz.c` | Count leading zeroes via bit scan |
| `endian.c` | Swap byte order: 0xAABBCCDD → 0xDDCCBBAA |
| `rotate.c` | Rotate-right by n positions using shifts and masking |
| `parity.c` | Compute parity bit |
| `major.c` | Menu driver wiring all four operations together |
| `major.h` | Shared header with function declarations |

**Compile and run:**
```bash
gcc -o major major.c clz.c endian.c rotate.c parity.c
./major
```

---

## rec01-ascii-bits-bytes — ASCII Character Representation in C

Converts a single printable ASCII character into its decimal, hexadecimal, and 8-bit binary
representation using bitwise operations. Binary output is produced via right shifts and a
bitmask (`& 1`) from bit 7 down to bit 0.

**Compile and run:**
```bash
gcc -o rec01 rec01.c
./rec01
```

---

## minor03-producer-consumer — POSIX Threads

A bounded buffer implemented with POSIX threads: one producer reads characters from stdin,
two consumers print them. Synchronization uses a mutex and two condition variables
(`cond_full` to block the producer when the buffer is full, `cond_empty` to block consumers
when it is empty).

**Compile and run:**
```bash
gcc -o minor03 minor03.c -lpthread
./minor03
```

---

## minor04-udp-ping — UDP Socket Programming

A UDP ping utility modeled after ICMP ping. The client sends 10 PING messages to the server
and measures round-trip time for each. The server randomly drops 30% of packets to simulate
network loss. At the end the client prints per-packet RTT and a summary of packets sent,
received, loss percentage, and min/max/avg RTT.

| File | Role |
|---|---|
| `minor04svr.c` | UDP server — binds to a port, receives packets, drops ~30%, sends PONG |
| `minor04cli.c` | UDP client — sends 10 PINGs, measures RTT, prints stats |

The client uses `getaddrinfo` and prefers IPv6 with automatic IPv4 fallback.

**Compile and run:**
```bash
gcc -o minor4svr minor04svr.c
gcc -o minor4cli minor04cli.c

./minor4svr 9000
./minor4cli localhost 9000
```

---

## minor2-bash-assistant — Bash Scripting

An interactive personal assistant written in Bash. Uses `set -euo pipefail` for strict
error handling, traps SIGINT/SIGTERM to prevent unsafe exits, and logs all actions with
timestamps to `~/minor2_actions.log`.

**Features:**
- Daily briefing: username, date/time, calendar, running process count
- File organizer: scans a directory, reports counts by type, moves `.log` files to a `logs/` subfolder
- Reminder list: add, view, and delete reminders (max 5, stored in memory)
- Health check: classifies heart rate input as low, normal, or high
- `--debug` flag enables `set -x` for tracing

**Run:**
```bash
chmod +x minor2.sh
./minor2.sh
# or with debug tracing:
./minor2.sh --debug
```

---

## rec10-unix-pipes — Process Forking and Pipes

Demonstrates how Unix pipelines work at the system call level using `fork`, `pipe`, `dup2`,
and `execvp`.

| File | What it implements |
|---|---|
| `rec10A.c` | `grep pattern < input_file > output_file` via fd redirection |
| `rec10B.c` | `cat input \| grep pattern \| cut -b 1-10` via two pipes and three forked children |

`rec10B.c` manually constructs the full three-stage pipeline: forks three children, connects
them with two pipes using `dup2`, closes all unused ends in each process, then waits for all
three to finish.

**Compile and run:**
```bash
gcc -o rec10A rec10A.c
gcc -o rec10B rec10B.c

./rec10A "search_term" input.txt output.txt
./rec10B input.txt "search_term"
```

---

## Requirements

- GCC with C99 or later
- POSIX-compliant Linux environment
- Bash 4.0+
