#ifndef LIST_H
#define LIST_H
#include <iostream>
#include <cstdlib>
using namespace std;

// Define rank type (for array indices)
typedef int Rank;
// Abbreviation for list node pointer
#define ListNodePosi(T) ListNode<T>*

// List node structure template
template <typename T>
struct ListNode {
    // Member variables
    T data;               // Data stored in the node
    ListNodePosi(T) pred; // Pointer to predecessor node
    ListNodePosi(T) succ; // Pointer to successor node

    // Constructors
    ListNode() : pred(nullptr), succ(nullptr) {}
    ListNode(T e, ListNodePosi(T) p = nullptr, ListNodePosi(T) s = nullptr)
        : data(e), pred(p), succ(s) {}

    // Operations
    ListNodePosi(T) insertAsPred(const T& e) {
        ListNodePosi(T) newNode = new ListNode(e, this->pred, this);
        if (this->pred != nullptr)
            this->pred->succ = newNode;
        this->pred = newNode;
        return newNode;
    }

    ListNodePosi(T) insertAsSucc(const T& e) {
        ListNodePosi(T) newNode = new ListNode(e, this, this->succ);
        if (this->succ != nullptr)
            this->succ->pred = newNode;
        this->succ = newNode;
        return newNode;
    }
};

// List class template
template <typename T>
class List {
private:
    int _size;
    ListNodePosi(T) header;
    ListNodePosi(T) trailer;

    void init() {
        header = new ListNode<T>();
        trailer = new ListNode<T>();
        header->succ = trailer;
        trailer->pred = header;
        _size = 0;
    }

    int clear() {
        int oldSize = _size;
        while (_size > 0) {
            remove(header->succ);
        }
        return oldSize;
    }

    void copyNodes(ListNodePosi(T) p, int n) {
        init();
        while (n-- > 0) {
            insertAsLast(p->data);
            p = p->succ;
        }
    }

    void merge(ListNodePosi(T)& p, int n, List<T>& L, ListNodePosi(T) q, int m) {
        ListNodePosi(T) pp = p->pred;
        while (m > 0) {
            if (n > 0 && p->data <= q->data) {
                if (q == (p = p->succ)) break;
                n--;
            } else {
                insertBefore(p, L.remove((q = q->succ)->pred));
                m--;
            }
        }
        p = pp->succ;
    }

    void mergeSort(ListNodePosi(T)& p, int n) {
        if (n < 2) return;
        int m = n >> 1;
        ListNodePosi(T) q = p;
        for (int i = 0; i < m; i++) q = q->succ;
        mergeSort(p, m);
        mergeSort(q, n - m);
        merge(p, m, *this, q, n - m);
    }

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

    void selectionSort(ListNodePosi(T) p, int n) {
        ListNodePosi(T) head = p->pred;
        ListNodePosi(T) tail = p;
        for (int i = 0; i < n; i++) tail = tail->succ;
        while (n > 1) {
            ListNodePosi(T) maxNode = selectMax(head->succ, n);
            insertBefore(tail, remove(maxNode));
            tail = tail->pred;
            n--;
        }
    }

    void insertionSort(ListNodePosi(T) p, int n) {
        for (int r = 0; r < n; r++) {
            insertAfter(search(p->data, r, p), p->data);
            p = p->succ;
            remove(p->pred);
        }
    }

public:
    List() { init(); }
    List(ListNodePosi(T) p, int n) { copyNodes(p, n); }
    List(const List<T>& L) { copyNodes(L.first(), L._size); }
    List(const List<T>& L, Rank r, int n) {
        ListNodePosi(T) p = L[r];
        copyNodes(p, n);
    }
    ~List() {
        clear();
        delete header;
        delete trailer;
    }

