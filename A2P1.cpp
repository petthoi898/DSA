
template <class K, class V>
class BKUTree {
public:
    class AVLTree;
    class SplayTree;

    class Entry {
    public:
        K key;
        V value;
        Entry(K key, V value) : key(key), value(value) {}
    };
    friend class SplayTree;
private:
    AVLTree* avl;
    SplayTree* splay;
    queue<K> keys;
    int maxNumOfKeys;

public:
    BKUTree(int maxNumOfKeys = 5) {
        avl = NULL;
        splay = NULL;
        this->maxNumOfKeys = maxNumOfKeys;
    }
    ~BKUTree() { this-> clear(); }

    void add(K key, V value);
    void remove(K key);
    bool checkkeyinq(K key) {
        queue<K> temp = keys;
        while (!temp.empty()) {
            if (temp.front() == key) return true;
            temp.pop();
        }
        return false;
    }
    V search(K key, vector<K>& traversedList);
    void traverseNLROnAVL(void (*func)(K key, V value)) {
        avl->traverseNLR(func);
    }
    void traverseNLROnSplay(void (*func)(K key, V value)) {
        splay->traverseNLR(func);
    }
    void clear() {
        avl = NULL;
        splay = NULL;
        while (!keys.empty()) {
            keys.pop();
        }
    }

    class SplayTree {
    public:
        class Node {
            Entry* entry;
            Node* left;
            Node* right;
            Node* parent;
            typename AVLTree::Node* corr;
            friend class BKUTree;
            Node(Entry* entry = NULL, Node* left = NULL, Node* right = NULL, Node* parent =NULL) {
                this->entry = entry;
                this->left = left;
                this->right = right;
                this->corr = NULL;
                this->parent = parent;
            }
        };

    public:
        Node* root;
        Node* newnode;
        SplayTree(): root(NULL), newnode(NULL) {};
        ~SplayTree() { this->clear(); };
        Node* find(K key, vector<K>& traversedList) {
            return find(root, key, traversedList);
        }
        Node* find(Node* node, K key, vector<K> &traversedList) {
            if (node->entry->key == key) return node;
            else if (node->entry->key > key) {
                traversedList.push_back(node->entry->key);
                return find(node->left, key, traversedList);
            }
            else if (node->entry->key < key) {
                traversedList.push_back(node->entry->key);
                return find(node->right, key, traversedList);
            }
            else return NULL;
        };
        void splay(Node* node);
        void splayonce(Node* node);
        Node* insert(Node* node, Entry* entry);
        Node* insert(Node* node, K key, V value);
        Node* leftrotate(Node* node);
        Node* rightrotate(Node* node);
        Node* getnewnode() { return newnode; };
        Node* maxnode(Node* node) {
            Node* cur = node;
            while (cur->right != NULL) cur = cur->right;
            return cur;
        }
        void splayrm(Node* node, Node* r);
        void add(K key, V value);
        void add(Entry* entry);
        void deletekey(Node* node, K key);
        void remove(K key);
        V searchkey(Node* node, K key);
        V search(K key);
        void printNLR(Node* node, void (*func)(K key, V value)) {
            if (node == NULL) return;
            func(node->entry->key, node->entry->value);
            printNLR(node->left, func);
            printNLR(node->right, func);
        }
        void traverseNLR(void (*func)(K key, V value)) {
            printNLR(root, func);
        }
        Node* getroot() { return root; };
        void clear(Node* node){
            clear(node->left);
            clear(node->right);
            delete node;
        }
        void clear() { 
            clear(root);
            newnode=NULL;
        };
    };

    class AVLTree {
    public:
        class Node {
            Entry* entry;
            Node* left;
            Node* right;
            int balance;
            typename SplayTree::Node* corr;
            friend class BKUTree;
            Node(Entry* entry = NULL, Node* left = NULL, Node* right = NULL) {
                this->entry = entry;
                this->left = left;
                this->right = right;
                this->balance = 0;
                this->corr = NULL;
            }
            friend class AVLTree;
        };

