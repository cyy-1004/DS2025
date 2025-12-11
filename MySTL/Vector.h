#ifndef VECTOR_H
#define VECTOR_H
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <algorithm> // 用于 std::max（兼容不同编译器）
using namespace std;

// 秩类型（数组索引）
typedef int Rank;
// 默认初始容量
const int DEFAULT_CAPACITY = 3;

template <typename T>
class Vector {
private:
    Rank _size;     // 当前元素个数
    Rank _capacity; // 数组容量（最大可存储元素数）
    T* _elem;       // 动态数组指针（存储元素）

    // 扩容：当元素个数达到容量时，容量翻倍
    void expand() {
        if (_size < _capacity) return; // 无需扩容
        // 初始容量至少为 DEFAULT_CAPACITY
        _capacity = max(_capacity, DEFAULT_CAPACITY);
        T* oldElem = _elem; // 保存原数组地址
        _capacity <<= 1;    // 容量翻倍（等价于 _capacity *= 2）
        _elem = new T[_capacity]; // 分配新内存

        // 复制原数组元素到新数组
        for (Rank i = 0; i < _size; i++) {
            _elem[i] = oldElem[i];
        }

        delete[] oldElem; // 释放原数组内存
    }

    // 缩容：当元素个数小于容量的 1/4 时，容量减半（节省内存）
    void shrink() {
        if (_capacity <= DEFAULT_CAPACITY || _size * 4 > _capacity) return;
        T* oldElem = _elem;
        _capacity >>= 1; // 容量减半
        _elem = new T[_capacity];

        // 复制元素
        for (Rank i = 0; i < _size; i++) {
            _elem[i] = oldElem[i];
        }

        delete[] oldElem;
    }

public:
    // 1. 构造函数
    // 默认构造：初始容量为 DEFAULT_CAPACITY
    Vector(Rank c = DEFAULT_CAPACITY) : _capacity(c), _size(0) {
        _elem = new T[_capacity]; // 分配初始内存
    }

    // 从数组构造：传入数组指针和元素个数
    Vector(T* A, Rank n) : _capacity(max(n, DEFAULT_CAPACITY)), _size(n) {
        _elem = new T[_capacity];
        for (Rank i = 0; i < n; i++) {
            _elem[i] = A[i]; // 复制数组元素
        }
    }

    // 拷贝构造：从另一个 Vector 复制
    Vector(const Vector<T>& V) : _capacity(V._capacity), _size(V._size) {
        _elem = new T[_capacity];
        for (Rank i = 0; i < _size; i++) {
            _elem[i] = V._elem[i]; // 深拷贝元素
        }
    }

    // 范围拷贝：从 Vector V 的第 r 个元素开始，复制 n 个元素
    Vector(const Vector<T>& V, Rank r, Rank n) {
        if (r < 0 || r + n > V._size) {
            cerr << "Vector 范围拷贝：索引越界！" << endl;
            _capacity = DEFAULT_CAPACITY;
            _size = 0;
            _elem = new T[_capacity];
            return;
        }
        _capacity = max(n, DEFAULT_CAPACITY);
        _size = n;
        _elem = new T[_capacity];
        for (Rank i = 0; i < n; i++) {
            _elem[i] = V._elem[r + i];
        }
    }

    // 2. 析构函数：释放动态内存
    ~Vector() {
        delete[] _elem; // 释放数组内存
        _elem = nullptr;
        _size = 0;
        _capacity = 0;
    }

    // 3. 只读访问接口
    // 返回当前元素个数
    Rank size() const { return _size; }

    // 判断是否为空
    bool empty() const { return _size == 0; }

    // 返回当前容量
    Rank capacity() const { return _capacity; }

    // 重载 [] 运算符：随机访问第 r 个元素（O(1)）
    T& operator[](Rank r) const {
        if (r < 0 || r >= _size) {
            cerr << "Vector 访问：索引越界！" << endl;
            exit(1); // 严重错误，终止程序
        }
        return _elem[r];
    }

    // 查找元素 e：返回第一个匹配元素的秩（O(n)），未找到返回 -1
    Rank find(const T& e) const {
        for (Rank i = 0; i < _size; i++) {
            if (_elem[i] == e) return i;
        }
        return -1;
    }

    // 范围查找：在 [lo, hi) 区间内查找 e，返回第一个匹配秩（O(n)）
    Rank find(const T& e, Rank lo, Rank hi) const {
        if (lo < 0 || hi > _size || lo >= hi) return -1;
        for (Rank i = lo; i < hi; i++) {
            if (_elem[i] == e) return i;
        }
        return -1;
    }

    // 统计元素 e 的出现次数（O(n)）
    Rank count(const T& e) const {
        Rank cnt = 0;
        for (Rank i = 0; i < _size; i++) {
            if (_elem[i] == e) cnt++;
        }
        return cnt;
    }

    // 4. 可修改访问接口
    // 插入元素 e 到秩 r 位置（O(n)），返回插入后的秩
    Rank insert(Rank r, const T& e) {
        expand(); // 先扩容（确保有空间）
        if (r < 0 || r > _size) r = _size; // 越界时插入到末尾
        // 元素后移（从末尾到 r）
        for (Rank i = _size; i > r; i--) {
            _elem[i] = _elem[i - 1];
        }
        _elem[r] = e; // 插入新元素
        _size++;      // 元素个数+1
        return r;
    }

