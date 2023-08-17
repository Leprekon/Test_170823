#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include "List/List.h"

using namespace std;

void PrintSignedIntAsBinary(int Input);
void RemoveDuplicateCharacters(char* InputString);

class List;

int main()
{
    //--------------------------- TEST 1
    PrintSignedIntAsBinary(123);
    PrintSignedIntAsBinary(-1000);

    printf("\n");

    //--------------------------- TEST 2
    printf("Removing duplicates\n");
    char TestStringA[] = "AAA BBB     AAA";
    printf("Input:  %s\n", TestStringA);
    RemoveDuplicateCharacters(TestStringA);
    printf("Output: %s\n", TestStringA);

    char TestStringB[] = "Thhiis   teeext dooesn'''t maaake sensee unntillll   yooou ccclean iiit.";
    printf("Input:  %s\n", TestStringB);
    RemoveDuplicateCharacters(TestStringB);
    printf("Output: %s\n", TestStringB);

    printf("\n");

    //--------------------------- TEST 3
    printf("Serializing List:\n");
    List* SourceList = new List();
    SourceList->ConstructTestExample();
    SourceList->Print();

    FILE* FileToWriteTo = fopen("SavedList.lst", "wb");
    SourceList->Serialize(FileToWriteTo);
    fclose(FileToWriteTo);

    printf("Deserializing List:\n");
    List* DestinationList = new List();
    FILE* FileToReadFrom = fopen("SavedList.lst", "rb");
    DestinationList->Deserialize(FileToReadFrom);
    fclose(FileToReadFrom);
    DestinationList->Print();

    return 0;
}

void PrintSignedIntAsBinary(int Input)
{
    cout << "Print Binary of " << Input << ":\n";

    const int BitSizeOfInt = sizeof(Input) * CHAR_BIT;

    for(int i = BitSizeOfInt; i >= 0; i--)
    {
        cout << (Input & (1 << i) ? '1' : '0');
    } 

    cout << '\n';

}

void RemoveDuplicateCharacters(char* InputString)
{
    char* CurrentChar = InputString;

    while (*InputString != '\0')
    {
        if (*InputString != *CurrentChar) {
            CurrentChar++;
            *CurrentChar = *InputString;
        }
        InputString++;
    }

    //Filling the tail with spaces to preserve the size of the array
    CurrentChar++;
    while (*CurrentChar != '\0') {
        *CurrentChar = ' ';
        CurrentChar++;
    }
}