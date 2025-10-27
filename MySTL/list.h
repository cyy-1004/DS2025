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
    // Default constructor (for sentinel nodes)
    ListNode() : pred(nullptr), succ(nullptr) {}
    // Constructor with data and optional neighbors
    ListNode(T e, ListNodePosi(T) p = nullptr, ListNodePosi(T) s = nullptr)
        : data(e), pred(p), succ(s) {}

    // Operations
    // Insert new node before current node
    ListNodePosi(T) insertAsPred(const T& e) {
        ListNodePosi(T) newNode = new ListNode(e, this->pred, this);
        if (this->pred != nullptr)  // Avoid null pointer for sentinel nodes
            this->pred->succ = newNode;
        this->pred = newNode;
        return newNode;
    }

    // Insert new node after current node
    ListNodePosi(T) insertAsSucc(const T& e) {
        ListNodePosi(T) newNode = new ListNode(e, this, this->succ);
        if (this->succ != nullptr)  // Avoid null pointer for sentinel nodes
            this->succ->pred = newNode;
        this->succ = newNode;
        return newNode;
    }
};

// List class template
template <typename T>
class List {
private:
    int _size;               // Number of valid elements
    ListNodePosi(T) header;  // Head sentinel (no valid data)
    ListNodePosi(T) trailer; // Tail sentinel (no valid data)

    // Initialize list with sentinel nodes
    void init() {
        header = new ListNode<T>();
        trailer = new ListNode<T>();
        header->succ = trailer;
        trailer->pred = header;
        _size = 0;
    }

    // Clear all valid nodes (preserve sentinels)
    int clear() {
        int oldSize = _size;
        while (_size > 0) {
            remove(header->succ); // Remove from head to tail
        }
        return oldSize;
    }

    // Copy n nodes starting from position p
    void copyNodes(ListNodePosi(T) p, int n) {
        init(); // Initialize sentinels first
        while (n-- > 0) {
            insertAsLast(p->data); // Copy to tail sequentially
            p = p->succ;
        }
    }

    // Merge two sorted sublists: current list's [p, p+n) and L's [q, q+m)
    void merge(ListNodePosi(T)& p, int n, List<T>& L, ListNodePosi(T) q, int m) {
        ListNodePosi(T) pp = p->pred; // Save predecessor of p
        while (m > 0) {
            if (n > 0 && p->data <= q->data) { // Element in current list is smaller
                if (q == (p = p->succ)) break; // Avoid cross-referencing
                n--;
            } else { // Element in L is smaller, move to current list
                insertBefore(p, L.remove((q = q->succ)->pred));
                m--;
            }
        }
        p = pp->succ; // Restore p to start of merged sublist
    }

    // Recursive merge sort for sublist [p, p+n)
    void mergeSort(ListNodePosi(T)& p, int n) {
        if (n < 2) return; // Single node is naturally sorted
        int m = n >> 1;    // Split into two halves
        ListNodePosi(T) q = p;
        for (int i = 0; i < m; i++) q = q->succ; // Find split point
        mergeSort(p, m);    // Sort left sublist
        mergeSort(q, n - m); // Sort right sublist
        merge(p, m, *this, q, n - m); // Merge two sorted sublists
    }

    // Find maximum node in sublist [p, p+n)
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

    // Selection sort for sublist [p, p+n)
    void selectionSort(ListNodePosi(T) p, int n) {
        ListNodePosi(T) head = p->pred; // Head of sorting range
        ListNodePosi(T) tail = p;
        for (int i = 0; i < n; i++) tail = tail->succ; // Tail of sorting range
        while (n > 1) { // Repeatedly select maximum and move to tail
            ListNodePosi(T) maxNode = selectMax(head->succ, n);
            insertBefore(tail, remove(maxNode));
            tail = tail->pred;
            n--;
        }
    }

    // Insertion sort for sublist [p, p+n)
    void insertionSort(ListNodePosi(T) p, int n) {
        for (int r = 0; r < n; r++) {
            // Find insertion position and insert current element
            insertAfter(search(p->data, r, p), p->data);
            p = p->succ;   // Move to next element
            remove(p->pred); // Remove original node (already copied)
        }
    }

public:
    // 1. Constructors
    // Default constructor (empty list)
    List() { init(); }

    // Copy constructor (copy n nodes starting from p)
    List(ListNodePosi(T) p, int n) { copyNodes(p, n); }

    // Copy constructor (copy entire list L)
    List(const List<T>& L) { copyNodes(L.first(), L._size); }

    // Copy constructor (copy n nodes from L starting at rank r)
    List(const List<T>& L, Rank r, int n) {
        ListNodePosi(T) p = L[r]; // Convert rank to position
        copyNodes(p, n);
    }

    // 2. Destructor
    ~List() {
        clear();         // Clear all valid nodes
        delete header;   // Release head sentinel
        delete trailer;  // Release tail sentinel
    }

    // 3. Read-only access interfaces
    // Return current size
    int size() const { return _size; }

    // Check if list is empty
    bool empty() const { return _size == 0; }

    // Overload [] for rank access (O(r) time)
    T& operator[](Rank r) const {
        ListNodePosi(T) p = first();
        while (r-- > 0) p = p->succ; // Move r steps from head
        return p->data;
    }

    // Return first valid node position
    ListNodePosi(T) first() const { return header->succ; }

