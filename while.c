#include "while.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef PRINT_AST_INDENT_STEP
#define PRINT_AST_INDENT_STEP 4
#endif

/*----------------------------------------------------------------------------*/
// Function definitions
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
// Statements
/*----------------------------------------------------------------------------*/
Statement *BuildAssignment(char *variable, ArithmeticExpression *value)
{
    Statement *result = (Statement *)malloc(sizeof(Statement));
    if (NULL == result)
        exit(-1);

    result->Type = Assignment;
    result->Variable = variable;
    result->AssignmentValue = value;

    return result;
}

Statement *BuildSkip()
{
    Statement *result = (Statement *)malloc(sizeof(Statement));
    if (NULL == result)
        exit(-1);

    result->Type = Skip;

    return result;
}

Statement *BuildStatementSequence(Statement *first, Statement *second)
{
    Statement *result = (Statement *)malloc(sizeof(Statement));
    if (NULL == result)
        exit(-1);

    result->Type = Sequence;
    result->Left = first;
    result->Right = second;

    return result;
}

Statement *BuildConditional(Statement *ifTrue, Statement *ifFalse, BooleanExpression *condition)
{
    Statement *result = (Statement *)malloc(sizeof(Statement));
    if (NULL == result)
        exit(-1);

    result->Type = Conditional;
    result->Left = ifTrue;
    result->Right = ifFalse;
    result->Condition = condition;

    return result;
}

Statement *BuildLoop(Statement *body, BooleanExpression *condition)
{
    Statement *result = (Statement *)malloc(sizeof(Statement));
    if (NULL == result)
        exit(-1);

    result->Type = Loop;
    result->Left = body;
    result->Condition = condition;

    return result;
}

void DeleteStatement(Statement *statement)
{
    if (NULL == statement)
        return;

    switch (statement->Type)
    {
    case Assignment:
        DeleteArithmeticExpression(statement->AssignmentValue);
        if (NULL != statement->Variable)
            free(statement->Variable);
        break;

    case Skip:
        break;

    case Conditional:
        DeleteBooleanExpression(statement->Condition);
        DeleteStatement(statement->Left);
        DeleteStatement(statement->Right);
        break;

    case Loop:
        DeleteBooleanExpression(statement->Condition);
        DeleteStatement(statement->Left);
        break;

    case Sequence:
        DeleteStatement(statement->Left);
        DeleteStatement(statement->Right);
        break;
    }

    free(statement);
}

/*----------------------------------------------------------------------------*/
// Arithmetic expressions
/*----------------------------------------------------------------------------*/
ArithmeticExpression *BuildNumber(int number)
{
    ArithmeticExpression *result = (ArithmeticExpression *)malloc(sizeof(ArithmeticExpression));
    if (NULL == result)
        exit(-1);

    result->Type = Number;
    result->NumberLiteral = number;

    return result;
}

ArithmeticExpression *BuildVariable(char *variableName)
{
    ArithmeticExpression *result = (ArithmeticExpression *)malloc(sizeof(ArithmeticExpression));
    if (NULL == result)
        exit(-1);

    result->Type = VariableEval;
    result->Variable = variableName;

    return result;
}

static ArithmeticExpression *BuildInfixArithmeticOp(ArithmeticExpression *left, ArithmeticExpression *right, ArithmeticExpressionType type)
{
    ArithmeticExpression *result = (ArithmeticExpression *)malloc(sizeof(ArithmeticExpression));
    if (NULL == result)
        exit(-1);

    result->Type = type;
    result->Left = left;
    result->Right = right;

    return result;
}

ArithmeticExpression *BuildSum(ArithmeticExpression *left, ArithmeticExpression *right)
{
    return BuildInfixArithmeticOp(left, right, Sum);
}

ArithmeticExpression *BuildDifference(ArithmeticExpression *left, ArithmeticExpression *right)
{
    return BuildInfixArithmeticOp(left, right, Difference);
}

ArithmeticExpression *BuildProduct(ArithmeticExpression *left, ArithmeticExpression *right)
{
    return BuildInfixArithmeticOp(left, right, Product);
}

