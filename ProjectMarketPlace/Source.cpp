#include "Header.h"

using namespace std;

Product cart[MAX_PRODUCTS];
int cartIndex = 0;

// Helper function not in header
bool IsValidPhone(const string& phone);

int main()
{
	CheckDatabase();
	
	while (true)
	{
		DisplayHeader();

		cout << "1. Login \n"
			 << "2. View products without login \n"
			 << "0. Exit \n";

		bool isValidChoice = false;  
		char viewChoice;
		viewChoice = _getch();
		
		while (viewChoice < '0' || viewChoice > '2')
		{
			if (!isValidChoice)
			{
				cout << "Invalid Choice! Try Again! :";
				isValidChoice = true;
			}
			viewChoice = _getch();
		}

		switch (viewChoice)
		{
		case '1':
			LoginMainLogic();
			break;
		case '2':
			ViewerOnlyLogic();
			break;
		case '0':
			return 0;
		}
	}
	return 0;
}

void CheckDatabase()
{
	fstream database;
	database.open("Userdb.txt", ios::in);
	if (!database.is_open()) 
	{
		database.open("Userdb.txt", ios::out);
	}
	database.close();
	
	database.open("Productdb.txt", ios::in);
	if (!database.is_open()) {
		database.open("Productdb.txt", ios::out);
	}
	database.close();
	
	database.open("Receiptsdb.txt", ios::in);
	if (!database.is_open()) {
		database.open("Receiptsdb.txt", ios::out);
	}
	database.close();
}

void LoginMainLogic()
{
	User user; 
	LoginOrRegister(&user);

	if (user.acctype) // Seller = true
	{	
		_getch();
		while (true)
		{
			DisplayHeader();
			cout << "======== Seller Menu =======\n";
			cout << "Do want to :\n1. Add a new item to sell\n2. View Items ordered from your store\n3. View your items\n0. Logout\n";
			
			bool isValidChoice = false;
			char choice;
			choice = _getch();
			while (choice < '0' || choice > '3')
			{
				if (!isValidChoice)
				{
					cout << "Invalid Choice! Try Again! :\n";
					isValidChoice = true;
				}
				choice = _getch();
			}
			
			if (choice == '1')
				CreateProduct(&user);
			else if (choice == '2')
				GetOrdersForSellers(&user);
			else if (choice == '3')
				SellerItems(&user);
			else if (choice == '0')
				break;
		}
	}
	else // Buyer
	{
		Product products[MAX_PRODUCTS];
		int productCount = 0;
		LoadProducts(products, &productCount);
		
		while (true)
		{
			_getch();
			DisplayHeader();
			cout << "======== Buyer Menu =======\n";
			cout << "Available Products:\n";
			
			if (ViewList(products, &productCount))
			{
				cout << "Select a product to view details:\n";
				SelectProduct(products, &productCount, false);
			}
			else
				cout << "No products available at the moment!\n";
			
			cout << "Do you want to continue shopping? (Y/N): ";
			bool isValidChoice = false;
			char continueChoice;
			continueChoice = _getch();
			
			while (continueChoice != 'Y' && continueChoice != 'y' && continueChoice != 'N' && continueChoice != 'n')
			{
				if (!isValidChoice)
				{
					cout << "Invalid Choice! Try Again! :\n";
					isValidChoice = true;
				}
				continueChoice = _getch();
			}
			
			if (continueChoice == 'N' || continueChoice == 'n')
				break;
		}
		Checkout(products);
		_getch();
	}
}

void ViewerOnlyLogic()
{
	Product products[MAX_PRODUCTS];
	int productCount = 0;
	LoadProducts(products, &productCount);
	
	while (true) {
		DisplayHeader();
		cout << "Available Products:\n";
		if (ViewList(products, &productCount))
		{
			cout << "Select a product to view details:\n";
			SelectProduct(products, &productCount, true);
			cout << "Do you want to check another product out? (Y/N): ";
			
			bool isValidChoice = false;
			char continueChoice;
			continueChoice = _getch();
			while (continueChoice != 'Y' && continueChoice != 'y' && continueChoice != 'N' && continueChoice != 'n')
			{
				if (!isValidChoice)
				{
					cout << "Invalid Choice! Try Again! :";
					isValidChoice = true;
				}
				continueChoice = _getch();
			}
			
			if (continueChoice == 'N' || continueChoice == 'n')
				break;
			continue;
		}
		else
			cout << "No products available at the moment!\n";
		break;
	}
	
	cout << "\nDo you want to login (Y)? or go to main menu (N): \n";
	bool isValidChoice = false;
	char loginChoice;
	loginChoice = _getch();
	
	while (loginChoice != 'Y' && loginChoice != 'y' && loginChoice != 'N' && loginChoice != 'n')
	{
		if (!isValidChoice)
		{
			cout << "Invalid Choice! Try Again! :\n";
			isValidChoice = true;
		}
		loginChoice = _getch();
	}
	
	if (loginChoice == 'Y' || loginChoice == 'y')
		LoginMainLogic();
}

