template <class T>
class FragmentLinkedList : IList<T>
{
public:
    class Node;
    class Iterator;

protected:
    Node** fragmentPointers;
    int fragmentMaxSize;
    int count;

public:
    FragmentLinkedList(int fragmentMaxSize = 5)
    {
        this->count = 0;
        this->fragmentMaxSize = fragmentMaxSize;
        this->fragmentPointers = new Node * [2];
        this->fragmentPointers[0] = NULL;
        this->fragmentPointers[1] = NULL;
    }
    virtual ~FragmentLinkedList();
    virtual void add(const T& element);
    virtual void add(int index, const T& element);
    virtual bool empty();
    virtual T removeAt(int index);
    virtual bool removeItem(const T& item);
    virtual int size();
    virtual void clear();
    virtual T get(int index);
    virtual void set(int index, const T& element);
    virtual int indexOf(const T& item);
    virtual bool contains(const T& item);
    Iterator begin(int index = 0) {
        return Iterator(index, this, true);
    }
    Iterator end(int index = -1) {
        return Iterator(index, this, false);
    }
    virtual string toString();
public:
    class Node
    {
    private:
        T data;
        Node* next;
        Node* prev;
        friend class FragmentLinkedList<T>;

    public:
        Node()
        {
            next = 0;
            prev = 0;
        }
        Node(Node* next, Node* prev)
        {
            this->next = next;
            this->prev = prev;
        }
        Node(T data, Node* next, Node* prev)
        {
            this->data = data;
            this->next = next;
            this->prev = prev;
        }
    };

