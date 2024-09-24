# Project README file

This is tyang431 <2024 fall CS6200 Project 1> Readme file.  

## Project 1 Warmup (Echo)
### Project Design  
Based on the warmup files ```echo-client.c``` and ```echo-server.c```, and according to the project description, I modified several parts: 
  
* Message Length: The message will not be longer than 15 bytes.  
* Server Response: The server sends the exact message back to the client.  
* IP Support: The programs should support both IPv4 and IPv6.  
* Server Termination: The echoserver should not terminate.  
	Implemented two while(1) loops:  
	Outer Loop: Continuously accepts new client connections.  
	Inner Loop: Handles each client's requests.  
 
### Implementation and Testing
Based on the course guidelines, the implementation consists of the following key functions:   

#### * client
```c
socket() 
connect() 
send()   // send message to server
read()  // receive message from server  
```
#### * server
```c
socket()  
setsockopt()  
bind()   
listen()  
while{  
	accept() 	//connection  
	while{  
		read() // receive message from client  
		write() // send message to client
```
Tested the implementation using: ```./echoserver``` & ```./echoclient -m messages```


## Project 1 Warmup (Transferfile)
### Project Design  
Based on the project description, server will open a assigned file, send to client and close connection. And client will also recevie data from server and write into a assigned file. 
  
* the bytes the server can read and send each time is not predetermined.  
	-> using while loop and keep tracking how many bytes server has read and send
  	-> remember to reset the read/write pointer for server  
* remember to check if there is any open file/tranfer error
 
### Implementation and Testing
#### * client
```c
socket() 
connect()  
open()  //output file  
recv()   // receive from server 
write()  // record to output file  
```
#### * server
```c
socket()  
setsockopt()  
bind()   
listen()  
while{  
	accept() 	// check connection  
	while{  
		read()  //read in chunks
		while{
			send()		//send in chunks
lseek()	 //reset the read/write pointer
```
Tested the implementation using: ```./transferserver -f server_file``` & ```./transferclient -o output_file```

## Project Description
