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

// Struct for User
struct User
{
    bool exists;
    bool hasPurchased;
    string username;
    string password;
    string email;
    string feedback;
    int productIndices[15];
    int productQuantities[15];
    int purchaseCount;
};

// Struct for Product
struct Product
{
    bool exists;
    string name;
    int quantity;
    double price;
    string expiryDate;
    int categoryIndex;
};

// Struct for Category
struct Category
{
    string name = ""; // Default value: empty string
};

////// FUNCTIONS PROTOTYPE //////

///////// DISPLAY //////////

void setCursorPosition(int x, int y);
void displayHeader();
void displayAdmin();
void displayUser();
void logout();
void systemExit(User users[], Product products[], Category categories[], int usize, int psize);
void inValid();
string getPasswordInput();
void userAuthentication(User users[], int usize, int &loggedInUserType, int &currentUser);
void adminAuthentication(int &loggedInUserType);
void saveDataToCSV(User users[], Product products[], Category categories[], int usize, int psize);
void loadDataFromCSV(Category categories[], Product products[], User users[], int usize, int psize, int &currentUserCount, int &currentProductCount, int &currentCategoryCount);

//////////.....Inputs;
int getAdminInput();
int getWareHouseInput();
int getStockInput(Category categories[], int currentCategoryCount);
int getUserHubInput();
int getUserInput(User users[], int currentUser);
int getPlaceOrderInput();
int getMainInput();

//////// FUNCTIONALITY ////////

