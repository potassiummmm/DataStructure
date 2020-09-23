#include<iostream>

struct CandidateInfoNode
{
    int candidateNum;
    int age;
    std::string name;
    std::string sex;
    std::string applicationType;
    CandidateInfoNode* next;

    CandidateInfoNode() : candidateNum(0), age(0), sex(std::string()),
        name(std::string()), applicationType(std::string()),
        next(nullptr) {}

    CandidateInfoNode(const int& candidateNum, const int& age, std::string sex,
        std::string name, std::string applicationType,
        CandidateInfoNode* next)
    {
        this->candidateNum = candidateNum;
        this->age = age;
        this->name = name;
        this->sex = sex;
        this->applicationType = applicationType;
        this->next = next;
    }

    ~CandidateInfoNode() = default;

    friend std::ostream&
        operator<<(std::ostream& os, const CandidateInfoNode& candidateInfo)
    {
        os << candidateInfo.candidateNum << '\t'
            << candidateInfo.name << '\t'
            << candidateInfo.sex << '\t'
            << candidateInfo.age << '\t'
            << candidateInfo.applicationType;
        return os;
    }
};



class CandidateInfoLinkedList
{
private:
    CandidateInfoNode *firstNode;
    int length;
public:
    CandidateInfoLinkedList() : firstNode(nullptr),length(0) {}

    ~CandidateInfoLinkedList();

    bool checkIndex(int index) const;

    bool find(int candidateNum) const;

    bool insert(int index,int candidateNum,int age,
                const std::string &name,
                const std::string &sex,
                const std::string &applicationType);

    bool modify(int selectedCandidateNum,int candidateNum,int age,
                const std::string &name,
                const std::string &sex,
                const std::string &applicationType);

    bool erase(int candidateNum);
    bool output();
};