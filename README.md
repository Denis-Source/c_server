# Simple TCP server written in C

<img src="https://github.com/Denis-Source/c_server/assets/58669569/45eb5fc6-c732-412e-be0f-d4418c6c2623" width="100%">

> Can be used with `netcat` or `telnet`

## Project Overview:

The project aims to develop a simple chat application that can handle incoming connections, manage client-server communication, and process messages efficiently using multithreading and message queues.

## Key Components:
1. **Server Context**:
    - Manages the state of the server, including the key-value table of connections and the buffer of recent messages.
2. **Connection Management**:
    - Provides functions for initializing, handling, and closing client connections.
3. **Message Handling**:
    - Defines message structures and functions for processing incoming and outgoing messages.
4. **Multithreading**:
    - Utilizes pthreads for concurrent execution of tasks, such as listening for incoming connections and handling client requests.
5. **Message Queues**:
    - Implements message queues for inter-thread communication, allowing seamless message passing between different components of the server.

## Workflow:
1. **Initialization**:
    - The server initializes its context, establishes a message queue for communication, and sets up data structures for connection and message management.
2. **Connection Acceptance**:
    - The server listens for incoming connections and accepts them using a listener thread.
3. **Message Handling**
    - Incoming messages from clients are processed using message queues.
    - Message content is decoded and appropriate actions are taken based on the message type.
4. **Response Generation**:
    - Upon receiving messages, the server generates appropriate responses and sends them back to clients or performs necessary actions.
5. **Termination**:
    - The server gracefully shuts down, freeing resources and closing connections before exiting.

## Technologies Used:
- **C Programming Language**: Core server logic is implemented in C for low-level control and performance optimization.
- **POSIX Threads (pthreads)**: Multithreading capabilities are leveraged using pthreads for concurrent execution of tasks.
- **Message Queues (POSIX mq)**: Inter-thread communication is facilitated through message queues, enabling seamless message passing between components.

## Target Environment:
- The server application is designed to run in a POSIX-compliant environment, such as Unix-like operating systems (e.g., Linux).

## Potential Extensions:
- **Security Enhancements**: Implement encryption and authentication mechanisms to ensure secure communication between clients and the server.
- **Feature Expansion**: Add support for additional functionalities, such as file transfer, user authentication, and real-time messaging.
- **Performance Optimization**: Fine-tune server performance by optimizing data structures, minimizing resource usage, and implementing caching strategies.