void initializeUserArrays(User users[], int usize);
void initializeProductArrays(Product products[], int psize);
void addProductCategory(Category categories[], int &currentCategoryCount, int csize);
void displayCategories(Category categories[], int currentCategoryCount);
void addProduct(Product products[], int &currentProductCount, int psize, Category categories[], int currentCategoryCount);
bool isNearExpiry(string expiryDate);
bool isValidExpiryDate(const string &expiryDate);
void deleteProduct(Product products[], int &currentProductCount, Category categories[], int currentCategoryCount);
void displayAllProducts(Product products[], int currentProductCount, Category categories[], int currentCategoryCount);
void updateProduct(Product products[], int currentProductCount, Category categories[], int currentCategoryCount);
void updateStockAndCheckThreshold(Product products[], int currentProductCount, int threshold);
void addUser(User users[], int &currentUserCount, int usize);
void deleteUser(User users[], int &currentUserCount);
void provideFeedback(User users[], int currentUserCount);
void displayAllUserInfo(User users[], int currentUserCount);
void displayAllFeedbacks(User users[], int currentUserCount);
void generateDetailedUserReport(User users[], Product products[], int currentUserCount, int currentProductCount);
void purchaseProduct(User users[], Product products[], Category categories[], int currentProductCount, int currentCategoryCount, int currentUser);
void removePurchasedItem(Product products[], User users[], int currentUser, int currentProductCount);
void editPurchaseQuantity(User users[], Product products[], int currentUser, int currentProductCount);
void trackUserPurchases(User users[], Product products[], int currentUser, int currentProductCount);
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

    // Arrays for structs
    User users[usize];
    Product products[psize];
    Category categories[csize];

    // Initialize all arrays
    initializeUserArrays(users, usize);
    initializeProductArrays(products, psize);

    // Load data from CSV files
    loadDataFromCSV(categories, products, users, usize, psize, currentUserCount, currentProductCount, currentCategoryCount);

    int input;
    int loggedInUserType = -10; // 1 for manager/administrator, 2 for use

    do
    {
        input = getMainInput();
        if (input == 0)
        {
            loggedInUserType = -10;
            systemExit(users, products, categories, usize, psize);
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
                userAuthentication(users, usize, loggedInUserType, currentUser);
                break;

            case 3:
                addUser(users, currentUserCount, usize);
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
                                    displayAllProducts(products, currentProductCount, categories, currentCategoryCount);
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
                                            addProduct(products, currentProductCount, psize, categories, currentCategoryCount);
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
                                    deleteProduct(products, currentProductCount, categories, currentCategoryCount);
                                    Sleep(1500);
                                    break;

                                case 4:
                                    updateProduct(products, currentProductCount, categories, currentCategoryCount);
                                    Sleep(1500);
                                    break;

                                case 5:

                                    updateStockAndCheckThreshold(products, currentProductCount, threshold);
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
                                    displayAllUserInfo(users, currentUserCount);
                                    cout << "\n\n\t     Press any key to continue ";
                                    getch();
                                    Sleep(1500);
                                    break;

                                case 2:
                                    displayAllFeedbacks(users, currentUserCount);
                                    cout << "\n\n\t     Press any key to continue ";
                                    getch();
                                    Sleep(1500);
                                    break;

                                case 3:
                                    generateDetailedUserReport(users, products, currentUserCount, currentProductCount);
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
                input = getUserInput(users, currentUser);
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
                        displayAllProducts(products, currentProductCount, categories, currentCategoryCount);
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
                                    purchaseProduct(users, products, categories, currentProductCount, currentCategoryCount, currentUser);
                                    input = -1;
                                    Sleep(1500);
                                    break;

                                case 2:
                                    trackUserPurchases(users, products, currentUser, currentProductCount);
                                    cout << "\n\n\t     Press any key to continue ";
                                    getch();
                                    Sleep(1500);
                                    break;

                                case 3:
                                    removePurchasedItem(products, users, currentUser, currentProductCount);

                                    Sleep(1500);
                                    break;

                                case 4:
                                    editPurchaseQuantity(users, products, currentUser, currentProductCount);
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

                        deleteUser(users, currentUserCount);
                        systemExit(users, products, categories, usize, psize);
                        cout << "data saved";
                        getch();
                        return 0;

                        break;

                    case 4:
                        provideFeedback(users, currentUserCount);
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
void initializeUserArrays(User users[], int usize)
{
    for (int i = 0; i < usize; i++)
    {
        users[i].exists = false;             // Default value: false
        users[i].hasPurchased = false;       // Default value: false
        users[i].username = "";              // Default value: empty string
        users[i].password = "";              // Default value: empty string
        users[i].email = "";                 // Default value: empty string
        users[i].feedback = "";              // Default value: empty string
        users[i].productIndices[15] = {};    // Default value: array of 0s
        users[i].productQuantities[15] = {}; // Default value: array of 0s
        users[i].purchaseCount = 0;          // Default value: 0
    }
}

// Function to initialize product-related arrays
void initializeProductArrays(Product products[], int psize)
{
    for (int i = 0; i < psize; i++)
    {
        products[i].exists = false;     // Default value: false
        products[i].name = "";          // Default value: empty string
        products[i].quantity = 0;       // Default value: 0
        products[i].price = 0.0;        // Default value: 0.0
        products[i].expiryDate = "";    // Default value: empty string
        products[i].categoryIndex = -1; // Default value: -1
    }
}

// Function to load data
void loadDataFromCSV(Category categories[], Product products[], User users[], int usize, int psize, int &currentUserCount, int &currentProductCount, int &currentCategoryCount)
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
        getline(ss, users[currentUserCount].username, ',');
        getline(ss, users[currentUserCount].email, ','); // Added email
        getline(ss, users[currentUserCount].password, ',');
        getline(ss, token, ',');
        users[currentUserCount].hasPurchased = (token == "1");
        getline(ss, users[currentUserCount].feedback, ',');

        // Parse purchases
        string purchaseData;
        getline(ss, purchaseData);
        stringstream purchaseStream(purchaseData);
        string purchaseEntry;
        users[currentUserCount].purchaseCount = 0;

        while (getline(purchaseStream, purchaseEntry, ';') && users[currentUserCount].purchaseCount < 15)
        {
            size_t colonPos = purchaseEntry.find(':');
            if (colonPos != string::npos)
            {
                string productName = purchaseEntry.substr(0, colonPos);
                int quantity = stoi(purchaseEntry.substr(colonPos + 1));

                // Find product index
                for (int i = 0; i < psize; ++i)
                {
                    if (products[i].exists && products[i].name == productName)
                    {
                        users[currentUserCount].productIndices[users[currentUserCount].purchaseCount] = i;
                        users[currentUserCount].productQuantities[users[currentUserCount].purchaseCount] = quantity;
                        users[currentUserCount].purchaseCount++;
                        break;
                    }
                }
            }
        }
        users[currentUserCount].exists = true;
        currentUserCount++;
    }

    // Load product data
    getline(productFile, line); // Skip header
    while (getline(productFile, line) && currentProductCount < psize)
    {
        stringstream ss(line);
        string category;
        getline(ss, products[currentProductCount].name, ',');
        getline(ss, token, ',');
        products[currentProductCount].quantity = stoi(token);
        getline(ss, token, ',');
        products[currentProductCount].price = stod(token);
        getline(ss, category, ',');
        getline(ss, products[currentProductCount].expiryDate, ','); // Load Expiry Date

        // Check or add category
        int categoryIndex = -1;
        for (int i = 0; i < currentCategoryCount; ++i)
        {
            if (categories[i].name == category)
            {
                categoryIndex = i;
                break;
            }
        }
        if (categoryIndex == -1 && currentCategoryCount < 10)
        {
            categories[currentCategoryCount].name = category;
            categoryIndex = currentCategoryCount++;
        }
        products[currentProductCount].categoryIndex = categoryIndex;

        products[currentProductCount].exists = true;
        currentProductCount++;
    }

    // Close files
    userFile.close();
    productFile.close();
    cout << "Data successfully loaded from CSV files." << endl;
}

