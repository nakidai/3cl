#include "cccl.h"

#include <stddef.h>


const char *cccl_strtoken(enum cccl_TokenType type)
{
    switch (type)
    {
    break; case cccl_Token_IDENTIFIER: return "IDENTIFIER";
    break; case cccl_Token_COMMAND: return "COMMAND";
    break; case cccl_Token_COMMANDWITHARG: return "COMMANDWITHARG";
    break; case cccl_Token_BLOCKSTART: return "BLOCKSTART";
    break; case cccl_Token_BLOCKEND: return "BLOCKEND";
    }
    return NULL;
}

const char *cccl_strnode(enum cccl_NodeType type)
{
    switch (type)
    {
    break; case cccl_Node_CODE: return "CODE";
    break; case cccl_Node_PUSHZERO: return "PUSHZERO";
    break; case cccl_Node_INCREMENT: return "INCREMENT";
    break; case cccl_Node_DECREMENT: return "DECREMENT";
    break; case cccl_Node_ADD: return "ADD";
    break; case cccl_Node_SUBTRACT: return "SUBTRACT";
    break; case cccl_Node_REVERSE: return "REVERSE";
    break; case cccl_Node_ASSIGN: return "ASSIGN";
    break; case cccl_Node_DELETE: return "DELETE";
    break; case cccl_Node_PUSHVAR: return "PUSHVAR";
    break; case cccl_Node_ASSIGNLOCAL: return "ASSIGNLOCAL";
    break; case cccl_Node_OUTPUT: return "OUTPUT";
    break; case cccl_Node_INPUT: return "INPUT";
    break; case cccl_Node_CALL: return "CALL";
    break; case cccl_Node_END: return "END";
    break; case cccl_Node_CONTINUE: return "CONTINUE";
    break; case cccl_Node_PROCEDURE: return "PROCEDURE";
    break; case cccl_Node_INFINITE: return "INFINITE";
    break; case cccl_Node_REPEAT: return "REPEAT";
    break; case cccl_Node_CONDITIONAL: return "CONDITIONAL";
    }
    return NULL;
}

const char *cccl_strstatus(enum cccl_ExecutorStatus status)
{
    switch (status)
    {
    break; case cccl_Executor_OK: return "OK";
    break; case cccl_Executor_ERROR: return "ERROR";
    break; case cccl_Executor_CONTINUE: return "CONTINUE";
    break; case cccl_Executor_END: return "END";
    }
    return NULL;
}
