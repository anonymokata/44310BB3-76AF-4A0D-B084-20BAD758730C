#ifndef ROMAN_NUMERAL_CALCULATOR
#define ROMAN_NUMERAL_CALCULATOR

typedef enum { SUCCESS, INVALID_ARGUMENT } ReturnCode;

ReturnCode add(char* addend1, char* addend2, char* sum);

ReturnCode subtract(char* minuend, char* subtrahend, char* difference);

#endif