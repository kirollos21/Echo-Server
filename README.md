# Echo Server

## Overview
This is a simple echo server implemented in C using BSD sockets. The server listens for incoming TCP connections and echoes back any messages received from clients. It can handle multiple clients simultaneously.

## Files
- `Echo.c`: The source code for the echo server.
- `Echo.h`: The header file containing function declarations and necessary includes.
- `Echo_mf`: The Makefile used to compile the server.

## Compilation
To compile the server, use the provided Makefile. Run the following command in the terminal:

```bash
make -f Echo_mf

## How to Run
To start the server, execute: ./echo_server
The server will listen on port 8080 and accept multiple client connections.