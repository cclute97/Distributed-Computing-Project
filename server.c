#include <stdio.h>
#include <socket.h>

// Most Comments are written for my own learning/troubleshooting purposes

int main(int argc, char *argv[]) {

    unsigned short port; // Port Number
    int name_length; // length of client name 
    int buffer[16]; // Buffer for sending/receiving data over stream -- current size chosen arbitrarily
    int server_socket; // socket descriptor
    int client_socket; // socket descriptor
    struct sockaddr_in server_info; // struct defined in socket.h for use with socket API functions 
    struct sockaddr_in client_info; 

    if (argc != 2) {
        fprintf(stderr, "Error: requires 1 arg (port number)"); // stderr is the non-buffered output stream used for errors
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
    * tcperror(char *s) prints s followed by the error message produced by socket() if -1 
    * More info: https://man7.org/linux/man-pages/man2/socket.2.html
    */

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
        tcperror("Socket()");
        exit(2);
    }

    // Assign data to our server struct

    server_info.sin_family = AF_INET;
    server_info.sin_port = htons(port); // converts unsigned short to TCP/IP network byte order, which is big-endian 
    server_info.sin_addr.s_addr = INADDR_ANY; // INADDR_ANY = 0.0.0.0 --  means any address for socket binding

    /* Socket Binding and Error Handling
    * bind() takes 3 args -- int sockfd, const struct sockaddr *addr, socklen_t addrlen

    * - sockfd -> descriptor for socket 
    * - addr -> pointer to server address
    * - addrlen -> size in bytes of address structure at addr

    * bind() binds a socket to our server address
    * On success, 0 is returned 
    * More info: https://man7.org/linux/man-pages/man2/bind.2.html
    */

    if ((bind(server_socket, (struct sockaddr *) &server_info, sizeof(server_info)) < 0)) {
        tcperror("Bind()")
        exit(3);
    }

    /* Listen for Connections
    * listen() takes 2 args -- int sockfd, int backlog

    * - sockfd -> descriptor of socket 
    * - backlog -> defines max length of queue for pending connections for sockfd

    * listen() marks the socket identified with sockfd as passive, meaning it will be used to accept incoming connections with accept()
    * On success, 0 is returned
    * More info: https://man7.org/linux/man-pages/man2/listen.2.html
    */

    if (listen(server_info, 1) < 0) {
        tcperror("listen()");
        exit(4);
    }

    /* Accept Connection
    * accept() takes 3 args -- int sockfd, struct sockaddr *_Nullable restrict addr, socklen_t *_Nullable restrict addrlen

    * sockfd -> socket descriptor 
    * addr -> pointer to sockaddr struct.  
    * addrlen -> size in bytes of struct pointed to by addr

    * accept() extracts the first connection request from queue of pending connections for the listening socket, sockfd, 
    * creates a new connected socket, and returns the new socket descriptor
    * More info: https://man7.org/linux/man-pages/man2/accept.2.html
    */

    name_length = sizeof(client_info);

    if ((client_socket = accept(server_socket, (struct sockaddr *) &client_info, &name_length)) == -1) {
        tcperror("accept()");
        exit(5);
    }

    /* Recieve Info from Client
    * recv() takes 4 args -- int sockfd, void buf[.len], size_t len, int flags

    * sockfd -> socket descriptor 


    * More info: https://man7.org/linux/man-pages/man2/recv.2.html
    */

}