#include"LinkedList.h"
#include<iostream>

CandidateInfoLinkedList::~CandidateInfoLinkedList()
{
    while(firstNode != nullptr)
    {
        CandidateInfoNode *nextNode = firstNode->next;
        delete firstNode;
        firstNode = nextNode;
    }
}

bool CandidateInfoLinkedList::checkIndex(int index) const
{
    return index >= 0 && index < length;
}

bool CandidateInfoLinkedList::find(int candidateNum) const
{
    CandidateInfoNode *currentNode = firstNode;
    for(int i = 0;i < length;i ++)
    {
        if(currentNode->candidateNum == candidateNum)
        {
            std::cout << "考号\t" << "姓名\t" << "性别\t" << "年龄\t"
                      << "报考类别\n" << *currentNode << std::endl;
            return true;
        }
    }
    return false;
}

bool CandidateInfoLinkedList::insert(int index,int candidateNum,int age,
                                     const std::string &name,
                                     const std::string &sex,
                                     const std::string &applicationType)
{
    if(index < 0 || index > length)
    {
        return false;
    }
    else
    {
        if(index == 0)
        {
            auto insertedNode = new CandidateInfoNode(candidateNum,age,sex,name,
                                                      applicationType,
                                                      firstNode);
            firstNode = insertedNode;
        }
        else
        {
            auto preNode = firstNode;
            for(int i = 0;i < index - 1;i ++)
            {
                preNode = preNode->next;
            }
            auto insertedNode = new CandidateInfoNode(candidateNum,age,sex,name,
                                                      applicationType,
                                                      preNode->next);
            preNode->next = insertedNode;
        }
        length ++;
    }
    return true;
}

bool CandidateInfoLinkedList::modify(int selectedCandidateNum,int candidateNum,int age,
                                     const std::string &name,
                                     const std::string &sex,
                                     const std::string &applicationType)
{
    CandidateInfoNode* currentNode = firstNode;
    for (int i = 0; i < length; i++)
    {
        if (currentNode->candidateNum == selectedCandidateNum)
        {
            currentNode->candidateNum = candidateNum;
            currentNode->age = age;
            currentNode->sex = sex;
            currentNode->applicationType = applicationType;
            return true;
        }
    }
    return false;
}

bool CandidateInfoLinkedList::erase(int candidateNum)
{
    CandidateInfoNode* currentNode = firstNode;
    for (int i = 0; i < length; i++)
    {
        if (currentNode->candidateNum == candidateNum)
        {
            std::cout << "您要删除的考生信息是：" << *currentNode << std::endl;
            auto deleteNode = firstNode;
            if (i == 0)
            {
                firstNode = firstNode->next;
            }
            else
            {
                auto preNode = firstNode;
                for (int j = 0; j < i - 1; j++)
                {
                    preNode = preNode->next;
                }
                deleteNode = preNode->next;
                preNode->next = preNode->next->next;
            }
            delete deleteNode;
            length--;
            return true;
        }
        currentNode = currentNode->next;
    }
    return false;
}

bool CandidateInfoLinkedList::output()
{
    auto currentNode = firstNode;
    if(length == 0)
    {
        return false;
    }
    std::cout << "考号\t" << "姓名\t" << "性别\t" << "年龄\t"
        << "报考类别" << std::endl;
    for(int i = 0;i < length;i ++)
    {
        std::cout << *currentNode << std::endl;
        currentNode = currentNode->next;
    }
    std::cout << std::endl;
    return true;
}


int main()
{
    auto* candidateInfoList = new CandidateInfoLinkedList();
    std::cout << "首先请建立考生信息系统！\n请输入考生人数:";
    int n;
    while (std::cin >> n, n <= 0)
    {
        std::cout << "请输入一个正整数！\n请输入考生人数:";
    }
    std::cout << "请依次输入考生的考号、姓名、性别、年龄及报考类别！" << std::endl;
    for (int i = 0; i < n; i++)
    {
        int candidateNum = 0;
        std::string name;
        int age = 0;
        std::string sex;
        std::string applicationType;
        std::cin >> candidateNum >> name >> sex >> age >> applicationType;
        candidateInfoList->insert(i, candidateNum, age, name, sex, applicationType);
    }
    candidateInfoList->output();

    std::cout << "请选择您要进行的操作（1为插入，2为删除，3为查找，4为修改，5为统计，0为取消操作\n"
        << std::endl;

    while (true)
    {
        std::cout << "请选择您要进行的操作:";
        int choiceNum = 0;
        std::cin >> choiceNum;
        switch (choiceNum)
        {
        case 1:
        {
            int index = 0;
            std::cout << "请选择您要插入的考生的位置:";
            std::cin >> index;
            if (!candidateInfoList->checkIndex(index - 1))
            {
                std::cout << "插入的位置不合法！" << std::endl;
                break;
            }
            std::cout << "请依次输入要插入的考生的考号、姓名、性别、年龄及报考类别！" << std::endl;
            int candidateNum = 0;
            std::string name;
            int age = 0;
            std::string sex;
            std::string applicationType;
            std::cin >> candidateNum >> name >> sex >> age
                >> applicationType;
            candidateInfoList->insert(index - 1, candidateNum, age, name, sex, applicationType);
            candidateInfoList->output();
            break;
        }
        case 2:
        {
            int candidateNum = 0;
            std::cout << "请选择要删除的考生的考号:";
            std::cin >> candidateNum;
            if (!candidateInfoList->erase(candidateNum))
            {
                std::cout << "要删除的考生信息不存在！" << std::endl;
            }
            else
            {
                candidateInfoList->output();
            }
            break;
        }
        case 3:
        {
            int candidateNum = 0;
            std::cout << "请选择要查找的考生的考号:";
            std::cin >> candidateNum;
            if (!candidateInfoList->find(candidateNum))
            {
                std::cout << "未查询到该考号的考生信息！" << std::endl;
            }
            break;
        }
        case 4:
        {
            int selectedCandidateNum = 0;
            std::cout << "请选择要修改信息的考生的考号:";
            std::cin >> selectedCandidateNum;
            if (!candidateInfoList->find(selectedCandidateNum))
            {
                std::cout << "要修改的的考生信息不存在！" << std::endl;
                break;
            }
            std::cout << "请依次输入要修改的考号、姓名、性别、年龄及报考类别！" << std::endl;
            int candidateNum = 0;
            std::string name;
            int age = 0;
            std::string sex;
            std::string applicationType;
            std::cin >> candidateNum >> name >> sex >> age
                >> applicationType;
            candidateInfoList->insert(selectedCandidateNum, candidateNum, age, name, sex,
                applicationType);
            candidateInfoList->output();
            break;
        }
        case 5:
        {
            candidateInfoList->output();
            break;
        }
        case 0:
            return 0;
        default:
        {
            std::cout << "操作数不合法，请重新输入！" << std::endl;
            break;
        }
        }
    }
}