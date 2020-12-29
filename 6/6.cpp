#include <iostream>

class Member
{
private:
    std::string name;
    Member *firstChild;
    Member *sibling;
public:
    Member()
    {
        name = nullptr;
        firstChild = nullptr;
        sibling = nullptr;
    };

    explicit Member(const std::string &name)
    {
        this->name = name;
        firstChild = nullptr;
        sibling = nullptr;
    }

    const std::string &getName() const
    {
        return name;
    }

    void setName(const std::string &nameVal)
    {
        this->name = nameVal;
    }

    void addChild(Member *child)
    {
        if(firstChild == nullptr)
        {
            firstChild = child;
        }
        else
        {
            auto pos = firstChild;
            while(pos != nullptr && pos->sibling != nullptr)
            {
                pos = pos->sibling;
            }
            pos->sibling = child;
        }
    }

    void outputChildren()
    {
        for(auto child = firstChild;child != nullptr;child = child->sibling)
        {
            std::cout << child->name << ' ';
        }
    }

    friend class Family;
};


class Family
{
private:
    Member *ancestor;
public:
    Family()
    {
        ancestor = nullptr;
    };

    explicit Family(const std::string &ancestorName)
    {
        ancestor = new Member(ancestorName);
    }

    Member *findMemberByName(Member *curNode, const std::string &name) const;

    void perfect(std::istream &is);

    void addMember(const std::string &parentName, const std::string &childName);

    void erase(Member *rootNode);

    void dismiss(const std::string &memberName);

    void rename(const std::string &oldName, const std::string &newName);
};

Member *Family::findMemberByName(Member *curNode, const std::string &name) const
{
    if(curNode == nullptr)
    {
        return nullptr;
    }
    if(curNode->getName() == name)
    {
        return curNode;
    }
    for(auto child = curNode->firstChild;
        child != nullptr;child = child->sibling)
    {
        auto tmp = findMemberByName(child, name);
        if(tmp != nullptr)
        {
            return tmp;
        }
    }
    return nullptr;
}

void Family::perfect(std::istream &is)
{
    std::string parentName, childName;
    int childrenNum;
    std::cout << "请输入要建立家庭的人的姓名: ";
    std::cin >> parentName;
    auto parent = findMemberByName(ancestor, parentName);
    if(parent == nullptr)
    {
        std::cout << "没有找到姓名为" + parentName + "的人" << std::endl;
    }
    else
    {
        std::cout << "请输入" + parentName + "的儿女人数: ";
        std::cin >> childrenNum;
        std::cout << "请依次输入" + parentName + "的儿女的姓名: ";
        while(childrenNum--)
        {
            std::cin >> childName;
            parent->addChild(new Member(childName));
        }
        std::cout << parentName + "的第一代子孙是: ";
        parent->outputChildren();
        std::cout << std::endl;
    }
}

void
Family::addMember(const std::string &parentName, const std::string &childName)
{
    if(findMemberByName(ancestor, parentName) == nullptr)
    {
        std::cout << "没有找到姓名为" + parentName + "的人" << std::endl;
    }
    else
    {
        findMemberByName(ancestor, parentName)->addChild(new Member(childName));
        std::cout << parentName + "的第一代子孙是: ";
        findMemberByName(ancestor, parentName)->outputChildren();
        std::cout << std::endl;
    }
}

void Family::erase(Member *rootNode)
{
    if(rootNode != nullptr)
    {
        erase(rootNode->firstChild);
        erase(rootNode->sibling);
        delete rootNode;
    }
}

void Family::dismiss(const std::string &memberName)
{
    if(findMemberByName(ancestor, memberName) == nullptr)
    {
        std::cout << "没有找到姓名为" + memberName + "的人" << std::endl;
    }
    else
    {
        std::cout << memberName + "的第一代子孙是: ";
        findMemberByName(ancestor, memberName)->outputChildren();
        std::cout << std::endl;
        erase(findMemberByName(ancestor, memberName)->firstChild);
        findMemberByName(ancestor, memberName)->firstChild = nullptr;
    }
}

void Family::rename(const std::string &oldName, const std::string &newName)
{
    if(findMemberByName(ancestor, oldName) == nullptr)
    {
        std::cout << "没有找到姓名为" + oldName + "的人" << std::endl;
    }
    else
    {
        findMemberByName(ancestor, oldName)->setName(newName);
        std::cout << oldName + "已更名为" + newName << std::endl;
    }
}


int main()
{
    std::cout << "**          家谱管理系统          **\n" << std::string(34, '=')
              << '\n' << "**       请选择要执行的操作 :      **\n"
              << "**        A --- 完善家谱         **\n"
              << "**        B --- 添加家庭成员      **\n"
              << "**        C --- 解散局部家庭      **\n"
              << "**        D --- 更改家庭成员姓名   **\n"
              << "**        E --- 退出程序         **\n" << std::string(34, '=')
              << std::endl;
    std::cout << "首先建立一个家谱!\n请输入祖先的姓名: ";
    std::string name;
    std::cin >> name;
    Family family(name);
    std::cout << "此家谱的祖先是: " << name << std::endl;
    char command;
    while(std::cout << "\n请选择要执行的操作: ", std::cin >> command)
    {
        switch(command)
        {
            case 'A':
            {
                family.perfect(std::cin);
                break;
            }
            case 'B':
            {
                std::cout << "请输入要添加儿子(或女儿)的人的姓名: ";
                std::string parentName, childName;
                std::cin >> parentName;
                std::cout << "请输入" + parentName + "新添加的儿子(或女儿)的姓名: ";
                std::cin >> childName;
                family.addMember(parentName, childName);
                break;
            }
            case 'C':
            {
                std::cout << "请输入要解散家庭的人的姓名: ";
                std::string parentName;
                std::cin >> parentName;
                std::cout << "要解散家庭的人是: " + parentName << std::endl;
                family.dismiss(parentName);
                break;
            }
            case 'D':
            {
                std::cout << "请输入要更改姓名的人的目前姓名: ";
                std::string oldName, newName;
                std::cin >> oldName;
                std::cout << "请输入更改后的姓名: ";
                std::cin >> newName;
                family.rename(oldName, newName);
                break;
            }
            case 'E':
            {
                return 0;
            }
            default:
            {
                std::cout << "操作名必须为ABCDE!\n" << std::endl;
                break;
            }
        }
    }
    return 0;
}