#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "msg_prop_reader.h"


#ifdef UNIT_TESTING
#define static
#endif

/* static method prototyles */
static void lower(char *msgId);
static struct message createEmptyMsg(void);
static FILE* openMsgPropFile (void);
static MessageNode* createMsgNodeList (FILE *fd);
static MessageNode* appendMsg (MessageNode *msgNode, Message msg);
static Message parseMsgProp(char *msgPropLine);

static void
lower (char *msgId)
{
    while (*msgId) {
        if (*msgId>='A' && *msgId<='F')
            *msgId -= UP_LOW_DIFF;
        msgId++;
    }
}

static Message
createEmptyMsg ()
{
    Message msg;
    msg.id[0] = '\0';
    msg.name[0] = '\0';
    return msg;
}

static FILE*
openMsgPropFile ()
{
    FILE *fd = NULL;
    fd = fopen(MSG_PROP_FILE, "r");
    if (fd == NULL)
        perror("Error opening msg prop file");
    return fd;
}

static MessageNode*
createMsgNodeList (FILE *msgPropFd)
{
    MessageNode *head;

    if (!msgPropFd)
        return (MessageNode *)NULL;

    head = (MessageNode *)malloc(sizeof(MessageNode));
    if (head != NULL) {
        char line[MAX_LINE_SZ];
        MessageNode *tail = head;
        while (fgets(line, MAX_LINE_SZ, msgPropFd) != NULL)
            tail = appendMsg(tail, parseMsgProp(line));
    }
    return head;
}

/*
 * append msg to message node listm, old tail will be updated,
 * pointer to new tail will be returned.
 * Note: msgid stored in lower case
 */
static MessageNode*
appendMsg (MessageNode *tail, Message msg)
{
    MessageNode *newNode;

    if (tail == NULL)
        return NULL;
    if (strlen(msg.id)==0 || strlen(msg.name)==0)
        return tail;

    newNode = (MessageNode *)malloc(sizeof(MessageNode));
    if (newNode != NULL) {
        strcpy(newNode->msg.name, msg.name);
        strcpy(newNode->msg.id, msg.id);
        lower(newNode->msg.id);
        newNode->next = NULL;
        tail->next = newNode;
    }
    return newNode;
}

static Message
parseMsgProp (char *line)
{
    Message msg = createEmptyMsg();
    char *tmp = (char *)malloc(strlen(line)+1);
    strcpy(tmp, line);

    if (line != NULL) {
        char *p = strchr(tmp, MSG_PROP_SEP);
        if (p) {
            *p = '\0';
            strcpy(msg.id, tmp);
            strcpy(msg.name, p+1);
        }else {
            perror("Read Msg Prop failed.");
        }
    }
    return msg;
}

/*
 * get msg properties from local file
 * head of linked list containing all the msg properties is returned to caller
 */
MessageNode*
getMsgProps ()
{
    MessageNode *listHead = NULL;
    FILE *fd = openMsgPropFile();
    if (fd != NULL) {
        listHead = createMsgNodeList(fd);
        fclose(fd);
    }
    return listHead;
}


#ifndef UNIT_TESTING
int main ()
{
    MessageNode *head = getMsgProps((char* )MSG_PROP_FILE);
    MessageNode *currNode;
    for (currNode=head; currNode; currNode=currNode->next)
        printf("msgId:%s, msgName:%s\n", currNode->msg.id, currNode->msg.name);
  return 0;
}
#endif