void saveDataToCSV(User users[], Product products[], Category categories[], int usize, int psize)
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
        if (users[i].exists)
        {
            userFile << users[i].username << ","
                     << users[i].email << "," // Added email
                     << users[i].password << ","
                     << (users[i].hasPurchased ? "1" : "0") << ","
                     << (users[i].feedback.empty() ? "No feedback" : users[i].feedback) << ",";

            // Write purchases
            for (int j = 0; j < users[i].purchaseCount; ++j)
            {
                userFile << products[users[i].productIndices[j]].name << ":"
                         << users[i].productQuantities[j];
                if (j < users[i].purchaseCount - 1)
                    userFile << ";"; // Delimiter for multiple purchases
            }
            userFile << "\n";
        }
    }

    // Write product data
    productFile << "ProductName,Quantity,Price,Category,ExpiryDate,TotalWorth\n";
    for (int i = 0; i < psize; ++i)
    {
        if (products[i].exists)
        {
            double totalWorth = products[i].quantity * products[i].price;
            productFile << products[i].name << ","
                        << products[i].quantity << ","
                        << fixed << setprecision(2) << products[i].price << ","
                        << categories[products[i].categoryIndex].name << ","
                        << products[i].expiryDate << ","
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
void systemExit(User users[], Product products[], Category categories[], int usize, int psize)
{
    SetConsoleTextAttribute(hConsole, 2);
    cout << "\n\t\t\t\t\t     Exiting the system....... ";
    Sleep(1000);
    cout << "\n\t\t\t\t\t\t     Saving Data....... ";
    SetConsoleTextAttribute(hConsole, 15);
    Sleep(700);
    // Save data before exiting
    saveDataToCSV(users, products, categories, usize, psize);

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
void userAuthentication(User users[], int usize, int &loggedInUserType, int &currentUser)
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
        if (users[i].username == user && users[i].password == password && users[i].email == inputEmail)
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
void addProductCategory(Category categories[], int &currentCategoryCount, int csize)
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
        if (categories[i].name == newCategory)
        {
            SetConsoleTextAttribute(hConsole, 4);
            cout << "\n\t\t\t\t\t\t  Category Already Exist!!.......... ";
            SetConsoleTextAttribute(hConsole, 15);
            return;
        }
    }
    // Add the new category
    categories[currentCategoryCount].name = newCategory;
    currentCategoryCount++;
    SetConsoleTextAttribute(hConsole, 2);
    cout << "\n\n\t\t\t\t\t Category '" << newCategory << "' Added Successfully." << endl;
    SetConsoleTextAttribute(hConsole, 15);
}

// Function to display available categories
void displayCategories(Category categories[], int currentCategoryCount)
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
        cout << "\t" << right << setw(74) << "  " << i + 1 << ".      " << left << setw(20) << categories[i].name << "\n\n";
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

void addProduct(Product products[], int &currentProductCount, int psize, Category categories[], int currentCategoryCount)
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

    int category_Index;
    string newProductName;
    int quantity;
    double price;
    string expiryDate;

    // Validate category selection
    do
    {
        cout << "\n\t     Enter the category number/ID to add a product : ";
        cin >> category_Index;
        if (cin.fail() || category_Index < 1 || category_Index > currentCategoryCount)
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
        if (products[i].exists && products[i].name == newProductName)
        {
            SetConsoleTextAttribute(hConsole, 4);
            cout << "\n\n\t\t\t\t\t\t  Product Already Exists!!!..........\n ";
            Sleep(600);
            SetConsoleTextAttribute(hConsole, 15);
            return;
        }
    }

    // Add the new product
    products[currentProductCount].name = newProductName;
    products[currentProductCount].quantity = quantity;
    products[currentProductCount].price = price;
    products[currentProductCount].expiryDate = expiryDate;
    products[currentProductCount].exists = true;
    products[currentProductCount].categoryIndex = category_Index - 1; // Store the category index (0-based)
    currentProductCount++;

    SetConsoleTextAttribute(hConsole, 2);
    cout << "\n\n\t\t\t\t\t Product '" << newProductName << "' Added Successfully Under The Category '" << categories[category_Index - 1].name << "'." << endl;
    SetConsoleTextAttribute(hConsole, 15);
}

