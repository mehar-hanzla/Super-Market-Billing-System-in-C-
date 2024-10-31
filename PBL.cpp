#include <iostream>
#include <fstream>
#include <string>
#include <limits> // For numeric_limits<streamsize>::max()

using namespace std;

const int MAX_ITEMS = 20;  // Maximum number of products that can be added to an order

// Structure to represent a product with its name, price, and quantity
struct Product {
    string name;
    float price;
    float quantity;
};

// Structure to represent a customer with their name and email
struct Customer {
    string name;
    string email;
};

// Structure to represent an order which includes a customer, a list of products, 
// the number of items, and the grand total amount
struct Order {
    Customer customer;
    Product products[MAX_ITEMS];
    int numItems;
    float grandTotal;
};

// Function declarations
void displayMenu();  // Displays the main menu
void getProducts(Product* products, int& numItems);  // Prompts user to enter product details
void displayBill(const Order& order);  // Displays the bill for the order
void writeBillToFile(const Order& order, const string& filename);  // Writes the bill to a file
bool signIn();  // Handles user sign-up
bool login();  // Handles user login
void getCustomerDetails(Customer& customer);  // Gets the customer details

int main() {
    if (!login()) {  // Check if the user successfully logs in
        cout << "Authentication failed. Exiting program." << endl;
        return 0;
    }

    char choice;
    Order order;
    order.numItems = 0;  // Initialize number of items in the order to zero
    order.grandTotal = 0;  // Initialize grand total to zero
    string filename = "bill.txt";  // Default filename to save the bill

    getCustomerDetails(order.customer);  // Get customer details before processing the order

    do {
        displayMenu();  // Show the main menu to the user
        cout << "Enter your choice: ";
        cin >> choice;

        // Input validation for menu choice
        if (cin.fail() || (choice < '1' || choice > '4')) {
            cin.clear();  // Clear the error state
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Discard invalid input
            cout << "Invalid choice. Please enter a number between 1 and 4." << endl;
            continue;  // Skip the rest of the loop and prompt again
        }

        switch (choice) {
            case '1':
                if (order.numItems < MAX_ITEMS) {
                    getProducts(order.products, order.numItems);  // Add product to the order
                    order.grandTotal = 0;  // Reset grand total to recalculate
                    // Calculate the grand total for the order
                    for (int i = 0; i < order.numItems; ++i) {
                        order.grandTotal += order.products[i].price * order.products[i].quantity;
                    }
                } else {
                    cout << "Maximum items reached!" << endl;
                }
                break;
            case '2':
                displayBill(order);  // Display the current bill
                break;
            case '3':
                writeBillToFile(order, filename);  // Save the bill to a file
                break;
            case '4':
                cout << "Exiting program." << endl;  // Exit the program
                break;
        }
    } while (choice != '4');  // Loop until the user chooses to exit

    return 0;
}

// Function to display the main menu
void displayMenu() {
    cout << "\n*** SUPERMARKET BILLING SYSTEM ***\n" << endl;
    cout << "1. Add Item" << endl;
    cout << "2. Display Bill" << endl;
    cout << "3. Write Bill to File" << endl;
    cout << "4. Exit" << endl;
}