    public:
        Node* root;
        Node* newnode;
        AVLTree(): root(NULL), newnode(NULL) {};
        ~AVLTree() { this->clear(); };
        Node* leftBalance(Node* node, bool& taller);
        Node* rightBalance(Node* node, bool& taller);
        Node* rotateRight(Node* node);
        Node* rotateLeft(Node* node);
        Node* insert(Node* node, Entry* entry, bool& taller);
        Node* insert(Node* node, K key, V value, bool& taller);
        Node* setnewnode(K key, V value) {
            Entry* entry = new Entry(key, value);
            newnode = new Node(entry, NULL, NULL);
            return newnode;
        };
        Node* setnewnode(Entry* entry) {
            newnode = new Node(entry, NULL, NULL);
            return newnode;
        }
        Node* deletenode(Node* node, K key, bool& shorter, bool& success);
        Node* maxvaluenode(Node* node) {
            Node* current = node;
            while (current->right != NULL)current = current->right;
            return current;
        }
        Node* deleterightbalance(Node* node, bool& shorter);
        Node* deleteleftbalance(Node* node, bool& shorter);
        Node* nodefind(K key) {
            return nodefind(root, key);
        }
        Node* nodefind(Node* node, K key) {
            if (node == NULL) return NULL;
            if (node->entry->key == key) return node;
            else if (node->entry->key > key) nodefind(node->left, key);
            else if (node->entry->key < key) nodefind(node->right, key);
        };
        void add(K key, V value);
        void add(Entry* entry);
        void remove(K key);
        V searchkey(Node* node, K key, vector<K>& traversedList, Node* r);
        V search(K key, vector<K>& traversedList, Node* r);
        void printNLR(Node* node, void (*func)(K key, V value)) {
            if (node == NULL)return;
            func(node->entry->key, node->entry->value);
            printNLR(node->left, func);
            printNLR(node->right, func);
        }
        void traverseNLR(void (*func)(K key, V value)) {
            printNLR(root, func);
        }
        void clear(Node* node){
            clear(node->left);
            clear(node->right);
            delete node;
        }
        void clear() { 
            clear(root);
            newnode=NULL;
        };
    };
};
template<class K, class V>
void BKUTree<K, V>::add(K key, V value) {
    queue<K> temp = keys;
    while (!temp.empty()) {
        K tmp = temp.front();
        if (tmp == key) {
            cout << "Duplicate key";
            return;
        }
        temp.pop();
    }
    int count = keys.size();
    if (count < maxNumOfKeys) keys.push(key);
    else {
        keys.pop();
        keys.push(key);
    }
    typename BKUTree<K, V>::AVLTree::Node* newnodeavl = new typename BKUTree<K, V>::AVLTree::Node();
    typename BKUTree<K, V>::SplayTree::Node* newnodesplay = new typename BKUTree<K, V>::SplayTree::Node();
    if (avl == NULL) {
        avl = new AVLTree();
        avl->add(key, value);
        newnodeavl = avl->newnode;
    }
    else {
        avl->add(key, value);
        newnodeavl = avl->newnode;
    }
    if (splay == NULL) {
        splay = new SplayTree();
        splay->add(key, value);
        newnodesplay = splay->getnewnode();
    }
    else {
        splay->add(key, value);
        newnodesplay = splay->getnewnode();
    }
    newnodeavl->corr = newnodesplay;
    newnodesplay->corr = newnodeavl;
}
template <class K, class V>
void BKUTree<K, V>::remove(K key) {
    vector<K> array;
    bool a = 0;
    while (!keys.empty()) {
        K temp = keys.front();
        if (temp != key) array.push_back(temp);
        else a = 1;
        keys.pop();
    }
    typename vector<K>::iterator it = array.begin();
    for (; it != array.end(); it++) {
        keys.push(*it);
    }
    if (a) {
        if (keys.size() < maxNumOfKeys) keys.push(key);
        else {
            keys.pop();
            keys.push(key);
        }
    }
    avl->remove(key);
    splay->remove(key);
}
template<class K, class V>
V BKUTree<K, V>::search(K key, vector<K>& traversedList) {
    if (key == splay->root->entry->key) {
        if (keys.size() < maxNumOfKeys) keys.push(key);
        else {
            keys.pop();
            keys.push(key);
        }
        return splay->root->entry->value;
    }
    else if (checkkeyinq(key)) {
        if (keys.size() < maxNumOfKeys) keys.push(key);
        else {
            keys.pop();
            keys.push(key);
        }
        typename BKUTree<K,V>::SplayTree::Node* sp = splay->find(key, traversedList);
        if (sp != NULL)
        {
            splay->splayonce(sp);
            return sp->entry->value;
        }
        else return NULL;
    }
    else if (!checkkeyinq(key)) {
        if (keys.size() < maxNumOfKeys) keys.push(key);
        else {
            keys.pop();
            keys.push(key);
        }
        typename BKUTree<K, V>::AVLTree::Node* r = splay->root->corr;
        V temp = avl->searchkey(r, key, traversedList, NULL);
        if (temp != NULL) return temp;
        else {
            V temp = avl->search(key, traversedList, r);
            typename BKUTree<K, V>::AVLTree::Node* f = avl->nodefind(temp);
            if (temp != NULL) {
                typename BKUTree<K, V>::SplayTree::Node* sp = f->corr;
                splay->splayonce(sp);
                return temp;
            }
            else throw "Not Found"; 

        }
    }

};
template<class K, class V>
void BKUTree<K, V>::AVLTree::add(K key, V value) {
    bool taller = 1;
    setnewnode(key, value);
    root = insert(root, key, value, taller);

}
template<class K, class V>
void BKUTree<K, V>::AVLTree::add(Entry* entry) {
    bool taller = 1;
    root = insert(root, entry, taller);
    setnewnode(entry);
}
template<class K, class V>
void BKUTree<K, V>::AVLTree::remove(K key) {
    bool shorter = 0, success = 0;
    root = deletenode(root, key, shorter, success);
}
template<class K, class V>
V BKUTree<K, V>::AVLTree::search(K key, vector<K>& traversedList, Node* r) {
    return searchkey(root, key, traversedList, r);
};
template<class K, class V>
V BKUTree<K, V>::AVLTree::searchkey(Node* node, K key , vector<K>& traversedList, Node* r) {
    if (node == NULL || node == r) return NULL;
    if (node->entry->key == key) return node->entry->value;
    else if (node->entry->key > key) {
        traversedList.push_back(node->entry->key);
        return searchkey(node->left, key, traversedList, r);
    }
    else if (node->entry->key < key) {
        traversedList.push_back(node->entry->key);
        return searchkey(node->right, key, traversedList, r);
    }
    else throw"Not found";
};
template<class K, class V>
typename BKUTree<K, V>::AVLTree::Node* BKUTree<K, V>::AVLTree::deleterightbalance(Node* node, bool& shorter) {
    if (node->balance == -1) {
        node->balance = 0;
    }
    else if (node->balance == 0) {
        node->balance = 1;
        shorter = 0;
    }
    else if (node->right != NULL) {

        Node* rightTree = node->right;
        if (rightTree->balance == -1) {
            Node* leftTree = rightTree->left;
            if (leftTree->balance == -1) {
                rightTree->balance = 1;
                node->balance = 0;
            }
            else if (leftTree->balance == 0) {
                node->balance = -1;
                rightTree->balance = 0;
            }
            else {
                node->balance = -1;
                rightTree->balance = 0;
            }
            leftTree->balance = 0;
            node->right = rotateRight(rightTree);
            node = rotateLeft(node);
        }
        else {
            if (rightTree->balance != 0) {
                node->balance = 0;
                rightTree->balance = 0;
            }
            else {
                node->balance = 1;
                rightTree->balance = -1;
                shorter = 0;
            }
            node = rotateLeft(node);
        }
    }
    else if (node->right == NULL && node->left != NULL) {
        node->left->balance = 0;
        node->balance = 0;
        node = rotateRight(node);
    }
    if (node->right == NULL && node->left != NULL) {

        node->left->balance = 0;
        node->balance = 0;
        node = rotateRight(node);
    }
    return node;

}
template<class K, class V>
typename BKUTree<K, V>::AVLTree::Node* BKUTree<K, V>::AVLTree::deleteleftbalance(Node* node, bool& shorter) {
    if (node->balance == 1) {
        node->balance = 0;
    }
    else if (node->balance == 0) {
        node->balance = -1;
        shorter = 0;
    }
    else if (node->right != NULL) {
        Node* leftTree = node->left;
        if (leftTree->balance == 1) {
            Node* rightTree = leftTree->right;
            if (rightTree->balance == -1) {
                leftTree->balance = -1;
                node->balance = 0;
            }
            else if (rightTree->balance == 0) {
                node->balance = 1;
                leftTree->balance = 0;
            }
            else {
                node->balance = 1;
                leftTree->balance = 0;
            }
            rightTree->balance = 0;
            node->left = rotateRight(leftTree);
            node = rotateRight(node);
        }
        else {
            if (leftTree->balance != 0) {
                node->balance = 0;
                leftTree->balance = 0;
            }
            else {
                node->balance = -1;
                leftTree->balance = 1;
                shorter = 0;
            }
            node = rotateRight(node);
        }
    }
    return node;
}
template<class K, class V>
typename BKUTree<K, V>::AVLTree::Node* BKUTree<K, V>::AVLTree::deletenode(Node* node, K key , bool& shorter, bool& success) {
    if (node == NULL) {
        shorter = 0;
        success = 0;
        return NULL;
    }
    if (key < node->entry->key) {
        node->left = deletenode(node->left, key, shorter, success);
        if (shorter) node = deleterightbalance(node, shorter);
    }
    else if (key > node->entry->key) {
        node->right = deletenode(node->right, key, shorter, success);
        if (shorter)node = deleteleftbalance(node, shorter);
    }
    else {
        Node* deleteNode = node;
        if (node->right == NULL) {
            Node* newNode = node->left;
            success = true;
            shorter = true;
            delete deleteNode;
            return newNode;
        }
        else if (node->left == NULL) {
            Node* newNode = node->right;
            success = true;
            shorter = true;
            delete deleteNode;
            return newNode;
        }
        else {
            Node* exchPtr = node->left;
            while (exchPtr->right != NULL) {
                exchPtr = exchPtr->right;
            }
            node->entry = exchPtr->entry;
            node->left = deletenode(node->left, exchPtr->entry->key, shorter, success);
            if (shorter) node = deleterightbalance(node, shorter);
        }
    }
    return node;
}
template<class K, class V>
typename BKUTree<K, V>::AVLTree::Node* BKUTree<K, V>::AVLTree::insert(Node* node, Entry* entry, bool& taller) {
    setnewnode(entry);
    if (node == NULL) {
        taller = 1;
        return newnode;
    }
    if (entry->key < node->entry->key) {
        node->left = insert(node->left, entry, taller);
        if (taller) {
            if (node->balance == -1) node = leftBalance(node, taller);
            else if (node->balance == 0) node->balance = -1;
            else {
                node->balance = 0;
                taller = false;
            }
        }
    }
    else if (entry->key > node->entry->key) {
        node->right = insert(node->right, entry, taller);
        if (taller) {
            if (node->balance == 1) {
                node = rightBalance(node, taller);
            }
            else if (node->balance == 0) node->balance = 1;
            else {
                node->balance = 0;
                taller = 0;
            }
        }
    }
    else throw"Douplicate key";
    return node;
}
template<class K, class V>
typename BKUTree<K, V>::AVLTree::Node* BKUTree<K, V>::AVLTree::insert(Node* node, K key, V value, bool& taller) {
    if (node == NULL) {
        taller = 1;
        return newnode;
    }
    if (key < node->entry->key) {
        node->left = insert(node->left, key, value, taller);
        if (taller) {
            if (node->balance == -1) node = leftBalance(node, taller);
            else if (node->balance == 0) node->balance = -1;
            else {
                node->balance = 0;
                taller = false;
            }
        }
    }
    else if (key > node->entry->key) {
        node->right = insert(node->right, key, value, taller);
        if (taller) {
            if (node->balance == 1) {
                node = rightBalance(node, taller);
            }
            else if (node->balance == 0) node->balance = 1;
            else {
                node->balance = 0;
                taller = 0;
            }
        }
    }
    else throw"Duplicate key";
    return node;
}
template<class K, class V>
typename BKUTree<K, V>::AVLTree::Node* BKUTree<K, V>::AVLTree::leftBalance(Node* node, bool& taller) {
    Node* lefttree = node->left;
    if (lefttree->balance == -1) {
        node->balance = 0;
        node = rotateRight(root);
        lefttree->balance = 0;
        taller = false;
    }
    else if (lefttree->right != NULL){
        Node* righttree = lefttree->right;
        if (righttree->balance == -1) {
            node->balance = 1;
            lefttree->balance = 0;
        }
        else if (righttree->balance == 0) lefttree->balance = 0;
        else {
            node->balance = 0;
            lefttree->balance = -1;
        }
        righttree->balance = 0;
        node->left = rotateLeft(lefttree);
        node = rotateRight(root);
        taller = false;
    }
    return node;
}
template<class K, class V>
typename BKUTree<K, V>::AVLTree::Node* BKUTree<K, V>::AVLTree::rightBalance(Node* node, bool& taller) {
    Node* righttree = node->right;
    if (righttree->balance == 1) {
        node->balance = 0;
        node = rotateLeft(node);
        righttree->balance = 0;
        taller = false;
    }
    else if (righttree->left != NULL) {
        Node* lefttree = righttree->left;
        if (lefttree->balance == -1) {
            node->balance = 0;
            righttree->balance = 1;
        }
        else if (lefttree->balance == 0)righttree->balance = 0;
        else {
            node->balance = -1;
            righttree->balance = 0;
        }
        lefttree->balance = 0;
        node->right = rotateRight(righttree);
        node = rotateLeft(node);
        taller = false;
    }
    return node;

}
template<class K, class V>
typename BKUTree<K, V>::AVLTree::Node* BKUTree<K, V>::AVLTree::rotateLeft(Node* node) {
    Node* temp = node->right;
    node->right = temp->left;
    temp->left = node;
    return temp;
}
template<class K, class V>
typename BKUTree<K, V>::AVLTree::Node* BKUTree<K, V>::AVLTree::rotateRight(Node* node) {
    Node* temp = node->left;
    node->left = temp->right;
    temp->right = node;
    return temp;
}
template<class K, class V>
void BKUTree<K, V>::SplayTree::add(K key, V value) {
    root = insert(root, key, value);
}
template<class K, class V>
void BKUTree<K, V>::SplayTree::add(Entry* entry) {
    setnewnode(entry);
    root = insert(root, entry);

}
template<class K, class V>
void BKUTree<K, V>::SplayTree::remove(K key) {
    deletekey(root, key);
}
template<class K , class V>
void BKUTree<K, V>::SplayTree::deletekey(Node* node, K key) {
    if ((key > node->entry->key && node->right == NULL) || (key < node->entry->key && node->left == NULL)) {
        splay(node);
    }
    else if (key > node->entry->key) {
        deletekey(node->right, key);
        return;
    }
    else if (key < node->entry->key) {
        deletekey(node->left, key);
        return;
    }
    else if (key == node->entry->key) {
        splay(node);
        Node* lefttree = node->left;
        Node* rightree = node->right;
        delete node;
        Node* max = maxnode(lefttree);
        if (max != lefttree)splayrm(max, lefttree);
        max->right = rightree;
        max->parent = NULL;
        root = max;
        return;
    }
    else throw"Not found";

}
template<class K, class V>
V BKUTree<K, V>::SplayTree::search(K key) {
    return searchkey(root, key);
};
template<class K, class V>
V BKUTree<K, V>::SplayTree::searchkey(Node* node, K key) {
    if (node->entry->key == key) {
        splay(node);
        return node->entry->value;
    }
    if (node->entry->key < key && node->right == NULL) {
        splay(node);
        return 0;
    }
    else if (node->entry->key > key && node->left == NULL) {
        splay(node);
        return 0;
    }
    else if (node->entry->key < key) return searchkey(node->right, key);
    else if (node->entry->key > key) return searchkey(node->left, key);
    else throw "Not found";
    
}
template<class K, class V>
typename BKUTree<K, V>::SplayTree::Node* BKUTree<K, V>::SplayTree::leftrotate(Node* node) {
    Node* x = node;
    Node* xparent = node->parent;
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;
    x->parent = y;
    if (T2 != NULL) T2->parent = x;

    y->parent = xparent;
    if (y->parent != NULL) {
        if (y->parent->left == x) y->parent->left = y;
        else y->parent->right = y;
    }
    return y;
}
template<class K, class V>
typename BKUTree<K, V>::SplayTree::Node* BKUTree<K, V>::SplayTree::rightrotate(Node* node) {
    Node* y = node;
    Node* yparent = node->parent;
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;
    y->parent = x;
    if (T2 != NULL) T2->parent = y;
    x->parent = yparent;
    if (x->parent != NULL) {
        if (x->parent->left == y) x->parent->left = x;
        else x->parent->right = x;
    }
    return x;
}
template<class K, class V>
typename BKUTree<K, V>::SplayTree::Node* BKUTree<K, V>::SplayTree::insert(Node* node, Entry* entry) {
    Node* newnode = new Node(entry, NULL, NULL);
    this->newnode = newnode;
    if (node == NULL) {
        node = newnode;
        return node;
    }
    if (node->left == NULL && node->entry->key > entry->key) {
        node->left = newnode;
        node->left->parent = node;
        splay(node->left);

    }
    else if (node->right == NULL && node->entry->key < entry->key) {
        node->right = newnode;
        node->right->parent = node;
        splay(node->right);

    }
    else if (entry->key < node->entry->key) {
        insert(node->left, entry->key, entry->value);
    }
    else if (entry->key > node->entry->key) {
        insert(node->right, entry->key, entry->value);
    }
    else throw"Duplicate key";
}
template<class K, class V>
typename BKUTree<K, V>::SplayTree::Node* BKUTree<K, V>::SplayTree::insert(Node* node, K key, V value) {
    Entry* entry = new Entry(key, value);
    Node* newnode = new Node(entry, NULL, NULL);
    this->newnode = newnode;
    if (node == NULL) {
        node = newnode;
        return node;
    }
    if (node->left == NULL && node->entry->key > key) {
        node->left = newnode;
        node->left->parent = node;
        splay(node->left);
        return newnode;
    }
    else if (node->right == NULL && node->entry->key < key) {
        node->right = newnode;
        node->right->parent = node;
        splay(node->right);
        return newnode;
    }
    else if (key < node->entry->key) {
        return insert(node->left, key, value);
    }
    else if (key > node->entry->key) {
        return insert(node->right, key, value);
    }
    else throw"Duplicate key";
}
template<class K, class V>
void BKUTree<K, V>::SplayTree::splay(Node* node) {
    if (node == root || node == NULL) return;
    //rotate zig
    if (node->parent == root) {
        if (node->parent->left == node) root = rightrotate(root);
        else root = leftrotate(root);
    }
    // zig zig 
    else if (node->parent->left == node && node->parent->parent->left == node->parent) {
        if (node->parent->parent == root) {
            root = rightrotate(root);
            root = rightrotate(root);
            return;
        }
        else {
            node->parent = rightrotate(node->parent->parent);
            node = rightrotate(node->parent);
            splay(node);
            return;
        }
    }
    // zag zag
    else if (node->parent->right == node && node->parent->parent->right == node->parent) {
        if (node->parent->parent == root) {
            root = leftrotate(root);
            root = leftrotate(root);
            return;
        }
        else {
            node->parent = leftrotate(node->parent->parent);
            node = leftrotate(node->parent);
            splay(node);
            return;
        }
    }
    // zag zig
    else if (node->parent->right == node && node->parent->parent->left == node->parent) {
        node = leftrotate(node->parent);
        if (node->parent == root) {
            root = rightrotate(root);
            return;
        }
        else {
            node = rightrotate(node->parent);
            splay(node);
            return;
        }
    }
    // zig zag
    else if (node->parent->left == node && node->parent->parent->right == node->parent) {
        node = rightrotate(node->parent);
        if (node->parent == root) {
            root = leftrotate(root);
            return;
        }
        else {
            node = leftrotate(node->parent);
            splay(node);
            return;
        }
    }


}
template<class K, class V>
void BKUTree<K, V>::SplayTree::splayrm(Node* node, Node * root) {
    if (node == root || node == NULL) return;
    //rotate zig
    if (node->parent == root) {
        if (node->parent->left == node) root = rightrotate(root);
        else root = leftrotate(root);
    }
    // zig zig 
    else if (node->parent->left == node && node->parent->parent->left == node->parent) {
        if (node->parent->parent == root) {
            root = rightrotate(root);
            root = rightrotate(root);
            return;
        }
        else {
            node->parent = rightrotate(node->parent->parent);
            node = rightrotate(node->parent);
            splay(node);
            return;
        }
    }
    // zag zag
    else if (node->parent->right == node && node->parent->parent->right == node->parent) {
        if (node->parent->parent == root) {
            root = leftrotate(root);
            root = leftrotate(root);
            return;
        }
        else {
            node->parent = leftrotate(node->parent->parent);
            node = leftrotate(node->parent);
            splay(node);
            return;
        }
    }
    // zag zig
    else if (node->parent->right == node && node->parent->parent->left == node->parent) {
        node = leftrotate(node->parent);
        if (node->parent == root) {
            root = rightrotate(root);
            return;
        }
        else {
            node = rightrotate(node->parent);
            splay(node);
            return;
        }
    }
    // zig zag
    else if (node->parent->left == node && node->parent->parent->right == node->parent) {
        node = rightrotate(node->parent);
        if (node->parent == root) {
            root = leftrotate(root);
            return;
        }
        else {
            node = leftrotate(node->parent);
            splay(node);
            return;
        }
    }


}
template<class K, class V>
void BKUTree<K, V>::SplayTree::splayonce(Node* node) {
    if (node == root || node == NULL) return;
    //rotate zig
    if (node->parent == root) {
        if (node->parent->left == node) root = rightrotate(root);
        else root = leftrotate(root);
    }
    // zig zig 
    else if (node->parent->left == node && node->parent->parent->left == node->parent) {
        if (node->parent->parent == root) {
            root = rightrotate(root);
            root = rightrotate(root);
            return;
        }
        else {
            node->parent = rightrotate(node->parent->parent);
            node = rightrotate(node->parent);
            return;
        }
    }
    // zag zag
    else if (node->parent->right == node && node->parent->parent->right == node->parent) {
        if (node->parent->parent == root) {
            root = leftrotate(root);
            root = leftrotate(root);
            return;
        }
        else {
            node->parent = leftrotate(node->parent->parent);
            node = leftrotate(node->parent);
            return;
        }
    }
    // zag zig
    else if (node->parent->right == node && node->parent->parent->left == node->parent) {
        node = leftrotate(node->parent);
        if (node->parent == root) {
            root = rightrotate(root);
            return;
        }
        else {
            node = rightrotate(node->parent);
            return;
        }
    }
    // zig zag
    else if (node->parent->left == node && node->parent->parent->right == node->parent) {
        node = rightrotate(node->parent);
        if (node->parent == root) {
            root = leftrotate(root);
            return;
        }
        else {
            node = leftrotate(node->parent);
            return;
        }
    }


}