void displayAllProducts(Product products[], int currentProductCount, Category categories[], int currentCategoryCount)
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
        if (products[i].exists) // Check if the product is flagged as available
        {
            Sleep(70);
            // Get the category index of the current product
            int category_Index = products[i].categoryIndex;
            if (category_Index >= 0 && category_Index < currentCategoryCount)
            {
                // Display product information
                cout << "\n\t\t  " << i + 1 << ".       " << left << setw(30) << products[i].name
                     << left << setw(30) << products[i].quantity
                     << left << setw(2) << "$" << fixed << setprecision(2) << left << setw(28) << products[i].price
                     << left << setw(30) << categories[category_Index].name << left << setw(15) << products[i].expiryDate;

                // Highlight the expiry date in color if the product is close to expiration
                string expiryDate = products[i].expiryDate;
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
void deleteProduct(Product products[], int &currentProductCount, Category categories[], int currentCategoryCount)
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
        if (products[i].exists && products[i].name == productToDelete)
        {
            cout << "\t     Product '" << productToDelete << "' found under category '" << categories[categoryIndex - 1].name << "'.\n";
            SetConsoleTextAttribute(hConsole, 2);
            cout << "\n\t\t\t\t\t\t        Deleting...... ";
            SetConsoleTextAttribute(hConsole, 15);
            Sleep(700);

            products[i].exists = false;
            products[i].name = "";
            products[i].quantity = 0;
            products[i].price = 0.0;
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
void updateStockAndCheckThreshold(Product products[], int currentProductCount, int threshold)
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
        if (products[i].exists)
        {
            if (products[i].quantity < threshold) // Check if the product is flagged as available)
            {
                cout << "\n\t\t  " << i + 1 << ".       " << left << setw(40) << products[i].name << left << setw(40) << products[i].quantity << left << setw(2) << "$" << fixed << setprecision(2) << left << setw(38) << products[i].price;
                SetConsoleTextAttribute(hConsole, 4);
                cout << "\b\b-----> Warning: Stock is below threshold! \n";
                SetConsoleTextAttribute(hConsole, 15);
                lowStockFlag = true;
            }
            else
            {
                cout << "\n\t\t  " << i + 1 << ".       " << left << setw(40) << products[i].name << left << setw(40) << products[i].quantity << left << setw(2) << "$" << fixed << setprecision(2) << left << setw(43) << products[i].price;
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
void updateProduct(Product products[], int currentProductCount, Category categories[], int currentCategoryCount)
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
        displayAllProducts(products, currentProductCount, categories, currentCategoryCount);

        // Prompt the user to select the product they want to update
        cout << "\n\n\t     Enter the product number to update : ";
        cin >> productIndex;

        if (productIndex < 1 || productIndex > currentProductCount || !products[productIndex - 1].exists)
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
                products[productIndex - 1].name = newName;
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
                products[productIndex - 1].quantity = newQuantity;
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
                products[productIndex - 1].price = newPrice;
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
                products[productIndex - 1].expiryDate = newExpiryDate;
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
void displayAllUserInfo(User users[], int currentUserCount)
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
        if (users[i].exists)
        { // Check if the user is active
            // Display user information
            cout << "\n\t\t  " << i + 1 << ".       " << left << setw(35) << users[i].username
                 << left << setw(35) << users[i].email
                 << left << setw(35) << users[i].password
                 << left << setw(35) << (users[i].hasPurchased ? "Made a purchase" : "No purchases yet") << "\n";
        }
    }
}

// Function to display all feedbacks
void displayAllFeedbacks(User users[], int currentUserCount)
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
        if (users[i].exists)
        { // Only display active users
            cout << "\n\t\t  " << i + 1 << ".       " << left << setw(30) << users[i].username
                 << left << setw(30) << users[i].email
                 << left << setw(30) << (users[i].feedback.empty() ? "No feedback provided" : users[i].feedback) << "\n\n";
        }
    }
}

// Function to generate detailed user report for all users in the system
void generateDetailedUserReport(User users[], Product products[], int currentUserCount, int currentProductCount)
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
        if (!users[i].exists)
            continue; // Skip inactive users

        SetConsoleTextAttribute(hConsole, 13);
        cout << "\n\t\t\t\t\t\t\t\t\t\t  Detailed Report for User: " << users[i].username;
        cout << "\n\t\t\b\b\b==========================================================================================================================================================================\n";
        SetConsoleTextAttribute(hConsole, 15);

        // Display user information including email and feedback
        cout << "\n\t\t\tEmail                       :             " << users[i].email;
        cout << "\n\t\t\tFeedback                    :             " << (users[i].feedback.empty() ? "No feedback provided" : users[i].feedback);
        cout << "\n\t\t\tPurchase Status             :             " << (users[i].hasPurchased ? "Made Purchases" : "No Purchases Yet");

        cout << "\n\t\t\b\b\b__________________________________________________________________________________________________________________________________________________________________________\n";
        SetConsoleTextAttribute(hConsole, 5);
        cout << "\n\t\t\t\t" << left << setw(40) << "Product Name" << setw(40) << "Quantity"
             << setw(40) << "Price per Unit" << setw(10) << "Total Cost";
        SetConsoleTextAttribute(hConsole, 15);
        cout << "\n\t\t\b\b\b__________________________________________________________________________________________________________________________________________________________________________\n";

        double totalUserCost = 0.0;

        // Loop through the purchases made by the user
        for (int j = 0; j < users[i].purchaseCount; j++)
        {
            int productIndex = users[i].productIndices[j];
            int quantity = users[i].productQuantities[j];
            double cost = quantity * products[productIndex].price;
            totalUserCost += cost;

            // Display product purchase details
            cout << "\n\t\t\t\t" << left << setw(40) << products[productIndex].name
                 << setw(40) << quantity
                 << setw(40) << fixed << setprecision(2) << products[productIndex].price
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
void addUser(User users[], int &currentUserCount, int usize)
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
        if (users[i].username == newUsername)
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
        if (users[i].email == newEmail)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
            cout << "\n\n\t\t\t\t     Email already exists!!!...... \n";
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
            Sleep(1000);
            return;
        }
    }

    // Add the new user to the arrays
    users[currentUserCount].username = newUsername;
    users[currentUserCount].password = newPassword;
    users[currentUserCount].email = newEmail;
    users[currentUserCount].exists = true;
    users[currentUserCount].hasPurchased = false; // User hasn't made a purchase yet
    currentUserCount++;

    SetConsoleTextAttribute(hConsole, 2);
    cout << "\n\n\t\t\t\t\t   User '" << newUsername << "' Signed-in Successfully!........" << endl;
    Sleep(1000);
    SetConsoleTextAttribute(hConsole, 15);
}

