#include "MySTL/Vector.h"
#include "MySTL/list.h"
#include "MySTL/Stack.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <queue>
#include <map>
#include <cctype>
using namespace std;

// 位图类（修复 const 限定符冲突，移除重复定义依赖）
class Bitmap {
private:
    unsigned char* M;  // 存储位图的字节数组
    Rank N;            // 字节数组长度（总比特数 = N * 8）
    Rank _sz;          // 已使用的比特数

    // 扩容：仅在非 const 操作中调用（避免 const 函数冲突）
    void expand(Rank k) {
        if (k < 8 * N) return;
        Rank oldN = N;
        unsigned char* oldM = M;
        N = max(N * 2, (k + 7) / 8);
        M = new unsigned char[N];
        memset(M, 0, N);
        memcpy(M, oldM, oldN);
        delete[] oldM;
    }

    // 初始化：指定初始比特数（默认 8 位）
    void init(Rank n = 8) {
        N = (n + 7) / 8;
        M = new unsigned char[N];
        memset(M, 0, N);
        _sz = 0;
    }

public:
    // 构造函数
    Bitmap(Rank n = 8) { init(n); }

    // 析构函数
    ~Bitmap() {
        delete[] M;
        M = nullptr;
        N = 0;
        _sz = 0;
    }

    // 返回已使用的比特数
    Rank size() const { return _sz; }

    // 设置第 k 位为 1（非 const 操作，可扩容）
    void set(Rank k) {
        expand(k);
        M[k >> 3] |= (0x80 >> (k & 0x07));
        _sz = max(_sz, k + 1);
    }

    // 清空第 k 位为 0（非 const 操作，可扩容）
    void clear(Rank k) {
        expand(k);
        M[k >> 3] &= ~(0x80 >> (k & 0x07));
        if (k == _sz - 1) {
            while (_sz > 0 && !test(_sz - 1)) _sz--;
        }
    }

    // 测试第 k 位是否为 1（const 函数，不扩容，超出范围返回 false）
    bool test(Rank k) const {
        if (k >= 8 * N) return false;
        return (M[k >> 3] & (0x80 >> (k & 0x07))) != 0;
    }

    // 将前 n 位转换为字符串（非 const 操作，确保容量）
    char* bits2string(Rank n) {
        if (n > 8 * N) expand(n - 1); // 提前扩容
        char* s = new char[n + 1];
        s[n] = '\0';
        for (Rank i = 0; i < n; i++) {
            s[i] = test(i) ? '1' : '0';
        }
        return s;
    }
};

// 哈夫曼编码类型（基于位图）
typedef Bitmap HuffCode;

// 哈夫曼树节点结构体
template <typename T>
struct HuffNode {
    T data;          // 存储字符（仅叶子节点有效）
    int weight;      // 权重（字符出现次数）
    HuffNode* left;  // 左子节点（编码 0）
    HuffNode* right; // 右子节点（编码 1）

    // 构造函数
    HuffNode(T d = T(), int w = 0, HuffNode* l = nullptr, HuffNode* r = nullptr)
        : data(d), weight(w), left(l), right(r) {}
};

// 哈夫曼树类
template <typename T>
class HuffTree {
private:
    HuffNode<T>* root;  // 根节点

