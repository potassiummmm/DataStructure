#include <iostream>

template<class T>
class Vector
{
    typedef T *iterator;
private:
    iterator beg;
    iterator end;
    int vecSize;
    int vecCapacity;
public:
    explicit Vector(int capacity = 10) : vecCapacity(capacity), vecSize(0)
    {
        beg = new T[capacity];
        end = beg;
    }

    ~Vector() = default;

    iterator begin() { return beg; }

    int size() const { return vecSize; }

    T capacity() const { return vecCapacity; }

    bool empty() const { return vecSize == 0; }

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

    void popBack()
    {
        vecSize--;
        end = beg + (vecSize? vecSize - 1 : 1);
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


template<class T>
struct LinkedNode
{
    T element;
    LinkedNode<T> *next;

    LinkedNode() = default;

    explicit LinkedNode(const T &element) { this->element = element; }

    LinkedNode(const T &element, LinkedNode<T> *next)
    {
        this->element = element;
        this->next = next;
    }
};

template<class T>
class LinkedQueue
{
private:
    LinkedNode<T> *queueFront;
    LinkedNode<T> *queueBack;
    int queueSize;
public:
    LinkedQueue()
    {
        queueFront = nullptr;
        queueSize = 0;
    }

    ~LinkedQueue();

    bool isEmpty() { return queueSize == 0; }

    void pop();

    T &front();

    void push(const T &);
};

template<class T>
LinkedQueue<T>::~LinkedQueue()
{
    while(queueFront != nullptr)
    {
        LinkedNode<T> *nextNode = queueFront->next;
        delete queueFront;
        queueFront = nextNode;
    }
}

template<class T>
void LinkedQueue<T>::pop()
{
    LinkedNode<T> *nextNode = queueFront->next;
    delete queueFront;
    queueFront = nextNode;
    queueSize--;
}

template<class T>
T &LinkedQueue<T>::front()
{
    return queueFront->element;
}

template<class T>
void LinkedQueue<T>::push(const T &theElement)
{
    auto *newNode = new LinkedNode<T>(theElement, nullptr);
    if(queueSize == 0)
    {
        queueFront = newNode;
    }
    else
    {
        queueBack->next = newNode;
    }
    queueBack = newNode;
    queueSize++;
}


template<class T>
class MinHeap
{
private:
    int heapSize;
    int arrayLength;
    T *heap;
public:
    explicit MinHeap(int initialCapacity = 10);

    ~MinHeap() { delete[] heap; }

    const T &top()
    {
        return heap[1];
    }

    void pop();

    void push(const T &);

    bool isEmpty();
};


template<class T>
MinHeap<T>::MinHeap(int initialCapacity)
{
    arrayLength = initialCapacity + 1;//堆元素从1开始存储
    heap = new T[arrayLength];
    heapSize = 0;
}

template<class T>
void MinHeap<T>::push(const T &theElement)
{
    if(heapSize == arrayLength - 1)//堆满
    {
        auto newHeap = new T[2 * arrayLength];
        for(int i = 1;i <= heapSize;i++)
        {
            newHeap[i] = heap[i];
        }
        delete[]heap;
        heap = newHeap;
        arrayLength *= 2;
    }
    int currentNode = ++heapSize;
    while(currentNode != 1 && heap[currentNode / 2] > theElement)
    {
        heap[currentNode] = heap[currentNode / 2];
        currentNode /= 2;
    }
    heap[currentNode] = theElement;
}

template<class T>
void MinHeap<T>::pop()
{
    heap[1].~T();
    T lastElement = heap[heapSize--];
    int currentNode = 1, child = 2;
    while(child <= heapSize)
    {
        if(child < heapSize && heap[child] > heap[child + 1])
        {
            child++;
        }
        if(lastElement <= heap[child])
        {
            break;
        }
        heap[currentNode] = heap[child];
        currentNode = child;
        child *= 2;
    }
    heap[currentNode] = lastElement;
}

template<class T>
bool MinHeap<T>::isEmpty()
{
    return heapSize == 0;
}


class Edge
{
private:
    std::string v;
    std::string w;
    double weight = 0;
public:
    Edge() = default;

    Edge(std::string v, std::string w, double weight) : v(std::move(v)),
                                                        w(std::move(w)),
                                                        weight(weight) { }

    std::string getEither() const
    {
        return v;
    }

    std::string getOther(const std::string &vertex) const
    {
        if(vertex == v)
        {
            return w;
        }
        else if(vertex == w)
        {
            return v;
        }
        else
        {
            return "";
        }
    }

    friend std::ostream &operator<<(std::ostream &os, const Edge &edge)
    {
        os << edge.v << "-<" << edge.weight << ">->" << edge.w << "      ";
        return os;
    }

    operator double() const { return weight; }
};

class EdgeWeightedGraph
{
private:
    int vertexNum;
    Vector<std::string> vertices;//顶点
    Vector<Vector<Edge>> adj;//邻接表
public:
    EdgeWeightedGraph() : vertexNum(0) { }

    ~EdgeWeightedGraph() = default;

    int getVertexNum() const
    {
        return vertexNum;
    }

    Vector<Edge> getAdj(int v)
    {
        return adj[v];
    }

    Vector<std::string> getVertices()
    {
        return vertices;
    }

    void addVertices()
    {
        std::cout << "请输入顶点的个数: ";
        std::cin >> vertexNum;
        if(vertexNum <= 0)
        {
            std::cout << "顶点个数必须为正整数!" << std::endl;
            return;
        }
        std::cout << "请依次输入各顶点的名称: " << std::endl;
        for(int i = 0;i < vertexNum;i++)
        {
            std::string a;
            std::cin >> a;
            vertices.pushBack(a);
            adj.pushBack(Vector<Edge>());
        }
    }

    void addEdges()
    {
        std::string v, w;
        int weight;
        while(std::cout << "请输入两个顶点及边: ", std::cin >> v >> w >> weight, v != "?")
        {
            if(vertices.find(v) == -1)
            {
                std::cout << "顶点" + v + "不存在!" << std::endl;
                return;
            }
            if(vertices.find(w) == -1)
            {
                std::cout << "顶点" + w + "不存在!" << std::endl;
                return;
            }
            if(v == w)
            {
                std::cout << "不能形成自环!" << std::endl;
                return;
            }
            Edge edge(v, w, weight);
            adj[vertices.find(v)].pushBack(edge);
        }
    }
};


class PrimLST
{
private:
    bool *marked;//最小生成树的顶点
    LinkedQueue<Edge> mst;//最小生成树的边
    MinHeap<Edge> pq;//横切边
    Vector<std::string> vertices;

    void visit(EdgeWeightedGraph graph, const std::string &v)
    {
        int index = vertices.find(v);
        marked[index] = true;
        for(int i = 0;i < graph.getAdj(index).size();i++)
        {
            if(!marked[vertices.find(graph.getAdj(index)[i].getOther(v))])
            {
                pq.push(graph.getAdj(index)[i]);
            }
        }
    }

public:
    PrimLST()
    {
        marked = new bool[10];
    }

    ~PrimLST()
    {
        delete[] marked;
    }

    void generate(EdgeWeightedGraph graph, const std::string &start)
    {
        vertices = graph.getVertices();
        if(graph.getVertexNum() > 10)
        {
            delete[] marked;
            marked = new bool[graph.getVertexNum()];
        }
        visit(graph, start);
        while(!pq.isEmpty())
        {
            Edge edge = pq.top();
            pq.pop();
            std::string v = edge.getEither();
            std::string w = edge.getOther(v);
            if(marked[vertices.find(v)] && marked[vertices.find(w)])
            {
                continue;
            }
            mst.push(edge);
            if(!marked[vertices.find(v)])
            {
                visit(graph, v);
            }
            if(!marked[vertices.find(w)])
            {
                visit(graph, w);
            }
        }
    }

    void show()
    {
        while(!mst.isEmpty())
        {
            std::cout << mst.front();
            mst.pop();
        }
        std::cout << std::endl;
    }
};

int main()
{
    std::cout << "**        电网造价格模拟系统         **\n"
              << "====================================\n"
              << "**        A --- 创建电网顶点        **\n"
              << "**        B --- 添加电网的边        **\n"
              << "**        C --- 构造最小生成树      **\n"
              << "**        D --- 显示最小生成树      **\n"
              << "**        E --- 退出程序           **\n"
              << "====================================\n";
    EdgeWeightedGraph graph;
    PrimLST primLst;
    char command;
    while(std::cout << "\n请选择操作: ", std::cin >> command)
    {
        switch(command)
        {
            case 'A':
            {
                graph.addVertices();
                break;
            }
            case 'B':
            {
                graph.addEdges();
                break;
            }
            case 'C':
            {
                std::string start;
                std::cout << "请输入起始顶点: ";
                std::cin >> start;
                primLst.generate(graph, start);
                std::cout << "生成Prim最小生成树!" << std::endl;
                break;
            }
            case 'D':
            {
                std::cout << "最小生成树的顶点及边为:\n" << std::endl;
                primLst.show();
                break;
            }
            case 'E':
            {
                std::cout << "退出成功!" << std::endl;
                return 0;
            }
            default:
            {
                std::cout << "请输入A、B、C、D、E!\n" << std::endl;
                break;
            }
        }
    }
}
