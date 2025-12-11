#include <iostream>
#include <random>
#include "MySTL/Vector.h"
#include "MySTL/Stack.h"

using namespace std;

// 计算柱状图中最大矩形面积
int largestRectangleArea(Vector<int>& heights) {
    Stack<int> stk;
    heights.push_back(0); // 哨兵，确保最后所有元素出栈
    int maxArea = 0;
    for (int i = 0; i < heights.size(); ++i) {
        while (!stk.empty() && heights[stk.top()] > heights[i]) {
            int h = heights[stk.pop()];
            int w = stk.empty() ? i : i - stk.top() - 1;
            maxArea = max(maxArea, h * w);
        }
        stk.push(i);
    }
    heights.pop_back(); // 还原原数组
    return maxArea;
}

// 随机生成测试数据并测试
void testRandomCases() {
    default_random_engine e;
    uniform_int_distribution<int> sizeDist(1, 105);
    uniform_int_distribution<int> valDist(0, 104);
    
    for (int i = 0; i < 10; ++i) {
        int n = sizeDist(e);
        Vector<int> heights;
        for (int j = 0; j < n; ++j) {
            heights.push_back(valDist(e));
        }
        
        cout << "测试用例 " << i + 1 << " (长度: " << n << "): ";
        for (int j = 0; j < min(n, 10); ++j) { // 只输出前10个元素（避免过长）
            cout << heights[j] << " ";
        }
        if (n > 10) cout << "...";
        cout << endl;
        
        int area = largestRectangleArea(heights);
        cout << "最大矩形面积: " << area << endl << endl;
    }
}

int main() {
    // 示例1测试
    Vector<int> heights1 = {2, 1, 5, 6, 2, 3};
    cout << "示例1输入: ";
    for (int i = 0; i < heights1.size(); ++i) {
        cout << heights1[i] << " ";
    }
    cout << "\n示例1输出: " << largestRectangleArea(heights1) << endl << endl;
    
    // 示例2测试
    Vector<int> heights2 = {2, 4};
    cout << "示例2输入: ";
    for (int i = 0; i < heights2.size(); ++i) {
        cout << heights2[i] << " ";
    }
    cout << "\n示例2输出: " << largestRectangleArea(heights2) << endl << endl;
    
    // 随机测试
    testRandomCases();
    
    return 0;
}

