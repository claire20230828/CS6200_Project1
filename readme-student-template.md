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
Based on the project description, the server will open an assigned file, send its contents to the client, and then close the connection. The client will receive the data from the server and write it into an assigned file.  
  
* The number of bytes the server can read and send at a time is not predetermined.  
	-> Implement a while loop to continuously read from the file, track how many bytes have been read and sent, and handle partial reads and sends.  
  	-> Ensure that the read/write pointer on the server is properly reset between operations using lseek()  
* remember to check if there is any open file/tranfer error
 
### Implementation and Testing
#### * client
```c
socket() 
connect()  
open()  //open output file  
recv()   // receive data from server 
write()  // record data to output file  
```
#### * server
```c
socket()  
setsockopt()  
bind()   
listen()  
while{  
	accept() 	// check and accept for client connection  
	while{  
		read()  //read file in chunks
		while{
			send()		//send data in chunks
lseek()	 //reset the read/write pointer
```
Tested the implementation using: ```./transferserver -f server_file``` & ```./transferclient -o output_file```

## Project 1 Part1 (gfclient)
### Project Design 
![](/image/Part1_client.jpeg)  


## Project 1 Part1 (gfserver)
### Project Design 
![](/image/Part_1%20gfserver.jpeg)


## Project 1 Part2 (multi-threaded gfclient)
### Project Design 
![](/image/Part2_gfclient.jpeg)


## Project 1 Part2 (multi-threaded gfserver)
### Project Design 
![](/image/Part2_gfserver.jpeg)

## References
#### Pizza no @80 Client Hung Error
https://piazza.com/class/lzwveb5orfe6by/post/80
### (multi-threaded gfclient)
#### Pizza no @81 Few gotchas for MT Project 1 client (part 2)
https://piazza.com/class/lzwveb5orfe6by/post/81
### (multi-threaded gfserver)
#### Pizza no @549 My notes on the server portion 
https://piazza.com/class/lzwveb5orfe6by/post/549