    // 在末尾插入元素 e（O(1)，扩容时 O(n)）
    Rank push_back(const T& e) {
        return insert(_size, e);
    }

    // 删除秩 r 位置的元素（O(n)），返回被删除的元素
    T remove(Rank r) {
        if (r < 0 || r >= _size) {
            cerr << "Vector 删除：索引越界！" << endl;
            exit(1);
        }
        T e = _elem[r]; // 保存被删除元素
        // 元素前移（从 r 到末尾）
        for (Rank i = r; i < _size - 1; i++) {
            _elem[i] = _elem[i + 1];
        }
        _size--;      // 元素个数-1
        shrink();     // 缩容（按需）
        return e;     // 返回被删除元素
    }

    // 删除 [lo, hi) 区间内的元素（O(n)），返回删除的元素个数
    Rank remove(Rank lo, Rank hi) {
        if (lo < 0 || hi > _size || lo >= hi) return 0;
        // 直接覆盖区间元素（无需逐个删除）
        while (hi < _size) {
            _elem[lo++] = _elem[hi++];
        }
        Rank delCnt = hi - lo; // 删除的元素个数
        _size = lo;            // 更新元素个数
        shrink();              // 缩容（按需）
        return delCnt;
    }

    // 删除末尾元素（O(1)，缩容时 O(n)）
    T pop_back() {
        if (empty()) {
            cerr << "Vector 弹出：空向量！" << endl;
            exit(1);
        }
        return remove(_size - 1);
    }

    // 替换秩 r 位置的元素为 e，返回原元素（O(1)）
    T replace(Rank r, const T& e) {
        if (r < 0 || r >= _size) {
            cerr << "Vector 替换：索引越界！" << endl;
            exit(1);
        }
        T old = _elem[r];
        _elem[r] = e;
        return old;
    }

    // 清空所有元素（O(1)，仅修改 size，不释放内存）
    void clear() {
        _size = 0;
    }

    // 5. 排序算法
    // 选择排序：升序（O(n2)）
    void selection_sort() {
        for (Rank i = 0; i < _size - 1; i++) {
            Rank minRank = i; // 记录当前最小元素的秩
            // 查找 [i+1, _size) 中的最小元素
            for (Rank j = i + 1; j < _size; j++) {
                if (_elem[j] < _elem[minRank]) minRank = j;
            }
            swap(_elem[i], _elem[minRank]); // 交换到当前位置
        }
    }

    // 归并排序：升序（O(n log n)）
    void merge_sort(Rank lo, Rank hi) {
        if (hi - lo < 2) return; // 单个元素无需排序
        Rank mid = (lo + hi) >> 1; // 二分区间
        merge_sort(lo, mid);       // 左区间排序
        merge_sort(mid, hi);       // 右区间排序
        merge(lo, mid, hi);        // 合并两个有序区间
    }

    // 归并排序（重载：排序整个向量）
    void merge_sort() {
        merge_sort(0, _size);
    }

    // 辅助函数：合并 [lo, mid) 和 [mid, hi) 两个有序区间
    void merge(Rank lo, Rank mid, Rank hi) {
        T* temp = new T[hi - lo]; // 临时数组存储合并结果
        Rank i = lo, j = mid, k = 0;

        // 合并两个区间（按升序）
        while (i < mid && j < hi) {
            temp[k++] = (_elem[i] <= _elem[j]) ? _elem[i++] : _elem[j++];
        }

        // 复制左区间剩余元素
        while (i < mid) temp[k++] = _elem[i++];
        // 复制右区间剩余元素
        while (j < hi) temp[k++] = _elem[j++];

        // 把合并结果复制回原数组
        for (k = 0; k < hi - lo; k++) {
            _elem[lo + k] = temp[k];
        }

        delete[] temp; // 释放临时数组内存
    }

    // 6. 遍历与输出
    // 遍历：传入函数指针（对每个元素执行操作）
    void traverse(void (*visit)(T&)) {
        for (Rank i = 0; i < _size; i++) {
            visit(_elem[i]);
        }
    }

    // 遍历：传入函数对象（支持更复杂操作）
    template <typename VST>
    void traverse(VST& visit) {
        for (Rank i = 0; i < _size; i++) {
            visit(_elem[i]);
        }
    }

    // 输出向量元素（空格分隔）
    void print() const {
        if (empty()) {
            cout << "Vector is empty" << endl;
            return;
        }
        for (Rank i = 0; i < _size; i++) {
            cout << _elem[i] << " ";
        }
        cout << endl;
    }

    // 7. 重载赋值运算符（深拷贝）
    Vector<T>& operator=(const Vector<T>& V) {
        if (this == &V) return *this; // 避免自赋值

        // 释放当前内存
        delete[] _elem;

        // 复制 V 的容量、大小和元素
        _capacity = V._capacity;
        _size = V._size;
        _elem = new T[_capacity];
        for (Rank i = 0; i < _size; i++) {
            _elem[i] = V._elem[i];
        }

        return *this;
    }
};

// 示例：遍历函数（打印元素）
template <typename T>
void printElement(T& e) {
    cout << e << " ";
}

// 示例：遍历函数对象（元素自增 1）
template <typename T>
struct Increase {
    void operator()(T& e) {
        e++;
    }
};

#endif // VECTOR_H
