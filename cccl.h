#ifndef __CCCL_H__
#define __CCCL_H__

#include <stddef.h>
#include <stdio.h>


#define TOKENS_LIMIT 16384

struct cccl_File
{
    char *buffer;
    size_t size;
};

enum cccl_TokenType
{
    cccl_Token_IDENTIFIER,
    cccl_Token_COMMAND,
    cccl_Token_COMMANDWITHARG,
    cccl_Token_BLOCKSTART,
    cccl_Token_BLOCKEND,
};

struct cccl_Token
{
    enum cccl_TokenType type;
    char value;
};

enum cccl_NodeType
{
    cccl_Node_CODE = 0,

    cccl_Node_PUSHZERO,
    cccl_Node_INCREMENT,
    cccl_Node_DECREMENT,
    cccl_Node_ADD,
    cccl_Node_SUBTRACT,
    cccl_Node_REVERSE,
    cccl_Node_ASSIGN,
    cccl_Node_DELETE,
    cccl_Node_PUSHVAR,
    cccl_Node_ASSIGNLOCAL,
    cccl_Node_OUTPUT,
    cccl_Node_INPUT,
    cccl_Node_CALL,
    cccl_Node_END,
    cccl_Node_CONTINUE,

    cccl_Node_PROCEDURE,
    cccl_Node_INFINITE,
    cccl_Node_REPEAT,
    cccl_Node_CONDITIONAL,
};

struct cccl_Node
{
    enum cccl_NodeType type;
    struct cccl_Node **in;
    size_t in_length;
    char value;
};

enum cccl_ExecutorStatus
{
    cccl_Executor_OK = 0,
    cccl_Executor_ERROR,
    cccl_Executor_CONTINUE,
    cccl_Executor_END,
};

struct cccl_Variables
{
    short buffer[52];
    char used[52];
};

struct cccl_Function
{
    struct cccl_Node **body;
    size_t length;
};

struct cccl_Stack
{
    short *buffer;
    size_t length;
    size_t allocated;
};

extern int verbose;
extern int interactive;
extern int dump;

int cccl_allocfile(const char *path, struct cccl_File *file);

void cccl(struct cccl_File file);
size_t cccl_tokenize(const char *code, size_t size, struct cccl_Token tokens[], size_t tokens_length);
struct cccl_Node *cccl_parse(struct cccl_Token tokens[], size_t tokens_length, enum cccl_NodeType type, char value);
enum cccl_ExecutorStatus cccl_execute(struct cccl_Node *code, struct cccl_Variables *scope, size_t depth);
void cccl_dump(FILE *f);

const char *strtoken(enum cccl_TokenType type);
const char *strnode(enum cccl_NodeType type);
const char *strstatus(enum cccl_ExecutorStatus status);

#endif /* __CCCL_H__ */
