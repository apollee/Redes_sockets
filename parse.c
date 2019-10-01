#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include "parse.h"

/* =============================================================================
 * input_command_user - input user when starting the program
 * =============================================================================
 */

void input_command_user(int argc, char *argv[], char *port, char *ip) {
    strcpy(port, DEFAULT_PORT);
    strcpy(ip, FLAG);

    if(argc == 1){
        return;
    }
    else if(argc == 3 && !(strcmp(argv[1],"-n"))) {
        strcpy(ip,argv[2]);
    }
    else if(argc == 3 && !(strcmp(argv[1],"-p"))) {
        strcpy(port,argv[2]);
    }
    else if(argc == 5 && !(strcmp(argv[1],"-n")) && !(strcmp(argv[3],"-p"))) {
        strcpy(ip, argv[2]);
        strcpy(port,argv[4]);
    }
    else{
        fprintf(stderr, "Invalid syntax!\n");
        exit(-1);
    }
}

/* =============================================================================
 * input_command_server - input server when starting the program
 * =============================================================================
 */

int input_command_server(int argc, char *argv[], char* port) {
    strcpy(port, DEFAULT_PORT);

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

/* =============================================================================
 * parse_input_action - parsing the command to perform
 * =============================================================================
 */
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
    input_action(numTokens, saveTokens, input);
}

/* =============================================================================
 * input_action - executes the command if valid
 * =============================================================================
 */
void input_action(int numTokens, char** saveTokens, char* input) {
        
    if(!strcmp(saveTokens[0], "register") || !strcmp(saveTokens[0],"reg")) {
        printf("register or reg\n");
    }
    else if(!strcmp(saveTokens[0], "topic_propose") || !strcmp(saveTokens[0], "tp")){
        printf("topic propose or tp\n");
    }
    else if(!strcmp(saveTokens[0], "question_submit") || !strcmp(saveTokens[0], "qs")){
        printf("question submit or qs\n");
    }
    else if(!strcmp(saveTokens[0], "answer_submit") || !strcmp(saveTokens[0], "as")){
        printf("answer submit\n");
    }
    else if(!strcmp(saveTokens[0], "topic_select")){
        printf("topic select\n");    
    }
    else if(!strcmp(saveTokens[0], "ts")){
        printf("ts\n");
    }
    else if(!strcmp(saveTokens[0], "question_get")){
        printf("question get\n");
    }
    else if(!strcmp(saveTokens[0], "qg")){
        printf("qg\n");
    }
    else if(!strcmp(saveTokens[0],"topic_list") || !strcmp(saveTokens[0], "tl")) {
        printf("topic list or tl\n");
    }
    else if(!strcmp(saveTokens[0], "question_list") || !strcmp(saveTokens[0], "ql")){
        printf("question list or ql\n");
    }
    else if(!strcmp(saveTokens[0], "exit")){
        exit(0);
    }
    else{
        printf("Invalid syntax!"); 
    }
}
