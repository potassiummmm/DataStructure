#include <iostream>

struct LinkedNode
{
    int element;
    LinkedNode *next;

    LinkedNode() = default;

    ~LinkedNode() = default;
};

class LinkedList
{
private:
    LinkedNode *head;
    int size;

public:
    LinkedList() : head(nullptr), size(0)
    {
    };

    ~LinkedList()
    {
        delete head;
    };

    void insert(const int &elem, int index);

    bool empty() const;

    bool isValid() const;

    LinkedList findIntersection(const LinkedList &linkedList);

    friend std::istream &operator>>(std::istream &is, LinkedList &linkedList);

    friend std::ostream &
    operator<<(std::ostream &os, const LinkedList &linkedList);
};

void LinkedList::insert(const int &elem, int index)
{
    auto *newNode = new LinkedNode();
    if(index == 0)//插入队首
    {
        newNode->next = head;
        newNode->element = elem;
        head = newNode;
    }
    else if(index == size)//插入队尾
    {
        LinkedNode *preNode = head;
        for(int i = 0;i < index - 1;i++)
        {
            preNode = preNode->next;
        }
        newNode->element = elem;
        preNode->next = newNode;
    }
    else
    {
        LinkedNode *preNode = head;
        for(int i = 0;i < index - 1;i++)
        {
            preNode = preNode->next;
        }
        newNode->next = preNode->next->next;
        newNode->element = elem;
        preNode->next = newNode;
    }
    size++;
}

bool LinkedList::empty() const
{
    return size == 0;
}

bool LinkedList::isValid() const
{
    auto curNode = head;
    while(curNode != nullptr && curNode->next != nullptr)
    {
        if(curNode->element > curNode->next->element || curNode->element < 0 ||
           curNode->next->element < 0)
        {
            return false;
        }
        curNode = curNode->next;
    }
    return true;
}

LinkedList LinkedList::findIntersection(const LinkedList &linkedList)
{
    LinkedList intersection = LinkedList();
    LinkedNode *curNode1 = this->head;
    LinkedNode *curNode2 = linkedList.head;
    while(curNode1 != nullptr && curNode2 != nullptr)
    {
        if(curNode1->element == curNode2->element)
        {
            intersection.insert(curNode1->element, intersection.size);
            curNode1 = curNode1->next;
            curNode2 = curNode2->next;
        }
        else if(curNode1->element < curNode2->element)
        {
            curNode1 = curNode1->next;
        }
        else
        {
            curNode2 = curNode2->next;
        }
    }
    return intersection;
}

std::istream &operator>>(std::istream &is, LinkedList &linkedList)
{
    int n;
    int cnt = 0;
    while(is >> n, n != -1)
    {
        linkedList.insert(n, cnt++);
    }
    return is;
}

std::ostream &operator<<(std::ostream &os, const LinkedList &linkedList)
{
    if(linkedList.head == nullptr)
    {
        os << "NULL";
    }
    else
    {
        LinkedNode *curNode = linkedList.head;
        while(true)
        {
            os << curNode->element;
            if(curNode->next != nullptr)
            {
                os << ' ';
                curNode = curNode->next;
            }
            else
            {
                break;
            }
        }
    }
    return os;
}


int main()
{
    while(true)
    {
        std::cout << "请输入两个非降序序列,用-1表示单个序列的结尾,输入EOF退出" << std::endl;
        LinkedList linkedList1, linkedList2;
        std::cin >> linkedList1 >> linkedList2;
        if(linkedList1.isValid() && linkedList2.isValid())
        {
            std::cout << "两序列的交集序列为\n"
                      << linkedList1.findIntersection(linkedList2) << '\n'
                      << std::endl;
        }
        else
        {
            std::cout << "链表必须为正整数构成的非降序序列!\n" << std::endl;
        }
    }
}