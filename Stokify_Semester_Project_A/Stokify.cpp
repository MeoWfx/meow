////////////////////////////////////////////////////////////////I N V E N T O R Y    M A N A G M E N T   S Y S T E M ////////////////////////////////////////////////////////////////////

#include <iostream>
#include <conio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cctype>
#include <windows.h>
#include <iomanip>
using namespace std;

////// GLOBAL VARIABLES //////
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

////// FUNCTIONS PROTOTYPE //////

///////// DISPLAY //////////

void setCursorPosition(int x, int y);
void displayHeader();
void displayAdmin();
void displayUser();
void logout();
void systemExit(bool uflag[], string username[], string passwords[], bool user_Purchase[], string feedback[], bool pflag[], string product_Name[], int product_Quantity[], double product_Price[], int productCategoryIndex[], string categories[], int usize, int psize, int userProductIndices[][15], int userProductQuantities[][15], int purchaseCounts[], string email[], string product_Expiry[]);
void inValid();
string getPasswordInput();
void userAuthentication(string email[], int &loggedInUserType, string username[], string passwords[], int usize, int &currentUser);
void adminAuthentication(int &loggedInUserType);
void saveDataToCSV(bool uflag[], string username[], string passwords[], bool user_Purchase[], string feedback[], bool pflag[], string product_Name[], int product_Quantity[], double product_Price[], int productCategoryIndex[], string categories[], int usize, int psize, int userProductIndices[][15], int userProductQuantities[][15], int purchaseCounts[], string email[], string product_Expiry[]);
void loadDataFromCSV(bool uflag[], string username[], string passwords[], bool user_Purchase[], string feedback[], bool pflag[], string product_Name[], int product_Quantity[], double product_Price[], int productCategoryIndex[], string categories[], int usize, int psize, int &currentUserCount, int &currentProductCount, int &currentCategoryCount, int userProductIndices[][15], int userProductQuantities[][15], int purchaseCounts[], string email[], string product_Expiry[]);
//////////.....Inputs;
int getAdminInput();
int getWareHouseInput();
int getStockInput(string categories[], int currentCategoryCount);
int getUserHubInput();
int getUserInput(string username[], int currentUser);
int getPlaceOrderInput();
int getMainInput();

//////// FUNCTIONALITY ////////

void initializeUserArrays(bool uflag[], bool user_Purchase[], string username[], string passwords[], string email[], string feedback[], int usize);
void initializeProductArrays(bool pflag[], string product_Name[], int product_Quantity[], double product_Price[], string product_Expiry[], int psize);
void addProductCategory(string categories[], int &currentCategoryCount, int csize);
void displayCategories(string categories[], int currentCategoryCount);
void addProduct(bool pflag[], string product_Name[], int product_Quantity[], double product_Price[], int &currentProductCount, int psize, string categories[], int currentCategoryCount, int productCategoryIndex[], string product_Expiry[]);
bool isNearExpiry(string expiryDate);
bool isValidExpiryDate(const string &expiryDate);
void deleteProduct(bool pflag[], string product_Name[], int product_Quantity[], double product_Price[], int &currentProductCount, string categories[], int currentCategoryCount);
void displayAllProducts(string product_Name[], int product_Quantity[], double product_Price[], int currentProductCount, string categories[], bool pflag[], int currentCategoryCount, int productCategoryIndex[], string product_Expiry[]);
void updateProduct(string product_Name[], int product_Quantity[], double product_Price[], string categories[], int currentProductCount, bool pflag[], int currentCategoryCount, int productCategoryIndex[], string product_Expiry[]);
void updateStockAndCheckThreshold(string product_Name[], int product_Quantity[], double product_Price[], int currentProductCount, bool pflag[], int threshold);
void addUser(bool uflag[], bool user_Purchase[], string username[], string passwords[], string email[], int &currentUserCount, int usize);
void deleteUser(bool uflag[], bool user_Purchase[], string feedback[], string username[], string email[], string passwords[], int &currentUserCount);
void provideFeedback(bool uflag[], string username[], string feedback[], int currentUserCount);
void displayAllUserInfo(bool uflag[], bool user_Purchase[], string username[], string passwords[], string email[], int currentUserCount);
void displayAllFeedbacks(string username[], string feedback[], string email[], bool uflag[], int currentUserCount);
void generateDetailedUserReport(bool uflag[], string username[], string email[], string feedback[], bool user_Purchase[], int userProductIndices[][15], int userProductQuantities[][15], int purchaseCounts[], string product_Name[], double product_Price[], int currentUserCount, int currentProductCount);
void purchaseProduct(bool uflag[], bool user_Purchase[], int userProductIndices[][15], int userProductQuantities[][15], int purchaseCounts[], string product_Name[], int product_Quantity[], double product_Price[], string categories[], int productCategoryIndex[], int currentProductCount, int currentCategoryCount, int currentUser);
void removePurchasedItem(int userProductIndices[][15], int userProductQuantities[][15], int purchaseCounts[], int product_Quantity[], string product_Name[], int currentUser, bool user_Purchase[], string username[], double product_Price[], int currentProductCount);
void editPurchaseQuantity(int userProductIndices[][15], int userProductQuantities[][15], int purchaseCounts[], int product_Quantity[], string product_Name[], int currentUser, bool user_Purchase[], string username[], double product_Price[], int currentProductCount);
void trackUserPurchases(int currentUser, bool user_Purchase[], string username[], string product_Name[], double product_Price[], int userProductQuantities[][15], int currentProductCount);

////// MAIN FUNC //////

int main()
{
    system("chcp 65001");
    // Constants for maximum sizes
    int usize = 10; // Maximum number of users
    int psize = 15; // Maximum number of products
    int csize = 10; // Maximum number of categories

    int currentCategoryCount = 0;
    int currentProductCount = 0;
    int totalPurchaseAmount = 0;
    int threshold = 5; // Set the threshold to 5 for low stock warning
    int currentUserCount = 0;
    int currentUser = -1;

    // Arrays for users
    bool uflag[usize];                          // Tracks whether a user exists at a specific index
    bool user_Purchase[usize];                  // Tracks whether a user has made a purchase
    string username[usize];                     // Array to store usernames
    string passwords[usize];                    // Array to store passwords
    string feedback[usize];                     // Array to store user feedback
    string email[usize];                        // Array to store user emails
    int userProductIndices[usize][15] = {0};    // 2D array that stores the indices of products purchased by each user.
    int userProductQuantities[usize][15] = {0}; // 2D array that stores the quantities of each product purchased by a user.
    int purchaseCounts[usize] = {0};            // Tracks the number of purchases made by each user.

    // Arrays for products
    bool pflag[psize];               // Tracks whether a product exists at a specific index
    string product_Name[psize];      // Array to store product names
    int product_Quantity[psize];     // Array to store product quantities
    double product_Price[psize];     // Array to store product prices
    string product_Expiry[psize];    // Array to store product expiries
    int productCategoryIndex[psize]; // Array to track the category of each product by storing an index corresponding to a category.
    string categories[csize];        // Stores the names of different product categories.

    // Initialize all arrays
    initializeUserArrays(uflag, user_Purchase, username, passwords, email, feedback, usize);
    initializeProductArrays(pflag, product_Name, product_Quantity, product_Price, product_Expiry, psize);

    // Load data from CSV files
    loadDataFromCSV(uflag, username, passwords, user_Purchase, feedback, pflag, product_Name, product_Quantity, product_Price, productCategoryIndex, categories, usize, psize, currentUserCount, currentProductCount, currentCategoryCount, userProductIndices, userProductQuantities, purchaseCounts, email, product_Expiry);

    int input;
    int loggedInUserType = -10; // 1 for manager/administrator, 2 for use

    do
    {
        input = getMainInput();
        if (input == 0)
        {
            loggedInUserType = -10;
            systemExit(uflag, username, passwords, user_Purchase, feedback, pflag, product_Name, product_Quantity, product_Price, productCategoryIndex, categories, usize, psize, userProductIndices, userProductQuantities, purchaseCounts, email, product_Expiry);
            cout << "data saved";
            getch();
            return 0;
        }

        else
        {
            switch (input)
            {
            case 1:
                adminAuthentication(loggedInUserType);
                break;

            case 2:
                userAuthentication(email, loggedInUserType, username, passwords, usize, currentUser);
                break;

            case 3:
                addUser(uflag, user_Purchase, username, passwords, email, currentUserCount, usize);
                loggedInUserType = -10;
                Sleep(1000);
                break;

            default:
                inValid();
                break;
            }
        }

        if (loggedInUserType == 1) ////////////////////////////////////////////////////////////// Administrator
        {
            do
            {
                input = getAdminInput();
                if (input == 0)
                {
                    logout();
                    break;
                }
                else
                {
                    switch (input)
                    {
                    case 1: /////////////////////////////////WARE-HOUSE
                        do
                        {
                            input = getWareHouseInput();

                            if (input == 0)
                            {
                                logout();
                                break;
                            }
                            else
                            {
                                switch (input)
                                {

                                case 1:
                                    displayAllProducts(product_Name, product_Quantity, product_Price, currentProductCount, categories, pflag, currentCategoryCount, productCategoryIndex, product_Expiry);
                                    cout << "\n\n\t     Press any key to continue  ";
                                    getch();
                                    Sleep(500);
                                    break;

                                case 2:
                                    char ch;
                                    do
                                    {
                                        input = getStockInput(categories, currentCategoryCount);
                                        switch (input)
                                        {
                                        case 1:
                                            addProductCategory(categories, currentCategoryCount, csize);
                                            Sleep(1500);
                                            break;

                                        case 2:
                                            addProduct(pflag, product_Name, product_Quantity, product_Price, currentProductCount, psize, categories, currentCategoryCount, productCategoryIndex, product_Expiry);
                                            Sleep(1500);
                                            break;

                                        default:
                                            inValid();
                                            break;
                                        }

                                        cout << "\n\n\t     Do You want to continue again (Enter 'N' to Return) : ";
                                        cin.ignore();
                                        cin.get(ch);

                                        if (ch == 'N')
                                        {
                                            logout();
                                        }

                                    } while (ch != 'N');

                                    input = -1;
                                    break;

                                case 3:
                                    deleteProduct(pflag, product_Name, product_Quantity, product_Price, currentProductCount, categories, currentCategoryCount);
                                    Sleep(1500);
                                    break;

                                case 4:
                                    updateProduct(product_Name, product_Quantity, product_Price, categories, currentProductCount, pflag, currentCategoryCount, productCategoryIndex, product_Expiry);
                                    Sleep(1500);
                                    break;

                                case 5:

                                    updateStockAndCheckThreshold(product_Name, product_Quantity, product_Price, currentProductCount, pflag, threshold);
                                    cout << "\n\n\t     Press any key to continue";
                                    getch();
                                    Sleep(500);
                                    break;

                                default:
                                    inValid();
                                    break;
                                }
                            }
                        } while (input != 0);
                        input = -1;
                        break;

                    case 2: ////////////////////////////////USER_HUB
                        do
                        {
                            input = getUserHubInput();
                            if (input == 0)
                            {
                                logout();
                                break;
                            }
                            else
                            {
                                switch (input)
                                {
                                case 1:
                                    displayAllUserInfo(uflag, user_Purchase, username, passwords, email, currentUserCount);
                                    cout << "\n\n\t     Press any key to continue ";
                                    getch();
                                    Sleep(1500);
                                    break;

                                case 2:
                                    displayAllFeedbacks(username, feedback, email, uflag, currentUserCount);
                                    cout << "\n\n\t     Press any key to continue ";
                                    getch();
                                    Sleep(1500);
                                    break;

                                case 3:
                                    generateDetailedUserReport(uflag, username, email, feedback, user_Purchase, userProductIndices, userProductQuantities, purchaseCounts, product_Name, product_Price, currentUserCount, currentProductCount);
                                    cout << "\n\n\t     Press any key to continue ";
                                    getch();
                                    Sleep(1500);
                                    break;

                                default:
                                    inValid();
                                    break;
                                }
                            }

                        } while (input != 0);
                        input = -1;
                        break;

                    default:
                        inValid();
                        break;
                    }
                }
            } while (input != 0);
            input = -1;
        }

        else if (loggedInUserType == 2) ///////////////////////////////////////////////////////////////////////// User
        {
            do
            {
                input = getUserInput(username, currentUser);
                if (input == 0)
                {
                    logout();
                    break;
                }
                else
                {

                    switch (input)
                    {
                    case 1:
                        displayAllProducts(product_Name, product_Quantity, product_Price, currentProductCount, categories, pflag, currentCategoryCount, productCategoryIndex, product_Expiry);
                        cout << "\n\n\t     Press any key to continue  ";
                        getch();
                        Sleep(500);
                        break;

                    case 2:

                        do
                        {
                            input = getPlaceOrderInput();
                            if (input == 0)
                            {
                                logout();
                                break;
                            }
                            else
                            {
                                switch (input)
                                {
                                case 1:
                                    purchaseProduct(uflag, user_Purchase, userProductIndices, userProductQuantities, purchaseCounts, product_Name, product_Quantity, product_Price, categories, productCategoryIndex, currentProductCount, currentCategoryCount, currentUser);
                                    input = -1;
                                    Sleep(1500);
                                    break;

                                case 2:
                                    trackUserPurchases(currentUser, user_Purchase, username, product_Name, product_Price, userProductQuantities, currentProductCount);
                                    cout << "\n\n\t     Press any key to continue ";
                                    getch();
                                    Sleep(1500);
                                    break;

                                case 3:
                                    removePurchasedItem(userProductIndices, userProductQuantities, purchaseCounts, product_Quantity, product_Name, currentUser, user_Purchase, username, product_Price, currentProductCount);
                                    Sleep(1500);
                                    break;

                                case 4:
                                    editPurchaseQuantity(userProductIndices, userProductQuantities, purchaseCounts, product_Quantity, product_Name, currentUser, user_Purchase, username, product_Price, currentProductCount);
                                    Sleep(1500);
                                    break;

                                default:
                                    inValid();
                                    break;
                                }
                            }

                        } while (input != 0);
                        input = -1;
                        break;
                    case 3:

                        deleteUser(uflag, user_Purchase, feedback, username, email, passwords, currentUserCount);
                        systemExit(uflag, username, passwords, user_Purchase, feedback, pflag, product_Name, product_Quantity, product_Price, productCategoryIndex, categories, usize, psize, userProductIndices, userProductQuantities, purchaseCounts, email, product_Expiry);
                        cout << "data saved";
                        getch();
                        return 0;

                        break;

                    case 4:
                        provideFeedback(uflag, username, feedback, currentUserCount);
                        break;

                    default:
                        inValid();
                        break;
                    }
                }
            } while (true);
            input = -1;
        }

        else if (loggedInUserType == -1)
        {
            SetConsoleTextAttribute(hConsole, 4);
            cout << "\n\t\t\t\t   You have entered invalid Credentials!!!....... ";
            SetConsoleTextAttribute(hConsole, 15);
            Sleep(1500);
            system("cls");
        }
    } while (true);

    return 0;
}
////////////////////////////////////////////////////////////////////////////

