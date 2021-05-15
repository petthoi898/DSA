#include <iostream>
#include <vector>
#include <algorithm>


using namespace std;


//int minPark(vector<Interval> lInterval) {
//    int count = 0;
//    int temp = 1;
//    AVLTree avl;
//
//    for (auto it = lInterval.begin(); it != lInterval.end(); it++) {
//        avl.insert(*it);
//    }
//    AVLTree::Node* min = avl.minnode();
//    int left = min->data.start;
//    AVLTree::Node* max = avl.maxnode();
//    int right = max->data.end;
//    AVLTree::Node* root = avl.getroot();
//
//    for (int i = left; i < right; i++) {
//        avl.LNR(count, i, root);
//        if (temp < count) {
//            temp = count;
//            count = 0;
//        }
//        else count = 0;
//    }
//
//    return temp;
//};
class Interval
{
public:
    int start;
    int end;

    Interval(int start, int end) : start(start), end(end) {}
    Interval(const Interval& i) : start(i.start), end(i.end) {}
    friend class Node;
};
class BusParking{
    enum BalanceValue
    {
        LH = -1,
        EH = 0,
        RH = 1
    };
public:


    class AVLTree
    {
    public:
        class Node;
        friend class BusParking;
    private:
        Node* root;
        vector<Interval> intervals;
    protected:
        int getHeightRec(Node* node)
        {
            if (node == NULL)
                return 0;
            int lh = this->getHeightRec(node->pLeft);
            int rh = this->getHeightRec(node->pRight);
            return (lh > rh ? lh : rh) + 1;
        }
    public:
        AVLTree() : root(nullptr) {}
        ~AVLTree() { root = NULL; }
        int getHeight()
        {
            return this->getHeightRec(this->root);
        }
        Node* insertAvlTree(Node* root, Interval value, bool& taller)
        {
            if (root == NULL)
            {
                root = new Node(value);
                taller = 1;
                return root;
            }
            if (value.start < root->data.start)
            {
                root->pLeft = insertAvlTree(root->pLeft, value, taller);
                if (taller)
                {
                    if (root->balance == BalanceValue::LH)
                        root = leftBalance(root, taller);
                    else if (root->balance == BalanceValue::EH)
                        root->balance = BalanceValue::LH;
                    else if (root->balance == BalanceValue::RH)
                    {
                        root->balance = EH;
                        taller = 0;
                    }
                }
            }
            else
            {
                root->pRight = insertAvlTree(root->pRight, value, taller);
                if (taller)
                {
                    if (root->balance == RH)
                        root = rightBalance(root, taller);
                    else if (root->balance == EH)
                        root->balance = RH;
                    else
                    {
                        root->balance = EH;
                        taller = 0;
                    }
                }
            }
            return root;
        }
        void numbercar(Interval a, Node* node, int& count) {
            if (node == NULL) return;
            node->check(a, count);
            if (node->pLeft != nullptr && a.start <= node->pLeft->max)
                numbercar(a, node->pLeft, count);
            if (node->pRight != nullptr && a.end >= node->pRight->data.start)
                numbercar(a, node->pRight, count);

            return;
        }
        Node* leftBalance(Node* root, bool& taller)
        {
            Node* leftTree = root->pLeft;
            if (leftTree->balance == LH)
            {
                root->balance = EH;
                root = rotateRight(root);
                leftTree->balance = EH;
                /*cout << '[' << root->value << ':' << root->Balance << ']' << endl;
                    cout << '[' << leftTree->value << ':' << leftTree->Balance << ']';*/
                taller = 0;
            }
            else if (leftTree->pRight != NULL)
            {
                Node* rightTree = leftTree->pRight;
                if (rightTree->balance == LH)
                {
                    root->balance = RH;
                    leftTree->balance = EH;
                }
                else if (rightTree->balance == EH)
                    leftTree->balance = EH;
                else
                {
                    root->balance = EH;
                    leftTree->balance = LH;
                }
                rightTree->balance = EH;
                root->pLeft = rotateLeft(leftTree);
                root = rotateRight(root);
                taller = 0;
            }
            return root;
        }
        Node* rightBalance(Node* root, bool& taller)
        {

            Node* rightTree = root->pRight;
            if (rightTree->balance == RH)
            {
                root->balance = EH;
                root = rotateLeft(root);
                rightTree->balance = EH;
                taller = 0;
            }
            else if (rightTree->pLeft != NULL)
            {
                Node* leftTree = rightTree->pLeft;
                if (leftTree->balance == LH)
                {
                    root->balance = EH;
                    rightTree->balance = RH;
                }
                else if (leftTree->balance == EH)
                {
                    rightTree->balance = EH;
                }
                else
                {
                    root->balance = LH;
                    rightTree->balance = EH;
                }
                leftTree->balance = EH;
                root->pRight = rotateRight(rightTree);
                root = rotateLeft(root);
                taller = 0;
            }
            return root;
        }
        Node* rotateLeft(Node* root)
        {

            Node* tempPtr = root->pRight;
            root->pRight = tempPtr->pLeft;
            tempPtr->pLeft = root;
            getMax(root);
            getMax(tempPtr);
            return tempPtr;
        }
        Node* rotateRight(Node* root)
        {
            Node* tempPtr = root->pLeft;
            root->pLeft = tempPtr->pRight;
            tempPtr->pRight = root;
            getMax(root);
            getMax(tempPtr);
            return tempPtr;
        }
        int insert(const Interval& value)
        {
            //TODO
            bool taller = 1;
            int count = 0;
            root = insertAvlTree(root, value, taller);
            numbercar(value, root, count);
            return count;
        }
        int oRemove()
        {
            int quan = intervals.size();
            int* arr = new int[quan], * dep = new int[quan];

            for (int i = 0; i < quan; i += 1)
            {
                arr[i] = intervals[i].start;
                dep[i] = intervals[i].end;
            }

            // * These sorts are O(N log N)
            sort(arr, arr + quan);
            sort(dep, dep + quan);

            int cur = 1, count = 1;
            int i = 1, j = 0;

            while (i < quan && j < quan)
            {
                if (arr[i] < dep[j])
                {
                    cur += 1;
                    i += 1;
                }
                else
                {
                    cur -= 1;
                    j += 1;
                }

                if (cur > count)
                    count = cur;
            }

            return count;
        }
        int findMax(int a, int b)
        {
            return (a > b) ? a : b;
        }
        Node* deleteRightBalance(Node* root, bool& shorter) {
            if (root->balance == LH) {
                root->balance = EH;
            }
            else if (root->balance == EH) {
                root->balance = RH;
                shorter = 0;
            }
            else if (root->pRight != NULL) {

                Node* rightTree = root->pRight;
                if (rightTree->balance == LH) {
                    Node* leftTree = rightTree->pLeft;
                    if (leftTree->balance == LH) {
                        rightTree->balance = RH;
                        root->balance = EH;
                    }
                    else if (leftTree->balance == EH) {
                        root->balance = LH;
                        rightTree->balance = EH;
                    }
                    else {
                        root->balance = LH;
                        rightTree->balance = EH;
                    }
                    leftTree->balance = EH;
                    root->pRight = rotateRight(rightTree);
                    root = rotateLeft(root);
                }
                else {
                    if (rightTree->balance != EH) {
                        root->balance = EH;
                        rightTree->balance = EH;
                    }
                    else {
                        root->balance = RH;
                        rightTree->balance = LH;
                        shorter = 0;
                    }
                    root = rotateLeft(root);
                }
            }
            else if (root->pRight == NULL && root->pLeft != NULL) {
                root->pLeft->balance = EH;
                root->balance = EH;
                root = rotateRight(root);
            }
            if (root->pRight == NULL && root->pLeft != NULL) {

                root->pLeft->balance = EH;
                root->balance = EH;
                root = rotateRight(root);
            }
            return root;
        }
        Node* deleteLeftBalance(Node* root, bool& shorter) {

            if (root->balance == RH) {
                root->balance = EH;
            }
            else if (root->balance == EH) {
                root->balance = LH;
                shorter = 0;
            }
            else if (root->pRight != NULL) {
                Node* leftTree = root->pLeft;
                if (leftTree->balance == RH) {
                    Node* rightTree = leftTree->pRight;
                    if (rightTree->balance == LH) {
                        leftTree->balance = LH;
                        root->balance = EH;
                    }
                    else if (rightTree->balance == EH) {
                        root->balance = RH;
                        leftTree->balance = EH;
                    }
                    else {
                        root->balance = RH;
                        leftTree->balance = EH;
                    }
                    rightTree->balance = EH;
                    root->pLeft = rotateRight(leftTree);
                    root = rotateRight(root);
                }
                else {
                    if (leftTree->balance != EH) {
                        root->balance = EH;
                        leftTree->balance = EH;
                    }
                    else {
                        root->balance = LH;
                        leftTree->balance = RH;
                        shorter = 0;
                    }
                    root = rotateRight(root);
                }
            }
            return root;
        }
        Node* AVLDelete(Node* root, Interval deleteKey, bool& shorter, bool& success)
        {
            if (root == NULL) {
                shorter = 0;
                success = 0;
                return NULL;
            }
            if (deleteKey.start < root->data.start) {
                root->pLeft = AVLDelete(root->pLeft, deleteKey, shorter, success);
                if (shorter) root = deleteRightBalance(root, shorter);
            }
            else if (deleteKey.start > root->data.start) {
                root->pRight = AVLDelete(root->pRight, deleteKey, shorter, success);
                if (shorter)root = deleteRightBalance(root, shorter);
            }
            else {
                Node* deleteNode = root;
                if (root->pRight == NULL) {
                    Node* newRoot = root->pLeft;
                    success = true;
                    shorter = true;
                    delete deleteNode;
                    return newRoot;
                }
                else if (root->pLeft == NULL) {
                    Node* newRoot = root->pRight;
                    success = true;
                    shorter = true;
                    delete deleteNode;
                    return newRoot;
                }
                else {
                    Node* exchPtr = root->pLeft;
                    while (exchPtr->pRight != NULL) {
                        exchPtr = exchPtr->pRight;
                    }
                    root->data = exchPtr->data;
                    root->pLeft = AVLDelete(root->pLeft, exchPtr->data, shorter, success);
                    if (shorter) root = deleteRightBalance(root, shorter);
                }
            }
            return root;
        }
        int remove(const Interval& value)
        {
            bool shorter = 0, success = 0;
            root = AVLDelete(root, value, shorter, success);
            return oRemove();
            //TODO
        }
        int getMax(Node* pR)
        {
            int max = findMax((pR->pLeft) ? pR->pLeft->max : 0, (pR->pRight) ? pR->pRight->max : 0);
            pR->max = findMax(max, pR->max);
            return pR->max;
        }
        Node* minnode() {
            Node* temp = root;
            while (temp->pLeft != NULL) {
                temp = temp->pLeft;
            }
            return temp;
        }
        Node* maxnode() {
            Node* temp = root;
            while (temp->pRight != NULL) {
                temp = temp->pRight;
            }
            return temp;
        }
        void LNR(int& count, int i, Node* node) {
            if (node == NULL) return;
            LNR(count, i, node->pLeft);
            if (node->data.start <= i && node->data.end > i) count++;
            LNR(count, i, node->pRight);
        }
        Node* getroot() {
            return root;
        }
        class Node
        {
        private:
            Interval data;
            Node* pLeft, * pRight;
            int max;
            BalanceValue balance;
            friend class AVLTree;

        public:
            
            Node(Interval i) : data(i), pLeft(NULL), pRight(NULL), max(i.end), balance(EH) {}
            ~Node() {}
            void check(Interval a, int &count) {
                if (data.start <= a.end && data.end >= a.start) count++;
            }
        };
    };
private:
    int minpark;
    AVLTree* list;
public:
    void add(int s, int t) {
        if (list == NULL) {
            list = new AVLTree();
        }
        Interval* newcar = new Interval(s, t);
        int temp = list->insert(*newcar);
        if (temp > minpark) minpark = temp;
    }
    void remove(int s, int t) {
        if (list == NULL) return;
        Interval* deletecar = new Interval(s, t);
        minpark = list->remove(*deletecar);
        return;
    }
    int minPark() {
        return minpark;
    }
    void clear() {
        minpark = 1;
        list = NULL;
    }
    BusParking() { list = NULL; minpark = 0; }
    ~BusParking() { clear(); }
};
int main()
{
    BusParking b;
    b.add(1, 5);
    cout << b.minPark();
}
