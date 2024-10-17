#include "System.hpp"
#include "API\pbPlots.hpp"
#include "API\pbPlots.cpp"
#include "API\supportLib.hpp"
#include "API\supportLib.cpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <iomanip>

using namespace std;

// <<<<<<< Node >>>>>>> //
Node::Node(const string &newData, int count) : data(newData), next(nullptr), count(count) {}

Node::~Node() {}

void Node::freeList()
{
    Node *temp;
    while (next != nullptr)
    {
        temp = next;
        next = next->next;
        delete temp;
    }
    delete this;
}

// <<<<<<< WordArray >>>>>>> //
WordArray::WordArray(int initialCapacity) : size(0), capacity(initialCapacity)
{
    array = new Word[capacity];
}

// Destructor for WordArray
WordArray::~WordArray()
{
    delete[] array;
}

// Add word to WordArray
void WordArray::addWord(const string &word, bool isPositive)
{
    int index = findWord(word);
    if (index != -1)
    {
        array[index].count++;
    }
    else
    {
        if (size == capacity)
        {
            resize();
        }
        array[size] = Word(word, isPositive);
        size++;
    }
}

// Find word in WordArray using linear search
int WordArray::findWord(const string &word) const
{
    for (int i = 0; i < size; ++i)
    {
        if (array[i].word == word)
        {
            return i;
        }
    }
    return -1;
}

// Resize WordArray when capacity is reached
void WordArray::resize()
{
    capacity *= 2;
    Word *newArray = new Word[capacity];
    for (int i = 0; i < size; ++i)
    {
        newArray[i] = array[i];
    }
    delete[] array;
    array = newArray;
}

// Sort words in WordArray to enable binary search
void WordArray::mergeSortWords()
{
    mergeSort(0, size - 1);
}

void WordArray::mergeSort(int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;
        mergeSort(left, mid);
        mergeSort(mid + 1, right);
        merge(left, mid, right);
    }
}