////// FUNCS DEFINATIONS //////F
// Function to initialize user-related arrays
void initializeUserArrays(bool uflag[], bool user_Purchase[], string username[], string passwords[], string email[], string feedback[], int usize)
{
    for (int i = 0; i < usize; i++)
    {
        uflag[i] = false;         // No users initially
        user_Purchase[i] = false; // No user has made a purchase yet
        username[i] = "";         // Empty usernames initially
        passwords[i] = "";        // Empty passwords initially
        email[i] = "";            // Empty emails initially
        feedback[i] = "";         // Empty feedback initially
    }
}

// Function to initialize product-related arrays
void initializeProductArrays(bool pflag[], string product_Name[], int product_Quantity[], double product_Price[], string product_Expiry[], int psize)
{
    for (int i = 0; i < psize; i++)
    {
        pflag[i] = false;        // No products initially
        product_Name[i] = "";    // Empty product names initially
        product_Quantity[i] = 0; // No product quantities initially
        product_Price[i] = 0.0;  // Price is zero initially
        product_Expiry[i] = "";  // Empty expiry dates initially
    }
}

// Function to load data
void loadDataFromCSV(
    bool uflag[], string username[], string passwords[], bool user_Purchase[],
    string feedback[], bool pflag[], string product_Name[], int product_Quantity[],
    double product_Price[], int productCategoryIndex[], string categories[],
    int usize, int psize, int &currentUserCount, int &currentProductCount,
    int &currentCategoryCount, int userProductIndices[][15], int userProductQuantities[][15], int purchaseCounts[],
    string email[], string product_Expiry[])
{
    // Open files for reading
    ifstream userFile("users.csv"), productFile("products.csv");

    // Validate file opening
    if (!userFile.is_open())
    {
        cerr << "Error: Unable to open 'users.csv' for reading!" << endl;
        return;
    }
    if (!productFile.is_open())
    {
        cerr << "Error: Unable to open 'products.csv' for reading!" << endl;
        return;
    }

    // Initialize counts
    currentUserCount = currentProductCount = currentCategoryCount = 0;

    // Load user data
    string line, token;
    getline(userFile, line); // Skip header
    while (getline(userFile, line) && currentUserCount < usize)
    {
        stringstream ss(line);
        getline(ss, username[currentUserCount], ',');
        getline(ss, email[currentUserCount], ','); // Added email
        getline(ss, passwords[currentUserCount], ',');
        getline(ss, token, ',');
        user_Purchase[currentUserCount] = (token == "1");
        getline(ss, feedback[currentUserCount], ',');

        // Parse purchases
        string purchaseData;
        getline(ss, purchaseData);
        stringstream purchaseStream(purchaseData);
        string purchaseEntry;
        purchaseCounts[currentUserCount] = 0;

        while (getline(purchaseStream, purchaseEntry, ';') && purchaseCounts[currentUserCount] < 15)
        {
            size_t colonPos = purchaseEntry.find(':');
            if (colonPos != string::npos)
            {
                string productName = purchaseEntry.substr(0, colonPos);
                int quantity = stoi(purchaseEntry.substr(colonPos + 1));

                // Find product index
                for (int i = 0; i < psize; ++i)
                {
                    if (pflag[i] && product_Name[i] == productName)
                    {
                        userProductIndices[currentUserCount][purchaseCounts[currentUserCount]] = i;
                        userProductQuantities[currentUserCount][purchaseCounts[currentUserCount]] = quantity;
                        purchaseCounts[currentUserCount]++;
                        break;
                    }
                }
            }
        }
        uflag[currentUserCount] = true;
        currentUserCount++;
    }

    // Load product data
    getline(productFile, line); // Skip header
    while (getline(productFile, line) && currentProductCount < psize)
    {
        stringstream ss(line);
        string category;
        getline(ss, product_Name[currentProductCount], ',');
        getline(ss, token, ',');
        product_Quantity[currentProductCount] = stoi(token);
        getline(ss, token, ',');
        product_Price[currentProductCount] = stod(token);
        getline(ss, category, ',');
        getline(ss, product_Expiry[currentProductCount], ','); // Load Expiry Date

        // Check or add category
        int categoryIndex = -1;
        for (int i = 0; i < currentCategoryCount; ++i)
        {
            if (categories[i] == category)
            {
                categoryIndex = i;
                break;
            }
        }
        if (categoryIndex == -1 && currentCategoryCount < 10)
        {
            categories[currentCategoryCount] = category;
            categoryIndex = currentCategoryCount++;
        }
        productCategoryIndex[currentProductCount] = categoryIndex;

        pflag[currentProductCount] = true;
        currentProductCount++;
    }

    // Close files
    userFile.close();
    productFile.close();
    cout << "Data successfully loaded from CSV files." << endl;
}

void saveDataToCSV(
    bool uflag[], string username[], string passwords[], bool user_Purchase[],
    string feedback[], bool pflag[], string product_Name[], int product_Quantity[],
    double product_Price[], int productCategoryIndex[], string categories[],
    int usize, int psize, int userProductIndices[][15], int userProductQuantities[][15], int purchaseCounts[],
    string email[], string product_Expiry[])
{
    // Open files for writing
    ofstream userFile("users.csv"), productFile("products.csv");

    // Validate file opening
    if (!userFile.is_open())
    {
        cerr << "Error: Unable to open 'users.csv' for writing!" << endl;
        return;
    }
    if (!productFile.is_open())
    {
        cerr << "Error: Unable to open 'products.csv' for writing!" << endl;
        return;
    }

    // Write user data
    userFile << "Username,Email,Password,PurchaseStatus,Feedback,Purchases\n";
    for (int i = 0; i < usize; ++i)
    {
        if (uflag[i])
        {
            userFile << username[i] << ","
                     << email[i] << "," // Added email
                     << passwords[i] << ","
                     << (user_Purchase[i] ? "1" : "0") << ","
                     << (feedback[i].empty() ? "No feedback" : feedback[i]) << ",";

            // Write purchases
            for (int j = 0; j < purchaseCounts[i]; ++j)
            {
                userFile << product_Name[userProductIndices[i][j]] << ":"
                         << userProductQuantities[i][j];
                if (j < purchaseCounts[i] - 1)
                    userFile << ";"; // Delimiter for multiple purchases
            }
            userFile << "\n";
        }
    }

    // Write product data
    productFile << "ProductName,Quantity,Price,Category,ExpiryDate,TotalWorth\n";
    for (int i = 0; i < psize; ++i)
    {
        if (pflag[i])
        {
            double totalWorth = product_Quantity[i] * product_Price[i];
            productFile << product_Name[i] << ","
                        << product_Quantity[i] << ","
                        << fixed << setprecision(2) << product_Price[i] << ","
                        << categories[productCategoryIndex[i]] << ","
                        << product_Expiry[i] << ","
                        << totalWorth << "\n";
        }
    }

    // Close files
    userFile.close();
    productFile.close();
    cout << "Data successfully saved to CSV files." << endl;
}