bool CheckDatabaseForUser(const string* phone, const string* password, string* username, bool* acctype)
{
	fstream database;
	database.open("Userdb.txt", ios::in);
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
			bool dbAccType = (line == "1"); // 1 for seller, 0 for buyer
			
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

void WriteToDatabase(User* newUser)
{
	fstream database;
	database.open("Userdb.txt", ios::out | ios::app);
	if (database.is_open()) {
		database << newUser->phone << "\n" << newUser->name << "\n" << newUser->password << "\n" << newUser->acctype << "\n";
		database.close();
		cout << "User registered successfully!\n";
	}
	else {
		cout << "Error opening database file!\n";
	}
}

bool IsValidPhone(const string& phone)
{
	if (phone.length() != 10)
		return false;
	for (size_t i = 0; i < 10; i++)
	{
		if (!isdigit(phone[i])) 
			return false;
	}
	return true;
}

void LoginOrRegister(User* currentUser)
{
	DisplayHeader();
	cout << "Please login or register to continue.\n1. Login\n2. Register\n";
	
	bool isValidChoice = false;
	char choice;
	choice = _getch();
	while (choice != '1' && choice != '2')
	{
		if (!isValidChoice)
		{
			cout << "Invalid Choice! Try Again! :";
			isValidChoice = true;
		}
		choice = _getch();
	}
	
	if (choice == '2')
	{
		cout << "Registering new User...\n";
		cout << "Are you a Seller (S) or Buyer? (B):";
		char accType;
		accType = _getch();
		while (accType != 's' && accType != 'S' && accType != 'B' && accType != 'b')
		{
			DisplayHeader();
			cerr << "Invaild account type! press 'S' for seller or 'B' for buyer :";
			accType = _getch();
		}

		currentUser->acctype = (accType == 'B' || accType == 'b') ? false : true;

		while(true)
		{
			cout << "Enter Phone Number: +92 ";
			cin >> currentUser->phone;
			if (!IsValidPhone(currentUser->phone))
			{
				DisplayHeader();
				cout << "Invalid Phone Number! Please try again.\n";
				continue;
			}
			break;
		}

		cout << "Enter Username: ";
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		getline(cin, currentUser->name);
		
		cout << "Enter password ( without spaces ) : ";
		cin >> currentUser->password;

		WriteToDatabase(currentUser);
		DisplayHeader();
		cout << "Registration Successful! You are now logged into your account.\n";
	}
	else if (choice == '1')
	{
		cout << "Logging in...\n";
		string phoneInput, passwordInput, username;
		do {
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
				break;
			}
			else
			{
				DisplayHeader();
				cout << "Invalid Phone Number or Password! Please try again.\n";
				continue;
			}
		} while (true);
	}
}

void CreateProduct(User* currentUser) {
	Product newProduct;
	cout << "Enter product name: ";
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	getline(cin, newProduct.name);
	
	while (newProduct.name.empty())
	{
		cout << "Enter a name for the Product! :";
		getline(cin, newProduct.name);
	}
	
	cout << "Enter product description: ";
	getline(cin, newProduct.description);

	if (newProduct.description.empty())
	{
		newProduct.description = "No Description!";
	}
	
	cout << "Enter product price without commas: PKR ";
	cin >> newProduct.price;
	
	while (cin.fail() || newProduct.price < 0)
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Invalid price entered. Please enter a valid positive number : \n";
		cin >> newProduct.price;
	}
	
	cout << "Enter product quantity: ";
	cin >> newProduct.quantity;
	
	while (cin.fail() || newProduct.quantity <= 0)
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Invalid quantity entered. Please enter a valid positive integer : \n";
		cin >> newProduct.quantity;
	}
	
	newProduct.sellerPhone = currentUser->phone;

	fstream database;
	database.open("Productdb.txt", ios::out | ios::app);
	if (database.is_open()) {
		database << newProduct.name << "\n" << newProduct.description << "\n" << fixed << setprecision(2)<< newProduct.price << "\n" << newProduct.quantity << "\n" << newProduct.sellerPhone << "\n";
		database.close();
		cout << "Product created successfully!\n";
	}
	else {
		cout << "Error opening database file!\n";
	}
}

