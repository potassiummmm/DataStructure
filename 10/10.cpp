#include <iostream>
#include <random>
#include <ctime>

int getDigitNum(int number)
{
    std::string s = std::to_string(number);
    return s.length();
}

class Sort
{
private:
    int *data;//随机数数组
    int *dataCopy;//每个算法运行前需进行拷贝
    int *auxArray;//归并排序的辅助数组
    int dataNum;
    long swapTimes;

    int partition(int left, int right)
    {
        swapTimes++;
        int i = left;
        int j = right + 1;
        int tmp = dataCopy[left];
        while(true)
        {
            swapTimes++;
            while(dataCopy[++i] < tmp)
            {
                swapTimes++;
                if(i == right)
                {
                    break;
                }
            }
            while(tmp < dataCopy[--j])
            {
                swapTimes++;
                if(j == left)
                {
                    break;
                }
            }
            if(i >= j)
            {
                break;
            }
            std::swap(dataCopy[i], dataCopy[j]);
        }
        std::swap(dataCopy[left], dataCopy[j]);
        return j;
    }

    void quickSort(int left, int right)
    {
        swapTimes++;
        if(right <= left)
        {
            return;
        }
        int j = partition(left, right);
        quickSort(left, j - 1);
        quickSort(j + 1, right);
    }

    void mergeSort(int left, int right)
    {
        if(right <= left)
        {
            return;
        }
        int mid = (right + left) / 2;
        mergeSort(left, mid);
        mergeSort(mid + 1, right);
        merge(left, mid, right);
    }

    void sink(int up, int down)
    {
        while(2 * up <= down)
        {
            int child = 2 * up;
            if(child < down && dataCopy[child - 1] < dataCopy[child])
            {
                child++;
            }
            if(dataCopy[up - 1] >= dataCopy[child - 1])
            {
                break;
            }
            std::swap(dataCopy[up - 1], dataCopy[child - 1]);
            swapTimes++;
            up = child;
        }
    }

    void merge(int left, int mid, int right)
    {
        int i = left;
        int j = mid + 1;
        for(int k = left;k <= right;k++)
        {
            auxArray[k] = dataCopy[k];
        }
        for(int k = left;k <= right;k++)
        {
            if(i > mid)
            {
                dataCopy[k] = auxArray[j++];
            }
            else if(j > right)
            {
                dataCopy[k] = auxArray[i++];
            }
            else if(auxArray[j] < auxArray[i])
            {
                dataCopy[k] = auxArray[j++];
            }
            else
            {
                dataCopy[k] = auxArray[i++];
            }
            swapTimes++;
        }
    }

    int findMaxDigitNum()
    {
        int maxDigitNum = 0;
        for(int i = 0;i < dataNum;i++)
        {
            maxDigitNum = std::max(maxDigitNum, getDigitNum(dataCopy[i]));
        }
        return maxDigitNum;
    }

public:
    Sort();

    ~Sort();

    explicit Sort(int num);

    void generateRandomData();

    void bubbleSort();

    void selectionSort();

    void insertionSort();

    void shellSort();

    void quickSort();

    void heapSort();

    void mergeSort();

    void radixSort();
};

Sort::Sort()
{
    data = new int[10];
    dataCopy = new int[10];
    auxArray = new int[11];
    dataNum = 10;
    swapTimes = 0;
    generateRandomData();
}

Sort::~Sort()
{
    delete[] data;
    delete[] dataCopy;
    delete[] auxArray;
}

Sort::Sort(int num)
{
    data = new int[num];
    dataCopy = new int[num];
    auxArray = new int[num + 1];
    dataNum = num;
    swapTimes = 0;
    generateRandomData();
}

void Sort::generateRandomData()
{
    std::random_device r;
    std::default_random_engine engine(r());
    for(int i = 0;i < dataNum;i++)
    {
        data[i] = engine();
    }
}

void Sort::bubbleSort()
{
    for(int i = 0;i < dataNum;i++)
    {
        dataCopy[i] = data[i];
    }
    long start = clock();
    for(int i = 0;i < dataNum - 1;i++)
    {
        for(int j = 0;j < dataNum - 1 - i;j++)
        {
            swapTimes++;
            if(dataCopy[j] > dataCopy[j + 1])
            {
                std::swap(dataCopy[j], dataCopy[j + 1]);
            }
        }
    }
    long end = clock();
    std::cout << "冒泡排序所用时间:         " << end - start;
    std::cout << "\n冒泡排序交换次数:         " << swapTimes << std::endl;
    swapTimes = 0;
}

void Sort::selectionSort()
{
    for(int i = 0;i < dataNum;i++)
    {
        dataCopy[i] = data[i];
    }
    long start = clock();
    for(int i = 0;i < dataNum;++i)
    {
        int minIndex = i;
        for(int j = i + 1;j < dataNum;++j)
        {
            swapTimes++;
            if(dataCopy[minIndex] > dataCopy[j])
            {
                minIndex = j;
            }
        }
        std::swap(dataCopy[i], dataCopy[minIndex]);
    }
    long end = clock();
    std::cout << "选择排序所用时间:         " << end - start;
    std::cout << "\n选择排序交换次数:         " << swapTimes << std::endl;
    swapTimes = 0;
}

void Sort::insertionSort()
{
    for(int i = 0;i < dataNum;i++)
    {
        dataCopy[i] = data[i];
    }
    long start = clock();
    for(int i = 1;i < dataNum;i++)
    {
        for(int j = i;j > 0;j--)
        {
            if(dataCopy[j] < dataCopy[j - 1])
            {
                swapTimes++;
                std::swap(dataCopy[j], dataCopy[j - 1]);
            }
        }
    }
    long end = clock();
    std::cout << "直接插入排序所用时间:      " << end - start;
    std::cout << "\n直接插入排序交换次数:      " << swapTimes << std::endl;
    swapTimes = 0;
}

