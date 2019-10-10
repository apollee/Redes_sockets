#ifndef DIRECTORY_STRUCTURE_H
#define DIRECTORY_STRUCTURE_H 1

#define FALSE 0
#define TRUE 1

int check_directory_existence(char *dirname);
int check_topic_existence(char* dirname);
void create_directory(char* dirname);
void create_topic_directory(char *dirname, char *userID);
char* topicList();
char* questionList();
char* topicID(char* dirname);
char* questionID(char* currTopic, char* dirname);
char* number_of_topics();
char* number_of_questions();
int getTopic_by_number(int topic_number);
int checkExistenceofTopic(char* dirname);
int check_directory_size();
char* numberOfAnswers(char* currTopic, char* currQuestion);

#endif