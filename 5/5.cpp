#include <iostream>

class Customer
{
private:
    int num;
    Customer *next;
public:
    friend class BankQueue;

    explicit Customer(int num) : num(num), next(nullptr) { }

    ~Customer() = default;
};


class BankQueue
{
private:
    Customer *frontNode;
    Customer *backNode;
    int queueSize;
public:
    BankQueue();

    ~BankQueue() = default;

    bool empty() const;

    void push(int elem);

    void pop();

    bool isValid();

    bool handle(int rate);
};

BankQueue::BankQueue()
{
    frontNode = backNode = nullptr;
    queueSize = 0;
}

bool BankQueue::empty() const
{
    return queueSize == 0;
}

void BankQueue::push(int elem)
{
    auto *newNode = new Customer(elem);
    if(empty())
    {
        frontNode = newNode;
    }
    else
    {
        backNode->next = newNode;
    }
    backNode = newNode;
    queueSize++;
}

void BankQueue::pop()
{
    auto deleteNode = frontNode;
    frontNode = frontNode->next;
    queueSize--;
    delete deleteNode;
}

bool BankQueue::isValid()
{
    for(auto curNode = frontNode;curNode != nullptr;curNode = curNode->next)
    {
        if(curNode->num <= 0)
        {
            return false;
        }
    }
    return true;
}

bool BankQueue::handle(int rate)
{
    if(empty())
    {
        return true;
    }
    for(int i = 0;i < rate;i++)
    {
        std::cout << frontNode->num;
        pop();
        if(empty())
        {
            return true;
        }
        std::cout << ' ';
    }
    return false;
}


int main()
{
    std::cout << "请输入一行正整数，第一个数字表示顾客总数，后面表示每个顾客的编号" << std::endl;
    int N;
    std::cin >> N;
    if(N <= 0)
    {
        std::cout << "顾客总数必须为正整数!" << std::endl;
        return 0;
    }
    BankQueue bqA, bqB;
    int num;
    for(int i = 0;i < N;i++)
    {
        std::cin >> num;
        num % 2? bqA.push(num) : bqB.push(num);
    }
    bool endA = bqA.empty();
    bool endB = bqB.empty();
    while(true)
    {
        if(!bqA.isValid() || !bqB.isValid())
        {
            std::cout << "顾客编号必须为正整数!" << std::endl;
            return 0;
        }
        if(!endA && bqA.handle(2))//A空了
        {
            if(bqB.empty())
            {
                std::cout << std::endl;
                return 0;
            }
            std::cout << ' ';
            endA = true;
        }
        if(!endB && bqB.handle(1))//B空了
        {
            if(bqA.empty())
            {
                std::cout << std::endl;
                return 0;
            }
            std::cout << ' ';
            endB = true;
        }
    }
}