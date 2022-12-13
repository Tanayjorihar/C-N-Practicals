#include <iostream>
#include <cstdlib>
#include <climits>
#include <iomanip>

#ifdef __linux__
#define CLRSCR "clear"
#else
#define CLRSCR "cls"
#endif

using namespace std;

class node
{
    int totalNodes;
    int numOfNeighbours;
    int nodeId;
    int *delayTable;
    int **routingTable;
    node **neighbouringNodes;
    int *neighbouringNodeDistance;

  public:
    node(char nodeId, int totalNodes)
    {
        this->nodeId = nodeId - 'A';
        this->totalNodes = totalNodes;
        this->delayTable = NULL;
        this->routingTable = NULL;
        this->neighbouringNodes = new node *[totalNodes];
        this->neighbouringNodeDistance = new int[totalNodes];
        this->numOfNeighbours = 0;
    }
    ~node()
    {
        if (delayTable != NULL)
            delete[] delayTable;
        if (routingTable != NULL)
        {
            for (int i = 0; i < totalNodes; i++)
                delete[] routingTable[i];
            delete[] routingTable;
        }
        if (neighbouringNodes != NULL)
            delete[] neighbouringNodes;
    }
    void addNeighbour(node *nodeArg, int distanceToNode)
    {
        this->neighbouringNodes[numOfNeighbours] = nodeArg;
        this->neighbouringNodeDistance[numOfNeighbours] = distanceToNode;
        numOfNeighbours++;
    }
    void inputDelayTable()
    {
        delayTable = new int[totalNodes];
        cout << "Enter delay table of node " << char(nodeId + 'A') << endl;
        for (int i = 0; i < totalNodes; i++)
        {
            cout << char(i + 'A') << " : ";
            cin >> delayTable[i];
        }
    }
    void displayDelayTable()
    {
        cout << "Delay table of " << char(nodeId + 'A') << endl;
        for (int i = 0; i < totalNodes; i++)
            cout << char(i + 'A') << " : " << delayTable[i] << endl;
    }
    void calculateRoutingTable()
    {
        cout << "DEBUG: " << numOfNeighbours << endl;
        routingTable = new int *[totalNodes];
        for (int i = 0; i < totalNodes; i++)
            routingTable[i] = new int[2];
        for (int i = 0; i < totalNodes; i++)
        {
            int min = INT_MAX, delayNode;
            for (int j = 0; j < numOfNeighbours; j++)
            {
                if (i == this->getNodeId())
                {
                    min = 0;
                    delayNode = '-' - 'A';
                }
                else
                {
                    if (min > neighbouringNodeDistance[j] + neighbouringNodes[j]->getDelayTo(i))
                    {
                        min = neighbouringNodeDistance[j] + neighbouringNodes[j]->getDelayTo(i);
                        delayNode = neighbouringNodes[j]->getNodeId();
                    }
                }
            }
            routingTable[i][0] = min;
            routingTable[i][1] = delayNode;
        }
    }
    void displayRoutingTable()
    {
        cout << "Routing table of " << char(this->getNodeId() + 'A') << endl
             << endl;
        for (int i = 0; i < 28; i++)
            cout << "-";
        cout << endl;
        cout << setw(1) << "|" << setw(8) << setfill(' ') << right << "node" << setw(1) << "|" << setw(8) << setfill(' ') << right
             << "delay" << setw(1) << "|" << setw(8) << setfill(' ') << right << "via" << setw(1) << "|" << endl;
        for (int i = 0; i < 28; i++)
            cout << "-";
        cout << endl;
        for (int i = 0; i < totalNodes; i++)
        {
            cout << setw(1) << "|" << setw(8) << setfill(' ') << right << char(i + 'A') << setw(1) << "|" << setw(8) << setfill(' ')
                 << right << routingTable[i][0] << setw(1) << "|" << setw(8) << setfill(' ') << right << char(routingTable[i][1] + 'A')
                 << setw(1) << "|" << endl;
        }
        for (int i = 0; i < 28; i++)
            cout << "-";
        cout << endl;
    }
    int getDelayTo(int toNode)
    {
        return this->delayTable[toNode];
    }
    int getNodeId()
    {
        return this->nodeId;
    }
};

int main(int argc, char const *argv[])
{
    int totalNodes, numOfNeighbours;
    int distance;
    node **neighbours;
    char nodeId;
    cout << "Total nodes: ";
    cin >> totalNodes;
    cout << "Current node id: ";
    cin >> nodeId;
    node primaryNode(nodeId, totalNodes);
    cout << "Num of Neighbours: ";
    cin >> numOfNeighbours;
    neighbours = new node *[numOfNeighbours];
    for (int i = 0; i < numOfNeighbours; i++)
    {
        system(CLRSCR);
        cout << "Id of neighbour " << i + 1 << " : ";
        cin >> nodeId;
        cout << "Distance from " << char('A' + primaryNode.getNodeId()) << " : ";
        cin >> distance;
        neighbours[i] = new node(nodeId, totalNodes);
        neighbours[i]->inputDelayTable();
        primaryNode.addNeighbour(neighbours[i], distance);
    }
    primaryNode.calculateRoutingTable();
    system(CLRSCR);
    primaryNode.displayRoutingTable();
    cout << "Press enter to continue...";
    cin.ignore();
    cin.get();
    for (int i = 0; i < numOfNeighbours; i++)
        delete neighbours[i];
    delete[] neighbours;
}