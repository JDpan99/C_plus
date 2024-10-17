#include "node.hpp"
#include <iostream>
#include <chrono>  

using namespace std;
using namespace std::chrono;

int main() {
    Node *pos = nullptr, *csv = nullptr, *neg = nullptr, *posScore = nullptr, *negScore = nullptr, *sentiment = nullptr;

    int totalPos = 0, totalNeg = 0, totalRev = 0, consistency = 0;
    
    // File paths
    string csvpath = "src/tripadvisor_hotel_reviews.csv";
    string posFpath = "src/positive-words.txt";
    string negFpath = "src/negative-words.txt";

    // Start overall timer
    auto beginning = high_resolution_clock::now();

    // Read file and store in linked list
    auto start = high_resolution_clock::now();
    readfile(csvpath, csv);
    auto stop = high_resolution_clock::now();
    auto durationCsv = duration_cast<milliseconds>(stop - start);

    start = high_resolution_clock::now(); 
    readfile(negFpath, neg);
    readfile(posFpath, pos); 
    stop = high_resolution_clock::now();
    auto durationFile = duration_cast<milliseconds>(stop - start);
    
    deleteHead(csv);

    // Check word count
    start = high_resolution_clock::now();
    checkWordCount(csv, pos, neg, posScore, negScore, totalPos, totalNeg, totalRev);
    stop = high_resolution_clock::now();
    auto durationCheck = duration_cast<milliseconds>(stop - start);

    // Merge Sort
    start = high_resolution_clock::now();
    //mergeSort(pos);
    //mergeSort(neg);
    pos = bubbleSort(pos);
    neg = bubbleSort(neg);
    stop = high_resolution_clock::now();
    auto durationSort = duration_cast<milliseconds>(stop - start);

    // Calculate score
    start = high_resolution_clock::now();
    calculateScore(posScore, negScore, sentiment);
    stop = high_resolution_clock::now();
    auto durationCalc = duration_cast<milliseconds>(stop - start);

    // Check consistency
    start = high_resolution_clock::now();
    CheckConsistency(csv, sentiment, consistency);
    stop = high_resolution_clock::now();
    auto durationConsistency = duration_cast<milliseconds>(stop - start);

    // Display final totals
    start = high_resolution_clock::now();
    displayTotal(totalRev, totalPos, totalNeg, pos, neg);
    stop = high_resolution_clock::now();
    auto durationDisplay = duration_cast<milliseconds>(stop - start);

    auto durationOverall = duration_cast<milliseconds>(stop - beginning);

    // Display all timing data in milliseconds
    cout << endl<< "Running Time (milliseconds):" << endl;
    cout << "Time taken for reading CSV: " << durationCsv.count() << " milliseconds" << endl;
    cout << "Time taken for reading positive/negative files: " << durationFile.count() << " milliseconds" << endl;
    cout << "Time taken for checkWordCount: " << durationCheck.count() << " milliseconds" << endl;
    cout << "Time taken for BubbleSort: " << durationSort.count() << " milliseconds" << endl;
    cout << "Time taken for calculateScore: " << durationCalc.count() << " milliseconds" << endl;
    cout << "Time taken for CheckConsistency: " << durationConsistency.count() << " milliseconds" << endl;
    
    // Consistency percentage calculation 
    double consistencyPercentage = (totalRev != 0) ? (static_cast<double>(consistency) / static_cast<double>(totalRev)) * 100 : 0.0;
    cout << "Rating consistency percentage: " << consistencyPercentage << "% " << endl;
    
    cout << "Time taken for displayTotal: " << durationDisplay.count() << " milliseconds" << endl;
    cout << "Overall execution time: " << durationOverall.count() << " milliseconds" << endl;

    // Freeing up allocated memory
    pos->freeList();
    neg->freeList();
    csv->freeList();
    posScore->freeList();
    negScore->freeList();

    return 0;  
}
