#include "while.h"

#include <stdlib.h>


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

ArithmeticExpression *BuildInfixArithmeticOp(ArithmeticExpression *left, ArithmeticExpression *right, ArithmeticExpressionType type)
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

    switch(expression->Type)
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

BooleanExpression *BuildArithmeticComparison(ArithmeticExpression *left, ArithmeticExpression* right, BooleanExpressionType type)
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

    switch(expression->Type)
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