// Function to PurchaseProduct
void purchaseProduct(User users[], Product products[], Category categories[], int currentProductCount, int currentCategoryCount, int currentUser)
{
    SetConsoleTextAttribute(hConsole, 5);
    cout << "\n\n\t\t\t\t\t\t\t\t\t            A D D   T O   C A R T \n";
    cout << "\t\t\t\t\t\t\t\t   ____________________________________________________________\n\n";
    SetConsoleTextAttribute(hConsole, 15);

    if (!users[currentUser].exists)
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
    cout << "\n\t\t\t\t\t\t\t\t\t\t   Products in " << categories[categoryChoice - 1].name;
    SetConsoleTextAttribute(hConsole, 13);
    cout << "\n\t\t\b\b\b==========================================================================================================================================================================\n";
    cout << "\t\t  " << "Sr.      " << left << setw(60) << "Product" << left << setw(60) << "Quantity" << left << setw(10) << "Price\n";
    cout << "\t\t\b\b\b==========================================================================================================================================================================\n\n";
    SetConsoleTextAttribute(hConsole, 15);

    bool foundProducts = false;
    for (int i = 0; i < currentProductCount; i++)
    {
        if (products[i].categoryIndex == categoryChoice - 1)
        {
            cout << "\t\t   " << i + 1 << ".       " << left << setw(60) << products[i].name
                 << left << setw(60) << products[i].quantity
                 << left << setw(30) << products[i].price << "\n ";
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
    while (!(cin >> productChoice) || productChoice < 1 || productChoice > currentProductCount || products[productChoice - 1].categoryIndex != categoryChoice - 1)
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
    while (!(cin >> quantity) || quantity <= 0 || quantity > products[productChoice].quantity)
    {
        cin.clear();            // Clear the error flag
        cin.ignore(1000, '\n'); // Discard invalid input
        SetConsoleTextAttribute(hConsole, 4);
        cout << "\t\tInvalid quantity! Please try again: ";
        SetConsoleTextAttribute(hConsole, 15);
    }

    // Update user purchase details
    bool alreadyPurchased = false;
    for (int i = 0; i < users[currentUser].purchaseCount; i++)
    {
        if (users[currentUser].productIndices[i] == productChoice)
        {
            users[currentUser].productQuantities[i] += quantity;
            alreadyPurchased = true;
            break;
        }
    }

    if (!alreadyPurchased)
    {
        users[currentUser].productIndices[users[currentUser].purchaseCount] = productChoice;
        users[currentUser].productQuantities[users[currentUser].purchaseCount] = quantity;
        users[currentUser].purchaseCount++;
    }

    users[currentUser].hasPurchased = true;

    // Decrease stock
    products[productChoice].quantity -= quantity;

    SetConsoleTextAttribute(hConsole, 2);
    cout << "\n\t\t\t\t\t      Purchase Successful!!!....... \n";
    SetConsoleTextAttribute(hConsole, 15);
}

// Function to RemovePurchased Item
void removePurchasedItem(Product products[], User users[], int currentUser, int currentProductCount)
{
    trackUserPurchases(users, products, currentUser, currentProductCount);
    cout << "\n\n\t     Enter the product number to return: ";
    int removeChoice;
    cin >> removeChoice;

    if (removeChoice < 1 || removeChoice > users[currentUser].purchaseCount)
    {
        SetConsoleTextAttribute(hConsole, 4);
        cout << "\n\n\t\t\t\t\t       Invalid Selection!!!......... \n";
        Sleep(600);
        SetConsoleTextAttribute(hConsole, 15);
        return;
    }

    removeChoice--; // Adjust for 0-based indexing
    int productIndex = users[currentUser].productIndices[removeChoice];
    int removedQuantity = users[currentUser].productQuantities[removeChoice];

    // Restore stock
    products[productIndex].quantity += removedQuantity;

    // Shift remaining purchases
    for (int i = removeChoice; i < users[currentUser].purchaseCount - 1; i++)
    {
        users[currentUser].productIndices[i] = users[currentUser].productIndices[i + 1];
        users[currentUser].productQuantities[i] = users[currentUser].productQuantities[i + 1];
    }

    users[currentUser].purchaseCount--;
    SetConsoleTextAttribute(hConsole, 2);
    cout << "\n\t\t\t\t\t    Item Returned Successfully!!!\n";
    SetConsoleTextAttribute(hConsole, 15);
}

// Function to EditPurchase
void editPurchaseQuantity(User users[], Product products[], int currentUser, int currentProductCount)
{
    trackUserPurchases(users, products, currentUser, currentProductCount);
    cout << "\n\n\t     Enter the product number to edit: ";
    int editChoice;
    cin >> editChoice;

    if (editChoice < 1 || editChoice > users[currentUser].purchaseCount)
    {
        SetConsoleTextAttribute(hConsole, 4);
        cout << "\n\n\t\t\t\t\t      Invalid Selection!!!.......\n";
        Sleep(600);
        SetConsoleTextAttribute(hConsole, 15);
        return;
    }

    editChoice--; // Adjust for 0-based indexing
    int productIndex = users[currentUser].productIndices[editChoice];
    int oldQuantity = users[currentUser].productQuantities[editChoice];

    cout << "\n\t     Enter the new quantity: ";
    int newQuantity;
    cin >> newQuantity;

    if (newQuantity < 0 || newQuantity > products[productIndex].quantity + oldQuantity)
    {
        SetConsoleTextAttribute(hConsole, 4);
        cout << "\n\t\t\t\t\t      Invalid Quantity!!!.......\n";
        SetConsoleTextAttribute(hConsole, 15);
        return;
    }

    // Update stock
    products[productIndex].quantity += oldQuantity - newQuantity;

    // Update user data
    users[currentUser].productQuantities[editChoice] = newQuantity;
    SetConsoleTextAttribute(hConsole, 2);
    cout << "\n\t\t\t\t\t    Quantity Updated Successfully!!!\n";
    SetConsoleTextAttribute(hConsole, 15);
}

// Function  to Track UserPurchases
void trackUserPurchases(User users[], Product products[], int currentUser, int currentProductCount)
{
    // Check if the user has made any purchases
    if (!users[currentUser].hasPurchased)
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
        if (users[currentUser].productQuantities[i] > 0)
        {
            double productCost = users[currentUser].productQuantities[i] * products[i].price;
            totalCost += productCost;

            cout << "\n\t\t " << i + 1 << ".       " << left << setw(45) << products[i].name << left << setw(45) << users[currentUser].productQuantities[i] << left << setw(2) << "$" << fixed << setprecision(2) << left << setw(43) << products[i].price << left << setw(30) << productCost << "\n";
            Sleep(70);
        }
    }

    SetConsoleTextAttribute(hConsole, 2);
    cout << "\n\n\t\t\t\t\t\t\t\t\t\t Total Amount Spent: " << totalCost << " USD\n";
    SetConsoleTextAttribute(hConsole, 15);
}

// Function to provide Feedback
void provideFeedback(User users[], int currentUserCount)
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
        if (users[i].username == reportUsername && users[i].exists == true)
        {
            found = true;

            // Prompt the user for feedback
            cout << "\n\t\t\b\bProvide your feedback (Press enter to submit) : ";
            cin.ignore(); // To clear any leftover newline from previous input
            getline(cin, users[i].feedback);

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
void deleteUser(User users[], int &currentUserCount)
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
            if (users[i].username == delUsername && users[i].exists == true)
            {
                users[i].exists = false;       // Mark user as deleted
                users[i].hasPurchased = false; // Reset user purchase status
                users[i].username = "";
                users[i].email = "";
                users[i].password = "";
                users[i].feedback = "";
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

int getStockInput(Category categories[], int currentCategoryCount)
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

int getUserInput(User users[], int currentUser)
{
    int input;
    do
    {
        displayHeader();
        SetConsoleTextAttribute(hConsole, 6);
        cout << "\t\t\b\b**************************************************************************************************************************************************************************\n";
        cout << "\t\t\t\t\t\t\t\t\t\t    W E L C O M E    " << users[currentUser].username << "   \n";
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