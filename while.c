#include "while.h"

#include <stdlib.h>
#include <string.h>

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

    result->Type = Variable;
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

    case Variable:
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
        // TODO: assign to variable (statement->Variable)
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
    case Variable:
        // TODO: retrieve the variable value
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