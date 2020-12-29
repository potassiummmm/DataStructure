#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
#include <iomanip>

const int TERM_NUM = 8;
const int DAY_NUM = 5;
const int CLASS_PER_DAY = 10;
const int CLASS_INDEX[6] = {0, 0, 2, 5, 7, 10};//每节课开始排课的索引,从1开始


template<class T>
struct QueueNode
{
    T elem;
    QueueNode<T> *next;

    explicit QueueNode(int num) : elem(num), next(nullptr) { }

    ~QueueNode() = default;
};

template<class T>
class Queue
{
private:
    QueueNode<T> *frontNode;
    QueueNode<T> *backNode;
    int queueSize;
public:
    Queue()
    {
        frontNode = backNode = nullptr;
        queueSize = 0;
    }

    ~Queue() = default;

    bool empty() const
    {
        return queueSize == 0;
    }

    T front() const
    {
        return frontNode->elem;
    }

    void push(T elem)
    {
        auto *newNode = new QueueNode<T>(elem);
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

    void pop()
    {
        auto deleteNode = frontNode;
        frontNode = frontNode->next;
        queueSize--;
        delete deleteNode;
    }
};


template<class T>
class Vector
{
    typedef T *iterator;
private:
    iterator beg;
    iterator end;
    int vecSize;
    int vecCapacity;

    void resize(int newCapacity)
    {
        T *tmp = new T[newCapacity];
        for(int i = 0;i < vecSize;i++)
        {
            tmp[i] = beg[i];
        }
        delete[]beg;
        beg = tmp;
        end = beg + vecSize - 1;
        vecCapacity = newCapacity;
    }

public:
    explicit Vector(int capacity = 10) : vecCapacity(capacity), vecSize(0)
    {
        beg = new T[capacity];
        end = beg;
    }

    ~Vector() = default;

    iterator begin() { return beg; }

    int size() const { return vecSize; }

    T &operator[](int n) { return *(begin() + n); }

    void pushBack(const T &elem)
    {
        if(vecSize == vecCapacity)
        {
            resize(2 * vecCapacity);
        }
        beg[vecSize++] = elem;
        end = beg + vecSize - 1;
    }

    int find(const T &val) const
    {
        for(int i = 0;i < size();i++)
        {
            if(beg[i] == val)
            {
                return i;
            }
        }
        return -1;
    }
};

class Course
{
private:
    std::string id;
    std::string name;
    int classHours = 0;
    int specifiedTerm = 0;
public:
    Course() = default;

    Course(std::string id, std::string name, int classHours, int specifiedTerm)
            : id(std::move(id)), name(std::move(name)), classHours(classHours),
              specifiedTerm(specifiedTerm) { }

    friend std::istream &operator>>(std::istream &is, Course &course)
    {
        is >> course.id >> course.name >> course.classHours
           >> course.specifiedTerm;
        return is;
    }

    friend class Timetable;
};


class Edge
{
private:
    int start;
    int end;
public:
    Edge() : start(0), end(0) { }

    ~Edge() = default;

    Edge(int start, int end) : start(start), end(end) { }

    bool operator==(const Edge &edge) const
    {
        return this->start == edge.start && this->end == edge.end;
    }

    friend class Digraph;
};

class Digraph
{
private:
    Vector<int> vertices;//顶点
    Vector<Vector<Edge>> adj;//邻接表
public:
    Digraph() = default;

    ~Digraph() = default;

    void addVertices(int vertex)
    {
        vertices.pushBack(vertex);
        adj.pushBack(Vector<Edge>());
    }

    int findVertices(int vertex) const
    {
        return vertices.find(vertex);
    }

    void addEdge(const Edge &edge)
    {
        adj[vertices.find(edge.start)].pushBack(edge);
    }

    bool topologicalOrder(int *order);
};

bool Digraph::topologicalOrder(int *order)
{
    int vertexNum = vertices.size();
    int *inDegree = new int[vertexNum + 1];
    std::fill(inDegree + 1, inDegree + vertexNum + 1, 0);//初始化为0

    for(int i = 0;i < adj.size();i++)//计算每个顶点的入度
    {
        for(int j = 0;j < adj[i].size();j++)
        {
            inDegree[adj[i][j].end]++;
        }
    }

    int index = 0;
    Queue<int> queue;
    for(int i = 0;i < vertexNum - 1;i++)
    {
        if(inDegree[vertices[i]] == 0)
        {
            queue.push(vertices[i]);
        }
    }

    while(!queue.empty())
    {
        int nextVertex = queue.front();
        queue.pop();
        order[index++] = nextVertex;
        // 更新入度
        for(int i = 0;i < adj[findVertices(nextVertex)].size();i++)
        {
            inDegree[adj[findVertices(nextVertex)][i].end]--;
            if(inDegree[adj[findVertices(nextVertex)][i].end] == 0)
            {
                queue.push(adj[findVertices(nextVertex)][i].end);
            }
        }
    }

    delete[]inDegree;
    if(index != vertexNum)
    {
        std::cout << "图中有环,拓扑排序失败!" << std::endl;
    }
    return (index == vertexNum);
}

class Timetable
{
private:
    Digraph digraph;
    std::ifstream input;
    std::ofstream output;
    int courseNum;//总课程数
    int *theOrder;//课程的序列
    int *courseNumArr;//每个学期的课程数
    bool isEnglish;//英文I/O
    Vector<Course> courses;
    std::string timetable[TERM_NUM][DAY_NUM][CLASS_PER_DAY];