void DeleteArithmeticExpression(ArithmeticExpression *expression)
{
    if (NULL == expression)
        return;

    switch (expression->Type)
    {
    case Number:
        break;

    case VariableEval:
        free(expression->Variable);
        break;

    case Sum:
    case Difference:
    case Product:
        DeleteArithmeticExpression(expression->Left);
        DeleteArithmeticExpression(expression->Right);
        break;
    }

    free(expression);
}

/*----------------------------------------------------------------------------*/
// Boolean expressions
/*----------------------------------------------------------------------------*/

BooleanExpression *BuildBooleanLiteral(int booleanLiteral)
{
    BooleanExpression *result = (BooleanExpression *)malloc(sizeof(BooleanExpression));
    if (NULL == result)
        exit(-1);

    result->Type = BooleanLiteral;
    result->BooleanLiteral = booleanLiteral;

    return result;
}

static BooleanExpression *BuildArithmeticComparison(ArithmeticExpression *left, ArithmeticExpression* right, BooleanExpressionType type)
{
    BooleanExpression *result = (BooleanExpression *)malloc(sizeof(BooleanExpression));
    if (NULL == result)
        exit(-1);

    result->Type = type;
    result->ArithmeticLeft = left;
    result->ArithmeticRight = right;

    return result;
}

BooleanExpression *BuildEquals(ArithmeticExpression *left, ArithmeticExpression* right)
{
    return BuildArithmeticComparison(left, right, Equals);
}

BooleanExpression *BuildLessThanOrEqualTo(ArithmeticExpression *left, ArithmeticExpression* right)
{
    return BuildArithmeticComparison(left, right, LessThanOrEqualTo);
}

BooleanExpression *BuildNot(BooleanExpression *expression)
{
    BooleanExpression *result = (BooleanExpression *)malloc(sizeof(BooleanExpression));
    if (NULL == result)
        exit(-1);

    result->Type = Not;
    result->Left = expression;

    return result;
}

BooleanExpression *BuildAnd(BooleanExpression *left, BooleanExpression* right)
{
    BooleanExpression *result = (BooleanExpression *)malloc(sizeof(BooleanExpression));
    if (NULL == result)
        exit(-1);

    result->Type = And;
    result->Left = left;
    result->Right = right;

    return result;
}

void DeleteBooleanExpression(BooleanExpression *expression)
{
    if (NULL == expression)
        return;

    switch (expression->Type)
    {
    case BooleanLiteral:
        break;

    case Equals:
    case LessThanOrEqualTo:
        DeleteArithmeticExpression(expression->ArithmeticLeft);
        DeleteArithmeticExpression(expression->ArithmeticRight);
        break;

    case Not:
        DeleteBooleanExpression(expression->Left);
        break;
    case And:
        DeleteBooleanExpression(expression->Left);
        DeleteBooleanExpression(expression->Right);
        break;
    }

    free(expression);
}

/*----------------------------------------------------------------------------*/
// Helper functions
/*----------------------------------------------------------------------------*/

void ReadString(char* source, char** target)
{
    *target = (char*)malloc(strlen(source) + 1);

    strcpy(*target, source);
}

typedef struct VariableStruct Variable;

struct VariableStruct {
    char* Name;
    int Value;
    Variable* Next;
};

Variable* Memory;

Variable* CreateVariable(char* name)
{
    Variable* result = (Variable*)malloc(sizeof(Variable));
    if (NULL == result)
        exit(-1);

    result->Name = (char*)malloc(strlen(name) + 1);
    if (NULL == result->Name)
        exit(-1);

    strcpy(result->Name, name);
    result->Value = 0;
    result->Next = NULL;

    return result;
}

Variable* GetOrCreateVariable(char* name)
{
    if (NULL == Memory)
    {
        Memory = CreateVariable(name);

        return Memory;
    }
    else
    {
        Variable *result = Memory;
        while (NULL != result->Next)
        {
            if (0 == strcmp(result->Name, name))
                return result;
            result = result->Next;
        }
        if (0 == strcmp(result->Name, name))
            return result;

        result->Next = CreateVariable(name);
        return result->Next;
    }
}

int GetVariableValue(char* name)
{
    return GetOrCreateVariable(name)->Value;
}

void SetVariableValue(char* name, int value)
{
    GetOrCreateVariable(name)->Value = value;
}

