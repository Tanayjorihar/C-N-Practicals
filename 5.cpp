#include &lt;iostream>
#include &lt;cstring>
#include &lt;cstdlib>
#include &lt;ctime>

using namespace std;

#ifdef __linux__
#define CLRSCR "clear"
#else
#define CLRSCR "cls"
#endif

class simulator
{
    const int FRAME_SIZE = 1024;
    char *channel;
    int isACKRecieved;
    int isFrameRecieved;
    char *frame;
    void sender();
    void reciever();
    void senderRecieveACK();
    void resendFrame();

  public:
    simulator();
    ~simulator();
    void simulate(int count, int isChannelNoisy);
};

int main()
{
    int choice, count;
    simulator sim;
    srand(time(0));
    while (1)
    {
        system(CLRSCR);
        cout &lt;&lt; "### STOP AND WAIT PROTOCOL SIMULATOR ###" &lt;&lt; endl;
        cout &lt;&lt; "1) Simulate stop and wait for noisy channel" &lt;&lt; endl;
        cout &lt;&lt; "2) Simulate stop and wait for non-noisy channel" &lt;&lt; endl;
        cout &lt;&lt; "Enter 0 to exit..." &lt;&lt; endl;
        cout &lt;&lt; "Enter your choice: ";
        cin >> choice;
        switch (choice)
        {
        case 0:
            return 0;
        case 1:
            cout &lt;&lt; "Number of frames to simulate: ";
            cin >> count;
            sim.simulate(count, 1);
            break;
        case 2:
            cout &lt;&lt; "Number of frames to simulate: ";
            cin >> count;
            sim.simulate(count, 0);
            break;
        default:
            cout &lt;&lt; "ERROR: wrong choice please try again..." &lt;&lt; endl;
            cin.ignore();
            cin.get();
        }
    }
}

simulator::simulator()
{
    this->channel = new char[FRAME_SIZE];
    this->frame = new char[FRAME_SIZE];
}

simulator::~simulator()
{
    delete[] this->channel;
    delete[] this->frame;
}

void simulator::sender()
{
    cout &lt;&lt; "Enter frame to send: ";
    cin >> this->frame;
    cout &lt;&lt; "SENDER: Sending frame: " &lt;&lt; this->frame &lt;&lt; endl;
    strcpy(this->channel, this->frame);
}

void simulator::reciever()
{
    while (!isFrameRecieved)
        resendFrame();
    cout &lt;&lt; "Recieved Frame: " &lt;&lt; channel &lt;&lt; endl;
    cout &lt;&lt; "Sending ACK" &lt;&lt; endl;
}

void simulator::resendFrame()
{
    cout &lt;&lt; "SENDER: Timer timed out resending frame (either frame is lost or ACK is lost)" &lt;&lt; endl;
    this->isFrameRecieved = rand() % 2;
    this->isACKRecieved = rand() % 2;
}

void simulator::senderRecieveACK()
{
    if (!isACKRecieved)
    {
        resendFrame();
        reciever();
    }

    cout &lt;&lt; "SENDER: Recieved ACK sending next frame if any" &lt;&lt; endl;
}

void simulator::simulate(int count, int isChannelNoisy = 0)
{
    while (count > 0)
    {
        if (isChannelNoisy)
        {
            this->isACKRecieved = rand() % 2;

            this->isFrameRecieved = rand() % 2;
        }
        else
        {
            this->isACKRecieved = 1;
            this->isFrameRecieved = 1;
        }
        sender();
        reciever();
        senderRecieveACK();
        count--;
    }
    cout &lt;&lt; "press enter to continue..." &lt;&lt; endl;
    cin.ignore();
    cin.get();
}