#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define DEFAULT_PORT "58041"
#define FLAG "flag"

/*------------------------*/

int input_command(int argc, char *argv[], char* port);

/*------------------------*/


int main(int argc, char *argv[]) {
    char port[6];

    strcpy(port, FLAG);
    input_command(argc, argv, port);
    
    if(!strcmp(port, FLAG)){
        strcpy(port, DEFAULT_PORT); //set the default port
    }
    printf("port: %s\n", port);
}

int input_command(int argc, char *argv[], char* port) {

    if(argc == 1) {
        return 0;
    }
    else if(argc == 3 && (strcmp(argv[1],"-p") == 0)) {
        strcpy(port, argv[2]);
    }
    else{
        printf("Invalid syntax.\n");
        return -1;
    }
}
