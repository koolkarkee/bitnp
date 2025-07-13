#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

//create a socket 
int main(){
    int sockfd;

    //family, type, protocol
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    //err condition
    if(sockfd == -1){
        printf("\nError while socket creation."); 
        exit(0);
    }

    printf("\nSocket created successfully");  

    struct sockaddr_in server_addr;
    //clear 
    memset(&server_addr, 0, sizeof(server_addr));

    //fill in the values
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5002);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    //pass the sas in connect function 
    int connected = connect(sockfd, (struct sockaddr*) &server_addr, sizeof(server_addr));
    if(connected == -1){
        perror("\nError while connecting to a server");
        exit(0);
    }

    printf("\nConnected to a server 127.0.0.1 at port 5002");
    return 0;
}