void setCursorPosition(int x, int y)
{
    COORD position = {static_cast<SHORT>(x), static_cast<SHORT>(y)};
    SetConsoleCursorPosition(hConsole, position);
}

// Function to display ASCII art centered in the console using raw literals
void displayHeader()
{
    system("cls");
    const int consoleWidth = 80;
    const int consoleHeight = 10;
    SetConsoleTextAttribute(hConsole, 1);

    // Calculate the starting position to center the art
    int startY = (consoleHeight - 7) / 2; // We have 7 lines in the ASCII art
    int startX = (consoleWidth - 75) / 2; // Adjust the width to the ASCII art width

    // Set the cursor to the calculated position and print the ASCII art
    setCursorPosition(startX, startY);
    // cout << "\t\t\t  **************************************************************************************************************************************************\n";
    cout << "\t\t" << R"(
                                                             ███████╗  ████████╗   ██████╗    ██████╗  ██╗  ██╗  ██╗  ███████╗  ██╗   ██╗
                                                             ██╔════╝  ╚══██╔══╝  ██╔═══██╗  ██╔════╝  ██║ ██╔╝  ██║  ██╔════╝  ╚██╗ ██╔╝
                                                             ███████╗     ██║     ██║   ██║  ██║       █████╔╝   ██║  █████╗     ╚████╔╝ 
                                                             ╚════██║     ██║     ██║   ██║  ██║       ██╔═██╗   ██║  ██╔══╝      ╚██╔╝  
                                                             ███████║     ██║     ╚██████╔╝  ╚██████╗  ██║  ██╗  ██║  ██║          ██║   
                                                             ╚══════╝     ╚═╝      ╚═════╝    ╚═════╝  ╚═╝  ╚═╝  ╚═╝  ╚═╝          ╚═╝   
    )" << endl;
    // cout << "\t\t\t  **************************************************************************************************************************************************\n";
    SetConsoleTextAttribute(hConsole, 15);
}

// For Admin
void displayAdmin()
{
    displayHeader();
    SetConsoleTextAttribute(hConsole, 6);
    cout << "\t\t\b\b**************************************************************************************************************************************************************************\n";
    cout << "\t\t\t\t\t\t\t\t                           A D M I N\n";
    cout << "\t\t\b\b**************************************************************************************************************************************************************************\n\n";
    SetConsoleTextAttribute(hConsole, 15);
}

// For User
void displayUser()
{
    displayHeader();
    SetConsoleTextAttribute(hConsole, 6);
    cout << "\t\t\b\b**************************************************************************************************************************************************************************\n";
    cout << "\t\t\t\t\t\t\t\t                            U S E R S   \n";
    cout << "\t\t\b\b**************************************************************************************************************************************************************************\n\n";
    SetConsoleTextAttribute(hConsole, 15);
}

// For Program Exit
void systemExit(bool uflag[], string username[], string passwords[], bool user_Purchase[], string feedback[], bool pflag[], string product_Name[], int product_Quantity[], double product_Price[], int productCategoryIndex[], string categories[], int usize, int psize, int userProductIndices[][15], int userProductQuantities[][15], int purchaseCounts[], string email[], string product_Expiry[])
{
    SetConsoleTextAttribute(hConsole, 2);
    cout << "\n\t\t\t\t\t     Exiting the system....... ";
    Sleep(1000);
    cout << "\n\t\t\t\t\t\t     Saving Data....... ";
    SetConsoleTextAttribute(hConsole, 15);
    Sleep(700);
    // Save data before exiting
    saveDataToCSV(uflag, username, passwords, user_Purchase, feedback, pflag, product_Name, product_Quantity, product_Price, productCategoryIndex, categories, usize, psize, userProductIndices, userProductQuantities, purchaseCounts, email, product_Expiry);

    displayHeader();
    SetConsoleTextAttribute(hConsole, 5);
    cout << "\t\t\t\t\t\t\t\t   *************************************************************\n";
    cout << "\t\t\t\t\t\t\t\t     T H A N K S    F O R    U S I N G    T H E    S Y S T E M \n";
    cout << "\t\t\t\t\t\t\t\t   *************************************************************\n\n";
    SetConsoleTextAttribute(hConsole, 15);
    cout << "\n";
    Sleep(2500);
    system("cls");
}

// For Default Cases
void inValid()
{
    if (cin.fail())
    {
        cin.clear();
        cin.ignore(70, '\n');
        SetConsoleTextAttribute(hConsole, 4);
        cout << "\n\t\t\t\t     Invalid Argument!! Try Again.......... ";
        Sleep(1500);
        SetConsoleTextAttribute(hConsole, 15);
    }
    else
    {
        SetConsoleTextAttribute(hConsole, 4);
        cout << "\n\t\t\t\t     Invalid Input!! Try Again.......... ";
        Sleep(1500);
        SetConsoleTextAttribute(hConsole, 15);
    }
}

// For Returning to previous Menu
void logout()
{
    SetConsoleTextAttribute(hConsole, 2);
    cout << "\n\t\t\t\t\t     Returning...... ";
    SetConsoleTextAttribute(hConsole, 15);
    Sleep(1000);
    system("cls");
}

// For User - Verification
void userAuthentication(string email[], int &loggedInUserType, string username[], string passwords[], int usize, int &currentUser)
{
    string user, password, inputEmail;
    bool userSearchFlag = false;

    SetConsoleTextAttribute(hConsole, 2);
    cout << "\n\t\t\t\t     You Choose to Log in as User....... ";
    SetConsoleTextAttribute(hConsole, 15);
    Sleep(700);
    displayHeader();
    SetConsoleTextAttribute(hConsole, 6);
    cout << "\t\t\b\b**************************************************************************************************************************************************************************\n";
    cout << "\t\t\t\t\t\t\t                      ENTER THE U S E R CREDENTIALS \n";
    cout << "\t\t\b\b**************************************************************************************************************************************************************************\n\n";
    SetConsoleTextAttribute(hConsole, 15);

    // Input username
    cout << "\n\t\t\b\bEnter  the  username  :  ";
    cin >> user;

    password = getPasswordInput();

    // Input email
    cout << "\n\n\t\t\b\bEnter  the  email     :  ";
    cin >> inputEmail;

    // Search user in the list
    for (int i = 0; i < usize; i++)
    {
        if (username[i] == user && passwords[i] == password && email[i] == inputEmail)
        {
            loggedInUserType = 2; // Standard User logged in
            currentUser = i;
            userSearchFlag = true;
            break;
        }
    }

    // If user found
    if (userSearchFlag)
    {
        loggedInUserType = 2;
        SetConsoleTextAttribute(hConsole, 2);
        cout << "\n\n\t\t\t\t     Logging in as User....... ";
        SetConsoleTextAttribute(hConsole, 15);
        Sleep(700);
    }
    // If invalid credentials
    else
    {
        loggedInUserType = -1;
    }
}

// For Admin-Authentication
void adminAuthentication(int &loggedInUserType)
{
    string user, password;
    SetConsoleTextAttribute(hConsole, 2);
    cout << "\n\t\t\t\t     You Choose to Log in as Admin....... ";
    SetConsoleTextAttribute(hConsole, 15);
    Sleep(700);
    displayHeader();
    SetConsoleTextAttribute(hConsole, 6);
    cout << "\t\t\b\b**************************************************************************************************************************************************************************\n";
    cout << "\t\t\t\t\t\t\t                      ENTER THE ADMINISTRATION CREDENTIALS \n";
    cout << "\t\t\b\b**************************************************************************************************************************************************************************\n\n";
    SetConsoleTextAttribute(hConsole, 15);

    cout << "\n\t\t\b\bEnter  the  username  :  ";
    cin >> user;

    password = getPasswordInput();

    if (user == "Admin" && password == "123")
    {
        SetConsoleTextAttribute(hConsole, 2);
        cout << "\n\n\t\t\t\t     Logging in as Admin....... ";
        SetConsoleTextAttribute(hConsole, 15);
        Sleep(700);
        loggedInUserType = 1;
    }
    else
        loggedInUserType = -1;
}

// Function to add a product category
void addProductCategory(string categories[], int &currentCategoryCount, int csize)
{
    SetConsoleTextAttribute(hConsole, 2);
    cout << "\n\t     Processing.......";
    SetConsoleTextAttribute(hConsole, 15);
    if (currentCategoryCount >= csize)
    {
        SetConsoleTextAttribute(hConsole, 4);
        cout << "\n\n\t\t\t\t\t\tMaximum Category reached!!!..........\n ";
        SetConsoleTextAttribute(hConsole, 15);
        return;
    }

    string newCategory;
    cout << "\n\t     Enter the name of the new category : ";
    cin.ignore(); // To handle the newline character left by previous inputs
    getline(cin, newCategory);

    // Check if the category already exists
    for (int i = 0; i < currentCategoryCount; i++)
    {
        if (categories[i] == newCategory)
        {
            SetConsoleTextAttribute(hConsole, 4);
            cout << "\n\t\t\t\t\t\t  Category Already Exist!!.......... ";
            SetConsoleTextAttribute(hConsole, 15);
            return;
        }
    }
    // Add the new category
    categories[currentCategoryCount] = newCategory;
    currentCategoryCount++;
    SetConsoleTextAttribute(hConsole, 2);
    cout << "\n\n\t\t\t\t\t Category '" << newCategory << "' Added Successfully." << endl;
    SetConsoleTextAttribute(hConsole, 15);
}

