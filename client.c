
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

// see "server.c" for explanations excluded from client.c to prevent redundancy

int main(int argc, char *argv[]) {

    unsigned short port;
    char buffer[16];
    struct hostent *host_name; 
    struct sockaddr_in server_address; 
    int client_socket;

    if (argc != 3) { // Requires host name + port args
        fprintf(stderr, "Error: requires 2 args -- port number and host name");
        exit(1);
    }

    port = (unsigned short) atoi(argv[1]);

    /*gethostbyname(const char *name)
    * returns struct of type hostent for given name
    * name is either a hostname or ipv4 address in standard dot notation
    */

    host_name = gethostbyname(argv[2]);

    if (host_name == (struct hostent *) 0) {
        fprintf(stderr, "Error: gethostbyname() failed\n");
        exit(2);
    }

    strcpy(buffer, "TEST MESSAGE");

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = *((unsigned long *) host_name->h_addr);

    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket()");
        exit(3);
    }

    if ((connect(client_socket, (struct sockaddr *) &server_address, sizeof(server_address))) < 0) {
        perror("connect()");
        exit(4);
    }

    if (send(client_socket, buffer, sizeof(buffer), 0) < 0) {
        perror("send()");
        exit(5);
    }

    if (recv(client_socket, buffer, sizeof(buffer), 0) < 0) {
        perror("recv()");
        exit(6);
    }

    close(client_socket);
    printf("Client successfully shut down\n");
    exit(0);
}

