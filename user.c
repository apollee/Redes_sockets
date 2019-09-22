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
int parse_input_action();
void input_action();

/*------------------------*/


int main(int argc, char *argv[]) {
    input_command(argc, argv);
    parse_input_action();
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

int parse_input_action() {
    int numTokens = 0;
    char *saveTokens[7];
    char input[50];
    
    if(fgets(input, 50, stdin) == NULL){
        return -1;
    }    
    
    input[strcspn(input, "\n")] = 0; /*remove the \n added by fgets*/
    char *token = strtok(input, " ");

    while(token != NULL) {
        saveTokens[numTokens] = token;
        numTokens++;
        token = strtok(NULL, " ");
    }
    input_action(numTokens, saveTokens);
}

void input_action(int numTokens, char** saveTokens) {
    int i;
    for(i = 0; i < numTokens; i++){
        printf("%s\n", saveTokens[i]);
    }
}