void PrintState(FILE *stream)
{
    if (NULL == Memory)
        return;

    Variable *iterator = Memory;
    while (NULL != iterator)
    {
        fprintf(stream, "[ %s -> %d ]\n", iterator->Name, iterator->Value);
        iterator = iterator->Next;
    }
}

void PrintTree(FILE* stream, Statement* statement, int indent)
{
    if (NULL == statement)
        return;

    switch (statement->Type)
    {
    case Assignment:
        fprintf(stream, "%*sAssignment:\n", indent, "");
        fprintf(stream, "%*s%s\n", indent + PRINT_AST_INDENT_STEP * 2, "",
            statement->Variable);
        fprintf(stream, "%*s:=\n", indent + PRINT_AST_INDENT_STEP, "");
        PrintArithmeticExpression(stream, statement->AssignmentValue, indent + PRINT_AST_INDENT_STEP * 2);
        break;
    case Sequence:
        fprintf(stream, "%*sSequence:\n", indent, "");
        PrintTree(stream, statement->Left, indent + PRINT_AST_INDENT_STEP);
        PrintTree(stream, statement->Right, indent + PRINT_AST_INDENT_STEP);
        break;
    case Conditional:
        fprintf(stream, "%*sConditional statement:\n", indent, "");
        fprintf(stream, "%*sif:\n", indent + PRINT_AST_INDENT_STEP, "");
        PrintBooleanExpression(stream, statement->Condition, indent + PRINT_AST_INDENT_STEP);
        fprintf(stream, "%*sthen:\n", indent + PRINT_AST_INDENT_STEP, "");
        PrintTree(stream, statement->Left, indent + PRINT_AST_INDENT_STEP);
        fprintf(stream, "%*selse:\n", indent + PRINT_AST_INDENT_STEP, "");
        PrintTree(stream, statement->Right, indent + PRINT_AST_INDENT_STEP);
        break;
    case Loop:
        fprintf(stream, "%*sLoop:\n", indent, "");
        fprintf(stream, "%*swhile:\n", indent + PRINT_AST_INDENT_STEP, "");
        PrintBooleanExpression(stream, statement->Condition, indent + PRINT_AST_INDENT_STEP);
        fprintf(stream, "%*sdo:\n", indent + PRINT_AST_INDENT_STEP, "");
        PrintTree(stream, statement->Left, indent + PRINT_AST_INDENT_STEP);
        break;
    case Skip:
        fprintf(stream, "%*sSkip\n", indent, "");
        break;
    }
}

void PrintArithmeticExpression(FILE* stream, ArithmeticExpression *expression, int indent)
{
    if (NULL == expression)
        return;

    switch (expression->Type)
    {
    case Number:
        fprintf(stream, "%*s%d\n", indent, "", expression->NumberLiteral);
        break;
    case VariableEval:
        fprintf(stream, "%*s%s\n", indent, "", expression->Variable);
        break;
    case Sum:
        PrintArithmeticExpression(stream, expression->Left, indent + PRINT_AST_INDENT_STEP);
        fprintf(stream, "%*s+\n", indent, "");
        PrintArithmeticExpression(stream, expression->Right, indent + PRINT_AST_INDENT_STEP);
        break;
    case Difference:
        PrintArithmeticExpression(stream, expression->Left, indent + PRINT_AST_INDENT_STEP);
        fprintf(stream, "%*s-\n", indent, "");
        PrintArithmeticExpression(stream, expression->Right, indent + PRINT_AST_INDENT_STEP);
        break;
    case Product:
        PrintArithmeticExpression(stream, expression->Left, indent + PRINT_AST_INDENT_STEP);
        fprintf(stream, "%*s*\n", indent, "");
        PrintArithmeticExpression(stream, expression->Right, indent + PRINT_AST_INDENT_STEP);
        break;
    }
}