    class Iterator
    {
    private:
        FragmentLinkedList<T>* pList;
        Node* pNode;
        int index;
    public:
        Iterator(FragmentLinkedList<T>* pList = 0, bool begin = true);
        Iterator(int fragmentIndex = 0, FragmentLinkedList<T>* pList = 0, bool begin = true);
        Iterator& operator=(const Iterator& iterator);
        T& operator*();
        bool operator!=(const Iterator& iterator);
        void remove();
        void set(const T& element);
        Iterator& operator++();
        Iterator operator++(int);
    };
};
template <class T>
FragmentLinkedList<T>::~FragmentLinkedList() {
    clear();
}
template <class T>
string FragmentLinkedList<T>::toString()
{
    stringstream ss;
    cout << "[";
    Node* ptr = this->fragmentPointers[0];

    if (this->count == 0)
        cout << "]";

    // TODO
    if (this->count == 0) return ss.str();
    int a = this->count;
    while (ptr->next != NULL) {
        cout << ptr->data << ", ";
        ptr = ptr->next;
        a--;
    }
    cout << ptr->data;
    if (a > 0) cout << "]";
    //int numFP = (this->count - 1) / fragmentMaxSize + 2;
    //for (int i = 0; i < numFP; i++) {
    //    cout << "fragmentPointer[" << i << "]" << fragmentPointers[i]->data << endl;
    //}
    // END: TODO
    return ss.str();

}
template <class T>
void FragmentLinkedList<T>::add(const T& element) {
    if (empty()) {
        this->fragmentPointers[0] = new Node(element, NULL, NULL);
        this->fragmentPointers[1] = this->fragmentPointers[0];
        this->count++;
    }
    else {
        Node* newnode = this->fragmentPointers[0];
        while (newnode->next != NULL) {
            newnode = newnode->next;
        }
        newnode->next = new Node(element, NULL, newnode);
        this->count++;
        // updated FP
        bool flag = 0;
        Node* pFP = this->fragmentPointers[0];
        int numFP = (this->count - 1) / fragmentMaxSize + 2;
        fragmentPointers = new Node * [numFP];
        this->fragmentPointers[0] = pFP;
        for (int i = 1; i < numFP; i++) {
            for (int j = 0; j < fragmentMaxSize; j++) {
                if (pFP->next == NULL) {
                    flag = 1;
                    break;
                }
                pFP = pFP->next;
            }
            fragmentPointers[i] = pFP;
            if (flag) {
                fragmentPointers[numFP - 1] = pFP;
                break;
            }
        }
        //updated FP
    }

}
template <class T>
void FragmentLinkedList<T>::add(int index, const T& element) {
    // not update fragment
    if (empty()) {
        this->fragmentPointers[0] = new Node(element, NULL, NULL);
        this->fragmentPointers[1] = this->fragmentPointers[0];
        this->count++;
    }
    else if (index == 0) {
        Node* newnode = new Node(element, this->fragmentPointers[0], NULL);
        newnode->next = this->fragmentPointers[0];
        this->fragmentPointers[0]->prev = newnode;
        this->fragmentPointers[0] = newnode;
        this->count++;
        //updated FP
        bool flag = 0;
        Node* pFP = this->fragmentPointers[0];
        int numFP = (this->count - 1) / fragmentMaxSize + 2;
        fragmentPointers = new Node * [numFP];
        this->fragmentPointers[0] = pFP;
        for (int i = 1; i < numFP; i++) {
            for (int j = 0; j < fragmentMaxSize; j++) {
                if (i == 1) break;
                if (pFP->next == NULL) {
                    flag = 1;
                    break;
                }
                pFP = pFP->next;
            }
            fragmentPointers[i] = pFP;
            if (flag) {
                fragmentPointers[numFP - 1] = pFP;
                break;
            }
        }
        //updated FP
    }
    else if (this->count > index && index > 0) {
        //not update fragment
        Node* ptr = this->fragmentPointers[0];
        for (int i = 0; i < index; i++) {
            ptr = ptr->next;
        }
        Node* newnode = new Node(element, ptr->next, ptr->next->prev);
        newnode->prev = ptr->prev;
        newnode->next = ptr->prev->next;
        ptr->prev->next = newnode;
        ptr->prev = newnode;
        this->count++;
        //updated FP
        bool flag = 0;
        Node* pFP = this->fragmentPointers[0];
        int numFP = (this->count - 1) / fragmentMaxSize + 2;
        fragmentPointers = new Node * [numFP];
        this->fragmentPointers[0] = pFP;
        for (int i = 1; i < numFP; i++) {
            for (int j = 0; j < fragmentMaxSize; j++) {
                if (pFP->next == NULL) {
                    flag = 1;
                    break;
                }
                pFP = pFP->next;
            }
            fragmentPointers[i] = pFP;
            if (flag) {
                fragmentPointers[numFP - 1] = pFP;
                break;
            }
        }
        //updated FP
    }
    else if (this->count == index) {
        Node* newnode = this->fragmentPointers[0];
        while (newnode->next != NULL) {
            newnode = newnode->next;
        }
        newnode->next = new Node(element, NULL, newnode);
        this->count++;
        // not update FP
        bool flag = 0;
        Node* pFP = this->fragmentPointers[0];
        int numFP = (this->count - 1) / fragmentMaxSize + 2;
        fragmentPointers = new Node * [numFP];
        this->fragmentPointers[0] = pFP;
        for (int i = 1; i < numFP; i++) {
            for (int j = 0; j < fragmentMaxSize; j++) {
                if (pFP->next == NULL) {
                    flag = 1;
                    break;
                }
                pFP = pFP->next;
            }
            fragmentPointers[i] = pFP;
            if (flag) {
                fragmentPointers[numFP - 1] = pFP;
                break;
            }
        }
        //updated FP
    }
    else {
        throw "The index is out of range!";
    }

}
template <class T>
bool FragmentLinkedList<T>::empty() {
    if (this->count > 0) return false;
    return true;
}
template <class T>
bool FragmentLinkedList<T>::removeItem(const T& item) {
    Node* ptr = this->fragmentPointers[0];
    int numFP = (this->count - 1) / fragmentMaxSize + 2;
    if (fragmentPointers[numFP - 1]->data == item) {
        fragmentPointers[numFP - 1] = fragmentPointers[numFP - 1]->prev;
        fragmentPointers[numFP - 1]->next = NULL;
        count--;
        //update FP
        bool flag = 0;
        Node* pFP = this->fragmentPointers[0];
        int numFP = (this->count - 1) / fragmentMaxSize + 2;
        fragmentPointers = new Node * [numFP];
        this->fragmentPointers[0] = pFP;
        for (int i = 1; i < numFP; i++) {
            for (int j = 0; j < fragmentMaxSize; j++) {
                if (pFP->next == NULL) {
                    flag = 1;
                    break;
                }
                pFP = pFP->next;
            }
            fragmentPointers[i] = pFP;
            if (flag) {
                fragmentPointers[numFP - 1] = pFP;
                break;
            }
        }
        // updated
        return true;
    }
    else if (fragmentPointers[0]->data == item) {
        this->fragmentPointers[0] = ptr->next;
        ptr->next = NULL;
        this->count--;
        //update FP
        bool flag = 0;
        Node* pFP = this->fragmentPointers[0];
        int numFP = (this->count - 1) / fragmentMaxSize + 2;
        fragmentPointers = new Node * [numFP];
        this->fragmentPointers[0] = pFP;
        for (int i = 1; i < numFP; i++) {
            for (int j = 0; j < fragmentMaxSize; j++) {
                if (pFP->next == NULL) {
                    flag = 1;
                    break;
                }
                pFP = pFP->next;
            }
            fragmentPointers[i] = pFP;
            if (flag) {
                fragmentPointers[numFP - 1] = pFP;
                break;
            }
        }
        //updated
        return true;
    }
    else {
        while (ptr != NULL) {
            if (ptr->data == item) {
                ptr->next->prev = ptr->prev;
                ptr->prev->next = ptr->next;
                this->count--;
                //updated FP
                bool flag = 0;
                Node* pFP = this->fragmentPointers[0];
                int numFP = (this->count - 1) / fragmentMaxSize + 2;
                fragmentPointers = new Node * [numFP];
                this->fragmentPointers[0] = pFP;
                for (int i = 1; i < numFP; i++) {
                    for (int j = 0; j < fragmentMaxSize; j++) {
                        if (pFP->next == NULL) {
                            flag = 1;
                            break;
                        }
                        pFP = pFP->next;
                    }
                    fragmentPointers[i] = pFP;
                    if (flag) {
                        fragmentPointers[numFP - 1] = pFP;
                        break;
                    }
                }
                //updated FP
                return true;
            }
            ptr = ptr->next;
        }
    }
    return false;
}
template <class T>
int FragmentLinkedList<T>::size() {
    return this->count;
}
template <class T>
void FragmentLinkedList<T>::clear() {
    this->fragmentPointers = new Node * [2];
    this->fragmentPointers[0] = NULL;
    this->fragmentPointers[1] = NULL;
    this->count = 0;
}
template <class T>
T FragmentLinkedList<T>::removeAt(int index) {
    if (this->count <= index) throw "The index is out of range!";
    Node* ptr = this->fragmentPointers[0];
    if (index == 0) {
        this->fragmentPointers[0] = ptr->next;
        ptr->next = NULL;
        this->count--;
    }
    else if (this->count - 1 == index) {
        int numFP = (this->count - 1) / fragmentMaxSize + 2;
        fragmentPointers[numFP - 1] = fragmentPointers[numFP - 1]->prev;
        fragmentPointers[numFP - 1]->next = NULL;
        count--;
    }
    else {
        for (int i = 0; i < index; i++) {
            ptr = ptr->next;
        }
        ptr->prev->next = ptr->next;
        ptr->next->prev = ptr->prev;
        this->count--;
    }

    //updated FP
    bool flag = 0;
    Node* pFP = this->fragmentPointers[0];
    int numFP = (this->count - 1) / fragmentMaxSize + 2;
    fragmentPointers = new Node * [numFP];
    this->fragmentPointers[0] = pFP;
    for (int i = 1; i < numFP; i++) {
        for (int j = 0; j < fragmentMaxSize; j++) {
            if (pFP->next == NULL) {
                flag = 1;
                break;
            }
            pFP = pFP->next;
        }
        fragmentPointers[i] = pFP;
        if (flag) {
            fragmentPointers[numFP - 1] = pFP;
            break;
        }
    }
    //end update
    return ptr->data;
}
template <class T>
T FragmentLinkedList<T>::get(int index) {
    if (this->count <= index || index < 0) throw "The index is out of range!";
    Node* ptr = this->fragmentPointers[0];
    for (int i = 0; i < index; i++) {
        ptr = ptr->next;
    }
    return ptr->data;
}
template <class T>
void FragmentLinkedList<T>::set(int index, const T& element) {
    if (this->count <= index || index < 0) throw "The index is out of range!";
    Node* ptr = this->fragmentPointers[0];
    for (int i = 0; i < index; i++) {
        ptr = ptr->next;
    }
    ptr->data = element;
}
template <class T>
int FragmentLinkedList<T>::indexOf(const T& item) {
    int index = 0;
    Node* ptr = this->fragmentPointers[0];
    while (ptr->next != NULL) {
        if (ptr->data == item) return index;
        index++;
        ptr = ptr->next;
    }
    if (ptr->data == item) return index;
    return -1;
}
template <class T>
bool FragmentLinkedList<T>::contains(const T& item) {
    Node* ptr = this->fragmentPointers[0];
    while (ptr->next != NULL) {
        if (ptr->data == item) return true;
        ptr = ptr->next;
    }
    if (ptr->data == item)return true;
    return false;
}
template <class T>
FragmentLinkedList<T>::Iterator::Iterator(FragmentLinkedList<T>* pList, bool begin) {
    this->pList = pList;
    if (begin) {
        pNode = this->pList->fragmentPointers[0];
        this->index = 0;
    }
    else {
        pNode = NULL;
        index = pList->count;
    }
}
template<class T>
FragmentLinkedList<T>::Iterator::Iterator(int fragmentindex, FragmentLinkedList<T>* pList, bool begin) {
    this->pList = pList;
    int a = (pList->count - 1) / pList->fragmentMaxSize;
    if (fragmentindex >= a && (pList->count % pList->fragmentMaxSize == 0)) throw "Segmentation fault!";
    else if (fragmentindex == -1)
    {
        this->pNode = NULL;
        index = pList->count;
    }
    else if (begin) {
        pNode = this->pList->fragmentPointers[fragmentindex];
        this->index = pList->fragmentMaxSize * fragmentindex;
    }
    else if (!begin) {
        bool a = 0;
        Node* ptr = this->pList->fragmentPointers[fragmentindex];
        for (int i = 0; i < this->pList->fragmentMaxSize; i++) {
            if (ptr->next == NULL) {
                a = 1;
                pNode = NULL;
                index = pList->count;
                break;
            }
            ptr = ptr->next;
        }
        if (!a) {
            pNode = ptr;
            index = pList->fragmentMaxSize * (fragmentindex + 1);
        }
    }
}
template <class T>
typename FragmentLinkedList<T>::Iterator& FragmentLinkedList<T>::Iterator::operator=(const Iterator& iterator) {
    this->pNode = iterator.pNode;
    this->pList = iterator.pList;
    return *this;
}
template <class T>
bool FragmentLinkedList<T>::Iterator::operator!=(const Iterator& iterator) {
    if ((pNode != iterator.pNode) || (index != iterator.index)) return true;
    return false;
}
template <class T>
T& FragmentLinkedList<T>::Iterator::operator*() {
    if (pNode == NULL) throw "Segmentation fault!";
    return this->pNode->data;
}
template <class T>
typename FragmentLinkedList<T>::Iterator& FragmentLinkedList<T>::Iterator::operator++() {
    if (pNode == NULL && index == count) throw "Segmentation fault!";
    else if (pNode == NULL) {
        pNode = pList->fragmentPointers[0];
        index = 0;
    }
    else {
        this->pNode = this->pNode->next;
        ++index;
    }
    return *this;
}
template <class T>
typename FragmentLinkedList<T>::Iterator FragmentLinkedList<T>::Iterator::operator++(int) {
    FragmentLinkedList<T>::Iterator temp = *this;
    if (pNode == NULL && index == pList->count) throw "Segmentation fault!";
    else if (pNode == NULL && index == -1) {
        pNode = pList->fragmentPointers[0];
        index = 0;
    }
    else {
        this->pNode = this->pNode->next;
        index++;
    }

    return temp;
}
template<class T>
void FragmentLinkedList<T>::Iterator::remove() {
    if (pNode == pList->fragmentPointers[0]) {
        pList->fragmentPointers[0] = pList->fragmentPointers[0]->next;
        delete pNode;
        pNode = NULL;
        index = -1;
        pList->count--;
    }
    else if (pNode == NULL) throw "Segmentation fault!";
    else {
        pNode->next->prev = pNode->prev;
        pNode->prev->next = pNode->next;
        pNode = pNode->prev;
        index--;
        pList->count--;
    }
    // update
    bool flag = 0;
    Node* pFP = pList->fragmentPointers[0];
    int numFP = (pList->count - 1) / pList->fragmentMaxSize + 2;
    pList->fragmentPointers = new Node * [numFP];
    pList->fragmentPointers[0] = pFP;
    for (int i = 1; i < numFP; i++) {
        for (int j = 0; j < pList->fragmentMaxSize; j++) {
            if (pFP->next == NULL) {
                flag = 1;
                break;
            }
            pFP = pFP->next;
        }
        pList->fragmentPointers[i] = pFP;
        if (flag) {
            pList->fragmentPointers[numFP - 1] = pFP;
            break;
        }
    }
    //updated
}
template <class T>
void FragmentLinkedList<T>::Iterator::set(const T& element) {
    pNode->data = element;
}
