#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <vector>
#include "token.h"
#include "cuberoot.h"
#include "invsqrt.h"
using namespace std;

#define LINE_BUF_SIZE (1024)

static char *st_line;
static int st_line_pos;

static Token st_look_ahead_token;
static int st_look_ahead_token_exists;

static double group[1024];
static double length = 0.0;

typedef enum{
	INITIAL_STATUS,
	IN_INT_PART_STATUS,
	DOT_STATUS,
	IN_FRAC_PART_STATUS
}LexerStatus;

void get_token(Token *token)
{
	int out_pos = 0;
	LexerStatus status = INITIAL_STATUS;
	char current_char;

	token->kind = BAD_TOKEN;
	while (st_line[st_line_pos] != '\0')
	{
		current_char = st_line[st_line_pos];
		if ((status == IN_INT_PART_STATUS || status == IN_FRAC_PART_STATUS) && !isdigit(current_char) && current_char != '.')
		{
			token->kind = NUMBER_TOKEN;
			//sscanf_s(token->str, "lf", &token->value);
			token->value = atof(token->str);
			return;
		}
		if (isspace(current_char))
		{
			if (current_char == '\n')
			{
				token->kind = END_OF_LINE_TOKEN;
				return;
			}
			st_line_pos++;
			continue;
		}

		if (out_pos >= MAX_TOKEN_SIZE - 1)
		{
			fprintf(stderr, "token too long.\n");
			exit(1);
		}
		token->str[out_pos] = st_line[st_line_pos];
		st_line_pos++;
		out_pos++;
		token->str[out_pos] = '\0';

		if (islower(current_char))
		{
			switch (current_char)
			{
			case 's':
				if (st_line[st_line_pos++] == 'i')
				{
					if (st_line[st_line_pos++] == 'n')
					{
						token->kind = SIN_OPERATOR_TOKEN;
						if (st_line[st_line_pos++] == 'h')
						{
							token->kind = SINH_OPERATOR_TOKEN;
							st_line_pos++;
						}
						//st_line_pos--;
						return;
					}
				}
				else if (st_line[--st_line_pos] == 'q')
				{
					st_line_pos++;
					if (st_line[st_line_pos++] == 'r')
					if (st_line[st_line_pos++] == 't')
					{
						token->kind = SQRT_OPERATOR_TOKEN;
						st_line_pos++;
						return;
					}
				}
				else if (st_line[st_line_pos++] == 'u')
				{
					if (st_line[st_line_pos++] == 'm')
					{
						token->kind = SUM_OPERATOR_TOKEN;
						st_line_pos++;
						return;
					}
				}
				else if (st_line[--st_line_pos] == 't')
				{
					st_line_pos++;
					if (st_line[st_line_pos++] == 'd')
					if (st_line[st_line_pos++] == 'e')
					if (st_line[st_line_pos++] == 'v')
						token->kind = STDEV_OPERATOR_TOKEN;
					if (st_line[st_line_pos++] == 'p')
					{
						st_line_pos++;
						token->kind = STDEVP_OPERATOR_TOKEN;
						return;
					}
				}
			case 'c':
				if (st_line[st_line_pos++] == 'o')
				{
					if (st_line[st_line_pos++] == 's')
						token->kind = COS_OPERATOR_TOKEN;
					if (st_line[st_line_pos++] == 'h')
					{
						token->kind = COSH_OPERATOR_TOKEN;
						st_line_pos++;
					}
					return;
				}
				else
				if (st_line[--st_line_pos] == 'u')
				{
					st_line_pos++;
					if (st_line[st_line_pos++] == 'b')
					if (st_line[st_line_pos++] == 'e')
					if (st_line[st_line_pos++] == 'r')
					if (st_line[st_line_pos++] == 'o')
					if (st_line[st_line_pos++] == 'o')
					if (st_line[st_line_pos++] == 't')
					{
						token->kind = CUBEROOT_OPERATOR_TOKEN;
						st_line_pos++;
						return;
					}
				}
			case 't':
				if (st_line[st_line_pos++] == 'a')
				{
					if (st_line[st_line_pos++] == 'n')
					{
						token->kind = TAN_OPERATOR_TOKEN;
						if (st_line[st_line_pos++] == 'h')
						{
							token->kind = TANH_OPERATOR_TOKEN;
							st_line_pos++;
						}
						return;
					}
				}
			case 'a':
				if (st_line[st_line_pos++] == 'r')
				{
					if (st_line[st_line_pos++] == 'c')
					{
						switch (st_line[st_line_pos++])
						{
						case 's':
							if (st_line[st_line_pos++] == 'i')
							{
								if (st_line[st_line_pos++] == 'n')
								{
									token->kind = ARCSIN_OPERATOR_TOKEN;
									st_line_pos++;
									return;
								}
							}
						case 'c':
							if (st_line[st_line_pos++] == 'o')
							{
								if (st_line[st_line_pos++] == 's')
								{
									token->kind = ARCCOS_OPERATOR_TOKEN;
									st_line_pos++;
									return;
								}
							}
						case 't':
							if (st_line[st_line_pos++] == 'a')
							{
								if (st_line[st_line_pos++] == 'n')
								{
									token->kind = ARCTAN_OPERATOR_TOKEN;
									st_line_pos++;
									return;
								}
							}
						default:
							break;
						}
					}
				}
				else
				if (st_line[--st_line_pos] == 'v')
				{
					st_line_pos++;
					if (st_line[st_line_pos++] == 'g')
					{
						token->kind = AVG_OPERATOR_TOKEN;
						st_line_pos++;
						return;
					}
				}
			case 'm':
				if (st_line[st_line_pos++] == 'o')
				{
					if (st_line[st_line_pos++] == 'd')
					if (st_line[st_line_pos++] == '(')
					{
						for (int i = st_line_pos; st_line[i]!=')'; i++)
						{
							if (st_line[i] == ',')
							{
								token->kind = MOD2_OPERATOR_TOKEN;
							//	st_line_pos--;
								return;
							}
						}
					}
					token->kind = MOD1_OPERATOR_TOKEN;
					st_line_pos--;
					return;
				}
			case 'l':
				if (st_line[st_line_pos++] == 'o')
				{
					if (st_line[st_line_pos] == 'g')
					{
						token->kind = LOG_OPERATOR_TOKEN;
						if (st_line[st_line_pos++] == '1')
						{
							if (st_line[st_line_pos++] == '0')
							{
								token->kind = LG_OPERATOR_TOKEN;
								st_line_pos++;
							}
						}
						return;
					}
				}
				else if (st_line[--st_line_pos] == 'n')
				{
					st_line_pos++;
					token->kind = LN_OPERATOR_TOKEN;
					return;
				}
			case 'p':
				if (st_line[st_line_pos++] == 'o')
				{
					if (st_line[st_line_pos++] == 'w')
					{
						token->kind = POW_OPERATOR_TOKEN;
						st_line_pos++;
						return;
					}
				}
			case 'e':
				if (st_line[st_line_pos++] == 'x')
				{
					if (st_line[st_line_pos++] == 'p')
					{
						token->kind = EXP_OPERATOR_TOKEN;
						st_line_pos++;
						return;
					}
				}
			case 'f':
				if (st_line[st_line_pos++] == 'a')
				{
					if (st_line[st_line_pos++] == 'c')
					{
						if (st_line[st_line_pos++] == 't')
						{
							token->kind = FACT_OPERATOR_TOKEN;
							st_line_pos++;
							return;
						}
					}
				}
			case 'y':
				if (st_line[st_line_pos++] == 'r')
				{
					if (st_line[st_line_pos++] == 'o')
					if (st_line[st_line_pos++] == 'o')
					if (st_line[st_line_pos++] == 't')
					{
						token->kind = YROOT_OPERATOR_TOKEN;
						st_line_pos++;
						return;
					}
				}
			case 'v':
				if (st_line[st_line_pos++] == 'a')
				{
					if (st_line[st_line_pos++] == 'r')
					{
						token->kind = VAR_OPERATOR_TOKEN;
						if (st_line[st_line_pos++] == 'p')
						{
							token->kind = VARP_OPERATOR_TOKEN;
							st_line_pos++;
							return;
						}
					}
				}
			default:
				break;
			}
		}
		if (current_char == '+')
		{
			token->kind = ADD_OPERATOR_TOKEN;
			return;
		}
		else if (current_char == '-')
		{
			token->kind = SUB_OPERATOR_TOKEN;
			return;
		}
		else if (current_char == '*')
		{
			token->kind = MUL_OPERATOR_TOKEN;
			return;
		}
		else if (current_char == '/')
		{
			token->kind = DIV_OPERATOR_TOKEN;
			return;
		}
		else if (current_char == '^')
		{
			token->kind = POWER_OPERATOR_TOKEN;
			return;
		}
		else if (current_char == '(')
		{
			token->kind = LEFT_PAREN_TOKEN;
			return;
		}
		else if (current_char == ')')
		{
			token->kind = RIGHT_PAREN_TOKEN;
			return;
		}
		else if (current_char == '[')
		{
			token->kind = LEFT_BACKET_TOKEN;
			return;
		}
		else if (current_char == ']')
		{
			token->kind = RIGHT_BACKET_TOKEN;
			return;
		}
		else if (isdigit(current_char))
		{
			if (status == INITIAL_STATUS)
			{
				status = IN_INT_PART_STATUS;
			}
			else if (status == DOT_STATUS)
			{
				status = IN_FRAC_PART_STATUS;
			}
		}
		else if (current_char == '.')
		{
			if (status == IN_INT_PART_STATUS)
			{
				status = DOT_STATUS;
			}
			else
			{
				fprintf(stderr, "syntax error.\n");
				exit(1);
			}
		}
		else if (current_char = ',')
		{
			token->kind = COMMA_TOKEN;
			return;
		}
		else
		{
			fprintf(stderr, "bad character(%c)\n", current_char);
			exit(1);
		}
	}
}

