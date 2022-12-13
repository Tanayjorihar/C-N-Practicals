#include &lt;iostream>
#include &lt;cstdlib>
#include &lt;string>
#include &lt;queue>
#include &lt;ctime>

using namespace std;

class frame
{
  public:
    string data;
    int seq;
};

class Simulator
{
    queue&lt;frame> channel;
    queue&lt;int> faultyFramesIndex;
    frame *frames;
    queue&lt;int> acks;
    int totalFrames;
    int windowSize;
    int i;
    bool isNoisy;

  public:
    Simulator();
    ~Simulator();
    void sender(int);
    void reciever();
    void recieveACK();
    void simulate(bool isNoisy = false);
    void sendFrame(int frameIndex);
};

int main()
{
    Simulator sim;
    int choice;
    while (true)
    {
        cout &lt;&lt; "SELECTIVE REPEAT SIMULATOR" &lt;&lt; endl;
        cout &lt;&lt; "1) Simulate selective repeat for noisy channel" &lt;&lt; endl;
        cout &lt;&lt; "2) Simulate selective repeat for non-noisy channel" &lt;&lt; endl;
        cout &lt;&lt; "Enter 0 to exit..." &lt;&lt; endl;
        cout &lt;&lt; "Enter your choice: ";
        cin >> choice;
        switch (choice)
        {
        case 0:
            exit(1);
        case 1:
            sim.simulate(true);
            break;
        case 2:
            sim.simulate(false);
            break;
        default:
            cout &lt;&lt; "Wrong choice please try again..." &lt;&lt; endl;
        }
        cout &lt;&lt; "Press enter to continue...";
        cin.ignore();
        cin.get();
    }
}

Simulator::Simulator()
{
    srand(time(0));
    cout &lt;&lt; "Enter total number of frames to simulate: ";
    cin >> totalFrames;
    cout &lt;&lt; "Enter window size: ";
    cin >> windowSize;
    frames = new frame[totalFrames];
    for (int i = 0; i &lt; totalFrames; i++)
    {
        cout &lt;&lt; "Enter frame number " &lt;&lt; i &lt;&lt; " : ";
        cin >> frames[i].data;
        frames[i].seq = i % windowSize;
    }
    isNoisy = false;
}

Simulator::~Simulator()
{
    if (frames != NULL)
        delete[] frames;
}

void Simulator::sender(int from)
{
    int fault = rand() % 2;
    for (int i = from; i &lt; from + windowSize &amp;&amp; i &lt; totalFrames; i++)
    {
        sendFrame(i);
    }
}

void Simulator::reciever()
{
    while (!channel.empty())
    {
        cout &lt;&lt; "RECIEVER: Recieved Frame: " &lt;&lt; channel.front().data &lt;&lt; " SEQ: " &lt;&lt; channel.front().seq &lt;&lt; "; sending ACK" &lt;&lt; endl;
        channel.pop();
    }
}

void Simulator::recieveACK()
{
    while (!acks.empty())
    {
        cout &lt;&lt; "SENDER: Recived ACK for seq: " &lt;&lt; acks.front() &lt;&lt; endl;
        acks.pop();
        i++;
    }
    while (!faultyFramesIndex.empty())
    {
        cout &lt;&lt; "SENDER: Haven't recieved ACK for frame SEQ: " &lt;&lt; faultyFramesIndex.front() % windowSize
             &lt;&lt; "; Resending frame" &lt;&lt; endl;
        sendFrame(faultyFramesIndex.front());
        faultyFramesIndex.pop();
        reciever();
        recieveACK();
    }
}

void Simulator::simulate(bool isNoisy)
{
    this->isNoisy = isNoisy;
    for (i = 0; i &lt; totalFrames;)
    {
        sender(i);
        reciever();
        recieveACK();
    }
}

void Simulator::sendFrame(int frameIndex)
{
    int faulty;
    if (rand() % 2 == 0 &amp;&amp; isNoisy)
        faulty = 1;
    else
        faulty = 0;
    cout &lt;&lt; "SENDER: sent frame " &lt;&lt; frames[i].data &lt;&lt; " SEQ: " &lt;&lt; frames[i].seq &lt;&lt; endl;
    if (!faulty)
    {
        channel.push(frames[i]);
        acks.push(frames[i].seq);
    }
    else
    {
        faultyFramesIndex.push(frameIndex);
    }
}