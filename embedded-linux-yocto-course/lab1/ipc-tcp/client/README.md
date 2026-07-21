# TCP Client

This directory contains the TCP client implementation and its documentation.

## Implemented Features

- create an IPv4 TCP socket;
- connect to the Raspberry Pi server;
- send messages to the server;
- receive server responses;
- handle server disconnection;
- close the socket correctly.

## Files

- `client.c`: commented TCP client source code;
- `client.md`: detailed description of the client workflow.

## Build

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic client.c -o client
```
