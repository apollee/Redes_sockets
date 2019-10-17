#ifndef DIRECTORY_STRUCTURE_FS_H
#define DIRECTORY_STRUCTURE_FS_H 1

#define FALSE 0
#define TRUE 1

int check_directory_existence(char *dirname);
int check_topic_existence(char* dirname);
void create_directory(char* dirname);
void create_topic_directory(char *dirname, char *userID);
char* topicList(char* numTopic);
char* questionList(char* currTopic,char* numDirec);
char* topicID(char* dirname);
char* questionID(char* currTopic, char* dirname);
char* number_of_topics();
char* number_of_answers(char* topic, char* question);
char* number_of_questions();
int check_max_directory_size(char* dirname);
char* numberOfdirectories(char* dirname);
char* numberOfAnswers(char* currTopic, char* currQuestion);
int checkExistenceofTopic(char* dirname);
void createQuestion(char* pathTopic, char** saveTokens);
void writeFileData(char** saveTokens, char* buffer);
void writeFileImg(char ** saveTokens, char* message, char* ext, long int n);
char* createAnswer(char** saveTokens);
char* questionTextSize(char* currTopic, char* dirname, char* ext);
void writeFileDataANS(char ** saveTokens, char* message, char* buffer, char* number);
void writeFileImgANS(char ** saveTokens, char* message, char* ext, long int n, char* number);
char** checkQuestionImage(char *currTopic, char* dirname);
char** checkAnswerImage(char *currTopic, char* dirname, char* numb);
char* answerID(char* currTopic, char* dirname, char* numb);
char* questionAnswerSize(char* currTopic, char* dirname, char*numb, char* ext);

//User functions
// int getTopic_by_number(int topic_number);
// int checkExistenceofTopic(char* dirname);

#endif