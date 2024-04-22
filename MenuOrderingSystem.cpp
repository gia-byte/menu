#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <ctime> // for timestamp

using namespace std;

// Struct to hold food item details
struct FoodItem {
    string description;
    double price;
};

// Function to print receipt and save to file
void printReceipt(const map<string, int>& quantityOrdered, const map<string, FoodItem>& menu, const map<string, string>& excludedIngredients, const string& orderType) {
    // Generate timestamp for filename
    time_t now = time(0);
    tm* ltm = localtime(&now);
    string timestamp = to_string(1900 + ltm->tm_year) + to_string(1 + ltm->tm_mon) + to_string(ltm->tm_mday) + to_string(ltm->tm_hour) + to_string(ltm->tm_min) + to_string(ltm->tm_sec);

    // Create filename with timestamp
    string filename = "receipt_" + timestamp + ".txt";

    ofstream receipt(filename);
    if (receipt.is_open()) {
        receipt << "Receipt:" << endl;
        receipt << "Order Type: " << orderType << endl;
        double total = 0.0; // Declare total here
        for (const auto& item : quantityOrdered) {
            if (menu.find(item.first) != menu.end()) {
                const FoodItem& menuItem = menu.at(item.first);
                double subtotal = item.second * menuItem.price;
                receipt << item.first << " x" << item.second << ": $" << fixed << setprecision(2) << subtotal;
                if (excludedIngredients.find(item.first) != excludedIngredients.end()) {
                    receipt << " (excluded: " << excludedIngredients.at(item.first) << ")";
                }
                receipt << endl;
                total += subtotal;
            }
        }
        receipt << "Total quantity of items: ";
        int totalQuantity = 0;
        for (const auto& item : quantityOrdered) {
            totalQuantity += item.second;
        }
        receipt << totalQuantity << endl;
        receipt << "Total: $" << fixed << setprecision(2) << total << endl;
        receipt.close();
        cout << "Receipt has been saved to " << filename << endl << endl;
    } else {
        cout << "Unable to create receipt file." << endl;
    }

    // Display receipt on console
    cout << "Receipt:" << endl;
    cout << "Order Type: " << orderType << endl;
    double total = 0.0; // Declare total here
    for (const auto& item : quantityOrdered) {
        if (menu.find(item.first) != menu.end()) {
            const FoodItem& menuItem = menu.at(item.first);
            double subtotal = item.second * menuItem.price;
            cout << item.first << " x" << item.second << ": $" << fixed << setprecision(2) << subtotal;
            if (excludedIngredients.find(item.first) != excludedIngredients.end()) {
                cout << " (excluded: " << excludedIngredients.at(item.first) << ")";
            }
            cout << endl;
            total += subtotal;
        }
    }
    cout << "Total quantity of items: ";
    int totalQuantity = 0;
    for (const auto& item : quantityOrdered) {
        totalQuantity += item.second;
    }
    cout << totalQuantity << endl;
    cout << "Total: $" << fixed << setprecision(2) << total << endl;
}

int main() {
    // Create maps to store menu items for each category
    map<string, FoodItem> burgers = {
        {"Classic Burger", {"Beef patty, lettuce, tomato, onion, pickles, cheese, ketchup, mayo", 5.99}},
        {"Cheeseburger", {"Beef patty, lettuce, tomato, onion, pickles, cheese, ketchup, mayo", 6.49}},
        {"Veggie Burger", {"Veggie patty, lettuce, tomato, onion, pickles, ketchup, mayo", 5.99}}
    };

    map<string, FoodItem> drinks = {
        {"Iced Tea", {"Iced tea", 1.49}},
        {"Coca Cola", {"Coca cola", 1.99}},
        {"Coffee", {"Coffee", 2.49}}
    };

    map<string, FoodItem> others = {
        {"Pizza", {"Pizza dough, tomato sauce, cheese, pepperoni, mushrooms, bell peppers", 7.99}},
        {"Fries", {"Potato fries, salt", 2.49}},
        {"Salad", {"Caesar salad mix, lettuce, tomato, croutons, Caesar dressing", 4.99}}
    };

    // Variable to store order type
    string orderType;

    // Ask for order type (takeout or dine-in)
    cout << "Welcome to the Restaurant!" << endl;
    cout << "Is your order for takeout or dine-in? ";
    getline(cin, orderType);

    // Display the menu
    cout << "\nMenu:" << endl;
    cout << "Burgers:" << endl;
    for (const auto& item : burgers) {
        cout << "    " << item.first << ": $" << fixed << setprecision(2) << item.second.price << endl;
    }
    cout << "Drinks:" << endl;
    for (const auto& item : drinks) {
        cout << "    " << item.first << ": $" << fixed << setprecision(2) << item.second.price << endl;
    }
    cout << "Others:" << endl;
    for (const auto& item : others) {
        cout << "    " << item.first << ": $" << fixed << setprecision(2) << item.second.price << endl;
    }

    // Map to store quantity of each item ordered
    map<string, int> quantityOrdered;
    // Map to store excluded ingredients for each ordered item
    map<string, string> excludedIngredients;

    // Order food
    string choice;
    cout << "\nEnter the item you'd like to order (type 'done' to finish): ";
    while (getline(cin, choice) && choice != "done") {
        if (burgers.find(choice) != burgers.end()) {
            const FoodItem& item = burgers.at(choice);
            cout << "Description: " << item.description << endl;
            cout << "Would you like to exclude any ingredient? (type 'none' to skip): ";
            string exclude;
            getline(cin, exclude);
            if (exclude != "none") {
                excludedIngredients[choice] = exclude;
            }
            // Increment quantity of ordered item
            quantityOrdered[choice]++;
            cout << "Added " << choice << " to your order." << endl;
        } else if (drinks.find(choice) != drinks.end()) {
            const FoodItem& item = drinks.at(choice);
            cout << "Description: " << item.description << endl;
            // Increment quantity of ordered item
            quantityOrdered[choice]++;
            cout << "Added " << choice << " to your order." << endl;
        } else if (others.find(choice) != others.end()) {
            const FoodItem& item = others.at(choice);
            cout << "Description: " << item.description << endl;
            // Increment quantity of ordered item
            quantityOrdered[choice]++;
            cout << "Added " << choice << " to your order." << endl;
        } else {
            cout << "Sorry, we don't have " << choice << " on the menu." << endl;
        }
        cout << "Enter the next item you'd like to order (type 'done' to finish): ";
    }

    // Print receipt
    printReceipt(quantityOrdered, burgers, excludedIngredients, orderType);

    cout << "Thank you for ordering!" << endl;

    return 0;
}
