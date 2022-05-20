/*
* Andrew Cattapan
* CSC 204: Final Program Coffee Shop
* April 25th, 2022
*/


#include <cmath>
#include <fstream>
#include <iostream>
#include <iomanip>

using namespace std;


string formatString(string input);
void menuAdd();
void displayMenu();
void getCash(double prices[], int purchases[]);
void printTotal();
void getCash();
bool CheckIfNumber(string s);
void printReceipt();



bool CheckIfNumber(string s)
{
    for (int i = 0; i < s.length(); i++)
        if (isdigit(s[i]) == false)
            return false;

    return true;
}


class Item
{
    public:
    string name;
    double price;
    int numPurchased;

    void addNumPurchased()
    {
        string input;
        int integer;
        while(true)
        {
            cout << "You've selected: " << name << endl;
            cout << "Please enter how many you are adding: ";
            cin >> input;
            if(CheckIfNumber(input))
            {
                integer = stoi(input);
                numPurchased += integer;
                break;
            }
            else
            {
                cout << "Please enter an integer." << endl;
            }
        }
    }

    void deleteNumPurchased()
    {
        string input;
        int integer;
        while(true)
        {
            if(numPurchased == 0)
            {
                cout << "You haven't ordered any " << name << "(s)." << endl;
                break;
            }
            cout << "Please enter how many " << name << "(s) you are removing: ";
            cin >> input;
            if(CheckIfNumber(input))
            {
                integer = stoi(input);
                if(numPurchased - integer < 0)
                {
                    cout << "You have ordered " << numPurchased << " " << name << "(s)." << endl;
                    cout << "You chose to delete: " << input << endl;
                    numPurchased = 0;
                    cout << "Number purchased has been reset to zero." << endl;
                }
                else
                {
                    numPurchased = numPurchased - integer;
                }
                break;
            }
            else
            {
                cout << "Please enter an integer." << endl;
            }
        }
    }
};


class Receipt: public Item
{
    public:
        string display()
        {
            return "You bought " + to_string(numPurchased) + " " + name + "(s) for $";
        }
};


Item items[10] =
{
    {"Small Coffee", 3.39, 0},
    {"Medium Coffee", round(items[0].price*1.5*100)/100, 0},
    {"Large Coffee", round(items[0].price*1.7*100)/100, 0},
    {"Single Donut", 1.39, 0},
    {"Half-Dozen Donuts", round(items[3].price*6*0.8*100)/100, 0},
    {"Dozen Donuts", round(items[4].price*2*0.8*100)/100, 0},
    {"Single Cinnamon Roll", 2.43, 0},
    {"Dozen Cinnamon Rolls", round(items[6].price*12*0.7*100)/100, 0},
    {"Pie Slice", 4.39, 0},
    {"Whole Pie", round(items[8].price*5*0.7*100)/100, 0}
};


void removePurchases()
{
    int loop = 0;
    int arrayLength = sizeof(items)/sizeof(items[0]);
    string userInput;

    cout << "Please enter the number of the item you want to remove:" << endl;
    for(int i = 0; i < arrayLength; i++)
    {
        if(items[i].numPurchased > 0)
        {
            cout << i << ". You ordered " << items[i].numPurchased << " " << items[i].name << "(s)." << endl;
        }
    }
    cin >> userInput;
    if(CheckIfNumber(userInput))
    {
        int input = stoi(userInput);
        if(input <= arrayLength && input > 0)
        {
            items[input].deleteNumPurchased();
        }
        else
        {
            cout << "Please enter one of the digits provided." << endl;
        }
    }
    else
    {
        cout << "Invalid input. Please enter a number." << endl;
    }
}


void addPurchases(void)
{
    string userInput;
    bool matchFound;
    int arrayLength = sizeof(items)/sizeof(items[0]);

    do
    {
        matchFound = false;

        cout << "Please enter the number of the item you want to order or type 'Exit': ";
        cin >> userInput;
        if(formatString(userInput) == "Exit")
        {
            break;
        }
        for(int i = 0; i < arrayLength; i++)
        {
            string temp = to_string(i + 1);
            if(temp == userInput)
            {
                items[i].addNumPurchased();
                matchFound = true;
                break;
            }
        }
        if(matchFound == false)
        {
            cout << "You must enter a number. Returning to menu..." << endl;
        }
    } while (matchFound == false);
    cout << endl;
}

class LinkedList
{
    public:
        string name;
    LinkedList *next;
};



void printLinkedList(LinkedList * n)
{
    while(n != NULL)
    {
        cout << n->name;
        n = n->next;
    }
}


int main()
{
    LinkedList * head = NULL;
    LinkedList * second = NULL;
    LinkedList * third = NULL;
    LinkedList * fourth = NULL;

    head = new LinkedList();
    second = new LinkedList();
    third = new LinkedList();
    fourth = new LinkedList();

    head->name = "Please enter one of the options by entering a number or typing 'Exit':\n";
    head->next = second;
    second->name = "1. Order items.\n";
    second->next = third;
    third->name = "2. Remove items.\n";
    third->next = fourth;
    fourth->name = "3. Print receipt.\n";


    int loop = 0;
    string userInput;

    do
    {
        printTotal();
        printLinkedList(head);
        cin >> userInput;

        if(CheckIfNumber(userInput))
        {
            if(userInput == "1")
            {
                displayMenu();
                addPurchases();
            }
            else if(userInput == "2")
            {
                removePurchases();
            }
            else if(userInput == "3")
            {
                printReceipt();
            }
        }
        else if(formatString(userInput) == "Exit")
        {
            loop = 1;
        }
        else
        {
            cout << "You must enter one of the options above." << endl;
        }
    } while(loop == 0);

    getCash();

    cout << "Exiting..." << endl;

    return 0;
}


