# Project README file

This is tyang431 <2024 fall CS6200 Project 1> Readme file.

## Project 1 Warmup (Echo)
### Project Design
Base on Warmup file echo-client.c/echo-server.c and according to description
I modfied some parts:
•	the message will not be longer than 15 bytes
	-> change BUFFER_SIZE
•	server sends the exact message back to the client
	 -> del capitalizeBuffer()
•	Your programs should support both IPv4 and IPv6.
  -> modify how to set socket 
•	echoserver won't terminate
	-> Using two while(1) loops
	* Outer Loop -> Continuously accept new client connections
	* Inner Loop -> Handle each client's requests
 
### Implementation and Testing
base on the course:
•	client
```c
socket() // to get a socket file descriptor
connect() // make a connection session
send()   // send message to server
read()  // receive message from server
//c




## Project Description

## Project Description
