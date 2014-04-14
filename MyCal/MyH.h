#ifndef MYH_H_INCLUDED
#define MYH_H_INCLUDED

#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cmath>
#include <string>
#include <stack>
#include <iostream>
#include <list>
using namespace std;

//�������ͣ����Ŵ��󡢺���ƴд���󡢺�����������󡢳�����󡢿ո���������ŷ������������
//			���Ŵ���
//                 ��һ������λ�ô��ڵ�һ������λ��
//					  û�������ţ�����������
//                 û�������ţ�����������
//        ����ƴд����
//        �������������PS���ڼ��㲿��          
//        �������PS���ڼ��㲿��
//        �ո���󣺺������ܱ��ո�ֿ�
//                 �������Լ����������Ų��ֿܷ�
//                 ͳ�ƺ����������ű����У���
//                 ˫Ŀ�������������ҽ���һ������ PS������Χ����
//                 ��Ŀ���������в����ж��� PS:����Χ����
//        �����ŷ������������
//
//

//Сд��
void cal_lower(char *line)
{
	string expression;
	expression = line;
	//cout << line << endl;
	for (string::iterator iter = expression.begin(); iter != expression.end(); ++iter)
	{
		*iter = tolower(*iter);
	}
	//cout << expression << endl;
	for (unsigned i = 0; i <= expression.length(); i++)
	{
		line[i] = expression[i];
	}
	//cout << line << endl;
}

