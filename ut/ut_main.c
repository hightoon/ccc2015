#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <string.h>

#include "msg_prop_reader.h"

extern void lower(char *msgId);
extern Message createEmptyMsg(void);
extern FILE* openMsgPropFile(void);
extern MessageNode* appendMsg (MessageNode *msgNode, Message msg);
extern Message parseMsgProp(char *msgPropLine);
extern MessageNode* createMsgNodeList (FILE *fd);

/* functions of rtm_log_parser.c */
extern bool startsWith(char *str, char *pre);

/*
 *    === test cases for msg_prop_reader.c ===
 */

/* function: lower
 * desc: msg id is digital only, for example 0000, 111, etc...
 * expected: msgid not changed
 */
static void rlp_lower_test_all_digitals(void **state)
{
    char msgid[5] = {'0', '1', '2', '3', '\0'};
    lower(msgid);
    assert_string_equal(msgid, "0123");
}

/* function: lower
 * desc: msg id is digit and upcase char mixed, for example 00a0, 111b, etc...
 * expected: msgid not changed
 */
static void rlp_lower_test_digit_up_mixed(void **state)
{
    char msgid[5] = {'0', 'A', '2', 'F', '\0'};
    lower(msgid);
    assert_string_equal(msgid, "0a2f");
}

/* function: lower
 * desc: msg id is digit and upcase, lowcase chars mixed, for example 00aB, 1D1b, etc...
 * expected: msgid updated with upper case replaced by its lower couterpart
 */
static void rlp_lower_test_digit_up_low_mixed(void **state)
{
    char msgid[5] = {'0', 'c', '2', 'D', '\0'};
    lower(msgid);
    assert_string_equal(msgid, "0c2d");
}


/* function: createEmptyMsg
 * desc: msgid and msgname should be 0-length string
 * expected: msg return with both id and name 0-len
 */
static void rlp_createEmptyMsg_test(void **state)
{
    Message m;
    m = createEmptyMsg();
    assert_int_equal(strlen(m.id)+strlen(m.name), 0);
}

/* function: openMsgPropFile
 * desc: if file path not existing, the function behavior
 * expected: return NULL
 */
static void rlp_openMsgPropFile_file_not_exist(void **state)
{
    FILE* fd;
    fd = openMsgPropFile();
    assert_int_equal(fd, NULL);
}

/* function: appendMsg
 * desc: if tail is null, just return null
 * expected: return NULL
 */
static void rlp_appendMsg_test_null_tail(void **state)
{
    Message m = {"0000", "dummy"};
    assert_int_equal(appendMsg(NULL, m), NULL);
}

/* function: appendMsg
 * desc: if msg is empty, just keep current (passed in) tail
 * expected: returned Node == tail passed in
 */
static void rlp_appendMsg_test_empty_msg(void **state)
{
    Message m = {"", ""};
    MessageNode mn;
    assert_int_equal(appendMsg(&mn, m), &mn);
}

/* function: appendMsg
 * desc: nornal case, new msg appended and new tail returned
 * expected: new node returned w/ new msg and as list tail
 */
static void rlp_appendMsg_test_normal(void **state)
{
    Message m = {"052a", "testmsg"};
    MessageNode tail = {{"01ab", "testmsg"}};
    tail.next = NULL;
    MessageNode *newTail;
    newTail = appendMsg(&tail, m);
    assert_string_equal(newTail->msg.id, "052a");
    assert_string_equal(newTail->msg.name, "testmsg");
    assert_int_equal(newTail->next, NULL);
    assert_int_equal(tail.next, newTail);
    assert_string_equal(tail.msg.id, "01ab");
    assert_string_equal(tail.msg.name, "testmsg");
}

/* function: parseMsgProp
 * desc: input empty line string, empty msg shall be returned
 * expected: return empty msg
 */
static void rlp_parseMsgProp_test_empty_line(void **state)
{
    Message m;
    m = parseMsgProp("");
    assert_int_equal(strlen(m.name)+strlen(m.id), 0);
}

/* function: parseMsgProp
 * desc: input invalid string w/0 '='
 * expected: return empty msg
 */
static void rlp_parseMsgProp_test_invalid_sep(void **state)
{
    Message m;
    m = parseMsgProp("1234:testmsg");
    assert_int_equal(strlen(m.name)+strlen(m.id), 0);
}

/* function: parseMsgProp
 * desc: input invalid line string with only msgid
 * expected: return empty name but with id
 */
static void rlp_parseMsgProp_test_empty_name(void **state)
{
    Message m;
    m = parseMsgProp("2222=");
    assert_int_equal(strlen(m.name), 0);
    assert_string_equal(m.id, "2222");
}

/* function: parseMsgProp
 * desc: input invalid line string with only msg name
 * expected: return empty id but with name
 */
static void rlp_parseMsgProp_test_empty_id(void **state)
{
    Message m;
    m = parseMsgProp("=testmsg");
    assert_int_equal(strlen(m.id), 0);
    assert_string_equal(m.name, "testmsg");
}

/* function: createMsgNodeList
 * desc: when passed fd is NULL
 * expected: do nth, return NULL
 */
static void rlp_createMsgNodeList_null_fd(void **state)
{
    MessageNode *mn = createMsgNodeList((FILE *)NULL);
    assert_int_equal(mn, NULL);
}


/*
 * === test cases for rtm_log_parser.c ===
 * (just put cases for different src files in same place)
 */

 /*
  * function: startsWith
  * desc: str starts with pre
  * expected: return true
  */
static void rlp_startsWith_test_match(void **state)
{
    bool ret;
    ret = startsWith("RTM hello, world", "RTM");
    assert_true(ret);
}

/*
 * function: startsWith
 * desc: pre longer than str
 * expected: return false
 */
static void rlp_startsWith_test_longer_pre(void **state)
{
   bool ret;
   ret = startsWith("RT", "RTM");
   assert_true(!ret);
}





/* ut main execution */
int main(void)
{
    const struct CMUnitTest tests[] = {
        /* test cases for msg_prop_reader.c */
        cmocka_unit_test(rlp_lower_test_all_digitals),
        cmocka_unit_test(rlp_lower_test_digit_up_mixed),
        cmocka_unit_test(rlp_lower_test_digit_up_low_mixed),
        cmocka_unit_test(rlp_createEmptyMsg_test),
        cmocka_unit_test(rlp_openMsgPropFile_file_not_exist),
        cmocka_unit_test(rlp_appendMsg_test_null_tail),
        cmocka_unit_test(rlp_appendMsg_test_empty_msg),
        cmocka_unit_test(rlp_appendMsg_test_normal),
        cmocka_unit_test(rlp_parseMsgProp_test_empty_line),
        cmocka_unit_test(rlp_parseMsgProp_test_empty_name),
        cmocka_unit_test(rlp_parseMsgProp_test_empty_id),
        cmocka_unit_test(rlp_parseMsgProp_test_invalid_sep),
        cmocka_unit_test(rlp_createMsgNodeList_null_fd),
        cmocka_unit_test(rlp_startsWith_test_match),
        cmocka_unit_test(rlp_startsWith_test_longer_pre),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
