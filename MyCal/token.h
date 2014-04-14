#ifndef TOKEN_H_INCLUDED
#define TOKEN_H_INCLUDED

typedef enum{
	//
	BAD_TOKEN,
	NUMBER_TOKEN,
	COMMA_TOKEN,
	//basic operator
	ADD_OPERATOR_TOKEN,
	SUB_OPERATOR_TOKEN,
	MUL_OPERATOR_TOKEN,
	DIV_OPERATOR_TOKEN,
	POWER_OPERATOR_TOKEN,
	MOD1_OPERATOR_TOKEN,
	//function operator
	SIN_OPERATOR_TOKEN,
	COS_OPERATOR_TOKEN,
	TAN_OPERATOR_TOKEN,
	ARCSIN_OPERATOR_TOKEN,
	ARCCOS_OPERATOR_TOKEN,
	ARCTAN_OPERATOR_TOKEN,
	SINH_OPERATOR_TOKEN,
	COSH_OPERATOR_TOKEN,
	TANH_OPERATOR_TOKEN,
	LOG_OPERATOR_TOKEN,
	LG_OPERATOR_TOKEN,
	LN_OPERATOR_TOKEN,
	POW_OPERATOR_TOKEN,
	EXP_OPERATOR_TOKEN,
	FACT_OPERATOR_TOKEN,
	MOD2_OPERATOR_TOKEN,
	SQRT_OPERATOR_TOKEN,
	CUBEROOT_OPERATOR_TOKEN,
	YROOT_OPERATOR_TOKEN,
	AVG_OPERATOR_TOKEN,
	SUM_OPERATOR_TOKEN,
	VAR_OPERATOR_TOKEN,
	VARP_OPERATOR_TOKEN,
	STDEV_OPERATOR_TOKEN,
	STDEVP_OPERATOR_TOKEN,
	//paren
	LEFT_PAREN_TOKEN,
	RIGHT_PAREN_TOKEN,
	LEFT_BRACKET_TOKEN,
	RIGHT_BRACKET_TOKEN,
	//end
	END_OF_LINE_TOKEN
}Tokenkind;

#define MAX_TOKEN_SIZE (100)
#define PI 3.1415926


typedef struct{
	Tokenkind kind;
	double value;
	char str[MAX_TOKEN_SIZE];
}Token;

void set_line(char *line);
void get_token(Token *token);

#endif