void Sort::shellSort()
{
    for(int i = 0;i < dataNum;i++)
    {
        dataCopy[i] = data[i];
    }
    long start = clock();
    int step = 1;
    while(step < dataNum / 3)
    {
        swapTimes++;
        step = 3 * step + 1;
    }
    while(step >= 1)
    {
        for(int i = step;i < dataNum;i++)
        {
            for(int j = i;j >= step && dataCopy[j] < dataCopy[j - step];j -= step)
            {
                swapTimes++;
                std::swap(dataCopy[j], dataCopy[j - step]);
            }
        }
        step /= 3;
    }
    long end = clock();
    std::cout << "希尔插入排序所用时间:      " << end - start;
    std::cout << "\n希尔插入排序交换次数:      " << swapTimes << std::endl;
    swapTimes = 0;
}

void Sort::quickSort()
{
    for(int i = 0;i < dataNum;i++)
    {
        dataCopy[i] = data[i];
    }
    long start = clock();
    quickSort(0, dataNum - 1);
    long end = clock();
    std::cout << "快速排序所用时间:         " << end - start;
    std::cout << "\n快速排序交换次数:         " << swapTimes << std::endl;
    swapTimes = 0;
}

void Sort::heapSort()
{
    for(int i = 0;i < dataNum;i++)
    {
        dataCopy[i] = data[i];
    }
    long start = clock();
    for(int i = dataNum / 2;i >= 1;i--)
    {
        sink(i, dataNum);
    }
    for(int i = dataNum;i > 1;i--)
    {
        std::swap(dataCopy[0], dataCopy[i - 1]);
        sink(1, i - 1);
    }
    long end = clock();
    std::cout << "堆排序所用时间:           " << end - start;
    std::cout << "\n堆排序交换次数:           " << swapTimes << std::endl;
    swapTimes = 0;
}

void Sort::mergeSort()
{
    for(int i = 0;i < dataNum;i++)
    {
        dataCopy[i] = data[i];
    }
    long start = clock();
    mergeSort(0, dataNum - 1);
    long end = clock();
    std::cout << "归并排序所用时间:         " << end - start;
    std::cout << "\n归并排序交换次数:         " << swapTimes << std::endl;
    swapTimes = 0;
}

void Sort::radixSort()
{
    for(int i = 0;i < dataNum;i++)
    {
        dataCopy[i] = data[i];
    }
    long start = clock();
    int radix = 10;//0-9
    int w = findMaxDigitNum();
    int count[11] = {0};
    int divisor = 1;
    for(int d = w - 1;d >= 0;d--)//从低位向高位逐个处理
    {
        for(int i = 0;i < radix + 1;i++)
        {
            swapTimes++;
            count[i] = 0;
        }
        for(int i = 0;i < dataNum;i++)
        {
            int num = (dataCopy[i] / divisor) % 10;
            count[num + 1]++;
            swapTimes++;
        }
        for(int i = 0;i < radix;i++)
        {
            count[i + 1] += count[i];
            swapTimes++;
        }
        for(int i = 0;i < dataNum;i++)
        {
            int num = (dataCopy[i] / divisor) % 10;
            auxArray[count[num]++] = dataCopy[i];
            swapTimes++;
        }
        for(int i = 0;i < dataNum;i++)
        {
            dataCopy[i] = auxArray[i];
            swapTimes++;
        }
        divisor *= 10;
        swapTimes++;
    }
    long end = clock();
    std::cout << "基数排序所用时间:         " << end - start;
    std::cout << "\n基数排序交换次数:         " << swapTimes << std::endl;
    swapTimes = 0;
}


int main()
{
    std::cout << "**                排序算法比较                **\n"
              << std::string(46, '=') << '\n'
              << "**                1 --- 冒泡排序             **\n"
              << "**                2 --- 选择排序             **\n"
              << "**                3 --- 直接插入排序          **\n"
              << "**                4 --- 希尔排序             **\n"
              << "**                5 --- 快速排序             **\n"
              << "**                6 --- 堆排序               **\n"
              << "**                7 --- 归并排序             **\n"
              << "**                8 --- 基数排序             **\n"
              << "**                9 --- 退出程序             **\n"
              << std::string(46, '=') << std::endl;
    std::cout << "请输入要产生的随机数的个数: ";
    int N, command;
    std::cin >> N;
    if(N <= 0)
    {
        std::cout << "随机数个数必须为正整数!" << std::endl;
        return 0;
    }
    Sort sort(N);
    while(std::cout << "\n请选择排序算法:           ", std::cin >> command)
    {
        switch(command)
        {
            case 1:
            {
                sort.bubbleSort();
                break;
            }
            case 2:
            {
                sort.selectionSort();
                break;
            }
            case 3:
            {
                sort.insertionSort();
                break;
            }
            case 4:
            {
                sort.shellSort();
                break;
            }
            case 5:
            {
                sort.quickSort();
                break;
            }
            case 6:
            {
                sort.heapSort();
                break;
            }
            case 7:
            {
                sort.mergeSort();
                break;
            }
            case 8:
            {
                sort.radixSort();
                break;
            }
            case 9:
            {
                std::cout << "退出成功!" << std::endl;
                return 0;
            }
            default:
            {
                std::cout << "请输入阿拉伯数字1-9!\n" << std::endl;
                break;
            }
        }
    }
    return 0;
}
