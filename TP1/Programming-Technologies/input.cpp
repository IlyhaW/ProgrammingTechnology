
#include < iostream >
#include < cstdlib >
#include < cctype >
#include < cstring >
#include < math.h > 
#include "parser.h"

#define PI 3.14159265358979323846 

using namespace std;

enum types { DELIMITER = 1, VARIABLE, NUMBER, FUNCTION };


parser::parser()
{
    int i;
    exp_ptr = NULL;
    for (i = 0; i < NUMVARS; i++)
        vars[i] = 0.0;
    errormsg[0] = '\0';
}

double parser::eval_exp(char* exp)
{
    errormsg[0] = '\0';
    double result;
    exp_ptr = exp;
    get_token();
    if (!*token)
    {
        strcpy_s(errormsg, "No Expression Present"); 
        return (double)0;
    }
    eval_exp1(result);
    if (*token) 
        strcpy_s(errormsg, "Syntax Error");
    return result;
}

void parser::eval_exp1(double& result)
{
    int slot;
    char temp_token[80];
    if (tok_type == VARIABLE)
    {
        
        char* t_ptr = exp_ptr;
        strcpy_s(temp_token, token);
        
        slot = *token - 'A';
        get_token();
        if (*token != '=')
        {
            exp_ptr = t_ptr; 
            strcpy_s(token, temp_token); 
            tok_type = VARIABLE;
        }
        else {
            get_token(); 
            eval_exp2(result);
            vars[slot] = result;
            return;
        }
    }
    eval_exp2(result);
}

void parser::eval_exp2(double& result)
{
    register char op;
    double temp;
    eval_exp3(result);
    while ((op = *token) == '+' || op == '-')
    {
        get_token();
        eval_exp3(temp);
        switch (op)
        {
        case '-':
            result = result - temp;
            break;
        case '+':
            result = result + temp;
            break;
        }
    }
}

void parser::eval_exp3(double& result)
{
    register char op;
    double temp;
    eval_exp4(result);
    while ((op = *token) == '*' || op == '/')
    {
        get_token();
        eval_exp4(temp);
        switch (op)
        {
        case '*':
            result = result * temp;
            break;
        case '/':
            result = result / temp;
            break;
        }
    }
}

void parser::eval_exp4(double& result)
{
    double temp;
    eval_exp5(result);
    while (*token == '^')
    {
        get_token();
        eval_exp5(temp);
        result = pow(result, temp);
    }
}

void parser::eval_exp5(double& result)
{
    register char op;
    op = 0;
    if ((tok_type == DELIMITER) && *token == '+' || *token == '-')
    {
        op = *token;
        get_token();
    }
    eval_exp6(result);
    if (op == '-')
        result = -result;
}

void parser::eval_exp6(double& result)
{
    bool isfunc = (tok_type == FUNCTION);
    char temp_token[80];
    if (isfunc)
    {
        strcpy_s(temp_token, token);
        get_token();
    }
    if ((*token == '('))
    {
        get_token();
        eval_exp2(result);
        if (*token != ')')
            strcpy_s(errormsg, "Unbalanced Parentheses");
        if (isfunc)
        {
            if (!strcmp(temp_token, "SIN"))
                result = sin(PI / 180 * result);
            else if (!strcmp(temp_token, "COS"))
                result = cos(PI / 180 * result);
            else if (!strcmp(temp_token, "TAN"))
                result = tan(PI / 180 * result);
            else if (!strcmp(temp_token, "ASIN"))
                result = 180 / PI * asin(result);
            else if (!strcmp(temp_token, "ACOS"))
                result = 180 / PI * acos(result);
            else if (!strcmp(temp_token, "ATAN"))
                result = 180 / PI * atan(result);
            else if (!strcmp(temp_token, "SINH"))
                result = sinh(result);
            else if (!strcmp(temp_token, "COSH"))
                result = cosh(result);
            else if (!strcmp(temp_token, "TANH"))
                result = tanh(result);
            else if (!strcmp(temp_token, "ASINH"))
                result = asinh(result);
            else if (!strcmp(temp_token, "ACOSH"))
                result = acosh(result);
            else if (!strcmp(temp_token, "ATANH"))
                result = atanh(result);
            else if (!strcmp(temp_token, "LN"))
                result = log(result);
            else if (!strcmp(temp_token, "LOG"))
                result = log10(result);
            else if (!strcmp(temp_token, "EXP"))
                result = exp(result);
            else if (!strcmp(temp_token, "SQRT"))
                result = sqrt(result);
            else if (!strcmp(temp_token, "SQR"))
                result = result * result;
            else if (!strcmp(temp_token, "ROUND"))
                result = round(result);
            else if (!strcmp(temp_token, "INT"))
                result = floor(result);
            else
                strcpy_s(errormsg, "Unknown Function");
        }
        get_token();
    }
    else
        switch (tok_type)
        {
        case VARIABLE:
            result = vars[*token - 'A'];
            get_token();
            return;
        case NUMBER:
            result = atof(token);
            get_token();
            return;
        default:
            strcpy_s(errormsg, "Syntax Error");
        }
}

void parser::get_token()
{
    register char* temp;
    tok_type = 0;
    temp = token;
    *temp = '\0';
    if (!*exp_ptr)  
        return;
    while (isspace(*exp_ptr))  
        ++exp_ptr;
    if (strchr("+-*%/^=()", *exp_ptr))
    {
        tok_type = DELIMITER;
        *temp++ = *exp_ptr++;  
    }
    else if (isalpha(*exp_ptr))
    {
        while (!strchr(" +-*%/^=()\t\r", *exp_ptr) && (*exp_ptr))
            *temp++ = toupper(*exp_ptr++);
        while (isspace(*exp_ptr))  
            ++exp_ptr;
        tok_type = (*exp_ptr == '(') ? FUNCTION : VARIABLE;
    }
    else if (isdigit(*exp_ptr) || *exp_ptr == '.')
    {
        while (!strchr(" +-*%/^=()\t\r", *exp_ptr) && (*exp_ptr))
            *temp++ = toupper(*exp_ptr++);
        tok_type = NUMBER;
    }
    *temp = '\0';
    if ((tok_type == VARIABLE) && (token[1]))
        strcpy_s(errormsg, "Only first letter of variables is considered");
}