// Function to display available categories
void displayCategories(string categories[], int currentCategoryCount)
{
    if (currentCategoryCount == 0)
    {

        SetConsoleTextAttribute(hConsole, 4);
        cout << "\n\n\t\t\t\t\t\t\t\t   No Categories Available!!..........\n ";
        SetConsoleTextAttribute(hConsole, 15);
        return;
    }
    cout << "\n";
    SetConsoleTextAttribute(hConsole, 13);
    cout << "\t\t\t\t\t\t\t\t   //////////  C U R R E N T   C A T E G O R I E S  ///////////  \n\n";
    SetConsoleTextAttribute(hConsole, 15);
    for (int i = 0; i < currentCategoryCount; i++)
    {
        cout << "\t" << right << setw(74) << "  " << i + 1 << ".      " << left << setw(20) << categories[i] << "\n\n";
        Sleep(70);
    }
}

bool isValidExpiryDate(const string &expiryDate)
{
    // Ensure the expiry date is in the correct format (YYYY-MM-DD)
    if (expiryDate.size() != 10 || expiryDate[4] != '-' || expiryDate[7] != '-')
    {
        return false;
    }

    // Check if year, month, and day are numeric
    for (int i = 0; i < expiryDate.size(); i++)
    {
        if (i == 4 || i == 7)
            continue; // Skip the '-' characters
        if (!isdigit(expiryDate[i]))
        {
            return false;
        }
    }

    // Extract year, month, and day
    int year = stoi(expiryDate.substr(0, 4));
    int month = stoi(expiryDate.substr(5, 2));
    int day = stoi(expiryDate.substr(8, 2));

    // Check if the year, month, and day are in valid ranges
    if (month < 1 || month > 12)
    {
        return false;
    }

    // Days in each month (for simplicity, assume no leap years)
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (day < 1 || day > daysInMonth[month - 1])
    {
        return false;
    }

    return true;
}

void addProduct(bool pflag[], string product_Name[], int product_Quantity[], double product_Price[], int &currentProductCount, int psize, string categories[], int currentCategoryCount, int productCategoryIndex[], string product_Expiry[])
{
    SetConsoleTextAttribute(hConsole, 2);
    cout << "\n\t     Processing.......";
    SetConsoleTextAttribute(hConsole, 15);
    if (currentProductCount >= psize)
    { // Assuming psize is the maximum product limit
        SetConsoleTextAttribute(hConsole, 4);
        cout << "\n\n\t\t\t\t\t\t Maximum Product Capacity reached!!!..........\n ";
        SetConsoleTextAttribute(hConsole, 15);
        return;
    }

    int categoryIndex;
    string newProductName;
    int quantity;
    double price;
    string expiryDate;

    // Validate category selection
    do
    {
        cout << "\n\t     Enter the category number/ID to add a product : ";
        cin >> categoryIndex;
        if (cin.fail() || categoryIndex < 1 || categoryIndex > currentCategoryCount)
        {
            SetConsoleTextAttribute(hConsole, 4);
            cout << "\n\n\t\t\t\t\t\t Invalid Category Selection!!!..........\n ";
            Sleep(600);
            SetConsoleTextAttribute(hConsole, 15);
            cin.clear();
            cin.ignore(70, '\n');
        }
        else
        {
            break;
        }
    } while (true);

    cin.ignore(); // Clear newline left in the buffer

    // Get product name
    cout << "\t     Enter the product name                       : ";
    getline(cin, newProductName);

    // Validate product quantity
    do
    {
        cout << "\t     Enter the product quantity                   : ";
        cin >> quantity;
        if (cin.fail() || quantity <= 0)
        {
            SetConsoleTextAttribute(hConsole, 4);
            cout << "\n\n\t\t\t\t\t\t    Invalid Quantities..........\n ";
            Sleep(600);
            SetConsoleTextAttribute(hConsole, 15);
            cin.clear();
            cin.ignore(70, '\n');
        }
        else
        {
            break;
        }
    } while (true);

    cin.ignore();

    // Validate product price
    do
    {
        cout << "\t     Enter the product price                      : ";
        cin >> price;
        if (cin.fail() || price <= 0.0)
        {
            SetConsoleTextAttribute(hConsole, 4);
            cout << "\n\n\t\t\t\t\t\t     Invalid Product Price!!!..........\n ";
            Sleep(600);
            SetConsoleTextAttribute(hConsole, 15);
            cin.clear();
            cin.ignore(70, '\n');
        }
        else
        {
            break;
        }
    } while (true);

    // Ask for expiry date or use default if the user chooses not to provide one
    cout << "\t     Does this product have an expiry date? (y/n) : ";
    char hasExpiry;
    cin >> hasExpiry;

    if (tolower(hasExpiry) == 'y')
    {
        cout << "\t     Enter the expiry date (YYYY-MM-DD)           : ";
        cin >> expiryDate;

        // Validate expiry date
        while (!isValidExpiryDate(expiryDate))
        {
            SetConsoleTextAttribute(hConsole, 4);
            cout << "\n\n\t\t\t\t\t\t    Invalid Expiry Date! Please enter a valid date (YYYY-MM-DD)!!!.......\n ";
            Sleep(600);
            SetConsoleTextAttribute(hConsole, 15);
            cout << "\t     Enter the expiry date (YYYY-MM-DD)           : ";
            cin >> expiryDate;
        }
    }
    else
    {
        // If no expiry date, use default value
        expiryDate = "YYYY-MM-DD";
    }

    // Check if the product already exists
    for (int i = 0; i < currentProductCount; i++)
    {
        if (pflag[i] && product_Name[i] == newProductName)
        {
            SetConsoleTextAttribute(hConsole, 4);
            cout << "\n\n\t\t\t\t\t\t  Product Already Exists!!!..........\n ";
            Sleep(600);
            SetConsoleTextAttribute(hConsole, 15);
            return;
        }
    }

    // Add the new product
    product_Name[currentProductCount] = newProductName;
    product_Quantity[currentProductCount] = quantity;
    product_Price[currentProductCount] = price;
    product_Expiry[currentProductCount] = expiryDate;
    pflag[currentProductCount] = true;
    productCategoryIndex[currentProductCount] = categoryIndex - 1; // Store the category index (0-based)
    currentProductCount++;

    SetConsoleTextAttribute(hConsole, 2);
    cout << "\n\n\t\t\t\t\t Product '" << newProductName << "' Added Successfully Under The Category '" << categories[categoryIndex - 1] << "'." << endl;
    SetConsoleTextAttribute(hConsole, 15);
}

void displayAllProducts(string product_Name[], int product_Quantity[], double product_Price[], int currentProductCount, string categories[], bool pflag[], int currentCategoryCount, int productCategoryIndex[], string product_Expiry[])
{
    if (currentProductCount == 0)
    {
        SetConsoleTextAttribute(hConsole, 4);
        cout << "\n\n\t\t\t       No Products Currently Available!!!..........\n ";
        Sleep(600);
        SetConsoleTextAttribute(hConsole, 15);
        return;
    }

    cout << "\n\n";
    SetConsoleTextAttribute(hConsole, 5);
    cout << "\t\t\t\t\t\t\t\t\t              I N V E N T O R Y  \n";
    cout << "\t\t\t\t\t\t\t\t  ____________________________________________________________\n\n";
    SetConsoleTextAttribute(hConsole, 15);
    SetConsoleTextAttribute(hConsole, 13);
    cout << "\t\t\b\b\b==========================================================================================================================================================================\n";
    cout << "\t\t  " << "Sr.      " << left << setw(30) << "PRODUCT " << left << setw(30) << "QUANTITY " << left << setw(30) << "PRICE " << left << setw(30) << "CATEGORY" << left << setw(15) << "EXPIRY DATE\n";
    cout << "\t\t\b\b\b==========================================================================================================================================================================\n";
    SetConsoleTextAttribute(hConsole, 15);

    // Loop through each product and display its details
    for (int i = 0; i < currentProductCount; i++)
    {
        if (pflag[i]) // Check if the product is flagged as available
        {
            Sleep(70);
            // Get the category index of the current product
            int categoryIndex = productCategoryIndex[i];
            if (categoryIndex >= 0 && categoryIndex < currentCategoryCount)
            {
                // Display product information
                cout << "\n\t\t  " << i + 1 << ".       " << left << setw(30) << product_Name[i]
                     << left << setw(30) << product_Quantity[i]
                     << left << setw(2) << "$" << fixed << setprecision(2) << left << setw(28) << product_Price[i]
                     << left << setw(30) << categories[categoryIndex] << left << setw(15) << product_Expiry[i];

                // Highlight the expiry date in color if the product is close to expiration
                string expiryDate = product_Expiry[i];
                // Assuming the expiry date is in the format YYYY-MM-DD, you can compare it to specified date

                if (isNearExpiry(expiryDate))
                {
                    SetConsoleTextAttribute(hConsole, 4);
                    cout << "  ---> Expiring soon!";
                    SetConsoleTextAttribute(hConsole, 15);
                }
                cout << "\n";
            }
        }
    }
}

// Helper function to check if the product is nearing expiry
bool isNearExpiry(string expiryDate)
{
    // Simplified check for expiry
    string currentDate = "2024-12-05";  // Replace with actual current date
    return (expiryDate <= currentDate); // Assume if expiry date is today or earlier, it's near expiry
}

// Function to remove product
void deleteProduct(bool pflag[], string product_Name[], int product_Quantity[], double product_Price[], int &currentProductCount, string categories[], int currentCategoryCount)
{

    SetConsoleTextAttribute(hConsole, 5);
    cout << "\n\n\n\t\t\t\t\t\t\t\t\t               D E L E T I N G   \n";
    cout << "\t\t\t\t\t\t\t\t  ____________________________________________________________\n";
    SetConsoleTextAttribute(hConsole, 15);
    displayCategories(categories, currentCategoryCount);
    cout << endl
         << endl;

    string productToDelete;
    int categoryIndex;

    // Validate category index input
    do
    {
        cout << "\t     Enter the category number to delete a product : ";
        cin >> categoryIndex;
        if (cin.fail() || categoryIndex < 1 || categoryIndex > currentCategoryCount)
        {
            SetConsoleTextAttribute(hConsole, 4);
            cout << "\n\n\t\t\t\t\t\t     Invalid Category ID!!!..........\n ";
            SetConsoleTextAttribute(hConsole, 15);
            cin.clear();
            cin.ignore(70, '\n');
        }
        else
        {
            break;
        }
    } while (true);

    cin.ignore();

    cout << "\t     Enter the name of the product to delete       : ";
    getline(cin, productToDelete);

    // Check if the product exists and delete it
    bool productFound = false;
    for (int i = 0; i < currentProductCount; i++)
    {
        if (pflag[i] && product_Name[i] == productToDelete)
        {
            cout << "\t     Product '" << productToDelete << "' found under category '" << categories[categoryIndex - 1] << "'.\n";
            SetConsoleTextAttribute(hConsole, 2);
            cout << "\n\t\t\t\t\t\t        Deleting...... ";
            SetConsoleTextAttribute(hConsole, 15);
            Sleep(700);

            pflag[i] = false;
            product_Name[i] = "";
            product_Quantity[i] = 0;
            product_Price[i] = 0.0;
            productFound = true;
            SetConsoleTextAttribute(hConsole, 2);
            cout << "\n\n\t\t\t\t\t\t          Product '" << productToDelete << "' deleted successfully." << endl;
            SetConsoleTextAttribute(hConsole, 15);
            break;
        }
    }

    if (!productFound)
    {
        SetConsoleTextAttribute(hConsole, 4);
        cout << "\n\t\t\t\t\t\t     Product Not Found!!...... \n";
        SetConsoleTextAttribute(hConsole, 15);
    }
}

