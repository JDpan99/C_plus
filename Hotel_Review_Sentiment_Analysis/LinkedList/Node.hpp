#ifndef NODE_HPP
#define NODE_HPP

#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <cmath>

using namespace std;
// positive = 1003
// negative = 2391
struct Node
{
    string data;
    Node* next;
    int count;
    Node(const string& newData, int count);

    ~Node() {

    }

    void freeList() {
        Node* current = this;
        while (current != nullptr) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
    }
};
Node::Node(const string& newData, int count) {
    data = newData;
    this->count= count;
    next = nullptr;
};


void append(Node*& head, const string newData) {
    int lastDigit = isdigit(newData.back()) ? newData.back() - '0' : 0;
    Node* newNode = new Node(newData, lastDigit);
    if (head == nullptr) {
        head = newNode;
    } else {
        Node* temp = head;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}
void appendWord(Node*& head, const string& newData) {
    for (Node* temp = head; temp != nullptr; temp = temp->next) {
        if (temp->data == newData) return; // Exit if duplicate found
    }
    Node* newNode = new Node(newData, 1);

    if (!head) {
        head = newNode;
    } else {
        Node* temp = head;
        while (temp->next) temp = temp->next; 
        temp->next = newNode;
    }
}
void readfile(const string& path, Node*& head) {  
    ifstream file(path);
    if (!file.is_open()) {
        cout << "File not found" << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        append(head, line);
    }
    file.close();
}
void deleteHead(Node*& head) {
    Node* temp = head; 
    head = head->next; 
    delete temp; 
}


string removePunctuation(string word){
    word.erase(remove_if(word.begin(), word.end(), ::ispunct), word.end());
    return word;
}

//Word Check and Display
void displayWord(Node* head, const string& wordType){
    if(head ==nullptr) return;
    int c = 0;
    Node* temp = head;
    
    while (temp != nullptr) {
        c++;
        temp = temp->next;
    }
    cout << wordType << " Words = " << c << endl;
    
    temp = head;
    while (temp != nullptr) {
        cout << "• " << temp->data << endl;
        temp = temp->next;
    }
}
// split the linked list into two halves
void splitList(Node* head, Node*& firstHalf, Node*& secondHalf) {
    Node* slow = head;
    Node* fast = head->next;

    while (fast != nullptr) {
        fast = fast->next;
        if (fast != nullptr) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    firstHalf = head;
    secondHalf = slow->next; 
    slow->next = nullptr;     
}
// compare a word against a linked list
bool compareWord(Node* head, const string& word) {
    
    // Traverse the list instead of splitting repeatedly
    while (head != nullptr) {
        if (head->data == word) {
            head->count++; // Increment count if word is found
            return true;
        }
        if (head->data > word) {
            return false; // Since the list is sorted, the word is not present
        }
        head = head->next; // Move to the next node
    }
    return false;
    // Recursively check the left and right halves
    //Node* firstHalf = nullptr;
    //Node* secondHalf = nullptr;
    //splitList(head, firstHalf, secondHalf);
    //return compareWord(firstHalf, word) || compareWord(secondHalf, word);
}
/*bool compareWord(Node* head, const string& word) {
    while (head != nullptr) {
        Node* firstHalf = nullptr;
        Node* secondHalf = nullptr;
        splitList(head, firstHalf, secondHalf);

        if (firstHalf->data == word) {
            firstHalf->count++; // Word found in first half
            return true;
        } else if (secondHalf != nullptr && secondHalf->data == word) {
            secondHalf->count++;
            return true;
        }

        // Decide which half to continue searching
        if (firstHalf->data < word) {
            head = secondHalf; // Continue with the second half
        } else {
            head = firstHalf; // Continue with the first half
        }
    }
    return false; // Word not found
}*/

void processWord(const string& cleaned, Node*& pos, Node*& neg, Node*& tempPos, Node*& tempNeg, int& pScore, int& nScore, int& totalPos, int& totalNeg) {
    // Check in positive list
    if (compareWord(pos, cleaned)) {
        appendWord(tempPos, cleaned);
        pScore++;
        totalPos++;
    }
    // Check in negative list
    if (compareWord(neg, cleaned)) {
        appendWord(tempNeg, cleaned);
        nScore++;
        totalNeg++;
    }
}

void processReview(Node* csv, Node*& pos, Node*& neg,Node*& posScore, Node*& negScore, Node*& tempPos, Node*& tempNeg, int& totalPos, int& totalNeg, int& totalRev) {
    int pScore = 0, nScore = 0;
    stringstream ss(csv->data);
    string word;
    //Check positive word and negative word
    while (ss >> word) {
        string cleaned = removePunctuation(word);
        processWord(cleaned, pos, neg, tempPos, tempNeg, pScore, nScore, totalPos, totalNeg);
    }   
 
    append(posScore, to_string(pScore));
    append(negScore, to_string(nScore));
    // After processing the words
    if (tempPos == nullptr) {
        cout << "No positive words found." << endl;
    } else {
        displayWord(tempPos, "Positive");
        tempPos->freeList();
    }

    if (tempNeg == nullptr) {
        cout << "No negative words found." << endl;
    } else {
        displayWord(tempNeg, "Negative");
        tempNeg->freeList();        
    }
}
void checkWordCount(Node* csv, Node*& pos, Node*& neg, Node*& posScore, Node*& negScore, int& totalPos, int& totalNeg, int& totalRev) {
    while (csv) {
        totalRev++;
        //if (totalRev == 100) break;
        Node* tempPos = nullptr;
        Node* tempNeg = nullptr; 
        // check each review
        processReview(csv, pos, neg, posScore, negScore, tempPos, tempNeg, totalPos, totalNeg, totalRev);

        // Move to next node
        csv = csv->next;
    }
}

// Sentiment Analysis
void calculateScore(Node* posScore, Node* negScore, Node* &sentiment){
     while(posScore && negScore){
        int positive = stoi(posScore->data);
        int negative = stoi(negScore->data);
        int rawScore = positive - negative;
        int n = positive + negative;
        double score = (n != 0) ? 1 + 4 * (rawScore + n) / (2.0 * n) : 1.0;
        append(sentiment, to_string(score));

        //Move to the next node
        posScore = posScore->next;
        negScore = negScore->next;
     }
}
// Check consistency
void displayConsistency(double score, int roundscore, const string overall, int rating, int& consistency){
    cout<< "Sentiment Score (1-5) is "<< score;
    cout<< ", thus the rating should be equal to " << roundscore << overall<< endl;
    cout<< "Sentiment Score (1-5) = " << roundscore << endl;
    cout<< "Rating given by user = " << rating << endl;
    if(roundscore != rating){
        cout<< "Analysis output: "<< endl;
        cout<< "User's subjective evaluation does not match the sentiment score provided by the analysis."<< endl;
        cout<< "There is an inconsistency between the sentiment score generated by the analysis and the user's evaluation of the sentiment."<< endl;
    } else{
        consistency++;
        cout<< "Analysis output: "<< endl;
        cout<< "User's subjective evaluation matches the sentiment score provided by the analysis. ";
        cout<< "There is a consistency between the sentiment score generated by the analysis and the user's evaluation of the sentiment."<< endl;
    }
    cout<< endl;
}
string checkOverall(int score){
    switch (score)
    {
    case 1: 
    case 2: 
        return " (Negative).";
    case 3: 
        return " (Neutral).";
    case 4: 
    case 5: 
        return " (Positive).";
    default:
        "Invalid Score";
    }
}

void CheckConsistency(Node* csv, Node* sentiment, int& consistency){
    while(csv && sentiment){
        cout<< " failed"<< endl;
        double sentiScore = stod(sentiment->data);
        int roundscore = round(sentiScore);
        string overall = checkOverall(roundscore);
        displayConsistency(sentiScore, roundscore, overall, csv->count, consistency);
        csv = csv->next;
        sentiment = sentiment->next;
    }
}
// Merge positive linked list and negative linked list
void appendtoTotal(Node*& head, const string& word, int count) {
    Node* newNode = new Node(word, count);

    if (head == nullptr) {
        head = newNode;
    } else {
        Node* temp = head;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}
Node* merge(Node* front, Node* back) {
    if (front == nullptr) return back;
    if (back == nullptr) return front;

    Node* result = nullptr;

    if (front->count >= back->count) {
        result = front;
        result->next = merge(front->next, back);
    } else {
        result = back;
        result->next = merge(front, back->next);
    }
    return result;
}
void mergeSort(Node*& head) {
    if (head == nullptr || head->next == nullptr) {
        return; 
    }

    Node* front;
    Node* back;
    splitList(head, front, back);
    mergeSort(front);
    mergeSort(back);
    head = merge(front, back);
}

// Bubble sort for linked list
Node *bubbleSort(Node *head) {
    bool flag;
    // Iterating over the whole linked list
    while (head) {
        Node *tra = head;
        Node *pre = head;
        flag = false;

        while (tra->next) {
            // Temporary pointer to store the next
            // pointer of traverseNode
            Node *ptr = tra->next;
            if (tra->count < ptr->count) {
                flag = true;
                if (tra == head) {
                    // Performing swap operations and
                    // updating the head of the linked list
                    tra->next = ptr->next;
                    ptr->next = tra;
                    pre = ptr;
                    head = pre;
                }
                else {
                    // Performing swap operation
                    tra->next = ptr->next;
                    ptr->next = tra;
                    pre->next = ptr;
                    pre = ptr;
                }
                continue;
            }
            pre = tra;
            tra = tra->next;
        }

        // If no swap occurred, break the loop
        if (!flag) {
            break;
        }
        head = head->next;
    }
    // Returning the head of the linked list
    return head;
}
void combineLists(Node* pos, Node* neg, Node*& fre, int& posTotalCount, int& negTotalCount, int& final) {
    // Add all positive words to the freNode list
    Node* temp = pos;
    while (temp != nullptr) {
        appendtoTotal(fre, temp->data, temp->count); 
        posTotalCount += temp->count;
        final++;
        temp = temp->next;
    }

    // Add all negative words to the freNode list
    temp = neg;
    while (temp != nullptr) {
        appendtoTotal(fre, temp->data, temp->count);
        negTotalCount += temp->count; 
        final++;
        temp = temp->next;
    }

    // Now sort the combined freNode list
    fre = bubbleSort(fre);
    //quickSort
    //mergeSort(fre);
}
// Display minimum and maximum words used
int findMaxCount(Node* head) {
    int maxCount = 0;
    Node* temp = head;
    
    while (temp) {
        if (temp->count > maxCount) {
            maxCount = temp->count;
        }
        temp = temp->next;
    }
    
    return maxCount;
}
void displayResult(Node* head, int targetCount, const string& message) {
    cout << message;
    Node* temp = head;
    bool found = false;
    
    while (temp) {
        if (temp->count == targetCount) {
            cout << temp->data << ", ";
            found = true;
        }
        temp = temp->next;
    }
    if (!found) cout << "None found.";
    cout << endl;
}
void displayMin(Node* head) {
    displayResult(head, 1, "Minimum used words in the reviews: ");
}
void displayMax(Node* head) {
    int maxCount = findMaxCount(head); 
    displayResult(head, maxCount, "Maximum used words in the reviews: ");
}
void displaytop(Node* head, string word){
    int count = 0;
    cout<< endl << "Top 5 most frequent "<< word <<" words:" << endl;
    while(head){
        if (count == 5) return;
        cout<< "• " << head->data << " = " << head->count << endl;
        head = head->next;
        count++;
    }
    
}
// display the overall sentiment of the reviews
void displayTotal(int totalRev, int totalPos, int totalNeg, Node* pos, Node* neg){
    Node* fre = nullptr;
    int positivetotalCount = 0, negativetotalCount =0, finalwordCount = 0;
    combineLists(pos, neg, fre, positivetotalCount, negativetotalCount, finalwordCount);
    cout << "Total Reviews = " << totalRev << endl;
    cout << "Total Counts of Positive Words = " << totalPos << endl;
    cout << "Total Counts of Negative Words = " << totalNeg << endl << endl;
    //Additional Information
    double average = static_cast<double>(positivetotalCount + negativetotalCount) / finalwordCount;
    double perPos = (static_cast<double>(positivetotalCount)/ (positivetotalCount + negativetotalCount)) *100;
    double perNeg = 100 - perPos;
    cout << "Additional report details: "<< endl;
    cout << "Average word frequency across all reviews: "<< average <<endl;
    cout << "Percentage of Positive Words: "<< perPos << "%"<< endl;
    cout << "Percentage of Negative Words: "<< perNeg << "%"<< endl << endl;
    /*cout << "Frequency of each word used in overall reviews, listed in ascending order based on frequency:" << endl << endl;
    
    Node* temp = fre;
    while (temp) {
        if(temp->count != 0){
            cout<< 
        }
        temp = temp->next;
    }*/
    
    // Display minimum and maximum words
    //displayMin(fre);
    //displayMax(fre);
    displaytop(pos, "positive");
    displaytop(neg, "negative");

    fre->freeList();

}
#endif
