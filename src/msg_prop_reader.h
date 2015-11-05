#ifndef RTM_LOG_PARSER_H
#define RTM_LOG_PARSER_H

#define MSG_PROP_FILE "messages.properties"
#define MAX_LINE_SZ 100
#define MAX_MSG_ID_SZ 10
#define MAX_MSG_NAME_SZ 100
#define MSG_PROP_SEP '='
#define UP_LOW_DIFF ('A'-'a')


/* data type definition */
struct message {
    char id[MAX_MSG_ID_SZ+1];
    char name[MAX_MSG_NAME_SZ+1];
};
typedef struct message Message;

struct msgNode {
    struct message msg;
    struct msgNode *next;
};
typedef struct msgNode MessageNode;


#endif /* #ifndef RTM_LOG_PARSER_H */
