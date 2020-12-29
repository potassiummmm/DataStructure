#include <iostream>

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

    void initialize(T *, int);
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
void MinHeap<T>::initialize(T *theHeap, int theSize)
{
    delete[] heap;
    heap = theHeap;
    heapSize = theSize;

    for(int root = heapSize / 2;root >= 1;root--)
    {
        T rootElement = heap[root];

        int child = 2 * root;
        while(child <= heapSize)
        {
            if(child < heapSize && heap[child] > heap[child + 1])
            {
                child++;
            }

            if(rootElement <= heap[child])
            {
                break;
            }

            heap[child / 2] = heap[child];
            child *= 2;
        }
        heap[child / 2] = rootElement;
    }

}


struct HuffmanNode
{
    int weight;

    HuffmanNode() : weight(0) { }

    explicit HuffmanNode(int weight) : weight(weight) { }

    operator int() const { return weight; }
};

class HuffmanTree
{
private:
    HuffmanNode *root;
    int cost = 0;
public:
    explicit HuffmanTree(HuffmanNode *rootNode);

    HuffmanTree(const int *weight, int n);

    void mergeTree(HuffmanTree *leftTree, HuffmanTree *rightTree);

    int getMinCost() const
    {
        return cost;
    }
};

HuffmanTree::HuffmanTree(HuffmanNode *rootNode)
{
    this->root = rootNode;
}

HuffmanTree::HuffmanTree(const int *weight, int n)
{
    root = nullptr;
    auto huffmanTree = new HuffmanNode[n + 1];
    for(int i = 1;i <= n;i++)
    {
        huffmanTree[i].weight = weight[i - 1];
    }
    MinHeap<HuffmanNode> heap;
    heap.initialize(huffmanTree, n);
    for(int i = 1;i < n;i++)
    {
        auto x = heap.top();
        heap.pop();
        auto y = heap.top();
        heap.pop();
        mergeTree(new HuffmanTree(&x), new HuffmanTree(&y));
        heap.push(*root);
    }
}

void HuffmanTree::mergeTree(HuffmanTree *leftTree, HuffmanTree *rightTree)
{
    root = new HuffmanNode;
    root->weight = leftTree->root->weight + rightTree->root->weight;
    cost += root->weight;
    leftTree->root = rightTree->root = nullptr;
}


int main()
{
    int N, lengthOfWood;
    std::cin >> N;
    if(N <= 0)
    {
        std::cout << "木头的段数必须为正整数!" << std::endl;
        return 0;
    }
    if(N > 10000)
    {
        std::cout << "木头的段数必须不大于10000!" << std::endl;
        return 0;
    }

    int *wood = new int[N];
    for(int i = 0;i < N;i++)
    {
        std::cin >> lengthOfWood;
        if(lengthOfWood <= 0)
        {
            std::cout << "木头的长度必须为正整数!" << std::endl;
            return 0;
        }
        wood[i] = lengthOfWood;
    }
    HuffmanTree huffmanTree(wood, N);
    std::cout << huffmanTree.getMinCost() << std::endl;
    return 0;
}
