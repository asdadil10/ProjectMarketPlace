#pragma once
#include <cstdlib>
#include <fstream>
#include <string>
#include <iostream>
#include <conio.h>
struct user {
	std::string phone;
	std::string name;
	std::string password;
	bool acctype = false; // true for seller, false for buyer
};

using namespace std;

inline void DisplayHeader()
{
	system("cls");
	cout << "===== Project Market Place =====" << endl;
}
inline void Welcome(string* userName)
{
	DisplayHeader();
	cout << "Welcome to the Market Place, " << *userName << "!" << endl;
}
bool CheckDatabaseForUser(const string* phone, const string* password, string* username, bool* acctype)
{
	fstream database;
	database.open("UserDatabase", ios::in);
	if (database.is_open()) {
		string line;
		while (getline(database, line))
		{

			string dbPhone = line;
			getline(database, line);
			string dbName = line;
			getline(database, line);
			string dbPassword = line;
			getline(database, line);
			bool dbAccType = (line == "1") ? true : false;
			if (dbPhone == *phone && dbPassword == *password)
			{
				*username = dbName;
				*acctype = dbAccType;
				database.close();
				return true;
			}
		}
		database.close();
	}
	return false;
}
inline void WritetoDatabase(user* newUser)
{
	fstream database;
	database.open("UserDatabase", ios::out | ios::app);
	if (database.is_open()) {
		database << newUser->phone << "\n" << newUser->name << "\n" << newUser->password << "\n" << newUser->acctype << "\n";
		database.close();
		cout << "User registered successfully!\n";
	}
	else {
		cout << "Error opening database file!\n";
	}
}

void LoginOrRegister(user* currentUser)
{
	DisplayHeader();
	cout << "Please login or register to continue.\n1.Register\n2.Login\n";
	bool isvalidchoice = false;
	char choice;
	choice = _getch();
	while (choice != '1' && choice != '2')
	{
		if (!isvalidchoice) 
		{
			cout << "Invalid Choice! Try Again! :";
			isvalidchoice = true;
		}
		choice = _getch();
	}
	if (choice == '1')
	{
		cout << "Registering new user...\n";
		cout << "Are you a Seller or Buyer? :";
		string accType;
	reg:
		cin.ignore();
		cin >> accType;
		if (accType == "Seller" || accType == "seller")
			currentUser->acctype = true;
		else if(accType == "Buyer" || accType == "buyer")
			currentUser->acctype = false;
		else
		{
			DisplayHeader();
			cout << "Invalid account type! Please enter 'Seller' or 'Buyer'.\n";
			goto reg;
		}
	registering:
		cout << "Enter Phone Number: +92 ";
		cin >> currentUser->phone;
		if (currentUser->phone.length() != 10)
		{
			DisplayHeader();
			cout << "Invalid Phone Number! Please try again.\n";
			goto registering;
		}
		cout << "Enter username: ";
		{
			string line;
			cin.ignore();
			getline(cin, line);
			currentUser->name = line;
		}
		cout << "Enter password: ";
		cin >> currentUser->password;
		WritetoDatabase(currentUser);
		DisplayHeader();
		cout << "Registration Successful! You can now login with your credentials.\n";
	}
	else if (choice == '2')
	{
		cout << "Logging in...\n";
		string phoneInput, passwordInput, username;
	login:
		cout << "Enter User Phone Number: +92 ";
		cin >> phoneInput;
		cout << "Enter Password: ";
		cin >> passwordInput;
		if (CheckDatabaseForUser(&phoneInput, &passwordInput, &username, &currentUser->acctype))
		{
			cout << "Login successful!\n";
			currentUser->name = username;
			currentUser->phone = phoneInput;
			phoneInput.clear();
			passwordInput.clear();
			Welcome(&username);
		}
		else
		{
			DisplayHeader();
			cout << "Invalid Phone Number or Password! Please try again.\n";
			goto login;
		}
	}
}
const int MAX_PRODUCTS = 100;
struct product
{
	int id;
	std::string name;
	std::string description;
	double price;
	int quantity;
	std::string sellerPhone;
};
product cart[MAX_PRODUCTS];
int cartIndex = 0;
void createProduct(user* currentUser) {
	// Function to create a new product
	product newProduct;
	cout << "Enter product name: ";
	cin.ignore();
	getline(cin, newProduct.name);
	cout << "Enter product description: ";
	getline(cin, newProduct.description);
	cout << "Enter product price: PKR ";
	cin >> newProduct.price;
	cout << "Enter product quantity: ";
	cin >> newProduct.quantity;
	newProduct.sellerPhone = currentUser->phone;

	// Save the product to the database
	fstream database;
	database.open("ProductDatabase", ios::out | ios::app);
	if (database.is_open()) {
		database << newProduct.name << "\n" << newProduct.description << "\n" << newProduct.price << "\n" << newProduct.quantity << "\n" << newProduct.sellerPhone << "\n";
		database.close();
		cout << "Product created successfully!\n";
	}
	else {
		cout << "Error opening database file!\n";
	}
}
void loadProducts(product* products, int* productCount) {
	// Function to load products from the database
	fstream database;
	database.open("ProductDatabase", ios::in);
	*productCount = 0;
	if (database.is_open()) {
		string name;
		while (!database.eof() && *productCount < MAX_PRODUCTS) 
		{
			getline(database, name);
			if (name.empty()) break; // To avoid reading empty lines at the end due to eof()
			products[*productCount].name = name;
			getline(database, products[*productCount].description);
			database >> products[*productCount].price;
			database >> products[*productCount].quantity;
			database.ignore(); // To ignore the newline character after quantity
			getline(database, products[*productCount].sellerPhone);
			(*productCount)++;
			products[*productCount - 1].id = *productCount - 1; // Assign ID
		}
		database.close();
	}
	else {
		cout << "Error opening database file!\n";
	}
}

