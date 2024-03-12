#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

// Most Comments are written for my own learning/troubleshooting purposes

int main(int argc, char *argv[]) {

    unsigned short port; // Port Number
    int name_length; // length of client name 
    char buffer[16]; // Buffer for sending/receiving data over stream -- current size chosen arbitrarily
    int server_socket; // socket descriptor
    int client_socket; // socket descriptor
    struct sockaddr_in server_info; // struct defined in socket.h for use with socket API functions 
    struct sockaddr_in client_info; 

    if (argc != 2) {
        fprintf(stderr, "Error: requires 1 arg -- port number"); // stderr is the non-buffered output stream used for errors
        exit(1);
    }

    port = (unsigned short) atoi(argv[1]);

    /* Socket Creation 
    * socket() takes 3 args -- int domain, int type, and int protocol

    * - domain -> communication domain
    * -- AF_LOCAL for local
    * -- AF_INET for IPV4
    * -- AF_INET6 for IPV6

    * - type -> communications type
    * -- SOCK_STREAM for TCP
    * -- SOCK_DGRAM for UDP

    * - protocol -> protocol value for IP
    * -- same number that appears on protocol field of IP header of packet 
    * -- should be 0

    * Returns a non-negative integer (the file descriptor) if socket creation successful, otherwise -1
    * perror(char *s) prints s followed by the error message produced by socket() if -1 
    * More info: https://man7.org/linux/man-pages/man2/socket.2.html
    */

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
        perror("Socket()");
        exit(2);
    }

    // Assign data to our server struct

    server_info.sin_family = AF_INET;
    server_info.sin_port = htons(port); // converts unsigned short to TCP/IP network byte order, which is big-endian 
    server_info.sin_addr.s_addr = INADDR_ANY; // INADDR_ANY = 0.0.0.0 --  means any address for socket binding

    /* Socket Binding and Error Handling
    * bind() binds a socket to our server address
    * bind() takes 3 args -- int sockfd, const struct sockaddr *addr, socklen_t addrlen

    * - sockfd -> descriptor for socket 
    * - addr -> pointer to server address
    * - addrlen -> size in bytes of address structure at addr

    * On success, 0 is returned 
    * More info: https://man7.org/linux/man-pages/man2/bind.2.html
    */

    if ((bind(server_socket, (struct sockaddr *) &server_info, sizeof(server_info)) < 0)) {
        perror("Bind()");
        exit(3);
    }

    /* Listen for Connections
    * listen() marks the socket identified with sockfd as passive, meaning it will be used to accept incoming connections with accept()   
    * listen() takes 2 args -- int sockfd, int backlog

    * - sockfd -> descriptor of socket 
    * - backlog -> defines max length of queue for pending connections for sockfd

    * On success, 0 is returned
    * More info: https://man7.org/linux/man-pages/man2/listen.2.html
    */

    if (listen(server_socket, 1) < 0) {
        perror("listen()");
        exit(4);
    }

    /* Accept Connection
    * accept() extracts the first connection request from queue of pending connections for the listening socket, sockfd, creates a new connected socket
    * accept() takes 3 args -- int sockfd, struct sockaddr *_Nullable restrict addr, socklen_t *_Nullable restrict addrlen

    * sockfd -> socket descriptor 
    * addr -> pointer to sockaddr struct.  
    * addrlen -> size in bytes of struct pointed to by addr

    * returns new socket descriptor (client)
    * More info: https://man7.org/linux/man-pages/man2/accept.2.html
    */

    name_length = sizeof(client_info);

    if ((client_socket = accept(server_socket, (struct sockaddr *) &client_info, &name_length)) == -1) {
        perror("accept()");
        exit(5);
    }

    /* Recieve Info from Client
    * recv() is used to receive messages from a client
    * recv() takes 4 args -- int sockfd, void buf[.len], size_t len, int flags

    * sockfd -> socket descriptor (client)
    * buf[.len] -> buffer to store message
    * len -> size of buffer
    * flags -> see "More info" for additional info on flags

    * returns number of bytes received 
    * More info: https://man7.org/linux/man-pages/man2/recv.2.html
    */

    if ((recv(client_socket, buffer, sizeof(buffer), 0)) == -1) {
        perror("recv()");
        exit(6);
    }

    /* Send Info to Client
    * send() is used to send messages to a client
    * send() takes 4 args -- int sockfd, const void buf[.len], size_t len, int flags

    * sockfd -> socket descriptor  (client)
    * buf[.len] -> buffer to store message
    * len -> size of buffer
    * flags -> see "More info" for additional info on flags

    * returns number of bytes sent 
    * More info: https://man7.org/linux/man-pages/man2/send.2.html
    */

    if ((send(client_socket, buffer, sizeof(buffer), 0)) == -1) {
        perror("send()");
        exit(7);
    }

    close(server_socket);
    close(client_socket);

    printf("Server successfully shut down.\n");
    exit(0);

}