#include <iostream>
#include <cstdlib>
using namespace std;

// 秩的定义（兼容向量结构）
typedef int Rank;
// 列表节点位置指针（简化类型声明）
#define ListNodePosi(T) ListNode<T>*

// 列表节点模板类
template <typename T>
struct ListNode {
    // 成员变量
    T data;               // 节点存储的数据
    ListNodePosi(T) pred; // 前驱节点指针
    ListNodePosi(T) succ; // 后继节点指针

    // 构造函数
    // 针对头哨兵/尾哨兵的空构造
    ListNode() : pred(nullptr), succ(nullptr) {}
    // 带数据及前后驱的构造（默认前后驱为空）
    ListNode(T e, ListNodePosi(T) p = nullptr, ListNodePosi(T) s = nullptr)
        : data(e), pred(p), succ(s) {
    }

    // 操作接口：在当前节点前插入新节点
    ListNodePosi(T) insertAsPred(const T& e) {
        ListNodePosi(T) newNode = new ListNode(e, this->pred, this);
        this->pred->succ = newNode;
        this->pred = newNode;
        return newNode;
    }

    // 操作接口：在当前节点后插入新节点
    ListNodePosi(T) insertAsSucc(const T& e) {
        ListNodePosi(T) newNode = new ListNode(e, this, this->succ);
        this->succ->pred = newNode;
        this->succ = newNode;
        return newNode;
    }
};
// 列表模板类
template <typename T>
class List {
private:
    int _size;               // 列表当前规模（有效节点数）
    ListNodePosi(T) header;  // 头哨兵节点（不存储有效数据）
    ListNodePosi(T) trailer;  // 尾哨兵节点（不存储有效数据）

    // 辅助函数：初始化列表（创建哨兵节点）
    void init() {
        header = new ListNode<T>();
        trailer = new ListNode<T>();
        header->succ = trailer;
        trailer->pred = header;
        _size = 0;
    }

    // 辅助函数：清除所有节点（保留哨兵）
    int clear() {
        int oldSize = _size;
        while (_size > 0) {
            remove(header->succ); // 从首节点开始删除
        }
        return oldSize;
    }

    // 辅助函数：复制从p开始的n个节点
    void copyNodes(ListNodePosi(T) p, int n) {
        init(); // 初始化哨兵节点
        while (n-- > 0) {
            insertAsLast(p->data); // 依次复制为尾节点
            p = p->succ;
        }
    }

    // 辅助函数：归并排序（有序子列表合并）
    void merge(ListNodePosi(T)& p, int n, List<T>& L, ListNodePosi(T) q, int m) {
        ListNodePosi(T) pp = p->pred; // 记录p的前驱（用于最终定位）
        while (m > 0) {
            if (n > 0 && p->data <= q->data) { // p所在子列表元素更小
                if (q == (p = p->succ)) break; // p到达q位置（避免交叉）
                n--;
            }
            else { // q所在子列表元素更小，将q转移到p前
                insertBefore(p, L.remove((q = q->succ)->pred));
                m--;
            }
        }
        p = pp->succ; // 恢复p为合并后子列表的起点
    }

    // 辅助函数：归并排序递归核心
    void mergeSort(ListNodePosi(T)& p, int n) {
        if (n < 2) return; // 单个节点自然有序
        int m = n >> 1;    // 二分拆分
        ListNodePosi(T) q = p;
        for (int i = 0; i < m; i++) q = q->succ; // 找到拆分点
        mergeSort(p, m);    // 左子列表排序
        mergeSort(q, n - m); // 右子列表排序
        merge(p, m, *this, q, n - m); // 合并两个有序子列表
    }

    // 辅助函数：选择排序（找最大节点）
    ListNodePosi(T) selectMax(ListNodePosi(T) p, int n) {
        ListNodePosi(T) maxNode = p;
        for (ListNodePosi(T) cur = p; n > 1; n--) {
            cur = cur->succ;
            if (cur->data >= maxNode->data) {
                maxNode = cur;
            }
        }
        return maxNode;
    }