    // 递归销毁哈夫曼树（释放内存）
    void destroy(HuffNode<T>* node) {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

    // 递归生成哈夫曼编码表（深度优先遍历）
    void generateCode(HuffNode<T>* node, HuffCode& code, map<T, string>& codeMap) {
        if (!node) return;

        // 叶子节点：记录当前字符的编码
        if (!node->left && !node->right) {
            char* bitStr = code.bits2string(code.size());
            codeMap[node->data] = string(bitStr);
            delete[] bitStr;
            return;
        }

        // 左子树：编码加 '0'
        code.set(code.size());
        generateCode(node->left, code, codeMap);
        code.clear(code.size() - 1);

        // 右子树：编码加 '1'
        code.set(code.size());
        generateCode(node->right, code, codeMap);
        code.clear(code.size() - 1);
    }

public:
    // 构造函数
    HuffTree() : root(nullptr) {}

    // 析构函数
    ~HuffTree() {
        destroy(root);
        root = nullptr;
    }

    // 构建哈夫曼树（输入字符-频率映射）
    void build(map<T, int>& freqMap) {
        auto cmp = [](HuffNode<T>* a, HuffNode<T>* b) {
            return a->weight > b->weight;
        };
        priority_queue<HuffNode<T>*, vector<HuffNode<T>*>, decltype(cmp)> pq(cmp);

        // 初始化叶子节点入队
        for (auto& pair : freqMap) {
            if (pair.second > 0) {
                pq.push(new HuffNode<T>(pair.first, pair.second));
            }
        }

        // 合并节点生成树
        while (pq.size() > 1) {
            HuffNode<T>* left = pq.top(); pq.pop();
            HuffNode<T>* right = pq.top(); pq.pop();
            HuffNode<T>* parent = new HuffNode<T>('\0', left->weight + right->weight, left, right);
            pq.push(parent);
        }

        if (!pq.empty()) {
            root = pq.top();
            pq.pop();
        }
    }

    // 生成哈夫曼编码表
    map<T, string> getCodeMap() {
        map<T, string> codeMap;
        if (!root) return codeMap;

        HuffCode code;
        generateCode(root, code, codeMap);
        return codeMap;
    }

    // 对字符串进行编码
    string encode(string str, map<T, string>& codeMap) {
        string res;
        for (char c : str) {
            if (isalpha(c)) {
                c = tolower(c);
                auto it = codeMap.find(c);
                if (it != codeMap.end()) {
                    res += it->second;
                }
            }
        }
        return res;
    }

    // 计算压缩率
    double calculateCompressionRate(string originalStr, string encodedStr) {
        int originalBits = 0;
        for (char c : originalStr) {
            if (isalpha(c)) originalBits += 8;
        }
        int encodedBits = encodedStr.size();
        if (originalBits == 0) return 0.0;
        return (1.0 - (double)encodedBits / originalBits) * 100;
    }
};

// 统计字符频率
map<char, int> countCharFrequency(const char* filename) {
    map<char, int> freqMap;
    for (char c = 'a'; c <= 'z'; c++) {
        freqMap[c] = 0;
    }

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "\n警告：未找到文本文件 \"" << filename << "\"，使用默认频率表！" << endl;
        freqMap = {
            {'a', 120}, {'b', 22}, {'c', 32}, {'d', 42}, {'e', 128},
            {'f', 22}, {'g', 20}, {'h', 40}, {'i', 76}, {'j', 4},
            {'k', 6}, {'l', 32}, {'m', 24}, {'n', 67}, {'o', 75},
            {'p', 20}, {'q', 2}, {'r', 56}, {'s', 56}, {'t', 73},
            {'u', 28}, {'v', 8}, {'w', 18}, {'x', 2}, {'y', 16}, {'z', 1}
        };
        return freqMap;
    }

    char c;
    while (file.get(c)) {
        if (isalpha(c)) {
            freqMap[tolower(c)]++;
        }
    }
    file.close();

    cout << "\n=== 字符频率统计结果 ===" << endl;
    for (auto& pair : freqMap) {
        if (pair.second > 0) {
            cout << pair.first << ": " << pair.second << "次" << endl;
        }
    }
    return freqMap;
}

// 本地定义遍历函数（避免依赖库中的重复定义）
template <typename T>
void printElem(T& e) {
    cout << e << " ";
}

template <typename T>
struct Inc {
    void operator()(T& e) {
        e++;
    }
};

int main() {
    cout << "===== 哈夫曼编码实验（exp2）=====" << endl;

    // 1. 统计字符频率
    map<char, int> freqMap = countCharFrequency("Ihaveadream.txt");

    // 2. 构建哈夫曼树
    HuffTree<char> huffTree;
    huffTree.build(freqMap);

    // 3. 生成编码表
    map<char, string> codeMap = huffTree.getCodeMap();
    cout << "\n=== 哈夫曼编码表（字符 -> 二进制编码）===" << endl;
    for (auto& pair : codeMap) {
        cout << pair.first << ": " << pair.second << "（长度：" << pair.second.size() << "）" << endl;
    }

    // 4. 测试单词编码
    cout << "\n=== 单词编码测试 ===" << endl;
    string testWords[] = {"dream", "freedom", "justice", "hope", "ihaveadream", "america"};
    for (string word : testWords) {
        string encoded = huffTree.encode(word, codeMap);
        double compressionRate = huffTree.calculateCompressionRate(word, encoded);
        cout << word << " -> " << encoded 
             << "（长度：" << encoded.size() << "，压缩率：" << compressionRate << "%" << "）" << endl;
    }

    // 5. 自定义输入编码
    cout << "\n=== 自定义输入编码 ===" << endl;
    string input;
    cout << "请输入要编码的字符串（仅处理字母）：";
    cin >> input;
    string customEncoded = huffTree.encode(input, codeMap);
    double customRate = huffTree.calculateCompressionRate(input, customEncoded);
    cout << input << " -> " << customEncoded 
         << "（长度：" << customEncoded.size() << "，压缩率：" << customRate << "%" << "）" << endl;

    cout << "\n===== 实验结束 =====" << endl;
    return 0;
}

