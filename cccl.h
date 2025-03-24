#ifndef __CCCL_H__
#define __CCCL_H__

#include <stddef.h>


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
    cccl_Node_CODE,

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

int cccl_allocfile(const char *path, struct cccl_File *file);

void cccl(struct cccl_File file);
size_t tokenize(const char *code, size_t size, struct cccl_Token tokens[], size_t tokens_length);

#endif /* __CCCL_H__ */
