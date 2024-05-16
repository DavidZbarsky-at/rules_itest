#include <dlfcn.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h> // For exit, hax

// Function pointer for the original socket function
int (*original_socket)(int domain, int type, int protocol);

// Wrapper function for the socket() function
int socket(int domain, int type, int protocol) {
    // This never prints?
    exit(-1);
    printf("LOADING SOCKET");
    // Load the original socket function
    original_socket = dlsym(RTLD_NEXT, "socket");

    // Call the original socket function to create the socket
    int sockfd = original_socket(domain, type, protocol);

    if (sockfd != -1) {
        // Set the SO_REUSEPORT option on the socket
        int optval = 1;
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
    }

    return sockfd;
}
