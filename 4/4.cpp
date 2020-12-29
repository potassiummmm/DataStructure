#include <iostream>
#include <cmath>
#include <climits>

enum Error
{
    DIVIDE_BY_ZERO, MOD_BY_ZERO, NONE, UNDEFINED_BEHAVIOR, INT_OVERFLOW
};

struct Message//单次运算的返回信息
{
    bool isValid;//运算有效性
    int result;//运算结果
    Error error;//运算报错
    Message(bool isValid, int result, Error error)
    {
        this->isValid = isValid;
        this->result = result;
        this->error = error;
    }
};

template<class T>
class Stack
{
private:
    T *elements;
    int maxSize;
    int topIndex;
public:
    Stack()
    {
        elements = new T[10];
        maxSize = 10;
        topIndex = -1;
    };

    ~Stack()
    {
        delete[] elements;
    };

    T top()
    {
        return elements[topIndex];
    };

    bool empty()
    {
        return topIndex == -1;
    };

    void push(const T &elem)
    {
        if(topIndex == maxSize - 1)
        {
            T *newArr = new T[2 * maxSize];
            for(int i = 0;i <= topIndex;++i)
            {
                newArr[i] = elements[i];
            }
            maxSize *= 2;
            delete[] elements;
            elements = newArr;
        }
        elements[++topIndex] = elem;
    };

    T pop()
    {
        if(empty())
        {
            return 0;
        }
        else
        {
            T tmp = top();
            elements[topIndex--].~T();
            return tmp;
        }
    };
};


int getPriority(char op)
{
    if(op == '(')
    {
        return 5;
    }
    else if(op == '+' || op == '-')
    {
        return 4;
    }
    else if(op == '*' || op == '/')
    {
        return 3;
    }
    else if(op == '%')
    {
        return 2;
    }
    else
    {
        return 1;
    }
}

bool isOperator(char c)
{
    return c == '(' || c == ')' || c == '+' || c == '-' || c == '*' || c == '/' ||
           c == '^' || c == '%';
}

Message getResult(char op, int leftVal, int rightVal)
{
    switch(op)
    {
        case '+':
        {
            if(leftVal > 0 && rightVal > 0)
            {
                if(leftVal > INT_MAX - rightVal)
                {
                    return Message{false, 0, INT_OVERFLOW};
                }
            }
            else if(leftVal < 0 && rightVal < 0)
            {
                if(leftVal < INT_MIN - rightVal)
                {
                    return Message{false, 0, INT_OVERFLOW};
                }
            }
            return Message{true, leftVal + rightVal, NONE};
        }
        case '-':
        {
            if(leftVal > 0 && rightVal < 0)
            {
                if(leftVal > INT_MAX + rightVal)
                {
                    return Message{false, 0, INT_OVERFLOW};
                }
            }
            else if(leftVal < 0 && rightVal > 0)
            {
                if(leftVal < INT_MIN + rightVal)
                {
                    return Message{false, 0, INT_OVERFLOW};
                }
            }
            return Message{true, leftVal - rightVal, NONE};
        }
        case '*':
        {
            if(abs(leftVal) > INT_MAX / abs(rightVal))
            {
                return Message{false, 0, INT_OVERFLOW};
            }
            return Message{true, leftVal * rightVal, NONE};
        }
        case '/':
        {
            if(rightVal == 0)
            {
                return Message{false, 0, DIVIDE_BY_ZERO};
            }
            return Message{true, leftVal / rightVal, NONE};
        }
        case '%':
        {
            if(rightVal == 0)
            {
                return Message{false, 0, MOD_BY_ZERO};
            }
            return Message{true, leftVal % rightVal, NONE};
        }
        case '^':
        {
            if(leftVal > std::log(INT_MAX) / std::log(rightVal))
            {
                return Message{false, 0, INT_OVERFLOW};
            }
            if(leftVal == 0 && rightVal < 0)
            {
                return Message{false, 0, DIVIDE_BY_ZERO};
            }
            return Message{true, static_cast<int>(std::pow(leftVal, rightVal)),
                           NONE};
        }
        default:
        {
            return Message{false, 0, UNDEFINED_BEHAVIOR};
        }
    }
}

class Expression
{
private:
    std::string expression;
    Stack<char> operators;
    Stack<int> values;
public:
    Expression() = default;

    ~Expression() = default;

    void setExpression(const std::string &expr)
    {
        this->expression = expr;
    }

    bool isValid();

    void process();
};

bool Expression::isValid()
{
    if(expression[expression.length() - 1] != '=')
    {
        std::cout << "表达式请以'='结尾!" << std::endl;
        return false;
    }
    for(int i = 0;i < expression.size() - 1;i++)
    {
        if(!isOperator(expression[i]) && !std::isdigit(expression[i]))
        {
            std::cout << "表达式中不得出现数字和运算符以外的字符!" << std::endl;
            return false;
        }
    }
    return true;
}