// Function For Stock
void updateStockAndCheckThreshold(string product_Name[], int product_Quantity[], double product_Price[], int currentProductCount, bool pflag[], int threshold)
{
    bool lowStockFlag = false;

    SetConsoleTextAttribute(hConsole, 5);
    cout << "\n\n\n\t\t\t\t\t\t\t\t\t             S T O C K   L E V E L S   \n";
    cout << "\t\t\t\t\t\t\t\t  ____________________________________________________________\n\n";
    SetConsoleTextAttribute(hConsole, 15);
    SetConsoleTextAttribute(hConsole, 13);
    cout << "\t\t\b\b\b==========================================================================================================================================================================\n";
    cout << "\t\t  " << "Sr.      " << left << setw(40) << "PRODUCT " << left << setw(40) << "QUANTITY " << left << setw(40) << "PRICE " << left << setw(5) << "STATUS\n";
    cout << "\t\t\b\b\b==========================================================================================================================================================================\n";
    SetConsoleTextAttribute(hConsole, 15);
    // Loop through all products and check their quantity
    for (int i = 0; i < currentProductCount; i++)
    {
        if (pflag[i])
        {
            if (product_Quantity[i] < threshold) // Check if the product is flagged as available)
            {
                cout << "\n\t\t  " << i + 1 << ".       " << left << setw(40) << product_Name[i] << left << setw(40) << product_Quantity[i] << left << setw(2) << "$" << fixed << setprecision(2) << left << setw(38) << product_Price[i];
                SetConsoleTextAttribute(hConsole, 4);
                cout << "\b\b-----> Warning: Stock is below threshold! \n";
                SetConsoleTextAttribute(hConsole, 15);
                lowStockFlag = true;
            }
            else
            {
                cout << "\n\t\t  " << i + 1 << ".       " << left << setw(40) << product_Name[i] << left << setw(40) << product_Quantity[i] << left << setw(2) << "$" << fixed << setprecision(2) << left << setw(43) << product_Price[i];
                cout << "\b\b\b\b\b---------" << endl;
            }
        }
    }

    if (!lowStockFlag)
    {
        SetConsoleTextAttribute(hConsole, 2);
        cout << "\n\n\t\t\t\t\t\t\t\t\t\t\b\bAll products are sufficiently stocked.\n";
        SetConsoleTextAttribute(hConsole, 15);
    }
}

// Function to update product
void updateProduct(string product_Name[], int product_Quantity[], double product_Price[], string categories[], int currentProductCount, bool pflag[], int currentCategoryCount, int productCategoryIndex[], string product_Expiry[])
{
    int input;
    do
    {
        displayAdmin();
        int productIndex;
        SetConsoleTextAttribute(hConsole, 5);
        cout << "\n\t\t\t\t\t\t\t\t\t                 U P D A T I G     \n";
        cout << "\t\t\t\t\t\t\t\t  ____________________________________________________________\n";
        SetConsoleTextAttribute(hConsole, 15);
        displayAllProducts(product_Name, product_Quantity, product_Price, currentProductCount, categories, pflag, currentCategoryCount, productCategoryIndex, product_Expiry);

        // Prompt the user to select the product they want to update
        cout << "\n\n\t     Enter the product number to update : ";
        cin >> productIndex;

        if (productIndex < 1 || productIndex > currentProductCount || !pflag[productIndex - 1])
        {
            SetConsoleTextAttribute(hConsole, 4);
            cout << "\n\n\t\t\t\tInvalid Product Selection!!!...... \n";
            SetConsoleTextAttribute(hConsole, 2);
            cout << "\n\t\t\t\t\t     Returning...... ";
            SetConsoleTextAttribute(hConsole, 15);
            cin.clear();
            cin.ignore(100, '\n');
            return;
        }

        SetConsoleTextAttribute(hConsole, 2);
        cout << "\n\t     Processing.......";
        SetConsoleTextAttribute(hConsole, 15);
        // Choose what to update (name, quantity, price, or expiry date)
        cout << "\n\t     What would you like to update?\n\n";
        Sleep(70);
        cout << "\t      1.  Update Product Name\n";
        Sleep(70);
        cout << "\t      2.  Update Product Quantity\n";
        Sleep(70);
        cout << "\t      3.  Update Product Price\n";
        Sleep(70);
        cout << "\t      4.  Update Product Expiry Date\n";
        Sleep(70);
        SetConsoleTextAttribute(hConsole, 14);
        cout << "\t      0.  Return \n\n";
        SetConsoleTextAttribute(hConsole, 15);

        cout << "\t     Enter your input (0-4) : ";
        cin >> input;
        cin.ignore();

        if (cin.fail())
        {
            inValid();
            continue;
        }
        else if (input == 0)
        {
            logout();
            return;
        }

        else
        {
            switch (input)
            {
            case 1:
            {
                string newName;
                cout << "\n\t     Enter the new name for the product : ";
                getline(cin, newName);

                // Update the product name
                product_Name[productIndex - 1] = newName;
                SetConsoleTextAttribute(hConsole, 2);
                cout << "\n\t\t\t\t\t\t     Product name updated successfully to '" << newName << "'.\n";
                SetConsoleTextAttribute(hConsole, 15);
                Sleep(1000);
                break;
            }

            case 2:
            {
                int newQuantity;
                cout << "\n\t     Enter the new quantity for the product : ";
                cin >> newQuantity;

                // Ensure that the quantity is not negative
                if (newQuantity < 0)
                {
                    SetConsoleTextAttribute(hConsole, 4);
                    cout << "\n\t\t\t\t\t\t     Negative Quantities !!!...... \n";
                    SetConsoleTextAttribute(hConsole, 15);
                }

                // Update the product quantity
                product_Quantity[productIndex - 1] = newQuantity;
                SetConsoleTextAttribute(hConsole, 2);
                cout << "\n\n\t\t\t\t\t\t     Product quantity updated successfully to " << newQuantity << ".\n";
                SetConsoleTextAttribute(hConsole, 15);
                Sleep(1000);
                break;
            }

            case 3: // Update Product Price
            {
                double newPrice;
                cout << "\n\t     Enter the new price for the product : $";
                cin >> newPrice;

                // Ensure that the price is a valid positive value
                if (newPrice <= 0)
                {
                    SetConsoleTextAttribute(hConsole, 4);
                    cout << "\n\n\t\t\t\t\t\t     Invalid Price Input !!!...... \n";
                    SetConsoleTextAttribute(hConsole, 15);
                }

                // Update the product price
                product_Price[productIndex - 1] = newPrice;
                SetConsoleTextAttribute(hConsole, 2);
                cout << "\n\t\t\t\t\t\t     Product price updated successfully to $" << fixed << setprecision(2) << newPrice << ".\n";
                SetConsoleTextAttribute(hConsole, 15);
                Sleep(1000);
                break;
            }

            case 4: // Update Product Expiry Date
            {
                string newExpiryDate;
                cout << "\n\t     Enter the new expiry date for the product (YYYY-MM-DD) : ";
                getline(cin, newExpiryDate);

                // Update the product expiry date
                product_Expiry[productIndex - 1] = newExpiryDate;
                SetConsoleTextAttribute(hConsole, 2);
                cout << "\n\n\t\t\t\t\t\t     Product expiry date updated successfully to " << newExpiryDate << ".\n";
                SetConsoleTextAttribute(hConsole, 15);
                Sleep(1000);
                break;
            }

            default:
                SetConsoleTextAttribute(hConsole, 4);
                cout << "\n\n\t\t\t\t\t\t     Invalid Option!!!...... \n";
                SetConsoleTextAttribute(hConsole, 15);
                break;
            }
        }
    } while (true);
}

// Function to display all user information (username, email, password, purchase status) in a formatted table
void displayAllUserInfo(bool uflag[], bool user_Purchase[], string username[], string passwords[], string email[], int currentUserCount)
{
    if (currentUserCount == 0)
    {
        SetConsoleTextAttribute(hConsole, 4);
        cout << "\n\n\t\t\t\t\t\t\b\b\b\b No Users Available!!!...... \n";
        SetConsoleTextAttribute(hConsole, 15);
        return;
    }

    cout << "\n\n";
    SetConsoleTextAttribute(hConsole, 5);
    cout << "\t\t\t\t\t\t\t\t\t        U S E R  I N F O R M A T I O N  \n";
    cout << "\t\t\t\t\t\t\t\t  ____________________________________________________________\n\n";
    SetConsoleTextAttribute(hConsole, 15);
    SetConsoleTextAttribute(hConsole, 13);
    cout << "\t\t\b\b\b==========================================================================================================================================================================\n";
    cout << "\t\t  " << "Sr.      " << left << setw(35) << "USERNAME" << left << setw(35) << "EMAIL" << left << setw(35) << "PASSWORD" << left << setw(17) << "PURCHASE STATUS\n";
    cout << "\t\t\b\b\b==========================================================================================================================================================================\n\n";
    SetConsoleTextAttribute(hConsole, 15);

    // Loop through each user and display their details
    // Loop through each user and display their details
    for (int i = 0; i < currentUserCount; i++)
    {
        if (uflag[i])
        { // Check if the user is active
            // Display user information
            cout << "\n\t\t  " << i + 1 << ".       " << left << setw(35) << username[i]
                 << left << setw(35) << email[i] // Display email
                 << left << setw(35) << passwords[i]
                 << left << setw(35) << (user_Purchase[i] ? "Made a purchase" : "No purchases yet") << "\n";
        }
    }
}