static void my_get_token(Token *token)
{
	if (st_look_ahead_token_exists)
	{
		*token = st_look_ahead_token;
		st_look_ahead_token_exists = 0;
	}
	else
	{
		get_token(token);
	}
}

static void unget_token(Token *token)
{
	st_look_ahead_token = *token;
	st_look_ahead_token_exists = 1;
}

double parse_expression(void);

static double parse_primary_expression()
{
	Token token;
	double value = 0.0;
	int minus_flag = 0;
	//double group[1024];

	my_get_token(&token);
	if (token.kind == SUB_OPERATOR_TOKEN)
	{
		minus_flag = 1;
	}
	else
	{
		unget_token(&token);
	}

	my_get_token(&token);
	if (token.kind == NUMBER_TOKEN)
	{
		//return token.value;
		value = token.value;
	}
	else if (token.kind == LEFT_PAREN_TOKEN)
	{
		value = parse_expression();
		my_get_token(&token);
		if (token.kind != RIGHT_PAREN_TOKEN && token.kind != COMMA_TOKEN)
		{
			fprintf(stderr, "missing ')' or ',' error.\n");
			exit(1);
		}
		return value;
	}
	else if (token.kind == LEFT_BACKET_TOKEN)
	{
		for (int i = 0;; i++)
		{
			group[i] = parse_expression();
			length++;
			my_get_token(&token);
			if (token.kind != RIGHT_BACKET_TOKEN && token.kind != COMMA_TOKEN)
			{
				fprintf(stderr, "missing ']' or ',' error.\n");
				exit(1);
			}
			if (token.kind == RIGHT_BACKET_TOKEN)
			{
				break;
			}
		}
		return 1;
	}
	else if (token.kind == COMMA_TOKEN)
	{
		value = parse_expression();
		my_get_token(&token);
		if (token.kind != RIGHT_PAREN_TOKEN && token.kind != COMMA_TOKEN)
		{
			fprintf(stderr, "missing ')' or ',' error.\n");
			exit(1);
		}
	}
	else if (token.kind == SIN_OPERATOR_TOKEN)
	{
		double x;
		x = parse_expression();
		value = sin(x*PI / 180.0f);
		my_get_token(&token);
		//		value = parse_expression();
	}
	else if (token.kind == COS_OPERATOR_TOKEN)
	{
		double x;
		x = parse_expression();
		value = cos(x*PI / 180.0f);
		my_get_token(&token);
	}
	else if (token.kind == TAN_OPERATOR_TOKEN)
	{
		double x;
		double fractional_part;
		int integer_part;
		x = parse_expression();
		fractional_part = x - 90 * floor(x / 90);
		integer_part = (int)floor(x / 90);
		if (fractional_part == 0.0 && integer_part % 1 == 0)
		{
			printf("the tan(x)`s x shouldn`t be 90 and more!\n");
			exit(1);
		}
		value = tan(x*PI / 180.0f);
		my_get_token(&token);
	}
	else if (token.kind == ARCSIN_OPERATOR_TOKEN)
	{
		double x;
		x = parse_expression();
		if (x > 1.0 || x < -1.0)
		{
			printf("the arcsin(x)`s x must be -1 to 1!\n");
			exit(1);
		}
		value = asin(x)*180.0f / PI;
		my_get_token(&token);
		//printf("%ld\n", value);
	}
	else if (token.kind == ARCCOS_OPERATOR_TOKEN)
	{
		double x;
		x = parse_expression();
		if (x > 1.0 || x < -1.0)
		{
			printf("the arccos(x)`s x must be -1 to 1!\n");
			exit(1);
		}
		value = (acos(x))*180.0f / PI;
		my_get_token(&token);
	}
	else if (token.kind == ARCTAN_OPERATOR_TOKEN)
	{
		double x;
		x = parse_expression();
		value = (atan(x))*180.0f / PI;
		my_get_token(&token);
	}
	else if (token.kind == SINH_OPERATOR_TOKEN)
	{
		double x;
		x = parse_expression();
		value = sinh(x);
		my_get_token(&token);
	}
	else if (token.kind == COSH_OPERATOR_TOKEN)
	{
		double x;
		x = parse_expression();
		value = cosh(x);
		my_get_token(&token);
	}
	else if (token.kind == TANH_OPERATOR_TOKEN)
	{
		double x;
		x = parse_expression();
		value = tanh(x);
		my_get_token(&token);
	}
	else if (token.kind == LOG_OPERATOR_TOKEN)
	{
		//printf("%ld\n", parse_expression());
		//printf("%ld\n", parse_expression());
		double x;
		double nBASE;
		x = parse_expression();
		//my_get_token(&token);
		nBASE = parse_expression();
		if (x <= 0 || x == 1 || nBASE <= 0)
		{
			printf("the log(x,nBASE)`s x must be grater than 0 and unequal to 1 and the nBASE must be grater than 0!\n");
			exit(1);
		}
		value = log(nBASE) / log(x);
		my_get_token(&token);
		//value = log(parse_expression()) / log(parse_expression());
	}
	else if (token.kind == LG_OPERATOR_TOKEN)
	{
		double x;
		x = parse_expression();
		if (x <= 0)
		{
			printf("the log10(x)`s x must be grater than 0!\n");
			exit(1);
		}
		value = log10(x);
		my_get_token(&token);
	}
	else if (token.kind == LN_OPERATOR_TOKEN)
	{
		double x;
		x = parse_expression();
		if (x <= 0)
		{
			printf("the ln(x)`s x must be grater than 0!\n");
			exit(1);
		}
		value = log(x);
		my_get_token(&token);
	}
	else if (token.kind == POW_OPERATOR_TOKEN)
	{
		double x;
		double nPower;
		x = parse_expression();
		nPower = parse_expression();
		value = pow(x, nPower);
		my_get_token(&token);
	}
	else if (token.kind == EXP_OPERATOR_TOKEN)
	{
		double x;
		x = parse_expression();
		value = exp(x);
		my_get_token(&token);
	}
	else if (token.kind == FACT_OPERATOR_TOKEN)
	{
		double x;
		value = 1;
		x = parse_expression();
		if ((x - 90 * floor(x / 90)) != 0.0)
		{
			printf("the fact(x)`s x must be natural number!\n");
			exit(1);
		}
		for (int i = 1; i <= x; i++)
		{
			value *= i;
		}
		my_get_token(&token);
	}
	else if (token.kind == MOD2_OPERATOR_TOKEN)
	{
		double x;
		double y;
		x = parse_expression();
		y = parse_expression();
		value = x - (int)(x / y)*y;
		my_get_token(&token);
	}
	else if (token.kind == SQRT_OPERATOR_TOKEN)
	{
		double x;
		x = parse_expression();
		if (x < 0)
		{
			printf("the sqrt(x)`s x must be grater than 0!\n");
			exit(1);
		}
		//value = InvSqrt(x);
		value = sqrt(x);
		my_get_token(&token);
	}
	else if (token.kind == CUBEROOT_OPERATOR_TOKEN)
	{
		double x;
		x = parse_expression();
		if (x < 0)
		{
			printf("the sqrt(x)`s x must be grater than 0!\n");
			exit(1);
		}
		//value = cuberoot(x);
		value = cuberoot(x);
		my_get_token(&token);
	}
	else if (token.kind == YROOT_OPERATOR_TOKEN)
	{
		double x;
		double y;
		x = parse_expression();
		y = parse_expression();
		if (x < 0)
		{
			printf("the yroot(x,y)`s x must be grater than 0!\n");
			exit(1);
		}
		value = pow(x, 1.0 / y);
		my_get_token(&token);
	}
	else if (token.kind == AVG_OPERATOR_TOKEN)
	{
		double the_sum = 0;
		parse_expression();
		for (int i = 0; i < length; i++)
		{
			the_sum += group[i];
		}
		value = the_sum / length;
		length = 0;
		for (int i = 0; i < length; i++)
		{
			group[i] = 0;
		}
		my_get_token(&token);
	}
	else if (token.kind == SUM_OPERATOR_TOKEN)
	{
		double the_sum = 0;
		parse_expression();
		for (int i = 0; i < length; i++)
		{
			the_sum += group[i];
		}
		value = the_sum;
		length = 0;
		for (int i = 0; i < length; i++)
		{
			group[i] = 0;
		}
		my_get_token(&token);
	}
	else if (token.kind == VAR_OPERATOR_TOKEN)
	{
		double the_sum = 0;
		double the_average;
		double the_varsum = 0;
		parse_expression();
		for (int i = 0; i < length; i++)
		{
			the_sum += group[i];
		}
		the_average = the_sum / length;
		for (int i = 0; i < length; i++)
		{
			the_varsum += (group[i] - the_average)*(group[i] - the_average);
		}
		value = the_varsum / (length - 1);
		length = 0;
		for (int i = 0; i < length; i++)
		{
			group[i] = 0;
		}
		my_get_token(&token);
	}
	else if (token.kind == VARP_OPERATOR_TOKEN)
	{
		double the_sum = 0;
		double the_average;
		double the_varsum = 0;
		parse_expression();
		for (int i = 0; i < length; i++)
		{
			the_sum += group[i];
		}
		the_average = the_sum / length;
		for (int i = 0; i < length; i++)
		{
			the_varsum += (group[i] - the_average)*(group[i] - the_average);
		}
		value = the_varsum / length;
		length = 0;
		for (int i = 0; i < length; i++)
		{
			group[i] = 0;
		}
		my_get_token(&token);
	}
	else if (token.kind == STDEV_OPERATOR_TOKEN)
	{
		double the_sum = 0;
		double the_average;
		double the_varsum = 0;
		parse_expression();
		for (int i = 0; i < length; i++)
		{
			the_sum += group[i];
		}
		the_average = the_sum / length;
		for (int i = 0; i < length; i++)
		{
			the_varsum += (group[i] - the_average)*(group[i] - the_average);
		}
		value = sqrt(the_varsum / (length - 1));
		length = 0;
		for (int i = 0; i < length; i++)
		{
			group[i] = 0;
		}
		my_get_token(&token);
	}
	else if (token.kind == STDEVP_OPERATOR_TOKEN)
	{
		double the_sum = 0;
		double the_average;
		double the_varsum = 0;
		parse_expression();
		for (int i = 0; i < length; i++)
		{
			the_sum += group[i];
		}
		the_average = the_sum / length;
		for (int i = 0; i < length; i++)
		{
			the_varsum += (group[i] - the_average)*(group[i] - the_average);
		}
		value = sqrt(the_varsum / length);
		length = 0;
		for (int i = 0; i < length; i++)
		{
			group[i] = 0;
		}
		my_get_token(&token);
	}
	else
	{
		unget_token(&token);
		//return 0.0;
	}
	if (minus_flag)
	{
		value = -value;
	}
	//printf("pri%ld\n", value);
	return value;
}