void cal_debug(char *line)
{
	string expression(line);
	stack<char> ParensStack;
	stack<char>	BracketsStack;
	string::size_type FirstRParenPos;
	string::size_type FirstLParenPos;
	string::size_type FirstRBracketPos;
	string::size_type FirstLBracketPos;

	FirstLParenPos = expression.find_first_of('(');
	FirstRParenPos = expression.find_first_of(')');

	//���Ŵ���
	if (FirstLParenPos == string::npos)
	{
		if (FirstRParenPos != string::npos)
		{
			cout << "missing'(' error!" << endl;
			exit(0);
		}
	}
	else
	{
		if (FirstLParenPos > FirstRParenPos)
		{
			cout << "the '(' should be righter than ')'" << endl;
			exit(0);
		}
	}
	FirstLBracketPos = expression.find_first_of('[');
	FirstRBracketPos = expression.find_first_of(']');
	if (FirstLBracketPos == string::npos)
	{
		if (FirstRBracketPos != string::npos)
		{
			cout << "missing '[' error!" << endl;
			exit(0);
		}
	}
	else
	{
		if (FirstLBracketPos > FirstRBracketPos)
		{
			cout << "the '[' should be righter than ']'" << endl;
			exit(0);
		}
	}
	for (unsigned i = 0; i < expression.length(); i++)
	{
		if (expression[i] == '(') ParensStack.push(expression[i]);
		if (expression[i] == '[') BracketsStack.push(expression[i]);
		if (expression[i] == ')')
		{
			if (ParensStack.empty())
			{
				cout << "missing '(' error!" << endl;
				exit(0);
			}
			ParensStack.pop();
		}
		if (expression[i] == ']')
		{
			if (BracketsStack.empty())
			{
				cout << "missing '[' error!" << endl;
			}
		}
	}
	if (!ParensStack.empty() || !BracketsStack.empty())
	{
		cout << "missing ']' or ')' error!" << endl;
		exit(0);
	}

	//ƴд����
	for (unsigned i = 0; i < expression.length(); i++)
	{
		//	string::size_type FLPpos;
		if (islower(expression[i]) || isupper(expression[i]))
		{
			if (i == 0);
			else if (
				expression[i - 1] != ' ' &&
				expression[i - 1] != '(' &&
				expression[i - 1] != '+' &&
				expression[i - 1] != '-' &&
				expression[i - 1] != '*' &&
				expression[i - 1] != '/' &&
				expression[i - 1] != '^'
				)
			{
				cout << "input error!" << endl;
				exit(0);
			}
		}
		switch (expression[i])
		{
		case 's':
			if (
				expression[i + 1] == 'i' &&
				expression[i + 2] == 'n' &&
				(expression[i + 3] == '(' || (expression[i + 3] == 'h' && expression[i + 4] == '('))
				)
			{
				i = i + 3;
				break;
			}
			else if (
				expression[i + 1] == 'q' &&
				expression[i + 2] == 'r'&&
				expression[i + 3] == 't'&&
				expression[i + 4] == '('
				)
			{
				i = i + 3;
				break;
			}
			else if (
				expression[i + 1] == 'u' &&
				expression[i + 2] == 'm' &&
				expression[i + 3] == '('
				)
			{
				i = i + 2;
				break;
			}
			else if (
				expression[i + 1] == 't' &&
				expression[i + 2] == 'd' &&
				expression[i + 3] == 'e' &&
				expression[i + 4] == 'v' &&
				(expression[i + 5] == '(' || (expression[i + 5] == 'p' && expression[i + 6] == '('))
				)
			{
				i = i + 5;
				break;
			}
			else
			{
				cout << "Do you want use sin or sinh?\nPlease check your input and try agin" << endl;
				exit(0);
			}
		case 'c':
			if (
				expression[i + 1] == 'o' &&
				expression[i + 2] == 's' &&
				(expression[i + 3] == '(' || (expression[i + 3] == 'h' && expression[i + 4] == '('))
				)
			{
				i = i + 3;
				break;
			}
			else if (
				expression[i + 1] == 'u' &&
				expression[i + 2] == 'b' &&
				expression[i + 3] == 'e' &&
				expression[i + 4] == 'r' &&
				expression[i + 5] == 'o' &&
				expression[i + 6] == 'o' &&
				expression[i + 7] == 't' &&
				expression[i + 8] == '('
				)
			{
				i = i + 7;
				break;
			}
			else
			{
				cout << "Do you want use cos or cosh?\nPlease check your input and try agin" << endl;
				exit(0);
			}
		case 't':
			if (
				expression[i + 1] == 'a' &&
				expression[i + 2] == 'n' &&
				(expression[i + 3] == '(' || (expression[i + 3] == 'h' && expression[i + 4] == '('))
				)
			{
				i = i + 3;
				break;
			}
			else
			{
				cout << "Do you want use tan or tanh?\nPlease check your input and try agin" << endl;
				exit(0);
			}
		case 'a':
			if (expression[i + 1] == 'r' && expression[i + 2] == 'c')
			{
				switch (expression[i + 3])
				{
				case 's':
					if (
						expression[i + 4] == 'i' &&
						expression[i + 5] == 'n' &&
						expression[i + 6] == '('
						)
					{
						i = i + 5;
						break;
					}
					else
					{
						cout << "Do you want use arcsin?\nPlease check your input and try agin" << endl;
						exit(0);
					}
				case 'c':
					if (
						expression[i + 4] == 'o' &&
						expression[i + 5] == 's' &&
						expression[i + 6] == '('
						)
					{
						i = i + 5;
						break;
					}
					else
					{
						cout << "Do you want use arccos?\nPlease check your input and try agin" << endl;
						exit(0);
					}
				case 't':
					if (
						expression[i + 4] == 'a' &&
						expression[i + 5] == 'n' &&
						expression[i + 6] == '('
						)
					{
						i = i + 5;
						break;
					}
					else
					{
						cout << "Do you want use arctan?\nPlease check your input and try agin" << endl;
						exit(0);
					}
				default:
					cout << "Do you want use arc?\nPlease check your input and try agin" << endl;
					exit(0);
					break;
				}
			}
			else if (expression[i + 1] == 'v' && expression[i + 2] == 'g' && expression[i + 3] == '(')
			{
				i = i + 2;
				break;
			}
			else
			{
				cout << "Do you want use arc?\nPlease check your input and try agin" << endl;
				exit(0);
			}
		case 'l':
			if (
				expression[i + 1] == 'o' &&
				expression[i + 2] == 'g' &&
				expression[i + 3] == '('
				)
			{
				i = i + 2;
				break;
			}
			else if (
				expression[i + 1] == 'o' &&
				expression[i + 2] == 'g' &&
				expression[i + 3] == '1' &&
				expression[i + 4] == '0'&&
				expression[i + 5] == '('
				)
			{
				i = i + 4;
				break;
			}
			else if (expression[i + 1] == 'n' && expression[i + 2] == '(')
			{
				i = i + 1;
				break;
			}
			else
			{
				cout << "Do you want use log or log10 or ln?\nPlease check your input and try agin" << endl;
				exit(0);
			}
		case 'p':
			if (expression[i + 1] == 'o' && expression[i + 2] == 'w' && expression[i + 3] == '(')
			{
				i = i + 2;
				break;
			}
			else
			{
				cout << "Do you want use pow?\nPlease check your input and try agin" << endl;
				exit(0);
			}
		case 'e':
			if (expression[i + 1] == 'x' && expression[i + 2] == 'p' && expression[i + 3] == '(')
			{
				i = i + 2;
				break;
			}
			else
			{
				cout << "Do you want use exp?\nPlease check your input and try agin" << endl;
				exit(0);
			}
		case 'f':
			if (
				expression[i + 1] == 'a' &&
				expression[i + 2] == 'c' &&
				expression[i + 3] == 't' &&
				expression[i + 4] == '('
				)
			{
				i = i + 3;
				break;
			}
			else
			{
				cout << "Do you want use fact?\nPlease check your input and try agin" << endl;
				exit(0);
			}
		case 'm':
			if (expression[i + 1] == 'o' && expression[i + 2] == 'd')
			{
				if (expression[i + 3] == '(')
				{
					i = i + 2;
					break;
				}
				else if (expression[i + 3] == ' ')
				{
					i = i + 2;
					break;
				}
				else
				{
					cout << "Do you want use mod?\nPlease check your input and try agin" << endl;
					exit(0);
				}
			}
		case 'y':
			if (
				expression[i + 1] == 'r' &&
				expression[i + 2] == 'o' &&
				expression[i + 3] == 'o' &&
				expression[i + 4] == 't' &&
				expression[i + 5] == '('
				)
			{
				i = i + 4;
				break;
			}
			else
			{
				cout << "Do you want use mod?\nPlease check your input and try agin" << endl;
				exit(0);
			}
		case 'v':
			if (
				expression[i + 1] == 'a' &&
				expression[i + 2] == 'r' &&
				(expression[i + 3] == '(' || (expression[i + 3] == 'p' && expression[i + 4] == '('))
				)
			{
				i = i + 3;
				break;
			}
			else
			{
				cout << "Do you want use var or varp?\nPlease check your input and try agin" << endl;
				exit(0);
			}
		default:
			break;
		}
	}
	//return expression;
}

//ѹ��
void cal_compress(char *line)
{
	char * fp = line;
	while (*line) {
		if (*line != ' ') { // ������ǿո�͸���
			*fp = *line;
			fp++;
		}
		line++;
	}
	*fp = '\0'; //����ַ���
}

#endif // ! MYH_H_INCLUDED