// Function to display all feedbacks
void displayAllFeedbacks(string username[], string feedback[], string email[], bool uflag[], int currentUserCount)
{
    if (currentUserCount == 0)
    {
        SetConsoleTextAttribute(hConsole, 4);
        cout << "\n\n\t\t\t\t\t\t No Users Available!!!...... \n";
        SetConsoleTextAttribute(hConsole, 15);
        Sleep(600);
        return;
    }

    cout << "\n\n";
    SetConsoleTextAttribute(hConsole, 5);
    cout << "\t\t\t\t\t\t\t\t\t             F E E D  B A C K S \n";
    cout << "\t\t\t\t\t\t\t\t  ____________________________________________________________\n\n";
    SetConsoleTextAttribute(hConsole, 15);
    SetConsoleTextAttribute(hConsole, 13);
    cout << "\t\t\b\b\b==========================================================================================================================================================================\n";
    cout << "\t\t  " << "Sr.      " << left << setw(30) << "USERNAME" << left << setw(30) << "EMAIL" << left << setw(10) << "FEEDBACK\n";
    cout << "\t\t\b\b\b==========================================================================================================================================================================\n\n";
    SetConsoleTextAttribute(hConsole, 15);

    // Print each user's feedback
    for (int i = 0; i < currentUserCount; i++)
    {
        if (uflag[i])
        { // Only display active users
            cout << "\n\t\t  " << i + 1 << ".       " << left << setw(30) << username[i]
                 << left << setw(30) << email[i]
                 << left << setw(30) << (feedback[i].empty() ? "No feedback provided" : feedback[i]) << "\n\n";
        }
    }
}

// Function to generate detailed user report for all users in the system
void generateDetailedUserReport(bool uflag[], string username[], string email[], string feedback[], bool user_Purchase[], int userProductIndices[][15], int userProductQuantities[][15], int purchaseCounts[], string product_Name[], double product_Price[], int currentUserCount, int currentProductCount)
{
    if (currentUserCount == 0)
    {
        SetConsoleTextAttribute(hConsole, 4);
        cout << "\n\n\t\t\t\t\t\t\b\b\b\b No Users Available!!!...... \n";
        SetConsoleTextAttribute(hConsole, 15);
        return;
    }

    for (int i = 0; i < currentUserCount; i++)
    {
        if (!uflag[i])
            continue; // Skip inactive users

        SetConsoleTextAttribute(hConsole, 13);
        cout << "\n\t\t\t\t\t\t\t\t\t\t  Detailed Report for User: " << username[i];
        cout << "\n\t\t\b\b\b==========================================================================================================================================================================\n";
        SetConsoleTextAttribute(hConsole, 15);

        // Display user information including email and feedback
        cout << "\n\t\t\tEmail                       :             " << email[i];
        cout << "\n\t\t\tFeedback                    :             " << (feedback[i].empty() ? "No feedback provided" : feedback[i]);
        cout << "\n\t\t\tPurchase Status             :             " << (user_Purchase[i] ? "Made Purchases" : "No Purchases Yet");

        cout << "\n\t\t\b\b\b__________________________________________________________________________________________________________________________________________________________________________\n";
        SetConsoleTextAttribute(hConsole, 5);
        cout << "\n\t\t\t\t" << left << setw(40) << "Product Name" << setw(40) << "Quantity"
             << setw(40) << "Price per Unit" << setw(10) << "Total Cost";
        SetConsoleTextAttribute(hConsole, 15);
        cout << "\n\t\t\b\b\b__________________________________________________________________________________________________________________________________________________________________________\n";

        double totalUserCost = 0.0;

        // Loop through the purchases made by the user
        for (int j = 0; j < purchaseCounts[i]; j++)
        {
            int productIndex = userProductIndices[i][j];
            int quantity = userProductQuantities[i][j];
            double cost = quantity * product_Price[productIndex];
            totalUserCost += cost;

            // Display product purchase details
            cout << "\n\t\t\t\t" << left << setw(40) << product_Name[productIndex]
                 << setw(40) << quantity
                 << setw(40) << fixed << setprecision(2) << product_Price[productIndex]
                 << setw(40) << fixed << setprecision(2) << cost;
        }

        if (totalUserCost > 0)
        {
            SetConsoleTextAttribute(hConsole, 2);
            cout << "\n\n\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\ttotal Amount Paid: $" << fixed << setprecision(2) << totalUserCost << "\n\n";
            SetConsoleTextAttribute(hConsole, 15);
        }
        else
        {
            SetConsoleTextAttribute(hConsole, 4);
            cout << "\n\n\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\tNo Purchases Recorded for This User.\n\n";
            SetConsoleTextAttribute(hConsole, 15);
        }
    }
}

// Function to Sign-up
void addUser(bool uflag[], bool user_Purchase[], string username[], string passwords[], string email[], int &currentUserCount, int usize)
{
    SetConsoleTextAttribute(hConsole, 5);
    cout << "\n\n\n\t\t\t\t\t\t\t\t\t       A C C O U N T    G E N E R A T I O N    \n";
    cout << "\t\t\t\t\t\t\t\t  ____________________________________________________________\n";
    SetConsoleTextAttribute(hConsole, 15);
    if (currentUserCount >= usize)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
        cout << "\n\n\t\t\t\t     Maximum Users Reached!!!...... \n";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
        Sleep(1000);
        return;
    }

    string newUsername, newPassword, newEmail;

    // Take user input for username, password, and email
    cout << "\n\t\t\b\b\b░ Enter your username      : ";
    cin >> newUsername;

    // Check if username already exists
    for (int i = 0; i < currentUserCount; i++)
    {
        if (username[i] == newUsername)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
            cout << "\n\n\t\t\t\t     Username already exists!! \n";
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
            Sleep(1000);
            return;
        }
    }

    cout << "\n\t\t\b\b\b░ Enter your password      : ";
    cin >> newPassword;

    // Take user input for email
    cout << "\n\t\t\b\b\b░ Enter your email address : ";
    cin >> newEmail;

    // Check if email already exists
    for (int i = 0; i < currentUserCount; i++)
    {
        if (email[i] == newEmail)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
            cout << "\n\n\t\t\t\t     Email already exists!!!...... \n";
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
            Sleep(1000);
            return;
        }
    }

    // Add the new user to the arrays
    username[currentUserCount] = newUsername;
    passwords[currentUserCount] = newPassword;
    email[currentUserCount] = newEmail;
    uflag[currentUserCount] = true;
    user_Purchase[currentUserCount] = false; // User hasn't made a purchase yet
    currentUserCount++;

    SetConsoleTextAttribute(hConsole, 2);
    cout << "\n\n\t\t\t\t\t   User '" << newUsername << "' Signed-in Successfully!........" << endl;
    Sleep(1000);
    SetConsoleTextAttribute(hConsole, 15);
}

// Function to PurchaseProduct
void purchaseProduct(bool uflag[], bool user_Purchase[], int userProductIndices[][15], int userProductQuantities[][15], int purchaseCounts[], string product_Name[], int product_Quantity[], double product_Price[], string categories[], int productCategoryIndex[], int currentProductCount, int currentCategoryCount, int currentUser)
{
    SetConsoleTextAttribute(hConsole, 5);
    cout << "\n\n\t\t\t\t\t\t\t\t\t            A D D   T O   C A R T \n";
    cout << "\t\t\t\t\t\t\t\t   ____________________________________________________________\n\n";
    SetConsoleTextAttribute(hConsole, 15);

    if (!uflag[currentUser])
    {
        SetConsoleTextAttribute(hConsole, 4);
        cout << "\n\n\t\t\t\t\t     User Doesn't Exist!!!........ \n";
        SetConsoleTextAttribute(hConsole, 15);
        Sleep(600);
        return;
    }

    if (currentProductCount == 0)
    {
        SetConsoleTextAttribute(hConsole, 4);
        cout << "\n\n\t\t\t\t\t     No Products Available!!!....... \n";
        SetConsoleTextAttribute(hConsole, 15);
        Sleep(600);
        return;
    }

    displayCategories(categories, currentCategoryCount);
    cout << "\n\n\t     Enter the category ID : ";
    int categoryChoice;
    while (!(cin >> categoryChoice) || categoryChoice < 1 || categoryChoice > currentCategoryCount)
    {
        cin.clear();            // Clear the error flag
        cin.ignore(1000, '\n'); // Discard invalid input
        SetConsoleTextAttribute(hConsole, 4);
        cout << "\t\tInvalid category! Please try again : ";
        SetConsoleTextAttribute(hConsole, 15);
    }

    SetConsoleTextAttribute(hConsole, 13);
    cout << "\n\t\t\t\t\t\t\t\t\t\t   Products in " << categories[categoryChoice - 1];
    SetConsoleTextAttribute(hConsole, 13);
    cout << "\n\t\t\b\b\b==========================================================================================================================================================================\n";
    cout << "\t\t  " << "Sr.      " << left << setw(60) << "Product" << left << setw(60) << "Quantity" << left << setw(10) << "Price\n";
    cout << "\t\t\b\b\b==========================================================================================================================================================================\n\n";
    SetConsoleTextAttribute(hConsole, 15);

    bool foundProducts = false;
    for (int i = 0; i < currentProductCount; i++)
    {
        if (productCategoryIndex[i] == categoryChoice - 1)
        {
            cout << "\t\t   " << i + 1 << ".       " << left << setw(60) << product_Name[i]
                 << left << setw(60) << product_Quantity[i]
                 << left << setw(30) << product_Price[i] << "\n ";
            Sleep(100);
            foundProducts = true;
        }
    }

    if (!foundProducts)
    {
        SetConsoleTextAttribute(hConsole, 4);
        cout << "\n\n\t\t\t\t\t     No Products in Selected Category!!!....... \n\n";
        Sleep(600);
        SetConsoleTextAttribute(hConsole, 15);
        return;
    }

    cout << "\n\n\t     Enter the product number    : ";
    int productChoice;
    while (!(cin >> productChoice) || productChoice < 1 || productChoice > currentProductCount || productCategoryIndex[productChoice - 1] != categoryChoice - 1)
    {
        cin.clear();            // Clear the error flag
        cin.ignore(1000, '\n'); // Discard invalid input
        SetConsoleTextAttribute(hConsole, 4);
        cout << "\t\tInvalid product! Please try again : ";
        SetConsoleTextAttribute(hConsole, 15);
    }

    productChoice--; // Adjust for 0-based indexing
    cout << "\t     Enter quantity to purchase  : ";
    int quantity;
    while (!(cin >> quantity) || quantity <= 0 || quantity > product_Quantity[productChoice])
    {
        cin.clear();            // Clear the error flag
        cin.ignore(1000, '\n'); // Discard invalid input
        SetConsoleTextAttribute(hConsole, 4);
        cout << "\t\tInvalid quantity! Please try again: ";
        SetConsoleTextAttribute(hConsole, 15);
    }

    // Update user purchase details
    bool alreadyPurchased = false;
    for (int i = 0; i < purchaseCounts[currentUser]; i++)
    {
        if (userProductIndices[currentUser][i] == productChoice)
        {
            userProductQuantities[currentUser][i] += quantity;
            alreadyPurchased = true;
            break;
        }
    }

    if (!alreadyPurchased)
    {
        userProductIndices[currentUser][purchaseCounts[currentUser]] = productChoice;
        userProductQuantities[currentUser][purchaseCounts[currentUser]] = quantity;
        purchaseCounts[currentUser]++;
    }

    user_Purchase[currentUser] = true;

    // Decrease stock
    product_Quantity[productChoice] -= quantity;

    SetConsoleTextAttribute(hConsole, 2);
    cout << "\n\t\t\t\t\t      Purchase Successful!!!....... \n";
    SetConsoleTextAttribute(hConsole, 15);
}