// Merge function for Merge Sort
void WordArray::merge(int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Word *L = new Word[n1];
    Word *R = new Word[n2];

    for (int i = 0; i < n1; i++)
        L[i] = array[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = array[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2)
    {
        if (L[i].word <= R[j].word)
        {
            array[k] = L[i];
            i++;
        }
        else
        {
            array[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        array[k] = L[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        array[k] = R[j];
        j++;
        k++;
    }

    delete[] L;
    delete[] R;
}

int WordArray::findWordUsingBinarySearch(const string &word)
{
    int low = 0, high = size - 1;
    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        if (array[mid].word == word)
            return mid;
        else if (array[mid].word < word)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return -1;
}

void WordArray::mergeSortByFrequency()
{
    mergeSortByFrequency(0, size - 1);
}

// Merge Sort function by frequency (count)
void WordArray::mergeSortByFrequency(int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;
        mergeSortByFrequency(left, mid);      // Sort left half
        mergeSortByFrequency(mid + 1, right); // Sort right half
        mergeFreq(left, mid, right);          // Merge the two halves
    }
}

void WordArray::mergeFreq(int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Create temporary arrays for the left and right halves
    Word *L = new Word[n1];
    Word *R = new Word[n2];

    // Copy data to temporary arrays L[] and R[]
    for (int i = 0; i < n1; i++)
        L[i] = array[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = array[mid + 1 + j];

    int i = 0, j = 0, k = left;

    // Merge the temporary arrays back into the main array
    while (i < n1 && j < n2)
    {
        // Compare frequencies in descending order
        if (L[i].count >= R[j].count)
        {
            array[k] = L[i];
            i++;
        }
        else
        {
            array[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[], if there are any
    while (i < n1)
    {
        array[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[], if there are any
    while (j < n2)
    {
        array[k] = R[j];
        j++;
        k++;
    }

    // Clean up memory for temporary arrays
    delete[] L;
    delete[] R;
}

// <<<<<<< Function >>>>>>> //
void readfile(const string &path, Node *&head)
{
    ifstream file(path);
    if (!file.is_open())
    {
        cout << "File not found" << endl;
        return;
    }

    string line;
    while (getline(file, line))
    {
        append(head, line);
    }
    file.close();
}

void deleteHead(Node *&head)
{
    Node *temp = head;
    head = head->next;
    delete temp;
}

void append(Node *&head, const string newData)
{
    int lastDigit = isdigit(newData.back()) ? newData.back() - '0' : 0;
    Node *newNode = new Node(newData, lastDigit);
    if (head == nullptr)
    {
        head = newNode;
    }
    else
    {
        Node *temp = head;
        while (temp->next != nullptr)
        {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

void loadWordsFromFile(WordArray &list, const string &filename, bool isPositive)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error: Could not open the file: " << filename << endl;
        return;
    }
    string word;
    while (file >> word)
    {
        list.addWord(word, isPositive);
    }
}

void calculateWordFrequency(Node *csv, WordArray &wordArray, Node *&posScore, Node *&negScore, int &totalPos, int &totalNeg, int &totalRev)
{
    Node *temp = csv;
    while (temp != nullptr) // Traverse the linked list correctly
    {
        totalRev++;
        int pScore = 0, nScore = 0;
        stringstream ss(temp->data); // Use temp->data instead of csv->data
        string word;

        while (ss >> word)
        {
            word = removePunctuation(word);

            int index = wordArray.findWordUsingBinarySearch(word);
            if (index != -1)
            {
                wordArray.array[index].count++;
                if (wordArray.array[index].isPositive)
                {
                    pScore++;
                    totalPos++;
                }
                else
                {
                    nScore++;
                    totalNeg++;
                }
            }
        }

        Node *posNode = new Node(to_string(pScore));
        Node *negNode = new Node(to_string(nScore));
        posNode->next = posScore;
        negNode->next = negScore;
        posScore = posNode;
        negScore = negNode;
        temp = temp->next; // Move to the next node
    }

    cout << "Total reviews: " << totalRev << endl;
    cout << "Total positive words: " << totalPos << endl;
    cout << "Total negative words: " << totalNeg << endl;
}

string removePunctuation(string &word)
{
    word.erase(remove_if(word.begin(), word.end(), ::ispunct), word.end());
    transform(word.begin(), word.end(), word.begin(), ::tolower);
    return word;
}

void calculateScore(Node *csv, Node *posScore, Node *negScore, Node *&sentiment, int &consistency)
{
    int printCount = 0;
    while (posScore && negScore && csv)
    {
        int positive = stoi(posScore->data);
        int negative = stoi(negScore->data);
        int rawScore = positive - negative;
        int n = positive + negative;

        double adjustedRating = (n != 0) ? 1 + 4 * (rawScore + n) / (2.0 * n) : 1.0;

        int originalRating = csv->count;

        string consistencyStatus = (round(adjustedRating) == originalRating) ? "Consistent" : "Not Consistent";
        if (consistencyStatus == "Consistent")
        {
            consistency++;
        }
        if (printCount < 10)
        {
            cout << "Review: \"" << csv->data << "\"" << endl;
            cout << "Original Rating: " << originalRating << endl;
            cout << "Adjusted Rating: " << round(adjustedRating) << endl; // Display the rounded adjusted rating
            cout << "Positive Words: " << positive << ", Negative Words: " << negative << endl;
            cout << "Consistency: " << consistencyStatus << endl;
            cout << "-----------------------------" << endl;
            printCount++;
        }

        Node *newNode = new Node(to_string(adjustedRating));
        newNode->next = sentiment;
        sentiment = newNode;

        posScore = posScore->next;
        negScore = negScore->next;
        csv = csv->next;
    }
}

void displayReport(Node *posScore, Node *negScore, int totalRev, int totalPos, int totalNeg, WordArray &wordArray, int consistency)
{
    int maxFrequency = wordArray.array[0].count;
    int minFrequency = -1;
    for (int i = wordArray.size - 1; i >= 0; --i)
    {
        if (wordArray.array[i].count > 0)
        {
            minFrequency = wordArray.array[i].count;
            break;
        }
    }

    std::cout << "Maximum used word(s) in the reviews: ";
    for (int i = 0; i < wordArray.size; ++i)
    {
        if (wordArray.array[i].count == maxFrequency)
        {
            std::cout << wordArray.array[i].word << " ";
        }
    }

    std::cout << std::endl;

    std::cout << "Minimum used word(s) in the reviews: ";
    for (int i = 0; i < wordArray.size; ++i)
    {
        if (wordArray.array[i].count == minFrequency && minFrequency > 0)
        {
            std::cout << wordArray.array[i].word << " ";
        }
    }
    std::cout << std::endl;

    // Calculate average word frequency and percentages
    double averageWordFrequency = (totalRev != 0) ? static_cast<double>(totalPos + totalNeg) / (totalPos + totalNeg) : 0.0;
    double percentagePositive = (totalPos + totalNeg != 0) ? (static_cast<double>(totalPos) / (totalPos + totalNeg)) * 100 : 0.0;
    double percentageNegative = 100.0 - percentagePositive;
    double percentageConsistency = (totalRev != 0) ? (static_cast<double>(consistency) / totalRev) * 100 : 0.0;
    double numberOfInconsistencies = totalRev - consistency;

    // Display the overall summary of the report
    // std::cout << "Maximum used word(s) in the reviews: " << maxUsedWord << std::endl;
    // std::cout << "Minimum used word(s) in the reviews: " << minUsedWord << std::endl;
    std::cout << "\nTotal Reviews = " << totalRev << std::endl;
    std::cout << "Total Counts of positive words = " << totalPos << std::endl;
    std::cout << "Total Counts of negative words = " << totalNeg << std::endl;
    std::cout << "\nAdditional Report Details:" << std::endl;
    std::cout << "Average word frequency across all reviews: " << std::fixed << std::setprecision(2) << averageWordFrequency << std::endl;
    std::cout << "Percentage of positive words: " << std::fixed << std::setprecision(2) << percentagePositive << "%" << std::endl;
    std::cout << "Percentage of negative words: " << std::fixed << std::setprecision(2) << percentageNegative << "%" << std::endl;
    std::cout << "\nPercentage of Consistency: " << std::fixed << std::setprecision(2) << percentageConsistency << "%" << std::endl;
    std::cout << "Number of consistent reviews: " << consistency << std::endl;
    std::cout << "Number of inconsistent reviews: " << numberOfInconsistencies << std::endl;

    // Display top 5 most frequent positive words
    std::cout << "\nTop 5 most frequent positive words:" << std::endl;
    int positiveWordCount = 0;
    for (int i = 0; i < wordArray.size && positiveWordCount < 5; ++i)
    {
        if (wordArray.array[i].isPositive)
        {
            std::cout << wordArray.array[i].word << " = " << wordArray.array[i].count << " times" << std::endl;
            positiveWordCount++;
        }
    }

    // Display top 5 most frequent negative words
    std::cout << "\nTop 5 most frequent negative words:" << std::endl;
    int negativeWordCount = 0;
    for (int i = 0; i < wordArray.size && negativeWordCount < 5; ++i)
    {
        if (!wordArray.array[i].isPositive)
        {
            std::cout << wordArray.array[i].word << " = " << wordArray.array[i].count << " times" << std::endl;
            negativeWordCount++;
        }
    }
}

void generateGraph(WordArray &wordArray)
{

    vector<string> x_pos;
    vector<double> y_pos;

    // std::cout << std::endl;
    // std::cout << "Top 10 most frequent positive words:" << std::endl;

    int topPositiveCount = 0;
    for (int i = 0; i < wordArray.size && topPositiveCount < 10; ++i)
    {
        if (wordArray.array[i].isPositive && wordArray.array[i].count > 0)
        {
            // std::cout << wordArray.array[i].word << " = " << wordArray.array[i].count << " times" << std::endl;
            topPositiveCount++;
            y_pos.push_back({static_cast<double>(wordArray.array[i].count)});
            x_pos.push_back(wordArray.array[i].word);
        }
    }

    // Draw Bar Plot for the top 10 most frequent positive words
    RGBABitmapImageReference *imageReference = CreateRGBABitmapImageReference();
    StringReference *errorMessage = CreateStringReferenceLengthValue(0, L' ');

    BarPlotSeries *series = GetDefaultBarPlotSeriesSettings();
    series->ys = &y_pos;

    BarPlotSettings *settings = GetDefaultBarPlotSettings();
    settings->title = toVector(L"Top 10 Most Frequent Positive Words");
    settings->width = 1200.0;
    settings->barPlotSeries->push_back(series);
    settings->autoLabels = false;
    settings->xLabels = Allocate<StringReference *>(10.0);

    for (size_t i = 0; i < 10; i++)
    {
        std::string str = x_pos[i];
        std::wstring wstr = std::wstring(x_pos[i].begin(), x_pos[i].end());
        const wchar_t *x_label = wstr.c_str();

        settings->xLabels->at(i) = CreateStringReference(toVector(x_label));
    }

    DrawBarPlotFromSettings(imageReference, settings, errorMessage);

    vector<double> *pngData = ConvertToPNG(imageReference->image);
    WriteToFile(pngData, "Top10PositiveWords.png");
    DeleteImage(imageReference->image);

    // std::cout << std::endl;
    // std::cout << "Top 10 most frequent negative words:" << std::endl;

    vector<string> x_neg;
    vector<double> y_neg;
    int topNegativeCount = 0;
    for (int i = 0; i < wordArray.size && topNegativeCount < 10; ++i)
    {
        if (!wordArray.array[i].isPositive && wordArray.array[i].count > 0)
        {
            // std::cout << wordArray.array[i].word << " = " << wordArray.array[i].count << " times" << std::endl;
            topNegativeCount++;
            y_neg.push_back({static_cast<double>(wordArray.array[i].count)});
            x_neg.push_back(wordArray.array[i].word);
        }
    }

    series->ys = &y_neg;
    settings->title = toVector(L"Top 10 Most Frequent Negative Words");
    settings->xLabels = Allocate<StringReference *>(10.0);

    for (size_t i = 0; i < 10; i++)
    {
        std::string str = x_neg[i];
        std::wstring wstr = std::wstring(x_neg[i].begin(), x_neg[i].end());
        const wchar_t *x_label = wstr.c_str();

        settings->xLabels->at(i) = CreateStringReference(toVector(x_label));
    }

    DrawBarPlotFromSettings(imageReference, settings, errorMessage);

    vector<double> *pngData2 = ConvertToPNG(imageReference->image);
    WriteToFile(pngData2, "Top10NegativeWords.png");
    DeleteImage(imageReference->image);
    std::cout << "Graphs generated successfully!" << std::endl;
}
