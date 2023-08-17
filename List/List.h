#include <iostream>
#include <vector>
#include <map>

using namespace std;

struct ListNode
{
    ListNode* Previous = nullptr;
    ListNode* Next = nullptr;
    ListNode* Random = nullptr;

    string Data;

    ListNode() {}

    ListNode(string _Data)
    {
        Data = _Data;
    }
};

class List
{
public:
    void Serialize(FILE* file);
    void Deserialize(FILE* file);

    ListNode* DeserializeNode(vector<ListNode*>& DeserializedNodes, FILE* file);

    void ConstructTestExample();
    void Push(ListNode* Node);
    void Clear();
    void Print();

    ~List();

private:
    ListNode* Head = nullptr;
    ListNode* Tail = nullptr;

    int Count = 0;

    static const short EXIT_CODE = 0;
    static const short NEXT_NODE = 1;
    static const short RANDOM_CONNECTION = 2;
    
    void SerializeNode(ListNode* Node, map<ListNode*, vector<int>>& PendingRandomLinks, int Index, FILE* file);

    vector<int> FindPendingLinksInMap(map<ListNode*, vector<int>>& PointingNodesIndexes, ListNode* NodeToFind);

    void WriteNodeToFile(ListNode* Node, FILE* file);
    ListNode* ReadNodeFromFile(FILE* file);

    void WriteConnectionToFile(const short ConnectionCode, int ConnectionIndex, FILE* file);


};