#include <iostream>
#include <string>
#include <cctype>
#include <unordered_map>
#include "Stack.h"

using namespace std;

// 优先级表
unordered_map<char, int> prior = {
    {'+', 1}, {'-', 1},
    {'*', 2}, {'/', 2},
    {'(', 0}, {')', 0}
};

// 中缀表达式转后缀表达式
string infixToPostfix(const string& s) {
    string postfix;
    Stack<char> opStk;
    for (char c : s) {
        if (isdigit(c) || c == '.') { // 数字或小数点
            postfix += c;
        } else if (c == '(') { // 左括号入栈
            opStk.push(c);
        } else if (c == ')') { // 弹出括号内所有操作符
            while (opStk.top() != '(') {
                postfix += ' ';
                postfix += opStk.pop();
            }
            opStk.pop(); // 弹出左括号
        } else { // 操作符
            postfix += ' ';
            while (!opStk.empty() && prior[opStk.top()] >= prior[c]) {
                postfix += opStk.pop();
            }
            opStk.push(c);
        }
    }
    // 弹出剩余操作符
    while (!opStk.empty()) {
        postfix += ' ';
        postfix += opStk.pop();
    }
    return postfix;
}

// 计算后缀表达式
double calculatePostfix(const string& postfix) {
    Stack<double> numStk;
    string numStr;
    for (char c : postfix) {
        if (isdigit(c) || c == '.') { // 数字或小数点，拼接数字
            numStr += c;
        } else if (c == ' ') { // 分隔符，处理数字
            if (!numStr.empty()) {
                numStk.push(stod(numStr));
                numStr.clear();
            }
        } else { // 操作符
            double b = numStk.pop();
            double a = numStk.pop();
            switch (c) {
                case '+': numStk.push(a + b); break;
                case '-': numStk.push(a - b); break;
                case '*': numStk.push(a * b); break;
                case '/': 
                    if (b == 0) throw runtime_error("Division by zero");
                    numStk.push(a / b); 
                    break;
            }
        }
    }
    return numStk.pop();
}

// 字符串计算器主函数
double calculate(const string& expr) {
    // 简单校验
    for (char c : expr) {
        if (!isdigit(c) && !ispunct(c) && !isspace(c)) {
            throw runtime_error("Invalid character");
        }
    }
    string postfix = infixToPostfix(expr);
    return calculatePostfix(postfix);
}

// 案例测试
int main() {
    try {
        string expr1 = "3+4*2";
        cout << expr1 << " = " << calculate(expr1) << endl;

        string expr2 = "(3+4)*2";
        cout << expr2 << " = " << calculate(expr2) << endl;

        string expr3 = "1.5+2.5*3";
        cout << expr3 << " = " << calculate(expr3) << endl;

        string expr4 = "10/2-3";
        cout << expr4 << " = " << calculate(expr4) << endl;

        // 测试无效式子（取消注释查看异常）
        // string expr5 = "5/0+2";
        // cout << expr5 << " = " << calculate(expr5) << endl;

        // string expr6 = "3+a*2";
        // cout << expr6 << " = " << calculate(expr6) << endl;

    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
    return 0;
}
