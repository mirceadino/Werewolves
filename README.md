# Werewolves
Mini-project with sockets and werewolves

Work in progress.

### Communication protocol
Clients and server always communicate through packages of 256 bytes.

1. Client connects to the server.

2. Client sends to server a username.

3. Server approves or not the username. Server sends to client a confirmation code.

4. If the confirmation code is not OK, client tries a different username (jump to step 2).

5. If the confirmation code is OK, client can send messages to the server.
