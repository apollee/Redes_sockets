#ifndef VECTOR_H
#define VECTOR_H 1

#define TRUE 1
#define FALSE 0

typedef struct node* Link;
typedef struct user User;

int searchID(Link head, int id);
int searchIP(Link head, char[] ip);
void printVector(Link head);
Link insertUser(Link head, int id, char[] ip);
Link newLink(Link head, int id, char[] ip);

#endif