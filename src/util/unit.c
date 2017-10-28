#include "util/unit.h"

#include "util/string.h"

#include <stdlib.h>

struct __unitTestResult
{
    bool didPass;
    const char* header;
    const char* msg;
};

_unitTestResult* _create_unitTestResult(bool didPass, const char* header, const char* msg)
{
    _unitTestResult* result = malloc(sizeof(_unitTestResult));
    result->didPass = didPass;
    result->header = copy_str(header);
    result->msg = copy_str(msg);

    return result;
}

void _free_unitTestResult(_unitTestResult* result)
{
    free((void*) result->header);
    free((void*) result->msg);
    free(result);
}

bool _didPass_unitTestResult(_unitTestResult* result) { return result->didPass; }
const char* _getHeader_unitTestResult(_unitTestResult* result) { return result->header; }
const char* _getMsg_unitTestResult(_unitTestResult* result) { return result->msg; }

const int PASS_TOTAL = 0;
const int FAIL_TOTAL = 1;
static int totals[] = { 0, 0 };

void _passTest()
{
    totals[PASS_TOTAL]++;
}

void _failTest()
{
    totals[FAIL_TOTAL]++;
}

struct failureMessage
{
    char* message;
    struct failureMessage* next;
};

static struct failureMessage* failureHead = NULL;
static struct failureMessage* failureTail = NULL;

void _addFailureMessage(const char* str)
{
    if (!failureHead)
    {
        failureHead = malloc(sizeof(struct failureMessage));
        failureTail = failureHead;
    }
    else
    {
        failureTail->next = malloc(sizeof(struct failureMessage));
        failureTail = failureTail->next;
    }

    failureTail->next = NULL;

    failureTail->message = malloc(sizeof(const char) * (strlen(str) + 1));
    strcpy(failureTail->message, str);
}

static void _writeFailureMessages()
{
    if (!failureHead)
    {
        return;
    }

    struct failureMessage* current = failureHead;
    struct failureMessage* next;
    while (current)
    {
        next = current->next;

        fprintf(UNIT_TEST_OUT, "%s", current->message);

        free(current->message);
        free(current);

        current = next;
    }

    failureHead = NULL;
    failureTail = NULL;
}

void writeUnitTestReport()
{
    if (totals[FAIL_TOTAL] == 0)
    {
        fprintf(UNIT_TEST_OUT, "All %d tests passed\n", totals[PASS_TOTAL]);
    }
    else
    {
        SEPARATOR(UNIT_TEST_OUT);

        fprintf(UNIT_TEST_OUT, "Failures\n");
        _writeFailureMessages();

        SEPARATOR(UNIT_TEST_OUT);

        fprintf(UNIT_TEST_OUT, "%d tests passed\n", totals[PASS_TOTAL]);
        fprintf(UNIT_TEST_OUT, "%d tests failed\n", totals[FAIL_TOTAL]);

        SEPARATOR(UNIT_TEST_OUT);
    }
}
