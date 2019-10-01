# SocketIO - a client-server library for C++

## Traits
The main selling point of SocketIO is the freedom it gives to the user. It
achieves this while also having unified and simple API.

It does not assume any messaging conventions - sending and receiving a message
is done via objects of the `std::string` class. it makes the whole
process simple to understand but expressive enough for most uses.

Support for both IPv4 and IPv6 is provided.

## Dependencies
* A POSIX-compliant operating system
* A compiler supporting C++17
* [Google Test](https://github.com/google/googletest) for running the tests

## Obtain and install
```
git clone https://github.com/dak98/SocketIO.git
cd SocketIO
make
make install
```

## Uninstall
```
make uninstall
make clean
```

## Usage
### Intialization
Creating a new server:
```cpp
#include <socket_io/server.hpp>
using namespace socket_io;

server handle{"8000", ip_protocol::IPv4};
```

Creating a new client:
```cpp
#include <socket_io/client.hpp>
using namespace socket_io;

client handle{"127.0.0.1", "8000"};
```
The connection with the server is established and managed automatically by the
library. A mismatch in the protocols produces a runtime exception.

The server assigns an ID number (starting from 100 by default) to the new client
and sends it back. It is necessary for communication.

### Communication
Receiving a message (blocking):
```cpp
std::string message = handle.receive();
```
The API here is the same for both the server and the client.

Sending a message from the server to the client:
```cpp
handle.send(100, "Hello from the server");
```
The ID numbers of all the clients are stored in the registry.

Sending a message from the client to the server:
```cpp
handle.send("Hello from the client");
```

It is up to the user of the library to implement the messaging between clients
alongside their custom communcation protocol.

## Example
The [TalkNet](https://github.com/dak98/TalkNet) project uses SocketIO with Qt
to create a messaging application.

## Testing
Building the tests:
```
make test
```

Running the tests:
```
./bin/tester
```