    // 辅助函数：选择排序核心
    void selectionSort(ListNodePosi(T) p, int n) {
        ListNodePosi(T) head = p->pred; // 排序区间的前驱
        ListNodePosi(T) tail = p;
        for (int i = 0; i < n; i++) tail = tail->succ; // 排序区间的后继
        while (n > 1) { // 反复选择最大节点移到尾部
            ListNodePosi(T) maxNode = selectMax(head->succ, n);
            insertBefore(tail, remove(maxNode));
            tail = tail->pred;
            n--;
        }
    }

    // 辅助函数：插入排序核心
    void insertionSort(ListNodePosi(T) p, int n) {
        for (int r = 0; r < n; r++) {
            // 找到待插入位置并插入当前节点的副本
            insertAfter(search(p->data, r, p), p->data);
            p = p->succ;   // 移动到下一个节点
            remove(p->pred); // 删除原节点（已复制）
        }
    }

public:
    // 1. 构造函数
    // 默认构造（空列表）
    List() { init(); }

    // 复制构造（复制从p开始的n个节点）
    List(ListNodePosi(T) p, int n) { copyNodes(p, n); }

    // 复制构造（复制整个列表L）
    List(const List<T>& L) { copyNodes(L.first(), L._size); }

    // 复制构造（复制L中从秩r开始的n个节点）
    List(const List<T>& L, Rank r, int n) {
        ListNodePosi(T) p = L[r]; // 转换秩为位置
        copyNodes(p, n);
    }

    // 2. 析构函数
    ~List() {
        clear();         // 清除所有有效节点
        delete header;   // 释放头哨兵
        delete trailer;  // 释放尾哨兵
    }

    // 3. 只读访问接口
    // 返回列表规模
    int size() const { return _size; }

    // 判断列表是否为空
    bool empty() const { return _size == 0; }

    // 重载下标：循秩访问（效率O(r)，需慎用）
    T& operator[](Rank r) const {
        ListNodePosi(T) p = first();
        while (r-- > 0) p = p->succ; // 从首节点向后移动r步
        return p->data;
    }

    // 返回首节点位置
    ListNodePosi(T) first() const { return header->succ; }

    // 返回末节点位置
    ListNodePosi(T) last() const { return trailer->pred; }

    // 判断位置p是否合法（非哨兵）
    bool valid(ListNodePosi(T) p) const {
        return p != nullptr && p != header && p != trailer;
    }

    // 判断列表是否有序（非降序）
    int disordered() const {
        int count = 0;
        ListNodePosi(T) p = first();
        while (p->succ != trailer) {
            if (p->data > p->succ->data) count++;
            p = p->succ;
        }
        return count; // 逆序对数量（0表示有序）
    }

    // 无序查找：在p的前n个前驱中找等于e的最后一个节点
    ListNodePosi(T) find(const T& e) const {
        return find(e, _size, trailer);
    }

    ListNodePosi(T) find(const T& e, int n, ListNodePosi(T) p) const {
        while (n-- > 0) {
            p = p->pred;
            if (p->data == e) return p;
        }
        return nullptr; // 未找到
    }

    // 有序查找：在p的前n个前驱中找不大于e的最大节点
    ListNodePosi(T) search(const T& e) const {
        return search(e, _size, trailer);
    }

    ListNodePosi(T) search(const T& e, int n, ListNodePosi(T) p) const {
        while (n-- > 0) {
            p = p->pred;
            if (p->data <= e) break; // 找到不大于e的节点
        }
        return p; // 可能是header（表示所有元素都大于e）
    }

    // 4. 可写访问接口
    // 作为首节点插入
    ListNodePosi(T) insertAsFirst(const T& e) {
        _size++;
        return header->insertAsSucc(e);
    }

    // 作为末节点插入
    ListNodePosi(T) insertAsLast(const T& e) {
        _size++;
        return trailer->insertAsPred(e);
    }

    // 在p节点前插入e
    ListNodePosi(T) insertBefore(ListNodePosi(T) p, const T& e) {
        _size++;
        return p->insertAsPred(e);
    }