static double parse_peak()
{
	double v1;
	double v2;
	Token token;

	v1 = parse_primary_expression();
	for (;;)
	{
		my_get_token(&token);
		if ((token.kind != POWER_OPERATOR_TOKEN && token.kind != MOD1_OPERATOR_TOKEN))
		{
			unget_token(&token);
			break;
		}
		v2 = parse_primary_expression();
		if (token.kind == POWER_OPERATOR_TOKEN)
		{
			v1 = pow(v1, v2);
		}
		else if (token.kind == MOD1_OPERATOR_TOKEN)
		{
			int newv1 = v1;
			int newv2 = v2;
			newv1 %= newv2;
			v1 = newv1;
		}
	}
	//printf("peak%ld\n", v1);
	return v1;
}

static double parse_term()
{
	double v1;
	double v2;
	Token token;

	v1 = parse_peak();
	for (;;)
	{
		my_get_token(&token);
		if ((token.kind != MUL_OPERATOR_TOKEN && token.kind != DIV_OPERATOR_TOKEN))
		{
			unget_token(&token);
			break;
		}
		v2 = parse_peak();
		if (token.kind == MUL_OPERATOR_TOKEN)
		{
			v1 *= v2;
		}
		else if (token.kind == DIV_OPERATOR_TOKEN)
		{
			v1 /= v2;
		}
	}
	//printf("term%ld\n", v1);
	return v1;
}

double parse_expression()
{
	double v1;
	double v2;
	Token token;

	v1 = parse_term();
	for (;;)
	{
		my_get_token(&token);
		if ((token.kind != ADD_OPERATOR_TOKEN && token.kind != SUB_OPERATOR_TOKEN))
		{
			unget_token(&token);
			break;
		}
		v2 = parse_term();
		if (token.kind == ADD_OPERATOR_TOKEN)
		{
			v1 += v2;
		}
		else if (token.kind == SUB_OPERATOR_TOKEN)
		{
			v1 -= v2;
		}
		else
		{
			unget_token(&token);
		}
	}
	//printf("expre%ld\n", v1);
	return v1;
}

void set_line(char *line)
{
	st_line = line;
	st_line_pos = 0;
}

double parse_line(void)
{
	double value;

	st_look_ahead_token_exists = 0;
	value = parse_expression();

	return value;
}

int main(int argc, char **argv)
{
	char line[LINE_BUF_SIZE];
	double value;

	while (fgets(line, LINE_BUF_SIZE, stdin) != NULL)
	{
		set_line(line);
		value = parse_line();
		printf(">>%f\n", value);
	}
	return 0;
}