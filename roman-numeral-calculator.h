#ifndef ROMAN_NUMERAL_CALCULATOR
#define ROMAN_NUMERAL_CALCULATOR

typedef enum { SUCCESS, INVALID_CHARACTER, RESULT_TOO_LARGE } ReturnCode;

ReturnCode add(char* addend1, char* addend2, char* sum);

ReturnCode subtract(char* minuend, char* subtrahend, char* difference);

#endif