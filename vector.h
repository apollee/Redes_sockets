#ifndef VECTOR_H
#define VECTOR_H 1

#define TRUE 1
#define FALSE 0

typedef struct node* Link;
typedef struct user User;

int searchID(Link head, int id);
<<<<<<< HEAD
int searchIP(Link head, char* ip);
void printVector(Link head);
Link insertUser(Link head, int id, const char* ip);
Link newLink(Link head, int id, const char* ip);
=======
int searchIP(Link head, char[] ip);
void printVector(Link head);
Link insertUser(Link head, int id, char[] ip);
Link newLink(Link head, int id, char[] ip);
>>>>>>> d01007aa409b5cec2df76ba2e3edd88b50fc86d6

#endif