    // 在p节点后插入e
    ListNodePosi(T) insertAfter(ListNodePosi(T) p, const T& e) {
        _size++;
        return p->insertAsSucc(e);
    }

    // 删除p节点，返回其数据
    T remove(ListNodePosi(T) p) {
        T e = p->data; // 备份数据
        p->pred->succ = p->succ;
        p->succ->pred = p->pred;
        delete p;      // 释放节点
        _size--;
        return e;      // 返回备份数据
    }

    // 归并两个有序列表（当前列表与L合并）
    void merge(List<T>& L) {
        merge(first(), _size, L, L.first(), L._size);
        _size += L._size;
        L._size = 0; // 清空原列表L
    }

    // 排序接口（统一入口，随机选择算法）
    void sort(ListNodePosi(T) p, int n) {
        switch (rand() % 3) {
        case 0: insertionSort(p, n); break;  // 插入排序
        case 1: selectionSort(p, n); break;  // 选择排序
        case 2: mergeSort(p, n); break;      // 归并排序
        }
    }

    // 整体排序
    void sort() { sort(first(), _size); }

    // 无序列表去重（删除重复节点）
    int deduplicate() {
        if (_size < 2) return 0;
        int oldSize = _size;
        ListNodePosi(T) p = first();
        Rank r = 0;
        while (p != trailer) {
            ListNodePosi(T) q = find(p->data, r, p);
            if (q != nullptr) {
                remove(q); // 找到重复节点，删除
            }
            else {
                r++;       // 无重复，秩递增
            }
            p = p->succ;
        }
        return oldSize - _size; // 返回删除的节点数
    }

    // 有序列表去重（批量删除相邻重复节点）
    int uniquify() {
        if (_size < 2) return 0;
        int oldSize = _size;
        ListNodePosi(T) p = first();
        ListNodePosi(T) q = p->succ;
        while (q != trailer) {
            if (p->data == q->data) {
                remove(q); // 重复节点，删除q
                q = p->succ; // q指向p的新后继
            }
            else {
                p = q;     // 不重复，移动到下一对
                q = q->succ;
            }
        }
        return oldSize - _size; // 返回删除的节点数
    }

    // 列表反转（习题实现）
    void reverse() {
        if (_size < 2) return;

        // 1. 逐个反转每个节点的前驱和后继指针
        ListNodePosi(T) current = first();
        ListNodePosi(T) temp = nullptr;
        while (current != trailer) {
            temp = current->succ;       // 保存原后继
            current->succ = current->pred; // 后继指向原前驱
            current->pred = temp;       // 前驱指向原后继（已保存的temp）
            current = temp;             // 移动到下一个节点
        }

        // 2. 调整哨兵节点指向（关键步骤）
        ListNodePosi(T) oldFirst = header->succ; // 原首节点（反转后变为尾节点）
        ListNodePosi(T) oldLast = trailer->pred; // 原尾节点（反转后变为首节点）

        // 头哨兵的后继指向新首节点（原尾节点）
        header->succ = oldLast;
        // 新首节点的前驱指向头哨兵
        oldLast->pred = header;

        // 尾哨兵的前驱指向新尾节点（原首节点）
        trailer->pred = oldFirst;
        // 新尾节点的后继指向尾哨兵
        oldFirst->succ = trailer;
    }

    // 5. 遍历接口
    // 函数指针版遍历（支持局部修改）
    void traverse(void (*visit)(T&)) {
        for (ListNodePosi(T) p = first(); p != trailer; p = p->succ) {
            visit(p->data);
        }
    }

    // 函数对象版遍历（支持全局修改）
    template <typename VST>
    void traverse(VST& visit) {
        for (ListNodePosi(T) p = first(); p != trailer; p = p->succ) {
            visit(p->data);
        }
    }
};

// 遍历操作示例：打印元素
template <typename T>
void printElement(T& e) {
    cout << e << " ";
}

// 函数对象示例：元素值加1
template <typename T>
struct Increase {
    void operator()(T& e) {
        e++;
    }
};
