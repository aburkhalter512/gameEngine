#pragma once

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#define __TO_STRING(x) #x
#define _TO_STRING(x) __TO_STRING(x)

extern FILE* UNIT_TEST_OUT;
extern FILE* UNIT_TEST_ERR;

struct __unitTestResult;
typedef struct __unitTestResult _unitTestResult;

_unitTestResult* _create_unitTestResult(bool didPass, const char* header, const char* msg);
void _free_unitTestResult(_unitTestResult* result);
bool _didPass_unitTestResult(_unitTestResult* result);
const char* _getHeader_unitTestResult(_unitTestResult* result);
const char* _getMsg_unitTestResult(_unitTestResult* result);

void _passTest();
void _failTest();
void _addFailureMessage(const char* str);
void writeUnitTestReport();

#define INDENT(file) fprintf(file, "    ")
#define SEPARATOR_STR "--------------------------------------------------------------------------------\n"
#define SEPARATOR(file) fprintf(file, SEPARATOR_STR)

#define TEST_SUCCESS_MSG "Pass\n"

#define TEST_SYMBOL_PREFIX_IMPL _UNIT_TEST_
#define TEST_SYMBOL_PREFIX_MAIN UNIT_TEST_

#define TEST_SYMBOL_NAME(prefix, name) prefix ## name
#define TEST_SYMBOL(name) void TEST_SYMBOL_NAME(TEST_SYMBOL_PREFIX_MAIN, name) ()
#define TEST_SYMBOL_IMPL(name) _unitTestResult* TEST_SYMBOL_NAME(TEST_SYMBOL_PREFIX_IMPL, name) ()

#define PROTOTYPE_TEST(name) TEST_SYMBOL(name);\
TEST_SYMBOL_IMPL(name)

#define IMPLEMENT_TEST(name) TEST_SYMBOL(name)\
{\
    fprintf(UNIT_TEST_OUT, #name  "\n");\
    _unitTestResult* result = TEST_SYMBOL_NAME(TEST_SYMBOL_PREFIX_IMPL, name)();\
    if (!_didPass_unitTestResult(result))\
    {\
        INDENT(UNIT_TEST_OUT);\
        fprintf(UNIT_TEST_OUT, "Fail: %s", _getHeader_unitTestResult(result));\
        fprintf(UNIT_TEST_OUT, "%s", _getMsg_unitTestResult(result));\
        fprintf(UNIT_TEST_OUT, "%s", "\n");\
        _failTest();\
        _addFailureMessage(#name  "\n");\
        _addFailureMessage("    Fail: ");\
        _addFailureMessage(_getHeader_unitTestResult(result));\
        _addFailureMessage(_getMsg_unitTestResult(result));\
        _addFailureMessage("\n");\
    }\
    else\
    {\
        INDENT(UNIT_TEST_OUT);\
        fprintf(UNIT_TEST_OUT, "%s", _getHeader_unitTestResult(result));\
        fprintf(UNIT_TEST_OUT, "%s", _getMsg_unitTestResult(result));\
        _passTest();\
    }\
    fflush(UNIT_TEST_OUT);\
    _free_unitTestResult(result);\
}\
TEST_SYMBOL_IMPL(name)

#define DEFINE_TEST(name) TEST_SYMBOL_IMPL(name);\
IMPLEMENT_TEST(name)

#define RUN_TEST(name) TEST_SYMBOL_NAME(TEST_SYMBOL_PREFIX_MAIN, name)();

#define _FAIL_TEST(file, line, msg) return _create_unitTestResult(false, file ": " _TO_STRING(line) ": ", msg)
#define FAIL_TEST(msg) _FAIL_TEST(__FILE__, __LINE__, msg)
#define PASS_TEST() return _create_unitTestResult(true, "", TEST_SUCCESS_MSG)