// Function to RemovePurchased Item
void removePurchasedItem(int userProductIndices[][15], int userProductQuantities[][15], int purchaseCounts[], int product_Quantity[], string product_Name[], int currentUser, bool user_Purchase[], string username[], double product_Price[], int currentProductCount)
{
    trackUserPurchases(currentUser, user_Purchase, username, product_Name, product_Price, userProductQuantities, currentProductCount);
    cout << "\n\n\t     Enter the product number to return: ";
    int removeChoice;
    cin >> removeChoice;

    if (removeChoice < 1 || removeChoice > purchaseCounts[currentUser])
    {
        SetConsoleTextAttribute(hConsole, 4);
        cout << "\n\n\t\t\t\t\t       Invalid Selection!!!......... \n";
        Sleep(600);
        SetConsoleTextAttribute(hConsole, 15);
        return;
    }

    removeChoice--; // Adjust for 0-based indexing
    int productIndex = userProductIndices[currentUser][removeChoice];
    int removedQuantity = userProductQuantities[currentUser][removeChoice];

    // Restore stock
    product_Quantity[productIndex] += removedQuantity;

    // Shift remaining purchases
    for (int i = removeChoice; i < purchaseCounts[currentUser] - 1; i++)
    {
        userProductIndices[currentUser][i] = userProductIndices[currentUser][i + 1];
        userProductQuantities[currentUser][i] = userProductQuantities[currentUser][i + 1];
    }

    purchaseCounts[currentUser]--;
    SetConsoleTextAttribute(hConsole, 2);
    cout << "\n\t\t\t\t\t    Item Returned Successfully!!!\n";
    SetConsoleTextAttribute(hConsole, 15);
}

// Function to EditPurchase
void editPurchaseQuantity(int userProductIndices[][15], int userProductQuantities[][15], int purchaseCounts[], int product_Quantity[], string product_Name[], int currentUser, bool user_Purchase[], string username[], double product_Price[], int currentProductCount)
{
    trackUserPurchases(currentUser, user_Purchase, username, product_Name, product_Price, userProductQuantities, currentProductCount);
    cout << "\n\n\t     Enter the product number to edit: ";
    int editChoice;
    cin >> editChoice;

    if (editChoice < 1 || editChoice > purchaseCounts[currentUser])
    {
        SetConsoleTextAttribute(hConsole, 4);
        cout << "\n\n\t\t\t\t\t      Invalid Selection!!!.......\n";
        Sleep(600);
        SetConsoleTextAttribute(hConsole, 15);
        return;
    }

    editChoice--; // Adjust for 0-based indexing
    int productIndex = userProductIndices[currentUser][editChoice];
    int oldQuantity = userProductQuantities[currentUser][editChoice];

    cout << "\n\t     Enter the new quantity: ";
    int newQuantity;
    cin >> newQuantity;

    if (newQuantity < 0 || newQuantity > product_Quantity[productIndex] + oldQuantity)
    {
        SetConsoleTextAttribute(hConsole, 4);
        cout << "\n\t\t\t\t\t      Invalid Quantity!!!.......\n";
        SetConsoleTextAttribute(hConsole, 15);
        return;
    }

    // Update stock
    product_Quantity[productIndex] += oldQuantity - newQuantity;

    // Update user data
    userProductQuantities[currentUser][editChoice] = newQuantity;
    SetConsoleTextAttribute(hConsole, 2);
    cout << "\n\t\t\t\t\t    Quantity Updated Successfully!!!\n";
    SetConsoleTextAttribute(hConsole, 15);
}

// Function  to Track UserPurchases
void trackUserPurchases(int currentUser, bool user_Purchase[], string username[], string product_Name[], double product_Price[], int userProductQuantities[][15], int currentProductCount)
{
    // Check if the user has made any purchases
    if (!user_Purchase[currentUser])
    {
        SetConsoleTextAttribute(hConsole, 4);
        cout << "\n\t\t\t\t\t      No Purchases Found!!!.......\n";
        SetConsoleTextAttribute(hConsole, 15);
        return;
    }

    // Display user's purchase histor
    cout << "\n\n";
    SetConsoleTextAttribute(hConsole, 5);
    cout << "\t\t\t\t\t\t\t\t\t                Y O U R  C A R T \n";
    cout << "\t\t\t\t\t\t\t\t   ____________________________________________________________\n\n";
    SetConsoleTextAttribute(hConsole, 13);
    cout << "\t\t\b\b\b==========================================================================================================================================================================";
    cout << "\n\t\t\b" << "Sr.      " << left << setw(45) << "PRODUCT " << left << setw(45) << "QUANTITY " << left << setw(45) << "PRICE PER UNIT " << left << setw(45) << "TOTAL COST";
    cout << "\n\t\t\b\b\b==========================================================================================================================================================================\n\n";
    SetConsoleTextAttribute(hConsole, 15);
    Sleep(70);
    double totalCost = 0.0;
    int purchaseIndex = 1;

    for (int i = 0; i < currentProductCount; i++)
    {
        if (userProductQuantities[currentUser][i] > 0)
        {
            double productCost = userProductQuantities[currentUser][i] * product_Price[i];
            totalCost += productCost;

            cout << "\n\t\t " << i + 1 << ".       " << left << setw(45) << product_Name[i] << left << setw(45) << userProductQuantities[currentUser][i] << left << setw(2) << "$" << fixed << setprecision(2) << left << setw(43) << product_Price[i] << left << setw(30) << productCost << "\n";
            Sleep(70);
        }
    }

    SetConsoleTextAttribute(hConsole, 2);
    cout << "\n\n\t\t\t\t\t\t\t\t\t\t Total Amount Spent: " << totalCost << " USD\n";
    SetConsoleTextAttribute(hConsole, 15);
}

// Function to provide Feedback
void provideFeedback(bool uflag[], string username[], string feedback[], int currentUserCount)
{
    displayUser();
    cout << "\n\n";
    SetConsoleTextAttribute(hConsole, 5);
    cout << "\t\t\t\t\t\t\t\t\t             F E E D  B A C K S \n";
    cout << "\t\t\t\t\t\t\t\t  ____________________________________________________________\n\n";
    SetConsoleTextAttribute(hConsole, 15);
    string reportUsername;
    bool found = false;

    // Prompt user for their username to provide feedback
    cout << "\n\t\t\b\bEnter your username to provide feedback : ";
    cin >> reportUsername;

    // Search for the user
    for (int i = 0; i < currentUserCount; i++)
    {
        if (username[i] == reportUsername && uflag[i] == true)
        {
            found = true;

            // Prompt the user for feedback
            cout << "\n\t\t\b\bProvide your feedback (Press enter to submit) : ";
            cin.ignore(); // To clear any leftover newline from previous input
            getline(cin, feedback[i]);

            SetConsoleTextAttribute(hConsole, 2);
            cout << "\n\t\t\t\t\t\t\b Processing......";
            Sleep(1000);
            cout << "\n\t\t\t\t\t\t     Your feedback has been submitted successfully!.....\n";
            SetConsoleTextAttribute(hConsole, 15);
            break;
        }
    }

    if (!found)
    {
        SetConsoleTextAttribute(hConsole, 4);
        cout << "\n\t\t\t\t\t\t    User not found..........\n ";
        Sleep(600);
        SetConsoleTextAttribute(hConsole, 15);
    }

    cout << "\n\t\t\b\bPress any key to continue  ";
    getch();
}

// Function to delete a user
void deleteUser(bool uflag[], bool user_Purchase[], string feedback[], string username[], string email[], string passwords[], int &currentUserCount)
{
    SetConsoleTextAttribute(hConsole, 5);
    cout << "\n\n\n\t\t\t\t\t\t\t\t\t               D E L E T I N G   \n";
    cout << "\t\t\t\t\t\t\t\t  ____________________________________________________________\n";
    SetConsoleTextAttribute(hConsole, 15);
    string delUsername;
    bool found = false;

    // Loop until valid username is found or user decides to quit
    while (true)
    {
        // Get username to delete
        cout << "\n\t\t\b\b\bEnter the username to delete your account : ";
        cin >> delUsername;

        // Search for the user
        for (int i = 0; i < currentUserCount; i++)
        {
            if (username[i] == delUsername && uflag[i] == true)
            {
                uflag[i] = false;         // Mark user as deleted
                user_Purchase[i] = false; // Reset user purchase status
                username[i] = "";
                email[i] = "";
                passwords[i] = "";
                feedback[i] = "";
                currentUserCount--; // Decrement current user count
                SetConsoleTextAttribute(hConsole, 2);
                cout << "\n\t\t\t\t\tUser '" << delUsername << "' Account Deleted Successfully !" << endl;
                Sleep(500);
                SetConsoleTextAttribute(hConsole, 15);
                found = true;
                break;
            }
        }

        // If user is found and deleted, break the loop
        if (found)
            break;

        // If user is not found, display an error and prompt again
        SetConsoleTextAttribute(hConsole, 4); // Set red text
        cout << "\n\t\t\t\t     Username does not exist!! Please try again." << endl;
        SetConsoleTextAttribute(hConsole, 15); // Reset to white text
    }
    return;
}

// Helper function to mask password
string getPasswordInput()
{
    string password;
    char ch;

    cout << "\n\t\t\b\bEnter  Password       :  ";

    // Read characters until Enter key is pressed
    while ((ch = _getch()) != '\r')
    {
        if (ch == '\b')
        { // Handle backspace
            if (!password.empty())
            {
                cout << "\b \b";     // Remove last character from console
                password.pop_back(); // Remove last character from string
            }
        }
        else
        {
            password += ch; // Append character to password
            cout << '*';    // Mask character
        }
    }

    return password;
}

