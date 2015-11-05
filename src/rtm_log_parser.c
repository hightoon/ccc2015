#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#ifdef UNIT_TESTING
#define static
#endif

enum Pattern {
    msgPat1,
    msgPat2,
    timeStamp,
    opType,
    numOfPatterns
};

static FILE* openLogFile(char *filepath);
static bool startsWith(char *str, char *pre);
static enum Pattern getPattern(char *str);

static FILE*
openLogFile(char *filepath)
{
    FILE *fd;
    fd = fopen(filepath, 'r');
    if (fd==NULL)
        fprintf(stderr, "open log file %s failed\n", filepath);
    return fd;
}

static bool
startsWith(char *str, char *pre)
{
    size_t strLen = strlen(str);
    size_t preLen = strlen(pre);
    return preLen>strLen?false:strncmp(str, pre, preLen)==0;
}

static enum Pattern
getPattern(char *str)
{
    enum Pattern p = numOfPatterns;
    if (startsWith(str, "RTM"))
        p = timeStamp;
    else if (startsWith(str, "[0x"))
        p = msgPat1;
    else if (startsWith(str, "0x"))
        p = msgPat2;
    else if (startsWith(str, "RTM "))
        p = opType;
    return p;
}

static parseLine(char *line)
{

}
