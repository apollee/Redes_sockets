#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include<netdb.h> 
#include <string.h>
#include <unistd.h>
#include <stdio.h>

int main(void){
    int fd;
    struct sockaddr_in addr;
    socklen_t addrlen;
    ssize_t n;
    char buffer[128];

    //addr.sin_port = 58001;

    addrlen = sizeof(addr);
    while (1){
        n = recvfrom(fd, buffer, 128, 0, (struct sockaddr*)&addr, &addrlen);

        //printf("port: %d\n", addr.sin_port);
        
        if(n == -1) //error
            exit(1);
        
        write(1, "echo: ", 6); //stdout
        write(1,buffer, n);
        close(fd);
        printf("\n");
    }

    exit(0);
}