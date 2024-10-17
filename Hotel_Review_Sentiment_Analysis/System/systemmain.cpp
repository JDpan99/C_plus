#include "System.hpp"
#include "System.cpp"
#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

int main()
{
    // Start overall timer
    auto beginning = high_resolution_clock::now();

    Node *csv = nullptr, *posScore = nullptr, *negScore = nullptr, *sentiment = nullptr;
    WordArray wordArray;

    int totalPos = 0, totalNeg = 0, totalRev = 0, consistency = 0;

    // File paths
    string csvpath = "tripadvisor_hotel_reviews.csv";
    string posFpath = "positive-words.txt";
    string negFpath = "negative-words.txt";

    // Read file and store in linked list
    auto start = high_resolution_clock::now();
    readfile(csvpath, csv);
    auto stop = high_resolution_clock::now();
    auto durationCsv = duration_cast<milliseconds>(stop - start);

    // Read Positive and Negative Word
    start = high_resolution_clock::now();
    cout << "Reading positive/negative files..." << endl;
    loadWordsFromFile(wordArray, posFpath, true);
    loadWordsFromFile(wordArray, negFpath, false);
    stop = high_resolution_clock::now();
    auto durationWord = duration_cast<milliseconds>(stop - start);
    deleteHead(csv);

    // Check word count
    start = high_resolution_clock::now();
    cout << "Checking word count..." << endl;
    wordArray.mergeSortWords();
    calculateWordFrequency(csv, wordArray, posScore, negScore, totalPos, totalNeg, totalRev);
    cout << "Total reviews: " << totalRev << endl;
    stop = high_resolution_clock::now();
    auto durationCheckWordFrequency = duration_cast<milliseconds>(stop - start);

    // Merge Sort
    start = high_resolution_clock::now();
    cout << "Sorting words by frequency..." << endl;
    wordArray.mergeSortByFrequency();
    stop = high_resolution_clock::now();
    auto durationSort = duration_cast<milliseconds>(stop - start);

    // Calculate score
    start = high_resolution_clock::now();
    cout << "Calculating score..." << endl;
    calculateScore(csv, posScore, negScore, sentiment, consistency);
    stop = high_resolution_clock::now();
    auto durationCalc = duration_cast<milliseconds>(stop - start);

    // Display final report
    start = high_resolution_clock::now();
    cout << "Displaying total..." << endl;
    displayReport(posScore, negScore, totalRev, totalPos, totalNeg, wordArray, consistency);
    stop = high_resolution_clock::now();
    auto durationDisplay = duration_cast<milliseconds>(stop - start);

    start = high_resolution_clock::now();
    cout << endl
         << "Generating graph..." << endl;
    generateGraph(wordArray);
    stop = high_resolution_clock::now();
    auto durationGraph = duration_cast<milliseconds>(stop - start);

    auto durationOverall = duration_cast<milliseconds>(stop - beginning);

    // Display all timing data in milliseconds
    cout << endl
         << "Running Time (milliseconds):" << endl;
    cout << "Time taken for reading CSV: " << durationCsv.count() << " milliseconds" << endl;
    cout << "Time taken for reading positive/negative files: " << durationWord.count() << " milliseconds" << endl;
    cout << "Time taken for checkWordCount: " << durationCheckWordFrequency.count() << " milliseconds" << endl;
    cout << "Time taken for mergeSort: " << durationSort.count() << " milliseconds" << endl;
    cout << "Time taken for calculateScore: " << durationCalc.count() << " milliseconds" << endl;
    cout << "Time taken for displayTotal: " << durationDisplay.count() << " milliseconds" << endl;
    cout << "Time taken for generateGraph: " << durationGraph.count() << " milliseconds" << endl;
    cout << "====================================" << endl;
    cout << "Overall execution time: " << durationOverall.count() << " milliseconds" << endl;
    cout << "====================================" << endl;

    // Freeing up allocated memory
    csv->freeList();
    posScore->freeList();
    negScore->freeList();
    sentiment->freeList();

    return 0;
}
