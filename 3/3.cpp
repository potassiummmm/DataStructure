#include <iostream>
#include <random>

struct Point
{
    int row;
    int col;

    Point() : row(0), col(0) { }

    Point(int theRow, int theCol) : row(theRow), col(theCol) { };

    ~Point() = default;

    bool operator==(Point point) const
    {
        return this->row == point.row && this->col == point.col;
    }

    friend Point operator+(Point point1, Point point2)
    {
        point1.row += point2.row;
        point1.col += point2.col;
        return point1;
    }

    friend std::ostream &operator<<(std::ostream &os, Point &point)
    {
        os << '<' << point.row << ',' << point.col << '>';
        return os;
    }
};

/*************** Stack实现 ***************/

class Stack
{
private:
    Point *elements;
    int maxSize;
    int topIndex;
public:
    Stack()
    {
        elements = new Point[10];
        maxSize = 10;
        topIndex = -1;
    };

    ~Stack() { delete elements; };

    Point top() { return elements[topIndex]; };

    bool empty() const { return topIndex == -1; };

    void push(const Point &point)
    {
        if(topIndex == maxSize - 1)
        {
            auto *newElements = new Point[2 * maxSize];
            for(int i = 0;i < topIndex;i++)
            {
                newElements[i] = elements[i];
            }
            maxSize *= 2;
            delete[] elements;
            elements = newElements;
        }
        elements[++topIndex] = point;
    };

    void pop() { topIndex--; };

    void output() const
    {
        for(int i = 0;i < topIndex;i++)
        {
            std::cout << elements[i] << " ---> ";
        }
        std::cout << elements[topIndex] << std::endl;
    };
};

/*************** 迷宫类 ***************/

class Maze
{
private:
    int rows;
    int cols;
    Point entrance;
    Point exit;
    char **maze;
    bool **visited;
    Stack way;
    Point dir[4];
public:
    Maze();

    Maze(int rows, int cols);

    ~Maze();

    bool inMaze(const Point &point) const;

    bool isValid() const;

    void solve();

    bool dfs(Point point);

    void makeMazeFromInput(std::istream &is);

    void init();

    void output();
};

Maze::Maze() : rows(0), cols(0), entrance(-1, -1), exit(-1, -1)
{
    maze = nullptr;
    visited = nullptr;
    dir[0] = Point(-1, 0);
    dir[1] = Point(1, 0);
    dir[2] = Point(0, -1);
    dir[3] = Point(0, 1);
}

Maze::Maze(int rows, int cols) : entrance(-1, -1), exit(-1, -1)
{
    this->rows = rows;
    this->cols = cols;
    maze = new char *[rows];
    visited = new bool *[rows];
    for(int i = 0;i < rows;i++)
    {
        maze[i] = new char[cols];
        visited[i] = new bool[cols];
    }
    dir[0] = Point(-1, 0);
    dir[1] = Point(1, 0);
    dir[2] = Point(0, -1);
    dir[3] = Point(0, 1);
}

Maze::~Maze()
{
    for(int i = 0;i < rows;i++)
    {
        delete maze[i];
    }
    delete[]maze;
}

bool Maze::inMaze(const Point &point) const
{
    return point.col >= 0 && point.col < this->cols && point.row >= 0 &&
           point.row < this->rows;
}

bool Maze::isValid() const
{
    if(!inMaze(entrance) || !inMaze(exit) ||
       maze[entrance.row][entrance.col] != '0' ||
       maze[exit.row][exit.col] != '0')
    {
        return false;
    }
    for(int i = 0;i < rows;i++)
    {
        for(int j = 0;j < cols;j++)
        {
            if(maze[i][j] != '0' && maze[i][j] != '#')
            {
                return false;
            }
        }
    }
    return true;
}

void Maze::solve()
{
    way.push(entrance);
    visited[entrance.row][entrance.col] = true;
    if(!isValid())
    {
        std::cout << "输入的迷宫无效" << std::endl;
        return;
    }
    if(dfs(entrance))
    {
        this->output();
        std::cout << "\n迷宫路径:\n";
        way.output();
    }
    else
    {
        std::cout << "没有道路可以到达终点！" << std::endl;
    }
}

bool Maze::dfs(Point point)
{
    if(point == exit)
    {
        return true;
    }
    for(auto d : dir)
    {
        if(inMaze(point + d) &&
           maze[point.row + d.row][point.col + d.col] != '#' &&
           !visited[point.row + d.row][point.col + d.col])
        {
            way.push(point + d);
            visited[point.row + d.row][point.col + d.col] = true;
            maze[point.row + d.row][point.col + d.col] = 'X';
            return dfs(point + d);
        }
    }
    if(way.empty())
    {
        return false;
    }
    else
    {
        maze[way.top().row][way.top().col] = '0';
        way.pop();
        return dfs(way.top());
    }
}

void Maze::makeMazeFromInput(std::istream &is)
{
    std::cout << "请输入迷宫:\n";
    for(int i = 0;i < rows;i++)
    {
        for(int j = 0;j < cols;j++)
        {
            is >> maze[i][j];
        }
    }
    std::cout << "请输入迷宫入口和出口:" << std::endl;
    is >> entrance.row >> entrance.col >> exit.row >> exit.col;
}

void Maze::init()
{
    char defaultMaze[7][8] = {"#######", "#0#000#", "#0#0###", "#000#0#",
                              "#0#000#", "#0#0#0#", "#######"};
    for(int i = 0;i < 7;i++)
    {
        for(int j = 0;j < 7;j++)
        {
            maze[i][j] = defaultMaze[i][j];
        }
    }
    entrance = Point(1, 1);
    exit = Point(5, 5);
}

void Maze::output()
{
    std::cout << "迷宫地图:\n \t";
    for(int i = 0;i < rows;i++)
    {
        std::cout << i << "列\t";
    }
    std::cout << std::endl;
    for(int i = 0;i < rows;i++)
    {
        std::cout << i << "行\t";
        for(int j = 0;j < cols;j++)
        {
            std::cout << maze[i][j] << '\t';
        }
        std::cout << std::endl;
    }
}


/*************** Main *************/

int main()
{
    int rows = 7, cols = 7;
    int choice;
    std::cout << "请选择由程序给定迷宫(输入0)或者手动输入迷宫(输入1):" << std::endl;
    while(std::cin >> choice, choice != 0 && choice != 1)
    {
        std::cout << "请输入0或者1!" << std::endl;
    }
    if(choice == 0)
    {
        Maze maze(rows, cols);
        maze.init();
        maze.solve();
        return 0;
    }
    else
    {
        std::cout << "请输入迷宫的行数、列数，以空格分割:\n";
        std::cin >> rows >> cols;
        Maze maze(rows, cols);
        maze.makeMazeFromInput(std::cin);
        maze.solve();
    }
    return 0;
}