void Expression::process()
{
    int flag = 1;
    if(!isValid())
    {
        return;
    }
    for(int i = 0;i < expression.length() - 1;i++)
    {
        if(std::isdigit(expression[i]))
        {
            int result = expression[i] - '0';
            while(i < expression.length() - 1 && std::isdigit(expression[i + 1]))
            {
                if(result - 5 > (INT_MAX - expression[i + 1] - 2) / 10)
                {
                    std::cout << "输入部分溢出int范围!" << std::endl;
                    return;
                }
                result = result * 10 + expression[++i] - '0';
            }
            values.push(result * flag);
            flag = 1;
        }
        else if(expression[i] == '(')
        {
            operators.push(expression[i]);
        }
        else if(isOperator(expression[i]) && expression[i] != ')')
        {
            if(operators.empty())//运算符为空
            {
                if(expression[i] == '-')
                {
                    if(i == 0 || (i > 0 && isOperator(expression[i - 1]) &&
                                  expression[i - 1] != ')'))
                    {
                        flag = -flag;//此时为负号而非减号
                        continue;
                    }
                }
                operators.push(expression[i]);
            }
            else//运算符非空
            {
                if(isOperator(expression[i - 1]) && expression[i - 1] != ')')
                {
                    if(expression[i] == '-')
                    {
                        flag = -flag;
                        continue;
                    }
                    else if(expression[i] == '+')
                    {
                        continue;
                    }
                }
                while(!operators.empty())
                {
                    char op = operators.top();
                    if(getPriority(op) <= getPriority(expression[i]))
                    {
                        int right = values.pop();
                        int left = values.pop();
                        bool isValid = getResult(op, left, right).isValid;
                        if(!isValid)
                        {
                            switch(getResult(op, left, right).error)
                            {
                                case DIVIDE_BY_ZERO:
                                    std::cout << "除数不得为0!" << std::endl;
                                    break;
                                case MOD_BY_ZERO:
                                    std::cout << "不能对0取模!" << std::endl;
                                    break;
                                case NONE:
                                    break;
                                case UNDEFINED_BEHAVIOR:
                                    std::cout << "出现未定义的行为!" << std::endl;
                                    break;
                                case INT_OVERFLOW:
                                    std::cout << "运算结果溢出int范围!" << std::endl;
                                    break;
                            }
                            return;
                        }
                        int result = getResult(op, left, right).result;
                        values.push(result);
                        operators.pop();
                    }
                    else
                    {
                        break;
                    }
                }
                operators.push(expression[i]);
            }
        }
        else if(expression[i] == ')')
        {
            while(operators.top() != '(')
            {
                char op = operators.pop();
                if(operators.empty() && op != '(')
                {
                    std::cout << "缺少匹配的左括号!" << std::endl;
                    return;
                }
                int right = values.pop();
                int left = values.pop();
                bool isValid = getResult(op, left, right).isValid;
                if(!isValid)
                {
                    switch(getResult(op, left, right).error)
                    {
                        case DIVIDE_BY_ZERO:
                            std::cout << "除数不得为0!" << std::endl;
                            break;
                        case MOD_BY_ZERO:
                            std::cout << "不能对0取模!" << std::endl;
                            break;
                        case NONE:
                            break;
                        case UNDEFINED_BEHAVIOR:
                            std::cout << "出现未定义的行为!" << std::endl;
                            break;
                        case INT_OVERFLOW:
                            std::cout << "运算结果溢出int范围!" << std::endl;
                            break;
                    }
                    return;
                }
                int result = getResult(op, left, right).result;
                values.push(result);
            }
            operators.pop();
        }
    }
    while(!operators.empty())
    {
        char op = operators.pop();
        if(op == '(')
        {
            std::cout << "缺少匹配的右括号!" << std::endl;
            return;
        }
        int right = values.pop();
        int left = values.pop();
        bool isValid = getResult(op, left, right).isValid;
        if(!isValid)
        {
            switch(getResult(op, left, right).error)
            {
                case DIVIDE_BY_ZERO:
                    std::cout << "除数不得为0!" << std::endl;
                    break;
                case MOD_BY_ZERO:
                    std::cout << "不能对0取模!" << std::endl;
                    break;
                case NONE:
                    break;
                case UNDEFINED_BEHAVIOR:
                    std::cout << "出现未定义的行为!" << std::endl;
                    break;
                case INT_OVERFLOW:
                    std::cout << "运算结果溢出int范围!" << std::endl;
                    break;
            }
            return;
        }
        int result = getResult(op, left, right).result;
        values.push(result);
    }
    std::cout << values.pop() << std::endl;
}


int main()
{
    Expression expression;
    std::string expr;
    while(std::cout << "输入表达式:\n", std::cin >> expr)
    {
        expression.setExpression(expr);
        expression.process();
        char flag;
        while(std::cout << "\n是否继续(y,n)?", std::cin >> flag)
        {
            if(flag == 'y')
            {
                break;
            }
            else if(flag == 'n')
            {
                std::cout << "退出成功!" << std::endl;
                return 0;
            }
            else
            {
                std::cout << "请输入y或者n!" << std::endl;
            }
        }
    }
    return 0;
}


