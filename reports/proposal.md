# IRSea: Networking in C

Sreekanth Sajjala, Siddharth Garimella, Kawin Nikomborirak

### Goals

The goal of this project is to become more familiar with networking in C. As this is a large topic, we have broken it down into two levels based on our specific interests. On a high-level, we will explore the basics of client-server app development. Through this, we hope to understand more about the use and implementation of sockets in C. We believe developing a simple IRC server in C can help us accomplish this.

Although our first exploration helps us learn about some protocols controlling exchanges of data on a high level, we also want to learn about the specifics of reliable data transfer via noisy channels. On this lower level, we plan to transmit information between two raspberry pi's using an LED and a phototransistor. C code will be written to process incoming signals and control for errors using Hamming codes. 

We shall build networking levels between these 2 extremes as stretch goals if time and resources allow us to.


## High level: Client-server application development

### Learning Goals: 
 - Sockets in C
 - IRC Protocol
 - Client-server Application Architecture

### Resources: 
 - Class materials from Computer Networks
 - https://en.wikibooks.org/wiki/C_Programming/Networking_in_UNIX
 - https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/
 - https://tools.ietf.org/html/rfc1459

### First steps:
 - Play with creating sockets in C: Sreekanth 
 - Make basic client to send character to server: Kawin 
 - Research IRC Channel protocol: Siddharth

### MVP:
An IRC server capable of managing multiple connections and exchanging text messages. 

## Low level: Error checking of data transmitted through a noisy channel

### Learning Goals: 
 - Implementing hamming codes
 - Basic signal processing in C
 - C Data manipulation

### Resources: 
 - Discrete Mathematics class material
 - https://vuiis.vumc.org/~dss/fast_io.htm

### First steps:
 - Look into implementing the hamming codes error checking in C: Siddharth
 - Set up raspberry Pis for LED information transmission: Sreekanth
 - Process phototransistor input with C: Kawin

### MVP:
An application capable of conducting data transfer via phototransistor input with error checking.




