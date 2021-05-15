#include <vector>
#include <sstream>
#include <string.h>
template <class T>
class IList
{
public:
    virtual void add(const T& element) = 0;
    virtual void add(int index, const T& element) = 0;
    virtual bool empty() = 0;
    virtual int size() = 0;
    virtual void clear() = 0;
    virtual T get(int index) = 0;
};
template <class T>
class FragmentLinkedList : IList<T>
{
public:
    class Node;
    class Iterator;
    friend class BusSystem;
protected:
    Node** fragmentPointers;
    int fragmentMaxSize;
    int count;

public:
    FragmentLinkedList(int fragmentMaxSize = 1)
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
    virtual int size();
    virtual void clear();
    virtual T get(int index);
public:
    class Node
    {
    private:
        T data;
        Node* next;
        Node* prev;
        friend class FragmentLinkedList<T>;
        friend class BusSystem;
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
        void setdata(T n) {
            data = n;
        }
        T getdata() {
            return this->data;
        }
    };
};
template <class T>
FragmentLinkedList<T>::~FragmentLinkedList() {
    clear();
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
    }
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
T FragmentLinkedList<T>::get(int index) {
    if (this->count <= index || index < 0) throw "The index is out of range!";
    Node* ptr = this->fragmentPointers[0];
    for (int i = 0; i < index; i++) {
        ptr = ptr->next;
    }
    return ptr->data;
}

