#include "List.h"
#include <stdexcept>

using namespace std;

void List::Serialize(FILE* file)
{
    map<ListNode*, vector<int>> PendingRandomLinks{};
    try {
        SerializeNode(Head, PendingRandomLinks, 0, file);
    } 
    catch (const exception& e)
    {
        printf("ERROR: failed to serialize List: %s", e.what());
    }
}

void List::SerializeNode(ListNode* Node, map<ListNode*, vector<int>>& PendingRandomLinks, int Index, FILE* file)
{
    if (!Node)
        return;
    
    WriteNodeToFile(Node, file);

    if (Node->Random) {
        auto Search = PendingRandomLinks.find(Node->Random);
        if (Search == PendingRandomLinks.end()) {
            PendingRandomLinks[Node->Random] = vector<int>();
        }
        PendingRandomLinks[Node->Random].push_back(Index);
    }

    if (Node->Next) 
    {
        fwrite(&NEXT_NODE, sizeof(short), 1, file);
        SerializeNode(Node->Next, PendingRandomLinks, Index+1, file);
    }

    vector<int> PointingNodes = FindPendingLinksInMap(PendingRandomLinks, Node);
    for (int NodeIndex : PointingNodes)
    {
        WriteConnectionToFile(RANDOM_CONNECTION, NodeIndex, file);
    }    

    std::fwrite(&EXIT_CODE, sizeof(short), 1, file);
}

vector<int> List::FindPendingLinksInMap(map<ListNode*, vector<int>>& PendingRandomLinks, ListNode* NodeToFind)
{
    auto Search = PendingRandomLinks.find(NodeToFind);
    if (Search != PendingRandomLinks.end())
    {
        return Search->second;
    } 
    else
    {
        return vector<int>();
    }
}

void List::WriteNodeToFile(ListNode* Node, FILE* file)
{
    int SizeOfData = sizeof(Node->Data);
    fwrite(&SizeOfData, sizeof(int), 1, file);
    fwrite(&(Node->Data), SizeOfData, 1, file);    
}

void List::WriteConnectionToFile(const short ConnectionCode, int ConnectionIndex, FILE* file)
{
    fwrite(&ConnectionCode, sizeof(short), 1, file);
    fwrite(&ConnectionIndex, sizeof(int), 1, file);
}

void List::Deserialize(FILE* file)
{
    Clear();
    vector<ListNode*> DeserializedNodes{};
    try {        
        Head = DeserializeNode(DeserializedNodes, file);
    }
    catch (const exception& e)
    {
        printf("ERROR: failed to deserialize List: %s", e.what());
    }
}

ListNode* List::DeserializeNode(vector<ListNode*>& DeserializedNodes, FILE* file)
{
    ListNode* CurrentNode = ReadNodeFromFile(file);
    DeserializedNodes.push_back(CurrentNode);

    ListNode *NextNode, *PointingNode;

    bool bReachedEndOfNode = false;
    while (!bReachedEndOfNode) 
    {
        short Code;
        fread(&Code, sizeof(short), 1, file);
        switch (Code)
        {
        case NEXT_NODE:
            NextNode = DeserializeNode(DeserializedNodes, file);
            CurrentNode->Next = NextNode;
            NextNode->Previous = CurrentNode;
            break;
        case RANDOM_CONNECTION:
            int PointintIndex;
            fread(&PointintIndex, sizeof(int), 1, file);
            PointingNode = DeserializedNodes.at(PointintIndex);
            PointingNode->Random = CurrentNode;
            break;
        case EXIT_CODE:
            bReachedEndOfNode = true;
            break;
        default:
            throw new std::exception("Unknown code");
            break;
        }
    }
    return CurrentNode;
}

ListNode* List::ReadNodeFromFile(FILE* file)
{
    int SizeOfData;
    fread(&SizeOfData, sizeof(int), 1, file);
    string Data;
    fread(&Data, SizeOfData, 1, file);

    return new ListNode{ Data };
}

void List::ConstructTestExample()
{
    auto FirstNode =    new ListNode{ "Node 1" };
    Push(FirstNode);

    auto SecondNode =   new ListNode{ "Node 2 pointing 4th " };
    Push(SecondNode);

    auto ThirdNode =    new ListNode{ "Node 3 pointing Self" };
    Push(ThirdNode);
    ThirdNode->Random = ThirdNode;

    auto FourthNode =   new ListNode{ "Node 4 pointing 1st " };
    FourthNode->Random = FirstNode;
    SecondNode->Random = FourthNode;
    Push(FourthNode);

    auto FifthNode =    new ListNode{ "Node 5 pointing 4th " };
    FifthNode->Random = FourthNode;
    Push(FifthNode);

    auto SixthNode =    new ListNode{ "Node 6" };
    Push(SixthNode);

}

void List::Push(ListNode* Node)
{
    if (Tail) {
        Tail->Next = Node;
        Tail = Node;
        Count++;
    }
    else
    {
        Head = Tail = Node;
        Count = 1;
    }
}

void List::Clear()
{
    auto CurrentNode = Head;
    while (CurrentNode)
    {
        auto NodeToDestroy = CurrentNode;
        CurrentNode = CurrentNode->Next;
        delete NodeToDestroy;
    }

    Head = Tail = nullptr;
}

void List::Print()
{
    auto CurrentNode = Head;
    while (CurrentNode)
    {
        printf("%s", CurrentNode->Data.c_str());
        if (CurrentNode->Random) {
            printf(" => %s", CurrentNode->Random->Data.c_str());
        }
        printf("\n");
        CurrentNode = CurrentNode->Next;
    }
}

List::~List()
{
    Clear();
}
