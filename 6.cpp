#include <iostream>
#include <cstring>
#include <cstdlib>
#include <ctime>

using namespace std;

char *crc(char *, char *); //function for crc check
char *getReminder(char *, char *); //helper function to get reminder by dividing data with poulynomial
char *substr(char *, int, int); //function to get substring of a data frame
bool isFrameCorrect(char *, char *); //fucntion to check whether the recieved data is correct or not    
void simulateNetwork(bool isNoisy = false); //function to simulate network

int main(int argc, char *argv[])
{
    while (true)
    {
        int choice;
        cout << "Circular Redundancy Check Simulator" << endl;
        cout << "1) transmit over non-noisy channel" << endl;
        cout << "2) transmit over noisy channel" << endl;
        cout << "enter 0 to exit..." << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice)
        {
        case 0:
            return 1;
        case 1:
            simulateNetwork();
            break;
        case 2:
            simulateNetwork(true);
            break;
        default:
            cout << "Error: wrong choice try again...";
        }
        cin.ignore();
        cin.get();
    }
}

char *crc(char *dataFrame, char *genFunc)
{
    char *result = new char[strlen(dataFrame) + strlen(genFunc)];
    strcpy(result, dataFrame);
    strcat(result, getReminder(dataFrame, genFunc));
    return result;
}

char *getReminder(char *dataFrame, char *genFunc)
{
    char *tempDataFrame = new char[strlen(dataFrame) + strlen(genFunc)];
    strcpy(tempDataFrame, dataFrame);
    for (int i = 0; i < strlen(genFunc) - 1; i++)
    {
        strcat(tempDataFrame, "0");
    }
    for (int i = 0, j = strlen(genFunc); j < strlen(tempDataFrame) + 1; j++, i++)
    {
        char *currentBlock = substr(tempDataFrame, i, j);
        if (currentBlock[0] == '1')
        {
            for (int k = 0; k < strlen(currentBlock); k++)
            {
                tempDataFrame[i + k] = ((currentBlock[k] - '0') ^ (genFunc[k] - '0')) + '0';
            }
        }
        delete[] currentBlock;
    }
    return substr(tempDataFrame, strlen(dataFrame), strlen(tempDataFrame));
}

char *substr(char *str, int i, int j)
{
    char *retValue = new char[j - i + 1];
    for (int k = 0; k < j - i; k++)
    {
        retValue[k] = str[i + k];
    }
    retValue[j - i] = '\0';
    return retValue;
}

bool isFrameCorrect(char *dataFrame, char *genFunc)
{
    char *reminder = getReminder(dataFrame, genFunc);
    if (atoi(reminder) == 0)
    {
        return true;
    }
    return false;
}

void simulateNetwork(bool isNoisy)
{
    srand(time(0));
    int frameSize, genFuncSize;
    char *dataFrame, *genFunc;
    cout << "Enter frame size: ";
    cin >> frameSize;
    dataFrame = new char[frameSize];
    cout << "Enter data frame: ";
    cin >> dataFrame;
    cout << "Enter generating function size: ";
    cin >> genFuncSize;
    genFunc = new char[genFuncSize];
    cout << "Enter generating fucntion: ";
    cin >> genFunc;
    char *transmittedFrame = crc(dataFrame, genFunc);
    cout << "Frame sent: " << transmittedFrame << endl;
    if (isNoisy)
    {
        transmittedFrame[rand() % strlen(transmittedFrame)] = (rand() % 2) + '0';
    }
    cout << "Frame recieved: " << transmittedFrame << endl;
    if (isFrameCorrect(transmittedFrame, genFunc))
    {
        cout << "Frame recieved correctly over the network" << endl;
    }
    else
    {
        cout << "Frame recieved incorrectly over the network" << endl;
    }
}