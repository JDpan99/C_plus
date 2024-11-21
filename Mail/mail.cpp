#include <iostream>
#include "mailSystem.hpp"

using namespace std;

void displayMainMenu()
{   
    // Large "Welcome to Tmail" in ASCII art
    cout << "W   W  EEEEE  L      CCCC  OOO  M   M  EEEEE\n";
    cout << "W   W  E      L     C     O   O MM MM  E    \n";
    cout << "W W W  EEEE   L     C     O   O M M M  EEEE \n";
    cout << "WW WW  E      L     C     O   O M   M  E    \n";
    cout << "W   W  EEEEE  LLLLL  CCCC  OOO  M   M  EEEEE\n";
    cout << "\n";

    // Updated ASCII Art for "Tmail"
    cout << " TTTTT  M   M  AAAAA III  L\n";
    cout << "   T    MM MM  A   A  I   L\n";
    cout << "   T    M M M  AAAAA  I   L\n";
    cout << "   T    M   M  A   A  I   L\n";
    cout << "   T    M   M  A   A III  LLLLL\n";
    cout << "\n";

    cout << "+---------------------+" << endl;
    cout << "|  Starting Program   |" << endl;
    cout << "+---------------------+" << endl;

    //displayLoading();
    cout << "Setup Complete! Get started now." << endl;
    cout << "--------------------------------------------------\n";
    cout << "Welcome to Email Management System\n\n";
    cout << "1. Login\n";
    cout << "2. Register\n";
    cout << "3. Exit\n";
    cout << "--------------------------------------------------\n";
    cout << "Select an option: ";
}
int main()
{
    Account Account;
    int choice;
    string email, username;
    while (true)
    {
        displayMainMenu();
        cin >> choice;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choice. Try again.\n";
            continue;
        }
        if (choice == 1){
            if(Account.run(email, username)) break;
        } else if (choice == 2){
            Account.registerAccount();
        } else if (choice == 3){
            cout << "--------------------------------------------------\n";
            cout << "Exiting...\n";
            cout << "--------------------------------------------------\n\n";
            cout << "---------- Thank You! Have a Nice Day! -----------\n";
            return 0;
        } else{
            cout << "Invalid choice. Try again.\n";
            cout << "--------------------------------------------------\n";
            cout << "Welcome to Email Management System\n\n";
            cout << "1. Login\n";
            cout << "2. Register\n";
            cout << "3. Exit\n";
            cout << "--------------------------------------------------\n";
            cout << "Select an option: ";
        }
    }
    User currUser(username, email);
    while(true){
        currUser.displayUserMenu();
        int choice;
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choice. Try again.\n";
            continue;
        }
        switch (choice)
        {
        case 1:
            currUser.newEmail();
            break;
        case 2:
            currUser.searchEmail();
            break;
        case 3:
            currUser.displayInbox();
            break;
        case 4:
            currUser.loadImportant();
            break;
        case 5:
            currUser.displaySent();
            break;
        case 6:
            currUser.displayAll();
            break;
        case 7:
            currUser.loadSpam();
            break;
        case 8:
            currUser.displayStarred();
            break;
        case 0:
            cout << "--------------------------------------------------\n";
            cout << "Logging out and exiting...\n";
            cout << "--------------------------------------------------\n\n";
            cout << "---------- Thank You! Have a Nice Day! -----------\n";
            return 0;
            break;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    }
    return 0;
}