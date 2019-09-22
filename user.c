#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

/*------------------------*/

int input_command();
int input_action();

/*------------------------*/


int main(int argc, char *argv[]) {
    input_command(argc, argv);
    input_action();
}

int input_command(int argc, char *argv[]) {
    
    if(argc == 2 && (strcmp(argv[1],"-n") == 0)) {
        printf("FSIP\n");
    }
    else if(argc == 2 && (strcmp(argv[1],"-p") == 0)) {
        printf("FSport\n");
    }
    else if(argc == 3 && (strcmp(argv[1],"-n") == 0)) {
        printf("FSIP  with ip\n");
    }
    else if(argc == 3 && (strcmp(argv[1],"-p") == 0)) {
        printf("FSport with port\n");
    }
    else{
        printf("Invalid syntax.\n");
        return -1;
    }
    return 0;
}