#include <iostream>
#include "Vector.h"
#include "Stack.h"
#include <ctime>  // 添加这行，包含time()函数的声明
using namespace std;


// 计算柱状图中最大矩形面积的函数
int largestRectangleArea(vector<int>& heights) {
    stack<int> stk; // 单调栈，存储柱子的索引
    int maxArea = 0;
    int n = heights.size();

    for (int i = 0; i <= n; ++i) {
        // 当栈不为空且当前柱子高度小于栈顶柱子高度时，计算以栈顶柱子高度为高的矩形面积
        while (!stk.empty() && (i == n || heights[i] < heights[stk.top()])) {
            int height = heights[stk.top()];
            stk.pop();
            int width;
            if (stk.empty()) {
                width = i;
            } else {
                width = i - stk.top() - 1;
            }
            maxArea = max(maxArea, height * width);
        }
        stk.push(i);
    }

    return maxArea;
}

// 随机生成测试数据并测试
int main() {
    srand(time(0)); // 初始化随机数种子

    for (int t = 0; t < 10; ++t) {
        int n = rand() % 105 + 1; // 生成 1 到 105 之间的柱子数量
        vector<int> heights(n);
        cout << "测试组 " << t + 1 << "：" << endl;
        cout << "柱子高度：";
        for (int i = 0; i < n; ++i) {
            heights[i] = rand() % 105; // 生成 0 到 104 之间的高度
            cout << heights[i] << " ";
        }
        cout << endl;
        int maxArea = largestRectangleArea(heights);
        cout << "最大矩形面积：" << maxArea << endl << endl;
    }

    return 0;
}