void LoadProducts(Product* products, int* productCount) {
	fstream database;
	database.open("Productdb.txt", ios::in);
	*productCount = 0;
	if (database.is_open()) {
		string name;
		while (getline(database, name) && *productCount < MAX_PRODUCTS) 
		{
			if (name.empty()) break;
			products[*productCount].name = name;
			getline(database, products[*productCount].description);
			database >> products[*productCount].price;
			database >> products[*productCount].quantity;
			database.ignore(); // To ignore the newline character after quantity
			getline(database, products[*productCount].sellerPhone);
			(*productCount)++;
			products[*productCount - 1].id = *productCount - 1;
		}
		database.close();
	}
	else {
		cout << "Error opening database file!\n";
	}
}

void AddToCart(Product* selectedProduct, int quantity)
{
	if (cartIndex > 99)
	{
		cout << "Error! Max ammount of items in cart reached!";
		return;
	}
	cart[cartIndex] = *selectedProduct;
	cart[cartIndex].quantity = quantity;
	cart[cartIndex].id = selectedProduct->id;
	cartIndex++;
	cout << quantity << " units of " << selectedProduct->name << " added to cart.\n";
}

bool ViewList(Product* products, int* productCount)
{
	DisplayHeader();
	cout << "Available Products:\n";
	for (int i = 0; i < *productCount; i++)
	{
		if (products[i].quantity != 0)
			cout << i + 1 << ". " << products[i].name << " - PKR " << fixed << setprecision(1) << products[i].price << " - Quantity: " << products[i].quantity << "\n";
		else
			cout << i + 1 << ". " << products[i].name << " - PKR " << products[i].price << " - Out of Stock\n";
	}
	return *productCount > 0;
}

void SelectProduct(Product* products, int* productCount, bool isViewOnly)
{
	int choice;
	cout << "Enter the number of the product you want to view details for: ";
	cin >> choice;
	
	while (choice < 1 || choice > *productCount || cin.fail()) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		ViewList(products, productCount);
		cout << "Invalid Choice! Try Again!\n";
		cout << "Enter the number of the product you want to view details for: ";
		cin >> choice;
	}
	
	Product selectedProduct = products[choice - 1];
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
	
	if (!isViewOnly) {
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
		AddToCart(&selectedProduct, quantity);
	}
}

void UpdateProducts(Product* products)
{
	fstream database;
	database.open("Productdb.txt", ios::out | ios::trunc);
	if (database.is_open()) {
		for (int i = 0; i < MAX_PRODUCTS; i++) 
		{
			if (products[i].name.empty()) break;
			database << products[i].name << "\n" << products[i].description << "\n" << fixed << setprecision(2) <<  products[i].price << "\n" << products[i].quantity << "\n" << products[i].sellerPhone << "\n";
		}
		database.close();
	}
	else {
		cout << "Error opening database file for updating!\n";
	}
}

void AddReceipts(Product* products, string* address)
{
	fstream receiptFile;
	receiptFile.open("Receiptsdb.txt", ios::out | ios::app);
	if (receiptFile.is_open()) {
		receiptFile << "New Receipt:\n";
		for (int i = 0; i < cartIndex; i++) {
			double itemTotal = cart[i].price * cart[i].quantity;
			receiptFile << products[cart[i].id].sellerPhone << ' ' << cart[i].name << " - PKR " << fixed <<setprecision(2) << cart[i].price << " x " << cart[i].quantity << " = PKR " << itemTotal << " Address: " << *address << "\n";
		}
		receiptFile << "-----------------------\n";
		receiptFile.close();
	}
	else {
		cout << "Error opening receipt file!\n";
	}
}

void Checkout(Product* products)
{
	if (cartIndex == 0) {
		cout << "Your cart is empty!\n";
		return;
	}
	double total = 0.0;
	cout << "\nCheckout:\n";
	for (int i = 0; i < cartIndex; i++) {
		double itemTotal = cart[i].price * cart[i].quantity;
		cout << i + 1 << ". " << cart[i].name << " - PKR " << cart[i].price << " x " << cart[i].quantity << " = PKR " << itemTotal << "\n";
		products[cart[i].id].quantity -= cart[i].quantity;
		total += itemTotal;
	}
	cout << "Total Amount: PKR " << total << "\n";
	cout << "Please enter your address to proceed.\n";
	string address;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	getline(cin, address);
	
	cout << "Proceeding to payment...\n";
	cout << "Thank you for your purchase!\n";
	
	UpdateProducts(products);
	AddReceipts(products, &address);
	cartIndex = 0;
}