// Function to get details for each product added to the order
void getProducts(Product* products, int& numItems) {
    cout << "\nEnter details for item " << (numItems + 1) << ":" << endl;
    cout << "Product name: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore the newline character from previous input
    getline(cin, products[numItems].name);  // Read the product name
    cout << "Price: ";
    cin >> products[numItems].price;  // Read the product price
    cout << "Quantity: ";
    cin >> products[numItems].quantity;  // Read the product quantity
    numItems++;  // Increment the number of items in the order

    // Clear input buffer for any potential invalid inputs
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Function to display the bill for the order
void displayBill(const Order& order) {
    cout << "\n\n*** SUPERMARKET BILLING SYSTEM ***\n" << endl;
    cout << "Customer Name: " << order.customer.name << endl;
    cout << "Customer Email: " << order.customer.email << endl;
    cout << "----------------------------------------------" << endl;
    cout << "  Product Name  | Price  | Quantity | Total  " << endl;
    cout << "----------------------------------------------" << endl;
    for (int i = 0; i < order.numItems; i++) {
        float total = order.products[i].price * order.products[i].quantity;  // Calculate total for each product
        cout << "   " << order.products[i].name << "     "
             << order.products[i].price << "     "
             << order.products[i].quantity << "     "
             << total << endl;
    }
    cout << "\nGrand Total: $" << order.grandTotal << endl;  // Display the grand total
}

// Function to write the bill to a file
void writeBillToFile(const Order& order, const string& filename) {
    ofstream outfile(filename);  // Open the file to write the bill
    if (!outfile.is_open()) {  // Check if the file was opened successfully
        cout << "Error: Could not open file " << filename << endl;
        return;
    }
    outfile << "\n\n*** SUPERMARKET BILLING SYSTEM ***\n" << endl;
    outfile << "Customer Name: " << order.customer.name << endl;
    outfile << "Customer Email: " << order.customer.email << endl;
    outfile << "----------------------------------------------" << endl;
    outfile << "  Product Name  | Price  | Quantity | Total  " << endl;
    outfile << "----------------------------------------------" << endl;
    for (int i = 0; i < order.numItems; i++) {
        float total = order.products[i].price * order.products[i].quantity;  // Calculate total for each product
        outfile << "  " << order.products[i].name << "  "
                << order.products[i].price << "  "
                << order.products[i].quantity << "  "
                << total << endl;
    }
    outfile << "\nGrand Total: $" << order.grandTotal << endl;  // Write the grand total to the file
    outfile.close();  // Close the file
    cout << "Bill successfully written to " << filename << endl;  // Notify the user of success
}

// Function to handle user sign-up
bool signIn() {
    string username, password, confPassword;
    cout << "Sign Up" << endl;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;
    cout << "Confirm password: ";
    cin >> confPassword;

    if (password != confPassword) {  // Check if passwords match
        cout << "Passwords do not match. Try again." << endl;
        return false;
    }

    ofstream outfile("users.txt", ios::app);  // Open the users file in append mode
    if (!outfile.is_open()) {  // Check if the file was opened successfully
        cout << "Error: Could not open file to save user data." << endl;
        return false;
    }

    outfile << username << " " << password << endl;  // Save the username and password
    outfile.close();  // Close the file
    cout << "Sign up successful. You can now log in." << endl;
    return true;
}

// Function to handle user login
bool login() {
    string username, password, storedUsername, storedPassword;
    char choice;

    cout << "Do you have an account? (y/n): ";
    cin >> choice;

    if (choice == 'n' || choice == 'N') {  // If the user does not have an account
        if (!signIn()) {  // Prompt the user to sign up
            return false;
        }
    }

    cout << "Log In" << endl;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    ifstream infile("users.txt");  // Open the users file to read stored credentials
    if (!infile.is_open()) {  // Check if the file was opened successfully
        cout << "Error: Could not open file to read user data." << endl;
        return false;
    }

    // Check if the entered username and password match any stored credentials
    while (infile >> storedUsername >> storedPassword) {
        if (username == storedUsername && password == storedPassword) {
            infile.close();
            cout << "Login successful." << endl;
            return true;
        }
    }

    infile.close();  // Close the file
    cout << "Invalid username or password." << endl;
    return false;
}

// Function to get the details of the customer placing the order
void getCustomerDetails(Customer& customer) {
    cout << "Enter customer details:" << endl;
    cout << "Customer name: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Ignore the newline character from previous input
    getline(cin, customer.name);  // Read the customer's name
    cout << "Customer email: ";
    getline(cin, customer.email);  // Read the customer's email
}