////////////Input Functions for Menus,Sub-Menues
int getAdminInput()
{
    int input;
    do
    {
        displayHeader();
        SetConsoleTextAttribute(hConsole, 6);
        cout << "\t\t\b\b**************************************************************************************************************************************************************************\n";
        cout << "\t\t\t\t\t\t\t\t\t\t    W E L C O M E    A D M I N   \n";
        cout << "\t\t\b\b**************************************************************************************************************************************************************************\n\n";
        SetConsoleTextAttribute(hConsole, 5);
        cout << "\t\t\t\t\t\t\t\t   ************************************************************\n";
        cout << "\t\t\t\t\t\t\t\t\t               A D M I N   M E N U \n";
        cout << "\t\t\t\t\t\t\t\t   ************************************************************\n\n";
        SetConsoleTextAttribute(hConsole, 15);
        Sleep(70);
        cout << "\t" << right << setw(73) << " " << left << setw(20) << " 1.   Warehouse Management\n";
        Sleep(70);
        cout << "\t" << right << setw(73) << " " << left << setw(20) << " 2.   User Management\n";
        Sleep(70);
        SetConsoleTextAttribute(hConsole, 14);
        cout << "\t" << right << setw(73) << " " << left << setw(20) << " 0.   Exit Admin Mode\n\n";
        SetConsoleTextAttribute(hConsole, 15);
        Sleep(70);

        cout.unsetf(ios::left);
        cout.unsetf(ios::right);

        cout << "\t     Select the desired option : ";
        cin >> input;
        if (cin.fail())
        {
            inValid();
            continue;
        }
        else
        {
            return input;
            break;
        }
    } while (true);
}

int getWareHouseInput()
{
    int input;
    do
    {
        displayAdmin();
        SetConsoleTextAttribute(hConsole, 5);
        cout << "\t\t\t\t\t\t\t\t   ************************************************************\n";
        cout << "\t\t\t\t\t\t\t\t\t               W A R E H O U S E     \n";
        cout << "\t\t\t\t\t\t\t\t   ************************************************************\n\n";
        SetConsoleTextAttribute(hConsole, 15);
        Sleep(70);
        cout << "\t" << right << setw(73) << " " << left << setw(20) << " 1.   View Current Inventory Status\n";

        Sleep(70);
        cout << "\t" << right << setw(73) << " " << left << setw(20) << " 2.   Stock In\n";

        Sleep(70);
        cout << "\t" << right << setw(73) << " " << left << setw(20) << " 3.   Remove an Existing Product\n";

        Sleep(70);
        cout << "\t" << right << setw(73) << " " << left << setw(20) << " 4.   Update Product Details\n";

        Sleep(70);
        cout << "\t" << right << setw(73) << " " << left << setw(20) << " 5.   Stock Alerts\n";

        Sleep(70);
        SetConsoleTextAttribute(hConsole, 14);
        cout << "\t" << right << setw(73) << " " << left << setw(20) << " 0.   Exit Inventory Management\n";
        SetConsoleTextAttribute(hConsole, 15);
        Sleep(70);
        cout.unsetf(ios::left);
        cout.unsetf(ios::right);

        cout << "\n\t     Select the desired option : ";
        cin >> input;
        if (cin.fail())
        {
            inValid();
            continue;
        }
        return input;
    } while (true);
}

int getStockInput(string categories[], int currentCategoryCount)
{
    int input;
    do
    {
        displayAdmin();
        SetConsoleTextAttribute(hConsole, 5);
        cout << "\t\t\t\t\t\t\t\t\t               S T O C K   I N   \n";
        cout << "\t\t\t\t\t\t\t\t  ____________________________________________________________\n\n";
        SetConsoleTextAttribute(hConsole, 15);
        Sleep(70);
        displayCategories(categories, currentCategoryCount);
        Sleep(70);

        // Display options
        cout << "\n\n\t     What would you like to add?\n";
        Sleep(70);
        cout << "\n\t        1.   Add New Category";
        Sleep(70);
        cout << "\n\t        2.   Add New Product";
        Sleep(70);

        cout << "\n\n\t     Select the desired option : ";
        cin >> input;
        if (cin.fail())
        {
            inValid();
            continue;
        }
        return input;
    } while (true);
}

int getUserHubInput()
{
    int input;
    do
    {
        displayAdmin();
        SetConsoleTextAttribute(hConsole, 5);
        cout << "\t\t\t\t\t\t\t\t   ************************************************************\n";
        cout << "\t\t\t\t\t\t\t\t\t                 U S E R  H U B    \n";
        cout << "\t\t\t\t\t\t\t\t   ************************************************************\n\n";
        SetConsoleTextAttribute(hConsole, 15);
        Sleep(70);
        cout << "\t" << right << setw(73) << " " << left << setw(20) << " 1.   View All Users Info\n";
        Sleep(70);
        cout << "\t" << right << setw(73) << " " << left << setw(20) << " 2.   Analyze Feedbacks\n";
        Sleep(70);
        cout << "\t" << right << setw(73) << " " << left << setw(20) << " 3.   Trace User Reports\n";
        Sleep(70);
        SetConsoleTextAttribute(hConsole, 14);
        cout << "\t" << right << setw(73) << " " << left << setw(20) << " 0.   Exit Mode\n";
        SetConsoleTextAttribute(hConsole, 15);
        Sleep(70);
        cout.unsetf(ios::left);
        cout.unsetf(ios::right);

        cout << "\t     Select the desired option : ";
        cin >> input;
        if (cin.fail())
        {
            inValid();
            continue;
        }
        return input;
    } while (true);
}

int getUserInput(string username[], int currentUser)
{
    int input;
    do
    {
        displayHeader();
        SetConsoleTextAttribute(hConsole, 6);
        cout << "\t\t\b\b**************************************************************************************************************************************************************************\n";
        cout << "\t\t\t\t\t\t\t\t\t\t    W E L C O M E    " << username[currentUser] << "   \n";
        cout << "\t\t\b\b**************************************************************************************************************************************************************************\n\n";
        SetConsoleTextAttribute(hConsole, 5);
        cout << "\t\t\t\t\t\t\t\t   ************************************************************\n";
        cout << "\t\t\t\t\t\t\t\t\t              U S E R S   M E N U \n";
        cout << "\t\t\t\t\t\t\t\t   ************************************************************\n\n";
        SetConsoleTextAttribute(hConsole, 15);
        Sleep(70);
        cout << "\t" << right << setw(73) << " " << left << setw(20) << " 1.   Browse Inventory \n";
        Sleep(70);
        cout << "\t" << right << setw(73) << " " << left << setw(20) << " 2.   Place Orders\n";
        Sleep(70);
        cout << "\t" << right << setw(73) << " " << left << setw(20) << " 3.   Delete Account\n";
        Sleep(70);
        cout << "\t" << right << setw(73) << " " << left << setw(20) << " 4.   Feedback & Reviews\n";
        Sleep(70);
        SetConsoleTextAttribute(hConsole, 14);
        cout << "\t" << right << setw(73) << " " << left << setw(20) << " 0.   Exit User Mode\n\n";
        SetConsoleTextAttribute(hConsole, 15);
        Sleep(70);
        cout.unsetf(ios::left);
        cout.unsetf(ios::right);

        cout << "\t     Select the desired option : ";
        cin >> input;
        if (cin.fail())
        {
            inValid();
            continue;
        }
        return input;
    } while (true);
}

int getPlaceOrderInput()
{
    int input;
    do
    {
        displayUser();
        SetConsoleTextAttribute(hConsole, 5);
        cout << "\t\t\t\t\t\t\t\t   ************************************************************\n";
        cout << "\t\t\t\t\t\t\t\t\t              P L A C E   O R D E R S \n";
        cout << "\t\t\t\t\t\t\t\t   ************************************************************\n\n";
        SetConsoleTextAttribute(hConsole, 15);
        Sleep(70);
        cout << "\t" << right << setw(73) << " " << left << setw(20) << " 1.   Buy A Product\n";
        Sleep(70);
        cout << "\t" << right << setw(73) << " " << left << setw(20) << " 2.   Track Your Purchase\n";
        Sleep(70);
        cout << "\t" << right << setw(73) << " " << left << setw(20) << " 3.   Return A Product from Your Cart\n";
        Sleep(70);
        cout << "\t" << right << setw(73) << " " << left << setw(20) << " 4.   Update A Product from Your Cart\n";
        Sleep(70);
        cout << "\t" << right << setw(73) << " " << left << setw(20) << " 0.   Exit mode \n";
        Sleep(70);
        cout.unsetf(ios::left);
        cout.unsetf(ios::right);

        cout << "\t     Select the desired option : ";
        cin >> input;
        if (cin.fail())
        {
            inValid();
            continue;
        }
        return input;
    } while (true);
}

int getMainInput()
{
    int input;
    do
    {
        displayHeader();
        SetConsoleTextAttribute(hConsole, 11);
        cout << "\t\t\b\b**************************************************************************************************************************************************************************\n";
        cout << "\t\t\t\t\t\t\t\t\t\t\bW E L C O M E    T O    S T O C K I F Y    \n";
        cout << "\t\t\b\b**************************************************************************************************************************************************************************\n\n";
        SetConsoleTextAttribute(hConsole, 5);
        cout << "\t\t\t\t\t\t\t\t   ************************************************************\n";
        cout << "\t\t\t\t\t\t\t\t\t               M A I N   M E N U \n";
        cout << "\t\t\t\t\t\t\t\t   ************************************************************\n\n";
        SetConsoleTextAttribute(hConsole, 15);
        Sleep(70);
        cout << "\t" << right << setw(73) << " " << left << setw(20) << "  1.   Login as Admin\n";
        Sleep(70);
        cout << "\t" << right << setw(73) << " " << left << setw(20) << "  2.   Login as Standard User\n";
        Sleep(70);
        cout << "\t" << right << setw(73) << " " << left << setw(20) << "  3.   Sign-up Account\n";
        Sleep(70);
        SetConsoleTextAttribute(hConsole, 14);
        cout << "\t" << right << setw(73) << " " << left << setw(20) << "  0.   Press 0 key to Logout\n\n";
        SetConsoleTextAttribute(hConsole, 15);
        Sleep(70);

        cout.unsetf(ios::left);
        cout.unsetf(ios::right);

        cout << "\t     Select the desired option from the menu : ";
        cin >> input;
        if (cin.fail())
        {
            inValid();
            continue;
        }
        return input;
    } while (true);
}