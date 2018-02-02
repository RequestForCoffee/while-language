#ifndef __WHILE_H__
#define __WHILE_H__

/*----------------------------------------------------------------------------*/
// Type declarations & definitions
/*----------------------------------------------------------------------------*/
typedef enum ArithmeticExpressionTypeEnum {
    Number,
    Variable,
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

void ReadString(char* source, char* target);

#endif
