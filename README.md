# Ft_Irc


### Client management

After creating the main listening socket we will have to start managing client.
Things to do are :

- Accepting new connexion from client: registering them as client
- listening to incoming data from client and buffering them in personnal buffer
- Reading buffer from client to extract command *Optionnal*
- Queuing command
- executing command


### Channel
- Channel names are string (beginning with a '&', a '#', ...)

## Command and RPL

### Registration

> Immediately upon establishing a connection the client must attempt registration, without waiting for any banner message from the server.

> Until registration is complete, only a limited subset of commands SHOULD be accepted by the server. This is because it makes sense to require a registered (fully connected) client connection before allowing commands such as JOIN, PRIVMSG and others.

> Unregistered client should be timeout after 2 second of not getting registered


|Done?| Command | Description | Reply | Error |
|-----|---------|-------------|-------|-------|
|- [ ]| NICK    |             |       | Error |
|- [ ]| USER    | Description | Reply | Error |
|- [ ]| PASS    | Description | Reply | Error |


## Help

### Function

- socket() :

- htons / htonl : convert value given to a valid value for network using. Network value can only be big-endian style.
- ntohl / ntohs : Convert value Network-style representation to system representation (Big or Little endian)


- in_addr_t inet_addr(char \*addr) : Convert string representation of ipv4 address to binary representation for further usage.
- char \*inet_ntoa(struct \*in_addr in) : Convert struct adress to string representation for printing (buffer returned is static stored so further call will overwrite it).


## Command syntax

- Command always start with COMMAND in uppercase followed by one or more argument. Last argument can begin with a ':' marking the beginning of the last argument. Last argument starting with ':' can include whitespace

- Server will answer with numerical Replies (named RPL). Replies with number >= to 400 are Error (named ERR). Syntax of Replies are always the same depending of RPL (or ERR) but some content can varies depending of command argument (like channel or comment) and user using them.

## Requirement

- Server / client socket connection

 - [x] Accept multiple client and register them under a user id
 - [ ] promotion of registered client as user

---
- Command management

 - [x] Get command on user-sockfd (Should use a buffer for eachuser)
 - [x] Parse command
 - [ ] Implementation of cmd
  

---
- Signal handling

 - [ ] CTRL+C : still have to check for cmd > to buffer size and management of lost fd
 - [x] CTRL+D : Management of partial cmd
 - [ ] CTRL+Z


- [ ] Define client structure including: socket-fd, Id, command buffer
- [ ] Define rooms with userid

---
- Server command

 - [ ]  [PING](https://modern.ircdocs.horse/#ping-message)
 - [ ]  [OPER](https://modern.ircdocs.horse/#oper-message)
 - [ ]  [KILL](https://modern.ircdocs.horse/#kill-message) 

---
- Operator command

> These are channel command that can only be used by operator of channel (you first need to be on the channel to do these operation)
 - [ ]  [MODE](https://modern.ircdocs.horse/#mode-message) : user('o', ), channel( )   'i', 't', 'k', 'o' 'l'
 - [ ]  [KICK](https://modern.ircdocs.horse/#kick-message)
 - [ ]  [INVITE](https://modern.ircdocs.horse/#invite-message)
 - [ ]  [TOPIC](https://modern.ircdocs.horse/#topic-message)
 
---

- [ ] Implement RPL system
 - [ ] Implement ERR

### Extra

- [ ] Do a script to automate connexion on server with nc

## External Ressource

- [Reference Repo](https://github.com/marineks/Ft_irc)

- [Network programming guide](https://beej.us/guide/bgnet/pdf/bgnet_a4_c_1.pdf)

- [Irc : Protocol explanation](http://chi.cs.uchicago.edu/chirc/irc.html)

- [ModernIrc : Complete command reference](https://modern.ircdocs.horse/)

- [IRC2812](https://www.tech-invite.com/y25/tinv-ietf-rfc-2812.html) 

- [Irssi Doc](https://irssi.org/New-users/)
