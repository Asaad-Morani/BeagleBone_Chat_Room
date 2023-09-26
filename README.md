# BeagleBone_Chat_Room

Download the gcc cross complier for BBB and then unzip the file
$tar -xf gcc-arm-8.3-2019.03-x86_64-arm-linux-gnueabihf.tar.xz
$mv gcc-arm-8.3-2019.03-x86_64-arm-linux-gnueabihf ~/Desktop/
$~/Desktop/gcc-arm-8.3-2019.03-x86_64-arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc domain_tcp_socket_server.c -o server
$./server
$scp server.c debian@192.168.7.2:/home/debian/desktop
 (move the server.c file to BBB)
1. ssh debian@192.168.7.2 (USB device address)
   --> use "sudo ifconfig eth0 192.168.1.15/24" (or any ip address you want) to set up ip adress of BBB
   this connect us to the Beaglebone Black.
2. connect the Ethernet cable to pc with BBB.
3. from your pc check connection using "$ping 192.168.1.15"
   (check the connection using ethernet cable)
4. in Beaglebone run "$gcc server.c -o server -pthread"
   (go to the path where the server.c located)
5. then $./server
6. terminal will open with "Server listening on port 8081..."

7. now open 3 or more clients and start send messages from each one.
8. the messages from every clients will appear at the server terminal in BeagleBone.
9. the BBB server will send back the messages to other clients.
10. thats it !! now we have chat room between clients (while the server handle chat).

