*This project has been created as part of the 42 curriculum by ieddaoud, mgarouj, ielouarr.*

# ft_irc

## Description

`ft_irc` is an implementation of a simplified Internet Relay Chat (IRC) server written in C++98. The objective of the project is to understand network programming, socket communication, multiplexing with `poll()`, and the IRC protocol defined by RFC 1459.

The server accepts multiple TCP client connections simultaneously and allows users to communicate through private messages and channels using a subset of the IRC protocol. The project emphasizes low-level networking, event-driven programming, client management, and protocol parsing.

The server is compatible with standard IRC clients (such as HexChat or irssi) and supports the mandatory commands and channel management features required by the 42 subject.

---

## Features

### Connection

- TCP server
- Multiple clients simultaneously
- Non-blocking sockets
- `poll()`-based event loop
- Password authentication

### User Registration

- PASS
- NICK
- USER

### Channel Management

- JOIN
- TOPIC
- INVITE
- KICK
- MODE

### Messaging

- PRIVMSG

### Channel Modes

- `+i` Invite-only channel
- `+t` Topic protection
- `+k` Channel password
- `+o` Operator privilege
- `+l` User limit

### IRC Replies

The server implements the appropriate numeric replies and error messages defined by RFC 1459 whenever applicable.

---

## Project Structure

```
.
├── Client/
├── Commands/
├── Headers/
├── Server/
├── Channel/
├── main.cpp
├── Makefile
└── README.md
```

---

## Instructions

### Requirements

- C++98 compiler
- Make

### Compilation

```bash
make
```

The executable produced is:

```text
ircserv
```

### Run

```bash
./ircserv <port> <password>
```

Example:

```bash
./ircserv 6667 password42
```

---

## Connecting with an IRC Client

Example using HexChat:

1. Create a new network.
2. Set the server to:

```
127.0.0.1/6667
```

3. Set the server password to the password used when launching the server.
4. Connect.

After connecting, you can use commands such as:

```irc
/join #42

/topic #42 :Welcome to ircserv

/invite Ali #42

/kick Ali Spamming

/msg smain Hello!
```

---

## Example Session

Create a channel:

```irc
/join #42
```

Set the topic:

```irc
/topic #42 :Welcome to the channel
```

Invite another user:

```irc
/invite Ali #42
```

Kick a user:

```irc
/kick Ali :Spamming
```

Send a private message:

```irc
/msg smain Hello!
```

---

## Technical Choices

- C++98 only
- No external libraries
- Event-driven architecture using `poll()`
- One server process
- Non-blocking sockets
- Modular command handlers
- Channel and client management separated into dedicated classes

---

## Resources

### IRC Protocol

- RFC 1459 — Internet Relay Chat Protocol
  https://datatracker.ietf.org/doc/html/rfc1459

### Socket Programming

- Beej's Guide to Network Programming
  https://beej.us/guide/bgnet/

### C++

- cppreference
  https://en.cppreference.com/

---

## AI Usage

Artificial Intelligence was used as a learning and debugging assistant throughout the development of this project.

It was primarily used for:

- Understanding and interpreting error messages during compilation and runtime
- Helping identify the possible causes of bugs based on error messages

All architectural decisions, implementation, testing, debugging, and final source code were completed and validated by the project authors.

---

## Authors

- ieddaoud
- mgarouj
- ielouarr
