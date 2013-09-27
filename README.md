udptcp
======

Convert between udp and tcp. 
Can handle som and eom to split messages from tcp. 
The program is usable as a code base for none blocking interrupt driven communication using poll.

Eclipse project.
Compile with -lrt

Run arguments:



tcpAddress <tcpAddress>           Forign address for tcp connection (client or server) 
[tcpPort <tcpPort>]               Forign server tcp port, its an error to use with tcpServerPort
[tcpServerPort <tcpServerPort>]   If set this program will work as server. 
                                  Multiple connections can be used, but is mainly to avoid old connection block of new 
udpLocalAddress <udpLocalAddress> Local address to bind for UDP
udpLocalPort <udpLocalPort>       Local Port to bind for UDP
udpAddress <udpAddress>           Forign address for UDP
udpPort <udpPort>                 Forign Port for UDP
som <som>                         Use a hex example 0x11 if packets uses a start of message char 
eom <eom>                         Use a hex example 0x13 if packets uses a start of message char
                                  Program also suports som == eom 
                                  