    // Return last valid node position
    ListNodePosi(T) last() const { return trailer->pred; }

    // Check if position p is valid (not sentinel or null)
    bool valid(ListNodePosi(T) p) const {
        return p != nullptr && p != header && p != trailer;
    }

    // Check disorder degree (0 means sorted)
    int disordered() const {
        int count = 0;
        ListNodePosi(T) p = first();
        while (p->succ != trailer) {
            if (p->data > p->succ->data) count++;
            p = p->succ;
        }
        return count;
    }

    // Unsorted search: find last occurrence of e in [p-n, p)
    ListNodePosi(T) find(const T& e) const {
        return find(e, _size, trailer);
    }

    ListNodePosi(T) find(const T& e, int n, ListNodePosi(T) p) const {
        while (n-- > 0) {
            p = p->pred;
            if (p->data == e) return p;
        }
        return nullptr; // Not found
    }

    // Sorted search: find last node <= e in [p-n, p)
    ListNodePosi(T) search(const T& e) const {
        return search(e, _size, trailer);
    }

    ListNodePosi(T) search(const T& e, int n, ListNodePosi(T) p) const {
        while (n-- > 0) {
            p = p->pred;
            if (p->data <= e) break; // Found candidate
        }
        return p; // May return header (all elements > e)
    }

    // 4. Mutable access interfaces
    // Insert as first node
    ListNodePosi(T) insertAsFirst(const T& e) {
        _size++;
        return header->insertAsSucc(e);
    }

    // Insert as last node
    ListNodePosi(T) insertAsLast(const T& e) {
        _size++;
        return trailer->insertAsPred(e);
    }

    // Insert e before position p
    ListNodePosi(T) insertBefore(ListNodePosi(T) p, const T& e) {
        _size++;
        return p->insertAsPred(e);
    }

    // Insert e after position p
    ListNodePosi(T) insertAfter(ListNodePosi(T) p, const T& e) {
        _size++;
        return p->insertAsSucc(e);
    }

    // Remove node p and return its data
    T remove(ListNodePosi(T) p) {
        T e = p->data; // Save data
        p->pred->succ = p->succ;
        p->succ->pred = p->pred;
        delete p;      // Release memory
        _size--;
        return e;      // Return saved data
    }

    // Merge with sorted list L
    void merge(List<T>& L) {
        merge(first(), _size, L, L.first(), L._size);
        _size += L._size;
        L._size = 0; // Clear original list L
    }

    // Sort sublist [p, p+n) with random algorithm
    void sort(ListNodePosi(T) p, int n) {
        switch (rand() % 3) {
        case 0: insertionSort(p, n); break;  // Insertion sort
        case 1: selectionSort(p, n); break;  // Selection sort
        case 2: mergeSort(p, n); break;      // Merge sort
        }
    }

    // Sort entire list
    void sort() { 
        if (_size > 1) sort(first(), _size); 
    }

    // Remove duplicates in unsorted list
    int deduplicate() {
        if (_size < 2) return 0;
        int oldSize = _size;
        ListNodePosi(T) p = first();
        Rank r = 0;
        while (p != trailer) {
            ListNodePosi(T) q = find(p->data, r, p);
            if (q != nullptr) {
                remove(q); // Remove duplicate
            } else {
                r++;       // No duplicate, increase range
            }
            p = p->succ;
        }
        return oldSize - _size; // Return number of removed nodes
    }

    // Remove consecutive duplicates in sorted list
    int uniquify() {
        if (_size < 2) return 0;
        int oldSize = _size;
        ListNodePosi(T) p = first();
        ListNodePosi(T) q = p->succ;
        while (q != trailer) {
            if (p->data == q->data) {
                remove(q); // Remove duplicate q
                q = p->succ; // Update q to new successor of p
            } else {
                p = q;     // Move to next pair
                q = q->succ;
            }
        }
        return oldSize - _size; // Return number of removed nodes
    }

    // Reverse the entire list
    void reverse() {
        if (_size < 2) return;

        // Swap predecessor and successor for each node
        ListNodePosi(T) current = first();
        ListNodePosi(T) temp = nullptr;
        while (current != trailer) {
            temp = current->succ;       // Save original successor
            current->succ = current->pred; // Swap successor to predecessor
            current->pred = temp;       // Swap predecessor to original successor
            current = temp;             // Move to next node
        }

        // Adjust sentinel pointers
        ListNodePosi(T) oldFirst = header->succ; // Original first node (now last)
        ListNodePosi(T) oldLast = trailer->pred; // Original last node (now first)

        header->succ = oldLast;
        oldLast->pred = header;

        trailer->pred = oldFirst;
        oldFirst->succ = trailer;
    }

    // 5. Traversal interfaces
    // Traverse with function pointer (supports partial modification)
    void traverse(void (*visit)(T&)) {
        for (ListNodePosi(T) p = first(); p != trailer; p = p->succ) {
            visit(p->data);
        }
    }

    // Traverse with function object (supports full modification)
    template <typename VST>
    void traverse(VST& visit) {
        for (ListNodePosi(T) p = first(); p != trailer; p = p->succ) {
            visit(p->data);
        }
    }
};

// Example traversal function: print element
template <typename T>
void printElement(T& e) {
    cout << e << " ";
}

// Example function object: increase element by 1
template <typename T>
struct Increase {
    void operator()(T& e) {
        e++;
    }
};