void printReceipt()
{
    //Declare receipt instances
    Receipt receipts[10];
    int receiptLength = sizeof(receipts)/sizeof(receipts[0]);
    for(int i = 0; i < receiptLength; i++)
    {
        receipts[i].name = items[i].name;
        receipts[i].price = items[i].price;
        receipts[i].numPurchased = items[i].numPurchased;
    }

    string output;
    int loop = 0;

    do
    {
        cout << "Please enter the name of the output file you want: ";
        cin >> output;

        //Check if there's an input file with the same name
        try
        {
            std::ifstream inf{output};
            if(inf)
            {
                throw output;
            }
            std::ofstream outf{output};

            for(int i = 0; i < receiptLength; i++)
            {
                if(receipts[i].numPurchased > 0)
                {
                   outf << receipts[i].display() << (receipts[i].price * receipts[i].numPurchased);
                }
            }
            cout << "Receipt printed." << endl;
            loop = 1;
        }
        catch(string output)
        {
            cout << "File already exists." << endl;
        }
    } while(loop == 0);
}


void getCash()
{
    /*
    * Asks the user to enter the amount of cash given to pay for the total
    * Tells the user if the customer still owes money and how much if so
    */
    int loop = 0;
    double total = 0;
    double cash;
    for(int i = 0; i < 10; i++)
    {
        if (items[i].numPurchased > 0)
        {
            total += (items[i].price*items[i].numPurchased);
        }
    }
    if(total != 0)
    {

        cout << "Your total is: $" << total << endl;
        do
        {
            cout << "Please enter how much cash you were given: ";
            cin >> cash;
            if(cash <= 0)
            {
                cout << "Input must be greater than zero." << endl;
            }
            else if(cash < total)
            {
                total = total - cash;
                cout << "You still owe $" << total << endl;
            }
            else if(cash == total)
            {
                cout << "Exact change received." << endl;
                loop = 1;
            }
            else if(cash > total)
            {
                double change = cash - total;
                cout << "Change owed to customer: $" << change << endl;
                loop = 1;
            }
        } while (loop == 0);
    }
    else
    {
        cout << "No items entered." << endl;
    }
}


void printTotal()
{
    /*
    * Prints the number of items purchased along with the total
    */
    double total = 0;
    cout << endl;
    for(int i = 0; i < 10; i++)
    {
        if (items[i].numPurchased > 0)
        {
            cout << items[i].numPurchased << " " << items[i].name << "(s): $" << (items[i].price*items[i].numPurchased) << endl;
            total += (items[i].price*items[i].numPurchased);
        }
    }
    cout << "Total: $" << total << endl;
    cout << endl;
}


void displayMenu()
{
    /*
    * Displays the menu and its options
    * May modify to make it more cashier-friendly and direct
    */
    string border = "================================================="; //45 "=" long
    string gap = "|\t\t\t\t\t\t|";
    cout << border << endl;
    cout << gap << endl;
    cout << "| Coffee:\t\t\t\t\t|" << endl;
    cout << "|\t1.  " << items[0].name << ": $" << items[0].price << "\t\t\t|" << endl;
    cout << "|\t2.  " << items[1].name << ": $" << items[1].price << "\t\t|" << endl;
    cout << "|\t3.  " << items[2].name << ": $" << items[2].price << "\t\t\t|" << endl;
    cout << gap << endl;
    cout << "| Donut:\t\t\t\t\t|" << endl;
    cout << "|\t4.  " << items[3].name << ": $" << items[3].price << "\t\t\t|" << endl;
    cout << "|\t5.  " << items[4].name << ": $" << items[4].price << "\t\t|" << endl;
    cout << "|\t6.  " << items[5].name << ": $" << items[5].price << "\t\t|" << endl;
    cout << gap << endl;
    cout << "| Cinnamon Roll:\t\t\t\t|" << endl;
    cout << "|\t7.  " << items[6].name << ": $" << items[6].price << "\t\t|" << endl;
    cout << "|\t8.  " << items[7].name << ": $" << items[7].price << "\t|" << endl;
    cout << gap << endl;
    cout << "| Pie:\t\t\t\t\t\t|" << endl;
    cout << "|\t9.  " << items[8].name << ": $" << items[8].price << "\t\t\t|" << endl;
    cout << "|\t10. " << items[9].name << ": $" << items[9].price << "\t\t\t|" << endl;
    cout << gap << endl;
    cout << border << endl;
}


string formatString(string input)
{
    /*
    * Formats the string passed to have proper capitalization.
    */
    string newInput;
    int stringLength = input.length();

    if (stringLength != 0)
    {
        newInput = toupper(input[0]);
        for (int i = 1; i < stringLength; i++)
        {
            newInput += tolower(input[i]);
        }
    }
    else
    {
        cout << "String used has no characters." << endl;
    }

    return newInput;
}