    static int courseNumToIndex(const std::string &courseNum)//将课程编号转化为索引
    {
        return std::stoi(courseNum.substr(1, courseNum.size()));
    }

public:
    explicit Timetable();

    void setLanguage(bool isEng);

    ~Timetable();

    bool generate();

    bool isEmpty(int term, int day, int classIndex);

    void insertCourse(const Course &course, int term, int day, int classIndex);

    bool addCourse(const Course &course, int term);

    bool CourseScheduling(const Course &course);

    void outputTimetable();

    bool setCoursePerTerm(int command);
};

Timetable::Timetable()
{
    isEnglish = false;
    input.open("./CourseInformation.txt");
    output.open("./OutputTimetable.txt");
    theOrder = new int[courseNum];
    courseNumArr = new int[TERM_NUM];
    courseNum = 0;
}

Timetable::~Timetable()
{
    input.close();
    output.close();
    delete[] theOrder;
    delete[] courseNumArr;
}

void Timetable::setLanguage(bool isEng)
{
    {
        this->isEnglish = isEng;
        if(isEnglish)
        {
            input.close();
            input.open("CourseInformation_en.txt");
        }
    }
}

bool Timetable::generate()
{
    int lines = 0;//课程行数，即课程数
    std::string line;
    getline(input, line);//跳过第一行
    while(!input.eof())//将所有课程加入Vector
    {
        getline(input, line);
        std::stringstream sStream(line);
        Course course;
        lines++;
        sStream >> course;
        digraph.addVertices(courseNumToIndex(course.id));
        courses.pushBack(course);
    }
    input.close();
    isEnglish? input.open("./CourseInformation_en.txt") : input
            .open("./CourseInformation.txt");
    while(!input.eof())//处理先修课程
    {
        getline(input, line);
        std::stringstream sStream(line);
        sStream << line;
        Course course;
        sStream >> course;
        std::string pre;
        while(sStream >> pre)
        {
            digraph.addEdge(
                    Edge(courseNumToIndex(pre), courseNumToIndex(course.id)));
        }
    }
    if(!digraph.topologicalOrder(theOrder))
    {
        std::cout << "排课失败" << std::endl;
        return false;
    }
    for(int i = 0;i < lines;i++)
    {
        CourseScheduling(courses[digraph.findVertices(theOrder[i])]);
    }
    courseNum = lines;
    return true;
}

bool Timetable::isEmpty(int term, int day, int classIndex)
{
    for(int i = CLASS_INDEX[classIndex];i < CLASS_INDEX[classIndex + 1];i++)
    {
        if(!timetable[term][day][i].empty())
        {
            return false;
        }
    }
    return true;
}

void
Timetable::insertCourse(const Course &course, int term, int day, int classIndex)
{
    for(int i = CLASS_INDEX[classIndex];i < CLASS_INDEX[classIndex + 1];i++)
    {
        timetable[term][day][i] = course.name;
    }
}

bool Timetable::addCourse(const Course &course, int term)
{
    int hours = course.classHours;

    for(int day = 0;day < DAY_NUM;day++)
    {
        int nextDay = (day + 2) % 5;

        switch(hours)
        {
            case 3:
            {
                for(int i = 2;i <= 4;i += 2)
                {
                    if(isEmpty(term, day, i))
                    {
                        insertCourse(course, term, day, i);
                        return true;
                    }
                }
                break;
            }
            case 4:
            {
                for(int i = 1;i <= 3;i += 2)
                {
                    for(int j = 1;j <= 3;j += 2)
                    {
                        if(isEmpty(term, day, i) && isEmpty(term, nextDay, j))
                        {
                            insertCourse(course, term, day, i);
                            insertCourse(course, term, nextDay, j);
                            return true;
                        }
                    }
                }
                break;
            }
            case 5:
            {
                for(int i = 2;i <= 4;i += 2)
                {
                    for(int j = 1;j <= 3;j += 2)
                    {
                        if(isEmpty(term, day, i) && isEmpty(term, nextDay, j))
                        {
                            insertCourse(course, term, day, i);
                            insertCourse(course, term, nextDay, j);
                            return true;
                        }
                    }
                }
                break;
            }
            case 6:
            {
                for(int i = 2;i <= 4;i += 2)
                {
                    for(int j = 2;j <= 4;j += 2)
                    {
                        if(isEmpty(term, day, i) && isEmpty(term, nextDay, j))
                        {
                            insertCourse(course, term, day, i);
                            insertCourse(course, term, nextDay, j);
                            return true;
                        }
                    }
                }
                break;
            }
            default:
            {
                std::cout << "存在不合理的课时!" << std::endl;
                return false;
            }
        }
    }
    return true;
}

bool Timetable::CourseScheduling(const Course &course)
{
    int terms = course.specifiedTerm;
    for(int term = 0;term < TERM_NUM;term++)
    {
        bool flag = false;
        if(terms != 0)//如果指定了学期
        {
            term = terms - 1;//转化为索引
            flag = true;
        }
        if(courseNumArr[term] <= 0)//这个学期不排课
        {
            if(!flag)
            {
                continue;
            }
            else
            {
                break;
            }
        }
        if(addCourse(course, term))
        {
            courseNumArr[term]--;
            return true;
        }
        if(flag)
        {
            break;
        }
    }
    return false;
}

void Timetable::outputTimetable()
{
    if(isEnglish)
    {
        for(int term = 0;term < TERM_NUM;term++)
        {
            output << "Term" << 1 + term << std::endl;
            output << "\t" << "    ";
            for(int day = 0;day < DAY_NUM;day++)
            {
                output << std::left << std::setw(30)
                       << "Day" + std::to_string(1 + day) << "";
            }
            output << std::endl;
            for(int course = 0;course < CLASS_PER_DAY;course++)
            {
                output << "Class" + std::to_string(1 + course) << "\t";
                for(int day = 0;day < 5;day++)
                {
                    if(timetable[term][day][course].empty())
                    {
                        output << std::left << std::setw(30) << "";
                    }
                    else
                    {
                        output << std::left << std::setw(30)
                               << timetable[term][day][course];
                    }
                }
                output << std::endl;
            }
            output << std::endl;
        }
        std::cout << "Timetable is generated successfully!" << std::endl;
    }
    else
    {
        for(int term = 0;term < TERM_NUM;term++)
        {
            output << "第" << 1 + term << "学期" << std::endl;
            output << "\t" << "    ";
            for(int day = 0;day < DAY_NUM;day++)
            {
                output << "星期" + std::to_string(1 + day) << "\t\t";
            }
            output << std::endl;
            for(int course = 0;course < CLASS_PER_DAY;course++)
            {
                output << "第" << 1 + course << "节\t";
                for(int day = 0;day < 5;day++)
                {
                    if(timetable[term][day][course].empty())
                    {
                        output << " " << "\t\t\t";
                    }
                    else
                    {
                        output << timetable[term][day][course] << "\t\t\t";
                    }
                }
                output << std::endl;
            }
            output << std::endl;
        }
        std::cout << "课表生成成功!" << std::endl;
    }
}

bool Timetable::setCoursePerTerm(int command)
{
    if(command == 0)
    {
        courseNumArr[0] = 4;
        courseNumArr[1] = 4;
        courseNumArr[2] = courseNumArr[3] = courseNumArr[4] = 5;
        courseNumArr[5] = courseNumArr[6] = courseNumArr[7] = 5;
    }
    else if(command == 1)
    {
        std::cout << "请输入每学期的课程数" << std::endl;
        int sum = 0;
        for(int i = 0;i < 8;i++)
        {
            std::cin >> courseNumArr[i];
            sum += courseNumArr[i];
        }
        if(sum != 38)
        {
            std::cout << "课程总数与输入不符！" << std::endl;
            return false;
        }
    }
    else
    {
        return false;
    }
    return true;
}


int main()
{
    Timetable timetable;
    std::cout << "请输入0(系统指定每学期课程数)或者1(手动输入每学期课程数):" << std::endl;
    int command;
    std::cin >> command;
    if(!timetable.setCoursePerTerm(command))
    {
        return 0;
    }
    std::cout << "请输入0(生成中文课表)或者1(生成英文课表),默认中文:" << std::endl;
    std::cin >> command;
    timetable.setLanguage(command);
    if(!timetable.generate())
    {
        return 0;
    }
    timetable.outputTimetable();
    return 0;
}