void FulfillOrders(User* currentUser, int* lineCount)
{
	fstream receiptFile, tempFile;
	receiptFile.open("Receiptsdb.txt", ios::in);
	tempFile.open("TempReceiptsdb.txt", ios::out);
	
	if (receiptFile.is_open() && tempFile.is_open())
	{
		int tempLineCount;
		cout << "Enter the order line no. you want to fulfill:\n";
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cin >> tempLineCount;
		
		string line;
		string orderToFulfill;
		
		while (tempLineCount < 1 || tempLineCount > *lineCount || cin.fail())
		{
			cout << "Invalid line number! Please enter a valid line number between 1 and " << *lineCount << ":\n";
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cin.clear();
			cin >> tempLineCount;
		}
		
		int choiceCount = 0;
		while (choiceCount != tempLineCount)
		{
			getline(receiptFile, line);

			if (line.find(currentUser->phone) != string::npos && line.find("Delivered") == string::npos)
			{
				orderToFulfill = line;
				choiceCount++;
			}
		}
		
		receiptFile.clear();
		receiptFile.seekg(0, ios::beg);
		bool orderFound = false;
		
		while (getline(receiptFile, line))
		{
			if (line == orderToFulfill)
			{
				orderFound = true;
				cout << "Order fulfilled: " << line << "\n";
				tempFile << line << " -- Delivered!\n";
				continue;
			}
			tempFile << line << "\n";
		}
		
		receiptFile.close();
		tempFile.close();
		
		if (orderFound) {
			remove("Receiptsdb.txt");
			rename("TempReceiptsdb.txt", "Receiptsdb.txt");
		}
		else {
			cout << "Order not found or does not belong to you.\n";
			remove("TempReceiptsdb.txt");
		}
	}
	else {
		cout << "Error opening receipt file!\n";
	}
}

void GetOrdersForSellers(User* currentUser)
{
	fstream receiptFile;
	int lineCount = 0;
	receiptFile.open("Receiptsdb.txt", ios::in);
	
	if (receiptFile.is_open())
	{
		string line;
		cout << "Orders for your products:\n";
		while (getline(receiptFile, line))
		{
			if ((line.find(currentUser->phone) != string::npos) && line.find("Delivered") == string::npos)
			{
				lineCount++;
				cout << lineCount << ". " << line << "\n";
			}
		}
		receiptFile.close();
	}
	else {
		cout << "Error opening receipt file!\n";
	}
	cout << "\n";
	
	if (lineCount)
	{
		cout << "Do you want to fulfill any orders? (Y/N): ";
		bool isValidChoice = false;
		char fulfillChoice;
		fulfillChoice = _getch();
		
		while (fulfillChoice != 'Y' && fulfillChoice != 'y' && fulfillChoice != 'N' && fulfillChoice != 'n')
		{
			if (!isValidChoice)
			{
				cout << "Invalid Choice! Try Again! :\n";
				isValidChoice = true;
			}
			fulfillChoice = _getch();
		}
		
		if (fulfillChoice == 'Y' || fulfillChoice == 'y')
			FulfillOrders(currentUser, &lineCount);
	}
	else
		cout << "No Items Ordered from your store, Press any key to continue";
	_getch();
}

void SellerItems(User* currentUser)
{
	Product products[MAX_PRODUCTS];
	int productCount = 0;
	int localProductCount = 0;
	LoadProducts(products, &productCount);
	
	cout << "Your Products:\n";
	for (int i = 0; i < productCount; i++) {
		if (products[i].sellerPhone == currentUser->phone) {
			localProductCount++;
			cout << localProductCount << ". " << products[i].name << " - PKR " << fixed << products[i].price << " - Quantity: " << products[i].quantity << "\n";
		}
	}
	cout << "\n";
	
	cout << "Do you want to update the quantity of any product? (Y/N): ";
	bool isValidChoice = false;
	char updateChoice;
	updateChoice = _getch();
	
	while (updateChoice != 'Y' && updateChoice != 'y' && updateChoice != 'N' && updateChoice != 'n')
	{
		if (!isValidChoice)
		{
			cout << "Invalid Choice! Try Again! :\n";
			isValidChoice = true;
		}
		updateChoice = _getch();
	}
	
	if (updateChoice == 'Y' || updateChoice == 'y')
	{
		int choice;
		cout << "Enter the number of the product you want to update: ";
		cin >> choice;
		
		while (choice < 1 || choice > localProductCount) {
			cout << "Invalid Choice! ";
			cout << "Enter the number of the product you want to update: ";
			cin >> choice;
		}
		
		localProductCount = 0;
		int newQuantity;
		cout << "Enter the new quantity: ";
		cin >> newQuantity;
		
		for (int i = 0; i < productCount; i++) {
			if (products[i].sellerPhone == currentUser->phone)
			{
				localProductCount++;
				if (localProductCount == choice)
				{
					products[i].quantity = newQuantity;
					UpdateProducts(products);
					cout << "Product quantity updated successfully!\n";
					return;
				}
			}
		}
	}
}