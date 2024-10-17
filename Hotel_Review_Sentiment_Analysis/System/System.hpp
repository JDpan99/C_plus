#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <cctype>

using namespace std;

// Linked list node for review data
struct Node
{
    std::string data; // Use std::string instead of string
    Node *next;
    int count;
    Node(const std::string &newData, int count = 0);
    ~Node();
    void freeList();
};

class WordArray
{
public:
    struct Word
    {
        std::string word; // Use std::string instead of string
        int count;
        bool isPositive;

        Word() : word(""), count(0), isPositive(false) {}
        Word(const std::string &word, bool isPositive) : word(word), count(1), isPositive(isPositive) {}
    };

    Word *array;
    int size;
    int capacity;

    WordArray(int initialCapacity = 100);
    ~WordArray();
    void addWord(const std::string &word, bool isPositive);
    int findWord(const std::string &word) const;
    int findWordUsingBinarySearch(const std::string &word);
    void mergeSortWords();
    void mergeSort(int left, int right);
    void merge(int left, int mid, int right);
    void mergeSortByFrequency();
    void mergeSortByFrequency(int left, int right);
    void mergeFreq(int left, int mid, int right);
    void clear();

private:
    void resize();
};

void readfile(const string &path, Node *&head);
void append(Node *&head, const string newData);
void deleteHead(Node *&head);
void loadWordsFromFile(WordArray &wordArray, const string &path, bool isPositive);
void insertInOrder(std::string *array, int &size, const std::string &word);
void calculateWordFrequency(Node *head, WordArray &wordArray, Node *&posScore, Node *&negScore, int &totalPos, int &totalNeg, int &totalRev);
std::string removePunctuation(std::string &word);
void calculateScore(Node *csv, Node *posScore, Node *negScore, Node *&sentiment, int &consistency);
void displayReport(Node *posScore, Node *negScore, int totalRev, int totalPos, int totalNeg, WordArray &wordArray, int consistency);
void generateGraph(WordArray &wordArray);

#endif