    int size() const { return _size; }
    bool empty() const { return _size == 0; }
    T& operator[](Rank r) const {
        ListNodePosi(T) p = first();
        while (r-- > 0) p = p->succ;
        return p->data;
    }
    ListNodePosi(T) first() const { return header->succ; }
    ListNodePosi(T) last() const { return trailer->pred; }
    bool valid(ListNodePosi(T) p) const {
        return p != nullptr && p != header && p != trailer;
    }
    int disordered() const {
        int count = 0;
        ListNodePosi(T) p = first();
        while (p->succ != trailer) {
            if (p->data > p->succ->data) count++;
            p = p->succ;
        }
        return count;
    }
    ListNodePosi(T) find(const T& e) const {
        return find(e, _size, trailer);
    }
    ListNodePosi(T) find(const T& e, int n, ListNodePosi(T) p) const {
        while (n-- > 0) {
            p = p->pred;
            if (p->data == e) return p;
        }
        return nullptr;
    }
    ListNodePosi(T) search(const T& e) const {
        return search(e, _size, trailer);
    }
    ListNodePosi(T) search(const T& e, int n, ListNodePosi(T) p) const {
        while (n-- > 0) {
            p = p->pred;
            if (p->data <= e) break;
        }
        return p;
    }

    ListNodePosi(T) insertAsFirst(const T& e) {
        _size++;
        return header->insertAsSucc(e);
    }
    ListNodePosi(T) insertAsLast(const T& e) {
        _size++;
        return trailer->insertAsPred(e);
    }
    ListNodePosi(T) insertBefore(ListNodePosi(T) p, const T& e) {
        _size++;
        return p->insertAsPred(e);
    }
    ListNodePosi(T) insertAfter(ListNodePosi(T) p, const T& e) {
        _size++;
        return p->insertAsSucc(e);
    }
    T remove(ListNodePosi(T) p) {
        T e = p->data;
        p->pred->succ = p->succ;
        p->succ->pred = p->pred;
        delete p;
        _size--;
        return e;
    }
    void merge(List<T>& L) {
        merge(first(), _size, L, L.first(), L._size);
        _size += L._size;
        L._size = 0;
    }
    void sort(ListNodePosi(T) p, int n) {
        switch (rand() % 3) {
        case 0: insertionSort(p, n); break;
        case 1: selectionSort(p, n); break;
        case 2: mergeSort(p, n); break;
        }
    }
    void sort() {
        if (_size > 1) sort(first(), _size);
    }
    int deduplicate() {
        if (_size < 2) return 0;
        int oldSize = _size;
        ListNodePosi(T) p = first();
        Rank r = 0;
        while (p != trailer) {
            ListNodePosi(T) q = find(p->data, r, p);
            if (q != nullptr) {
                remove(q);
            } else {
                r++;
            }
            p = p->succ;
        }
        return oldSize - _size;
    }
    int uniquify() {
        if (_size < 2) return 0;
        int oldSize = _size;
        ListNodePosi(T) p = first();
        ListNodePosi(T) q = p->succ;
        while (q != trailer) {
            if (p->data == q->data) {
                remove(q);
                q = p->succ;
            } else {
                p = q;
                q = q->succ;
            }
        }
        return oldSize - _size;
    }
    void reverse() {
        if (_size < 2) return;
        ListNodePosi(T) current = first();
        ListNodePosi(T) temp = nullptr;
        while (current != trailer) {
            temp = current->succ;
            current->succ = current->pred;
            current->pred = temp;
            current = temp;
        }
        ListNodePosi(T) oldFirst = header->succ;
        ListNodePosi(T) oldLast = trailer->pred;
        header->succ = oldLast;
        oldLast->pred = header;
        trailer->pred = oldFirst;
        oldFirst->succ = trailer;
    }

    void traverse(void (*visit)(T&)) {
        for (ListNodePosi(T) p = first(); p != trailer; p = p->succ) {
            visit(p->data);
        }
    }
    template <typename VST>
    void traverse(VST& visit) {
        for (ListNodePosi(T) p = first(); p != trailer; p = p->succ) {
            visit(p->data);
        }
    }
};

#endif // LIST_H
