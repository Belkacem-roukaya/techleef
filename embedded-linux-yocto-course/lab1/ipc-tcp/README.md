# IPC and TCP Socket Programming

This directory contains my POSIX IPC and TCP socket programming exercises.

## Projects

- `server/`: TCP server implementation and documentation.
- `client/`: TCP client implementation and documentation.

## Progress

| Component | Status |
|---|---|
| Single-client TCP server | Completed |
| TCP client | Completed |
| Multithreaded server | Planned |
| Group chat and broadcast | Planned |

## Build

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic server/server.c -o server/server
gcc -std=c11 -Wall -Wextra -Wpedantic client/client.c -o client/client
```
