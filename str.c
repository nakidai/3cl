#include "cccl.h"

#include <stddef.h>


const char *cccl_strtoken(enum cccl_TokenType type)
{
    switch (type)
    {
    case cccl_Token_IDENTIFIER: return "IDENTIFIER";
    case cccl_Token_COMMAND: return "COMMAND";
    case cccl_Token_COMMANDWITHARG: return "COMMANDWITHARG";
    case cccl_Token_BLOCKSTART: return "BLOCKSTART";
    case cccl_Token_BLOCKEND: return "BLOCKEND";
    }
    return NULL;
}

const char *cccl_strnode(enum cccl_NodeType type)
{
    switch (type)
    {
    case cccl_Node_CODE: return "CODE";
    case cccl_Node_PUSHZERO: return "PUSHZERO";
    case cccl_Node_INCREMENT: return "INCREMENT";
    case cccl_Node_DECREMENT: return "DECREMENT";
    case cccl_Node_ADD: return "ADD";
    case cccl_Node_SUBTRACT: return "SUBTRACT";
    case cccl_Node_REVERSE: return "REVERSE";
    case cccl_Node_ASSIGN: return "ASSIGN";
    case cccl_Node_DELETE: return "DELETE";
    case cccl_Node_PUSHVAR: return "PUSHVAR";
    case cccl_Node_ASSIGNLOCAL: return "ASSIGNLOCAL";
    case cccl_Node_OUTPUT: return "OUTPUT";
    case cccl_Node_INPUT: return "INPUT";
    case cccl_Node_CALL: return "CALL";
    case cccl_Node_END: return "END";
    case cccl_Node_CONTINUE: return "CONTINUE";
    case cccl_Node_PROCEDURE: return "PROCEDURE";
    case cccl_Node_INFINITE: return "INFINITE";
    case cccl_Node_REPEAT: return "REPEAT";
    case cccl_Node_CONDITIONAL: return "CONDITIONAL";
    }
    return NULL;
}

const char *cccl_strstatus(enum cccl_ExecutorStatus status)
{
    switch (status)
    {
    case cccl_Executor_OK: return "OK";
    case cccl_Executor_ERROR: return "ERROR";
    case cccl_Executor_CONTINUE: return "CONTINUE";
    case cccl_Executor_END: return "END";
    }
    return NULL;
}