void addtocart(product* selectedProduct, int quantity)
{
	if (cartIndex > 99)
	{
		cout << "Error! Max ammount of items in cart reached!";
		return;
	}
	cart[cartIndex] = *selectedProduct; // For simplicity, adding to first position
	cart[cartIndex].quantity = quantity;
	cart[cartIndex].id = selectedProduct->id;
	cartIndex++;
	cout << quantity << " units of " << selectedProduct->name << " added to cart.\n";
}
bool viewlist(product* products, int* productCount)
{
	DisplayHeader();
	cout << "Available Products:\n";
	for (int i = 0; i < *productCount; i++) {
		cout << i + 1 << ". " << products[i].name << " - PKR " << products[i].price << " - Quantity: " << products[i].quantity << "\n";
	}
	return *productCount > 0;
}
void selectproduct(product* products, int* productCount,bool isviewonly)
{
	int choice;
	cout << "Enter the number of the product you want to view details for: ";
	cin >> choice;
	while (choice < 1 || choice > *productCount) {
		viewlist(products, productCount);
		cout << "Invalid Choice! Try Again!\n";
		cout << "Enter the number of the product you want to view details for: ";
		cin >> choice;
	}
	product selectedProduct = products[choice - 1];
	cout << "Product Details:\n";
	cout << "Name: " << selectedProduct.name << "\n";
	cout << "Description: " << selectedProduct.description << "\n";
	cout << "Price: PKR " << selectedProduct.price << "\n";
	cout << "Quantity: " << selectedProduct.quantity << "\n";
	cout << "Seller Phone: +92 " << selectedProduct.sellerPhone << "\n";
	if (selectedProduct.quantity == 0) {
		cout << "This product is currently out of stock.\n";
		return;
	}
	if (!isviewonly) {
		cout << "Do you want to add this product to your cart? (yes/no): ";
	}
	else
		return;
	string purchaseChoice;
	cin >> purchaseChoice;
	if (purchaseChoice != "yes" && purchaseChoice != "no") {
		cout << "Invalid choice! Please enter 'yes' or 'no': ";
		cin >> purchaseChoice;
	}
	if (purchaseChoice == "yes" && selectedProduct.quantity > 0) {
		int quantity;
		cout << "Enter quantity: ";
		cin >> quantity;
		while (quantity < 1 || quantity > selectedProduct.quantity) {
			cout << "Invalid quantity! Please enter a quantity between 1 and " << selectedProduct.quantity << ": ";
			cin >> quantity;
		}
		addtocart(&selectedProduct, quantity);
	}
}
void updateproducts(product* products)
{
	fstream database;
	database.open("ProductDatabase", ios::out | ios::trunc);
	if (database.is_open()) {
		for (int i = 0; i < MAX_PRODUCTS; i++) {
			if (products[i].name.empty()) break; // Stop if no more products
			database << products[i].name << "\n" << products[i].description << "\n" << products[i].price << "\n" << products[i].quantity << "\n" << products[i].sellerPhone << "\n";
		}
		database.close();
	}
	else {
		cout << "Error opening database file for updating!\n";
	}
}
void addrecipts(product* products,string*address)
{
	fstream receiptFile;
	receiptFile.open("Receipts", ios::out | ios::app);
	if (receiptFile.is_open()) {
		receiptFile << "New Receipt:\n";
		for (int i = 0; i < cartIndex; i++) {
			double itemTotal = cart[i].price * cart[i].quantity;
			receiptFile << products[cart[i].id].sellerPhone << ' ' << cart[i].name << " - PKR " << cart[i].price << " x " << cart[i].quantity << " = PKR " << itemTotal << " Address: "<< *address << "\n";
		}
		receiptFile << "-----------------------\n";
		receiptFile.close();
	}
	else {
		cout << "Error opening receipt file!\n";
	}
}
void checkout(product* products)
{
	if (cartIndex == 0) {
		cout << "Your cart is empty!\n";
		return;
	}
	double total = 0.0;
	cout << "Checkout:\n";
	for (int i = 0; i < cartIndex; i++) {
		double itemTotal = cart[i].price * cart[i].quantity;
		cout << cart[i].name << " - PKR " << cart[i].price << " x " << cart[i].quantity << " = PKR " << itemTotal << "\n";
		products[cart[i].id].quantity -= cart[i].quantity;
		total += itemTotal;
	}
	cout << "Total Amount: PKR " << total << "\n";
	cout << "Please enter your address to proceed.\n";
	string address;
	cin.ignore();
	getline(cin, address);
	cout << "Proceeding to payment...\n";
	cout << "Thank you for your purchase!\n";
	// Clear the cart after checkout
	updateproducts(products);
	addrecipts(products,&address);
	cartIndex = 0;
}

void getordersforsellers(user* currentUser)
{
	fstream receiptFile;
	receiptFile.open("Receipts", ios::in);
	if (receiptFile.is_open()) 
	{
		string line;
		cout << "Orders for your products:\n";
		while (getline(receiptFile, line))
		{
			if (line.find(currentUser->phone) != string::npos)
			{
				cout << line << "\n";
			}
		}
		receiptFile.close();
	}
	else {
		cout << "Error opening receipt file!\n";
	}
}