class Xe {
    string code;
    string lp;
    int timeA;
    int timeB;
    int Case;
public:
    Xe(string code, string lp, int Case, int timeA, int timeB) {
        this->code = code; this->lp = lp; this->timeA = timeA; this->timeB = timeB;
    }
    Xe() :code(""), lp(""), timeA(0), timeB(0), Case(-1) {};
    friend class BusSystem;
};
class BusSystem {
private:
    vector<vector<Xe>> bus;
    FragmentLinkedList<string> listinput;
    FragmentLinkedList<int> listcount;
    int max;
    int counttomax;
public:
    BusSystem() {
        this->max = 0;
        this->counttomax = 0;
    }
    string query(string instruction) {
        string str = instruction;
        if (str[0] == ' ' || str[str.length() - 1] == ' ') return "-1";
        int lengths = str.length();
        for (int i = 0; i < lengths - 1; i++) {
            if (str[i] == ' ' && str[i + 1] == ' ') return "-1";
        }
        string token = instruction.substr(0, 3);
        if (token == "SQ ")  return setmax(instruction);
        if (token == "INS") {
            instruction.erase(0, 4);
            return add(instruction);
        }
        else if (token == "DEL") {
            instruction.erase(0, 4);
            return del(instruction);
        }
        else if (token == "CS ") {
            instruction.erase(0, 3);
            return cs(instruction);
        }
        else if (token == "CE ") {
            instruction.erase(0, 3);
            return ce(instruction);
        }
        else if (token == "GS ") {
            instruction.erase(0, 3);
            return gs(instruction);
        }
        else if (token == "GE ") {
            instruction.erase(0, 3);
            return ge(instruction);
        }
        else return "-1";
    }
    string setmax(string s);
    string add(string s);
    string del(string s);
    string ge(string s);
    string gs(string s);
    string cs(string s);
    string ce(string s);
    bool checkcode(string c) {
        if (c.length() > 5) return true;
        return false;
    }
    bool checklp(string l) {
        if (l.length() > 10) return true;
        return false;
    }
    void process(string s) {
        string str = s;
        char strc[1024];
        strcpy(strc, str.c_str());
        char* token;
        token = strtok(strc, " ");
        listinput.add(token);
        while (token != NULL) {
            token = strtok(NULL, " ");
            if (token != NULL) listinput.add(token);
        }
        return;
    }
};
string BusSystem::setmax(string s) {
    process(s);
    if (listinput.size() == 2) {
        max = stoi(listinput.get(1));
        listinput.clear();
        return "1";
    }
    else return "-1";
}
string BusSystem::add(string s) {
    process(s);
    Xe addbus;
    if (listinput.size() == 5) {
        addbus.code = listinput.get(0);
        addbus.lp = listinput.get(1);
        addbus.Case = stoi(listinput.get(2));
        addbus.timeA = stoi(listinput.get(3));
        addbus.timeB = stoi(listinput.get(4));
    }
    else if (listinput.size() == 4) {
        addbus.code = listinput.get(0);
        addbus.lp = listinput.get(1);
        addbus.timeA = stoi(listinput.get(2));
        addbus.timeB = stoi(listinput.get(3));
        addbus.Case = 0;
    }
    else return "-1";
    if (checkcode(addbus.code) || checklp(addbus.lp)) return "-1";
    if (addbus.timeA >= addbus.timeB) return "-1";
    listinput.clear(); // clear listinput to get new data
    if (bus.size() == 0) {
        vector<Xe> x;
        x.push_back(addbus);
        bus.push_back(x);
        listcount.add(1);
        // return 
        std::stringstream rt;
        rt << listcount.fragmentPointers[0]->getdata();
        return rt.str();
    }
    else {
        bool checkcode = 0;
        int i = 0;
        int siz = bus.size();
        for (; i < siz; i++) {
            if (bus[i].at(0).code == addbus.code) {
                checkcode = 1;
                break;
            }
        }
        if (checkcode) {
            if (listcount.fragmentPointers[i]->getdata() >= max) return "-1";
            int isiz = bus[i].size();
            for (int j = 0; j < isiz; j++) {
                if (bus[i].at(j).timeA == addbus.timeA && bus[i].at(j).Case == addbus.Case) return "-1";
                if (bus[i].at(j).timeA > addbus.timeA) return "-1";
                else if (bus[i].at(j).timeA > addbus.timeB && bus[i].at(j).Case == addbus.Case) return"-1";
                else if (bus[i].at(j).lp == addbus.lp) {
                    if (bus[i].at(j).timeA >= addbus.timeA || bus[i].at(j).timeA >= addbus.timeB || addbus.timeA <= bus[i].at(j).timeB || addbus.timeB <= bus[i].at(j).timeB)
                        return "-1";
                }
                else if (bus[i].at(j).timeA < addbus.timeA && bus[i].at(j).timeB > addbus.timeB && bus[i].at(j).Case == addbus.Case) return "-1";
                
            }
            bus[i].push_back(addbus);
            int a = listcount.fragmentPointers[i]->getdata() + 1;
            listcount.fragmentPointers[i]->setdata(a);
            //return 
            std::stringstream rt;
            rt << listcount.fragmentPointers[i]->getdata();;
            return rt.str();
        }
        else {
            vector<Xe> x;
            x.push_back(addbus);
            bus.push_back(x);
            listcount.add(1);
            //return 
            std::stringstream rt;
            rt << listcount.fragmentPointers[bus.size()-1]->getdata();
            return rt.str();
        }
    }
    return "-1";
}
string BusSystem::del(string s) {
    process(s);
    Xe delbus;
    if (listinput.size() == 1) {
        delbus.code = listinput.get(0);
    }
    else if (listinput.size() == 2) {
        delbus.code = listinput.get(0);
        delbus.timeA = stoi(listinput.get(1));
    }
    else if (listinput.size() == 3) {
        delbus.code = listinput.get(0);
        delbus.timeA = stoi(listinput.get(1));
        delbus.timeB = stoi(listinput.get(2));
    }
    if (checkcode(delbus.code) || checklp(delbus.lp)) return "-1";
    listinput.clear(); // clear li to get new data;
    int i = 0;
    int siz = bus.size();
    for (; i < siz; i++) {
        if (bus[i].at(0).code == delbus.code) break;
    }
    int countdel = 0;
    if (delbus.timeA == 0) {
        countdel = bus[i].size();
        bus[i].erase(bus[i].begin(), bus[i].end());
        listcount.fragmentPointers[i]->setdata(0);
        // return 
        std::stringstream rt;
        rt << countdel;
        return rt.str();
    }
    else if (delbus.timeB == 0) {
        int isiz = bus[i].size();
        for (int j = 0; j < isiz; j++) {
            if (bus[i].at(j).timeA == delbus.timeA) {
                countdel++;
                bus[i].erase(bus[i].begin() + j);
                int a = listcount.fragmentPointers[i]->getdata() - 1;
                listcount.fragmentPointers[i]->setdata(a);
                j--;
                isiz = bus[i].size();
            }
        }
        // return 
        std::stringstream rt;
        rt << countdel;
        return rt.str();
    }
    else if (delbus.timeB != 0) {
        int siz = bus[i].size();
        for (int j = 0; j < siz; j++) {
            if (bus[i].at(j).timeA >= delbus.timeA && bus[i].at(j).timeA <= delbus.timeB) {
                countdel++;
                bus[i].erase(bus[i].begin() + j);
                int a = listcount.fragmentPointers[i]->getdata() - 1;
                listcount.fragmentPointers[i]->setdata(a);
                siz = bus[i].size();
                j--;
            }
        }
        // return 
        std::stringstream rt;
        rt << countdel;
        return rt.str();
    }
    else return "-1";
}
string BusSystem::cs(string s) {
    process(s);
    Xe csbus;
    if (listinput.size() == 2) {
        csbus.code = listinput.get(0);
        csbus.timeA = stoi(listinput.get(1));
    }
    else if (listinput.size() == 3) {
        csbus.code = listinput.get(0);
        csbus.timeA = stoi(listinput.get(1));
        csbus.Case = stoi(listinput.get(2));
    }
    else return "-1";
    if (checkcode(csbus.code) || checklp(csbus.lp)) return "-1";
    listinput.clear();
    int i = 0;
    int siz = bus.size();
    for (; i < siz; i++) {
        if (bus[i].at(0).code == csbus.code) break;
    }
    int count = 0;
    int isiz = bus[i].size();
    for (int j = 0; j < isiz; j++) {
        if (csbus.Case == -1) {
            if (bus[i].at(j).timeA <= csbus.timeA && bus[i].at(j).timeB >= csbus.timeA) {
                count++;
            }
        }
        else {
            if (bus[i].at(j).timeA <= csbus.timeA && bus[i].at(j).timeB >= csbus.timeA && bus[i].at(j).Case == csbus.Case) {
                count++;
            }
        }
    }
    // return 
    std::stringstream rt;
    rt << count;
    return rt.str();
}
string BusSystem::ce(string s) {
    {
        process(s);
        Xe cebus;
        if (listinput.size() == 2) {
            cebus.code = listinput.get(0);
            cebus.timeA = stoi(listinput.get(1));
        }
        else if (listinput.size() == 3) {
            cebus.code = listinput.get(0);
            cebus.timeA = stoi(listinput.get(1));
            cebus.Case = stoi(listinput.get(2));
        }
        else return "-1";
        if (checkcode(cebus.code) || checklp(cebus.lp)) return "-1";
        listinput.clear();
        int i = 0;
        int siz = bus.size();
        for (; i < siz; i++) {
            if (bus[i].at(0).code == cebus.code) break;
        }
        int count = 0;
        int isiz = bus[i].size();
        for (int j = 0; j < isiz; j++) {
            if (cebus.Case == -1) {
                if (bus[i].at(j).timeB < cebus.timeA) {
                    count++;
                }
            }
            else {
                if (bus[i].at(j).timeB < cebus.timeA && bus[i].at(j).Case == cebus.Case) {
                    count++;
                }
            }
        }
        // return 
        std::stringstream rt;
        rt << count;
        return rt.str();
    }
}
string BusSystem::gs(string s) {
    process(s);
    Xe gebus;
    if (listinput.size() == 2) {
        gebus.code = listinput.get(0);
        gebus.timeA = stoi(listinput.get(1));
    }
    else if (listinput.size() == 3) {
        gebus.code = listinput.get(0);
        gebus.timeA = stoi(listinput.get(1));
        gebus.Case = stoi(listinput.get(2));
    }
    if (checkcode(gebus.code) || checklp(gebus.lp)) return "-1";
    listinput.clear();
    int i = 0;
    int siz = bus.size();
    for (; i < siz; i++) {
        if (bus[i].at(0).code == gebus.code) break;
    }
    bool a = 0;
    int j = 0;
    int isiz = bus[i].size();
    int min = 0;
    for (; j < isiz; j++) {
        if (gebus.Case != -1) {
            if (gebus.Case == bus[i].at(j).Case && gebus.timeA >= bus[i].at(j).timeA) {
                a = 1;
                min = gebus.timeA - bus[i].at(j).timeA;
                break;
            }
        }
        else {
            if (gebus.timeA >= bus[i].at(j).timeA) {
                min = gebus.timeA - bus[i].at(j).timeA;
                break;
            }
        }

    }
    string lpp = "-1";
    if (a) {
        int k = 0;
        for (; k < isiz; k++) {
            if (gebus.Case != -1) {
                if (bus[i].at(k).timeA <= gebus.timeA && bus[i].at(k).Case == gebus.Case) {
                    int minn = abs(-bus[i].at(k).timeA + gebus.timeA);
                    if (minn <= min) {
                        min = minn;
                        lpp = bus[i].at(k).lp;
                    }
                }
            }
        }
    }
    else {
        int k = 0;
        for (; k < isiz; k++) {
            if (bus[i].at(k).timeA <= gebus.timeA) {
                int minn = gebus.timeA - bus[i].at(j).timeA;
                if (minn <= min) {
                    min = minn;
                    lpp = bus[i].at(k).lp;
                }
            }

        }
    }
    return lpp;
}
string BusSystem::ge(string s) {
    process(s);
    Xe gsbus;
    if (listinput.size() == 2) {
        gsbus.code = listinput.get(0);
        gsbus.timeA = stoi(listinput.get(1));
    }
    else if (listinput.size() == 3) {
        gsbus.code = listinput.get(0);
        gsbus.timeA = stoi(listinput.get(1));
        gsbus.Case = stoi(listinput.get(2));
    }
    if (checkcode(gsbus.code) || checklp(gsbus.lp)) return "-1";
    listinput.clear();
    int i = 0;
    int siz = bus.size();
    for (; i < siz; i++) {
        if (bus[i].at(0).code == gsbus.code) break;
    }
    bool a = 0;
    int j = 0;
    int isiz = bus[i].size();
    int min = 0;
    for (; j < isiz; j++) {
        if (gsbus.Case != -1) {
            if (gsbus.Case == bus[i].at(j).Case && gsbus.timeA > bus[i].at(j).timeB) {
                min = gsbus.timeA - bus[i].at(j).timeB;
                a = 1;
                break;
            }
        }
        else {
            if (gsbus.timeA > bus[i].at(j).timeB) {
                min = gsbus.timeA - bus[i].at(j).timeB;
                break;
            }
        }
    }
    string lpp = "-1";
    if (a) {
        int k = 0;
        for (; k < isiz; k++) {
            if (gsbus.Case != -1) {
                if (bus[i].at(k).timeB < gsbus.timeA && bus[i].at(k).Case == gsbus.Case) {
                    int minn = abs(-bus[i].at(k).timeB + gsbus.timeA);
                    if (minn <= min) {
                        min = minn;
                        lpp = bus[i].at(k).lp;
                    }
                }
            }
        }
    }
    else {
        int k = 0;
        for (; k < isiz; k++) {
            if (bus[i].at(k).timeB < gsbus.timeA) {
                int minn = abs(-bus[i].at(k).timeB + gsbus.timeA);
                if (minn <= min) {
                    min = minn;
                    lpp = bus[i].at(k).lp;
                }
            }

        }
    }
    return lpp;
}
