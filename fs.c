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

/*------------------------*/


int main(int argc, char *argv[]) {
    int port = input_command(argc, argv);
    printf("%d\n", port);
}

int input_command(int argc, char *argv[]) {
    
    if(argc == 1) {
        return 58041;
    }
    else if(argc == 3 && (strcmp(argv[1],"-p") == 0)) {
        return atoi(argv[2]);
    }
    else{
        printf("Invalid syntax.\n");
        return -1;
    }
}
