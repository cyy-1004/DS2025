#include <iostream>
#include <cstring>
#include <cctype>
using namespace std;

// 栈的模板类实现
template <typename T>
class Stack {
private:
    T* data;       // 存储栈元素的数组
    int top;       // 栈顶指针（-1表示空栈）
    int capacity;  // 栈的容量

    // 扩容栈（当栈满时自动扩容为原来的2倍）
    void expand() {
        T* newData = new T[capacity * 2];
        for (int i = 0; i < capacity; ++i) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        capacity *= 2;
    }

public:
    // 构造函数（初始容量为10）
    Stack(int cap = 10) : capacity(cap), top(-1) {
        data = new T[capacity];
    }

    // 析构函数
    ~Stack() {
        delete[] data;
    }

    // 入栈操作
    void push(T elem) {
        if (top == capacity - 1) {  // 栈满则扩容
            expand();
        }
        data[++top] = elem;
    }

    // 出栈操作（返回栈顶元素并删除）
    T pop() {
        if (isEmpty()) {
            throw "Stack is empty";  // 空栈异常
        }
        return data[top--];
    }

    // 获取栈顶元素（不删除）
    T getTop() const {
        if (isEmpty()) {
            throw "Stack is empty";  // 空栈异常
        }
        return data[top];
    }

    // 判断栈是否为空
    bool isEmpty() const {
        return top == -1;
    }

    // 清空栈
    void clear() {
        top = -1;
    }
};

// 运算符优先级判断（返回优先级数值，数字越大优先级越高）
int getPriority(char op) {
    switch (op) {
        case '(': return 0;  // 左括号优先级最低（入栈后暂不计算）
        case '+':
        case '-': return 1;  // 加减优先级
        case '*':
        case '/': return 2;  // 乘除优先级（高于加减）
        default: return -1;  // 无效运算符
    }
}

// 执行二元运算（返回运算结果，处理除零错误）
double calculate(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': 
            if (b == 0) {
                throw "Division by zero";  // 除零异常
            }
            return a / b;
        default: throw "Invalid operator";
    }
}

// 字符串计算器主函数（输入表达式，返回计算结果或错误信息）
double stringCalculator(const char* expr) {
    Stack<double> numStack;  // 存储操作数的栈
    Stack<char> opStack;     // 存储运算符的栈
    int i = 0;
    int len = strlen(expr);

    while (i < len) {
        // 跳过空格
        if (isspace(expr[i])) {
            i++;
            continue;
        }

        // 处理数字（支持多位数和小数）
        if (isdigit(expr[i]) || expr[i] == '.') {
            double num = 0.0;
            int decimal = 0;  // 标记是否进入小数部分
            double scale = 0.1;  // 小数位的权重（0.1, 0.01, ...）

            // 处理整数部分和小数部分
            while (i < len && (isdigit(expr[i]) || expr[i] == '.')) {
                if (expr[i] == '.') {
                    if (decimal) {  // 重复出现小数点（如"12.3.4"）
                        throw "Invalid decimal point";
                    }
                    decimal = 1;
                    i++;
                    continue;
                }
                if (decimal) {  // 小数部分
                    num += (expr[i] - '0') * scale;
                    scale *= 0.1;
                } else {  // 整数部分
                    num = num * 10 + (expr[i] - '0');
                }
                i++;
            }
            numStack.push(num);  // 数字入栈
        }

        // 处理左括号
        else if (expr[i] == '(') {
            opStack.push(expr[i]);
            i++;
        }

        // 处理右括号（计算括号内的表达式）
        else if (expr[i] == ')') {
            // 弹出运算符直到遇到左括号
            while (!opStack.isEmpty() && opStack.getTop() != '(') {
                char op = opStack.pop();
                if (numStack.isEmpty()) {  // 操作数不足（如"3+)"）
                    throw "Missing operand";
                }
                double b = numStack.pop();
                if (numStack.isEmpty()) {  // 操作数不足（如"(+3)"）
                    throw "Missing operand";
                }
                double a = numStack.pop();
                numStack.push(calculate(a, b, op));  // 计算结果入栈
            }
            if (opStack.isEmpty()) {  // 没有匹配的左括号（如"3+5)"）
                throw "Mismatched parentheses";
            }
            opStack.pop();  // 弹出左括号（不参与计算）
            i++;
        }

        // 处理运算符（+、-、*、/）
        else if (strchr("+-*/", expr[i])) {
            // 当前运算符与栈顶运算符比较优先级：
            // 若栈顶运算符优先级更高或相等，则先计算栈顶运算符
            while (!opStack.isEmpty() && getPriority(opStack.getTop()) >= getPriority(expr[i])) {
                char op = opStack.pop();
                if (numStack.isEmpty()) {  // 操作数不足（如"3+"）
                    throw "Missing operand";
                }
                double b = numStack.pop();
                if (numStack.isEmpty()) {  // 操作数不足（如"+3"，但允许"-3"作为负数）
                    // 特殊处理：允许表达式以负数开头（如"-3+5"）
                    if (op == '-' && numStack.isEmpty()) {
                        numStack.push(-b);
                        continue;
                    }
                    throw "Missing operand";
                }
                double a = numStack.pop();
                numStack.push(calculate(a, b, op));
            }
            opStack.push(expr[i]);  // 当前运算符入栈
            i++;
        }

        // 无效字符（如字母、符号等）
        else {
            throw "Invalid character";
        }
    }

    // 处理剩余的运算符（计算栈中所有剩余运算）
    while (!opStack.isEmpty()) {
        char op = opStack.pop();
        if (op == '(') {  // 有未匹配的左括号（如"(3+5"）
            throw "Mismatched parentheses";
        }
        if (numStack.isEmpty()) {
            throw "Missing operand";
        }
        double b = numStack.pop();
        if (numStack.isEmpty()) {  // 处理类似"3+"的情况
            throw "Missing operand";
        }
        double a = numStack.pop();
        numStack.push(calculate(a, b, op));
    }

    // 修正：最终结果检查（栈中必须只有一个元素）
    if (numStack.isEmpty()) {
        throw "Invalid expression (no result)";
    }
    double result = numStack.pop();  // 取出结果
    if (!numStack.isEmpty()) {  // 若栈中还有元素，说明表达式无效
        throw "Invalid expression (extra operands)";
    }
    return result;  // 返回计算结果
}

// 测试案例
int main() {
    // 测试用例
    const char* testCases[] = {
        "3+5*2",                // 13（乘法优先）
        "(3+5)*2",              // 16（括号优先）
        "10/2-3",               // 2（除法优先）
        "2.5+3.5*2",            // 9.5（小数运算）
        "10/(4-2)",             // 5（括号+除法）
        " -3 + 5 * 2 ",         // 7（忽略空格，处理负数）
        "3+5*2/(7-3)",          // 5.5（混合运算）
        "10/0",                 // 除零错误
        "3+",                   // 缺少操作数
        "(3+5",                 // 括号不匹配
        "3a+5",                 // 无效字符
        "12.3.4+5"              // 无效小数点
    };
    int caseCount = sizeof(testCases) / sizeof(testCases[0]);

    // 执行测试
    for (int i = 0; i < caseCount; ++i) {
        cout << "测试表达式: " << testCases[i] << endl;
        try {
            double result = stringCalculator(testCases[i]);
            cout << "计算结果: " << result << endl;
        } catch (const char* err) {
            cout << "错误: " << err << endl;
        }
        cout << "-------------------------" << endl;
    }

    return 0;
}