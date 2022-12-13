#include <iostream>

using namespace std;

#define INF 9999
#define VISITED 1
#define NOT_VISITED 0

class Router
{
    int **graph;
    int *dist;
    int *set;
    int *parent;
    int numOfNodes;
    void dijkstra(int src);
    int emptySet();
    int lowestDistInSet();
    void printPath(int dst);

  public:
    Router();
    ~Router();
    void getRoute(int src, int dst);
};

int main()
{
    Router router;
    int choice;
    while (1)
    {
        cout << "DIJKSTRA ROUTE FINDER" << endl;
        cout << "1) Enter src and destination to get path" << endl;
        cout << "Enter 0 to exit..." << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice)
        {
        case 0:
            exit(1);
        case 1:
            int from,to;
            cout<<"Enter source: ";
            cin>>from;
            cout<<"Enter destination: ";
            cin>>to;
            router.getRoute(from,to);
            break;
        default:
            cout<<"ERROR: wrong choice please try again..."<<endl;
        }
        cout<<"Press enter to continue...";
        cin.ignore();
        cin.get();
    }
}

Router::Router()
{
    cout << "Enter number of nodes: ";
    cin >> numOfNodes;
    graph = new int *[numOfNodes];
    cout << "Enter weighted graph" << endl;
    for (int i = 0; i < numOfNodes; i++)
    {
        graph[i] = new int[numOfNodes];
        cout << "Enter row number " << i + 1 << " : ";
        for (int j = 0; j < numOfNodes; j++)
            cin >> graph[i][j];
    }
    dist = new int[numOfNodes];
    set = new int[numOfNodes];
    parent = new int[numOfNodes];
}

Router::~Router()
{
    for (int i = 0; i < numOfNodes; i++)
    {
        delete[] graph[i];
    }
    delete[] graph;
    delete[] set;
    delete[] dist;
}

void Router::dijkstra(int src)
{
    int current;
    for (int i = 0; i < numOfNodes; i++)
    {
        dist[i] = INF;
        set[i] = NOT_VISITED;
    }
    dist[src] = 0;
    parent[src] = -1;
    while (!emptySet())
    {
        current = lowestDistInSet();
        set[current] = VISITED;
        for (int i = 0; i < numOfNodes; i++)
        {
            if (set[i] != VISITED && graph[current][i] >= 0)
            {
                if (dist[i] > dist[current] + graph[current][i])
                {
                    dist[i] = dist[current] + graph[current][i];
                    parent[i] = current;
                }
            }
        }
    }
}

void Router::getRoute(int src, int dst)
{
    dijkstra(src);
    cout << "Dist from " << src << " to " << dst << " is " << dist[dst] << endl;
    cout << "Path: ";
    printPath(dst);
    cout << endl;
}

int Router::emptySet()
{
    for (int i = 0; i < numOfNodes; i++)
    {
        if (set[i] == NOT_VISITED)
        {
            return 0;
        }
    }
    return 1;
}

int Router::lowestDistInSet()
{
    int current = INF, currentIndex = 0;
    for (int i = 0; i < numOfNodes; i++)
    {
        if (set[i] == NOT_VISITED && dist[i] < current)
        {
            current = dist[i];
            currentIndex = i;
        }
    }
    return currentIndex;
}

void Router::printPath(int dst)
{
    if (parent[dst] == -1)
    {
        return;
    }
    printPath(parent[dst]);
    cout << parent[dst] << " ";
}