void PrintBooleanExpression(FILE* stream, BooleanExpression *expression, int indent)
{
    if (NULL == expression)
        return;

    switch (expression->Type)
    {
    case BooleanLiteral:
        fprintf(stream, "%*s%s\n", indent, "", expression->BooleanLiteral ? "TRUE" : "FALSE");
        break;
    case Equals:
        PrintArithmeticExpression(stream, expression->ArithmeticLeft, indent + PRINT_AST_INDENT_STEP);
        fprintf(stream, "%*s==\n", indent, "");
        PrintArithmeticExpression(stream, expression->ArithmeticRight, indent + PRINT_AST_INDENT_STEP);
        break;
    case LessThanOrEqualTo:
        PrintArithmeticExpression(stream, expression->ArithmeticLeft, indent + PRINT_AST_INDENT_STEP);
        fprintf(stream, "%*s<=\n", indent, "");
        PrintArithmeticExpression(stream, expression->ArithmeticRight, indent + PRINT_AST_INDENT_STEP);
        break;
    case Not:
        fprintf(stream, "%*s!\n", indent, "");
        PrintBooleanExpression(stream, expression->Left, indent + PRINT_AST_INDENT_STEP);
        break;
    case And:
        PrintBooleanExpression(stream, expression->Left, indent + PRINT_AST_INDENT_STEP);
        fprintf(stream, "%*s&&\n", indent, "");
        PrintBooleanExpression(stream, expression->Right, indent + PRINT_AST_INDENT_STEP);
        break;
    }
}

void PrintAST(FILE* stream, Statement* statement)
{
    PrintTree(stream, statement, 0);
}

void FreeMemory()
{
    Variable *iterator = Memory;
    while (NULL != iterator)
    {
        Variable *temp = iterator;
        iterator = iterator->Next;
        free(temp);
    }
}

/*----------------------------------------------------------------------------*/
// Evaluation functions
/*----------------------------------------------------------------------------*/

void EvaluateStatement(Statement* statement)
{
    if (NULL == statement)
        return;

    int value = 0;
    switch (statement->Type)
    {
    case Assignment:
        value = EvaluateArithmeticExpression(statement->AssignmentValue);
        SetVariableValue(statement->Variable, value);
        break;
    case Sequence:
        EvaluateStatement(statement->Left);
        EvaluateStatement(statement->Right);
        break;
    case Conditional:
        value = EvaluateBooleanExpression(statement->Condition);
        if (0 != value)
        {
            EvaluateStatement(statement->Left);
        }
        else
        {
            EvaluateStatement(statement->Right);
        }
        break;
    case Loop:
        value = EvaluateBooleanExpression(statement->Condition);
        while (0 != value)
        {
            EvaluateStatement(statement->Left);
            value = EvaluateBooleanExpression(statement->Condition);
        }
        break;
    case Skip:
        break;
    }
}

int EvaluateArithmeticExpression(ArithmeticExpression *expression)
{
    if (NULL == expression)
        return 0;

    int value = 0;

    switch (expression->Type)
    {
    case Number:
        value = expression->NumberLiteral;
        break;
    case VariableEval:
        value = GetVariableValue(expression->Variable);
        break;
    case Sum:
        value = EvaluateArithmeticExpression(expression->Left)
            + EvaluateArithmeticExpression(expression->Right);
        break;
    case Difference:
        value = EvaluateArithmeticExpression(expression->Left)
            - EvaluateArithmeticExpression(expression->Right);
        break;
    case Product:
        value = EvaluateArithmeticExpression(expression->Left)
            * EvaluateArithmeticExpression(expression->Right);
        break;
    }

    return value;
}

int EvaluateBooleanExpression(BooleanExpression *expression)
{
    if (NULL == expression)
        return 0;

    int value = 0;

    switch (expression->Type)
    {
    case BooleanLiteral:
        value = expression->BooleanLiteral;
        break;
    case Equals:
        value = EvaluateArithmeticExpression(expression->ArithmeticLeft)
            == EvaluateArithmeticExpression(expression->ArithmeticRight);
        break;
    case LessThanOrEqualTo:
        value = EvaluateArithmeticExpression(expression->ArithmeticLeft)
            <= EvaluateArithmeticExpression(expression->ArithmeticRight);
        break;
    case Not:
        value = EvaluateBooleanExpression(expression->Left);
        if (0 == value)
            value = 1;
        else
            value = 0;
        break;
    case And:
        value = EvaluateBooleanExpression(expression->Left)
            && EvaluateBooleanExpression(expression->Right);
        break;
    }

    return value;
}