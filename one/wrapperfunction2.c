#include<stdio.h>
#include <stdlib.h>
#include <sys/socket.h> 
#include <netinet/in.h> // Required for IPPROTO_TCP

//wrapper function for socket 
int Socket(int family, int type, int protocol){
    int n;
    if(n = socket(family, type, protocol) < 0){
        perror("Socket Error");
        exit(EXIT_FAILURE);
    }
    return n;
}

int main(){
    //Using common values for IPv4 TCP socket
    int sockfd = Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    if(sockfd > -1)
        printf("Socket created successfully: %d\n", sockfd);
    
    return 0;
}