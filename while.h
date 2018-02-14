/*
    MIT License

    Copyright (c) 2018 Robertas Maleckas

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#ifndef __WHILE_H__
#define __WHILE_H__

#include <stdio.h>

/*----------------------------------------------------------------------------*/
// Type declarations & definitions
/*----------------------------------------------------------------------------*/
typedef enum ArithmeticExpressionTypeEnum {
    Number,
    VariableEval,
    Sum,
    Difference,
    Product
} ArithmeticExpressionType;

typedef struct ArithmeticExpressionStruct ArithmeticExpression;
struct ArithmeticExpressionStruct
{
    ArithmeticExpressionType Type;
    ArithmeticExpression *Left;
    ArithmeticExpression *Right;
    char *Variable;
    int NumberLiteral;
};

typedef enum BooleanExpressionTypeEnum {
    BooleanLiteral,
    Equals,
    LessThanOrEqualTo,
    Not,
    And
} BooleanExpressionType;

typedef struct BooleanExpressionStruct BooleanExpression;
struct BooleanExpressionStruct
{
    BooleanExpressionType Type;
    BooleanExpression *Left;
    BooleanExpression *Right;
    ArithmeticExpression *ArithmeticLeft;
    ArithmeticExpression *ArithmeticRight;
    int BooleanLiteral;
};

typedef enum StatementTypeEnum {
    Assignment,
    Skip,
    Sequence,
    Conditional,
    Loop
} StatementType;

typedef struct StatementStruct Statement;
struct StatementStruct
{
    StatementType Type;
    Statement *Left;
    Statement *Right;
    BooleanExpression *Condition;
    ArithmeticExpression *AssignmentValue;
    char *Variable;
};


/*----------------------------------------------------------------------------*/
// Function definitions
/*----------------------------------------------------------------------------*/

Statement *BuildAssignment(char *variable, ArithmeticExpression *value);
Statement *BuildSkip();
Statement *BuildStatementSequence(Statement *first, Statement *second);
Statement *BuildConditional(Statement *ifTrue, Statement *ifFalse, BooleanExpression *condition);
Statement *BuildLoop(Statement *body, BooleanExpression *condition);
void DeleteStatement(Statement *statement);

ArithmeticExpression *BuildNumber(int number);
ArithmeticExpression *BuildVariable(char *variableName);
ArithmeticExpression *BuildSum(ArithmeticExpression *left, ArithmeticExpression *right);
ArithmeticExpression *BuildDifference(ArithmeticExpression *left, ArithmeticExpression *right);
ArithmeticExpression *BuildProduct(ArithmeticExpression *left, ArithmeticExpression *right);
void DeleteArithmeticExpression(ArithmeticExpression *expression);

BooleanExpression *BuildBooleanLiteral(int booleanLiteral);
BooleanExpression *BuildEquals(ArithmeticExpression *left, ArithmeticExpression* right);
BooleanExpression *BuildLessThanOrEqualTo(ArithmeticExpression *left, ArithmeticExpression* right);
BooleanExpression *BuildNot(BooleanExpression *expression);
BooleanExpression *BuildAnd(BooleanExpression *left, BooleanExpression* right);
void DeleteBooleanExpression(BooleanExpression *expression);

void ReadString(char* source, char** target);

void PrintState(FILE *stream);
void PrintAST(FILE* stream, Statement *statement);
void PrintArithmeticExpression(FILE* stream, ArithmeticExpression *expression, int indent);
void PrintBooleanExpression(FILE* stream, BooleanExpression *expression, int indent);

void FreeMemory();

void EvaluateStatement(Statement* statement);
int EvaluateArithmeticExpression(ArithmeticExpression *expression);
int EvaluateBooleanExpression(BooleanExpression *expression);

#endif
