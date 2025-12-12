#include "Header.h"

bool isvalidchoice = false;

void checkdb() //check if db files exist else create them
{
	fstream database;
	database.open("Userdb.txt", ios::in);
	if (!database.is_open()) {
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
void Loginmainlogic()
{
	user User;
	LoginOrRegister(&User);
	if (User.acctype) // Seller
	{
		_getch();
		while (true)
		{
			DisplayHeader();
			cout << "======== Seller Menu =======\n";
			cout << "Do want to :\n1. Add a new item to sell\n2. View Items oredered from your store\n3. View your items\n0. Logout\n";
			isvalidchoice = false;
			char choice;
			choice = _getch();
			while (choice < '0' || choice > '3')
			{
				if (!isvalidchoice)
				{
					cout << "Invalid Choice! Try Again! :\n";
					isvalidchoice = true;
				}
				choice = _getch();
			}
			if (choice == '1')
				createProduct(&User);
			else if (choice == '2')
				getordersforsellers(&User);
			else if (choice == '3')
				selleritems(&User);
			else if (choice == '0')
				break;
		}
	}
	else // Buyer
	{
		product products[MAX_PRODUCTS];
		int productCount = 0;
		loadProducts(products, &productCount);
		while (true)
		{
			_getch();
			DisplayHeader();
			cout << "======== Buyer Menu =======\n";
			cout << "Available Products:\n";
			if (viewlist(products, &productCount))
			{
				cout << "Select a product to view details:\n";
				selectproduct(products, &productCount, false);
			}
			else
				cout << "No products available at the moment!\n";
			cout << "Do you want to continue shopping? (Y/N): ";
			isvalidchoice = false;
			char continueChoice;
			continueChoice = _getch();
			while (continueChoice != 'Y' && continueChoice != 'y' && continueChoice != 'N' && continueChoice != 'n')
			{
				if (!isvalidchoice)
				{
					cout << "Invalid Choice! Try Again! :\n";
					isvalidchoice = true;
				}
				continueChoice = _getch();
			}
			if (continueChoice == 'N' || continueChoice == 'n')
				break;
		}
		checkout(products);
		_getch();
	}
}
void Vieweronlylogic()
{
	product products[MAX_PRODUCTS];
	int productCount = 0;
	loadProducts(products, &productCount);
	while (true) {
		DisplayHeader();
		cout << "Available Products:\n";
		if (viewlist(products, &productCount))
		{
			cout << "Select a product to view details:\n";
			selectproduct(products, &productCount, true);
			cout << "Do you want to check another product out? (Y/N): ";
			isvalidchoice = false;
			char continueChoice;
			continueChoice = _getch();
			while (continueChoice != 'Y' && continueChoice != 'y' && continueChoice != 'N' && continueChoice != 'n')
			{
				if (!isvalidchoice)
				{
					cout << "Invalid Choice! Try Again! :";
					isvalidchoice = true;
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
	isvalidchoice = false;
	char loginChoice;
	loginChoice = _getch();
	while (loginChoice != 'Y' && loginChoice != 'y' && loginChoice != 'N' && loginChoice != 'n')
	{
		if (!isvalidchoice)
		{
			cout << "Invalid Choice! Try Again! :\n";
			isvalidchoice = true;
		}
		loginChoice = _getch();
	}
	if (loginChoice == 'Y' || loginChoice == 'y')
		Loginmainlogic();
}
int main() {
	checkdb();
	while (true) 
	{
		DisplayHeader();
		cout << "1. Login \n2. View products without login \n0. exit \n";
		isvalidchoice = false;
		char viewChoice;
		viewChoice = _getch();
		while (viewChoice != '1' && viewChoice != '2' && viewChoice != '0')
		{
			if (!isvalidchoice)
			{
				cout << "Invalid Choice! Try Again! :";
				isvalidchoice = true;
			}
			viewChoice = _getch();
		}
		if (viewChoice == '1')
			Loginmainlogic();
		else if (viewChoice == '2')
			Vieweronlylogic();
		else if (viewChoice == '0')
			break;
	}
	return 0;
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

void LoginOrRegister(user* currentUser)
{
	DisplayHeader();
	cout << "Please login or register to continue.\n1. Login\n2. Register\n";
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
	if (choice == '2')
	{
		cout << "Registering new user...\n";
		cout << "Are you a Seller or Buyer? :";
		string accType;
		do {
			cin.ignore();
			cin >> accType;
			if (accType == "Seller" || accType == "seller")
				currentUser->acctype = true;
			else if (accType == "Buyer" || accType == "buyer")
				currentUser->acctype = false;
			else
			{
				DisplayHeader();
				cout << "Invalid account type! Please enter 'Seller' or 'Buyer'.\n";
				continue;
			}
			break;
		} while (true);
		do {
			cout << "Enter Phone Number: +92 ";
			cin >> currentUser->phone;
			if (currentUser->phone.length() != 10)
			{
				DisplayHeader();
				cout << "Invalid Phone Number! Please try again.\n";
				continue;
			}
			break;
		} while (true);
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
			}
			else
			{
				DisplayHeader();
				cout << "Invalid Phone Number or Password! Please try again.\n";
				continue;
			}
			break;
		} while (true);
	}
}
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
	while (cin.fail() || newProduct.price < 0)
	{
		cin.clear(); // clear the fail state
		cin.ignore();
		cout << "Invalid price entered. Please enter a valid positive number : \n";
		cin >> newProduct.price;
	}
	cout << "Enter product quantity: ";
	cin >> newProduct.quantity;
	while (cin.fail() || newProduct.quantity <= 0)
	{
		cin.clear(); // clear the fail state
		cin.ignore();
		cout << "Invalid quantity entered. Please enter a valid positive integer : \n";
		cin >> newProduct.quantity;
	}
	newProduct.sellerPhone = currentUser->phone; // Associate product with seller's phone number
	// Save the product to the database
	fstream database;
	database.open("Productdb.txt", ios::out | ios::app);
	if (database.is_open()) {
		database << newProduct.name << "\n" << newProduct.description << "\n" << fixed << newProduct.price << "\n" << newProduct.quantity << "\n" << newProduct.sellerPhone << "\n";
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
	database.open("Productdb.txt", ios::in);
	*productCount = 0;
	if (database.is_open()) {
		string name;
		while (getline(database, name) && *productCount < MAX_PRODUCTS) //removing eof() check to prevent reading empty lines
		{
			if (name.empty()) break;// To avoid reading empty lines at the end due to eof()
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
	for (int i = 0; i < *productCount; i++)
	{
		if (products[i].quantity != 0)
			cout << i + 1 << ". " << products[i].name << " - PKR " <<fixed<<setprecision(1)<< products[i].price << " - Quantity: " << products[i].quantity << "\n";
		else
			cout << i + 1 << ". " << products[i].name << " - PKR " << products[i].price << " - Out of Stock\n";
	}
	return *productCount > 0;
}
void selectproduct(product* products, int* productCount, bool isviewonly)
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
	database.open("Productdb.txt", ios::out | ios::trunc);
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
void addrecipts(product* products, string* address)
{
	fstream receiptFile;
	receiptFile.open("Receiptsdb.txt", ios::out | ios::app);
	if (receiptFile.is_open()) {
		receiptFile << "New Receipt:\n";
		for (int i = 0; i < cartIndex; i++) {
			double itemTotal = cart[i].price * cart[i].quantity;
			receiptFile << products[cart[i].id].sellerPhone << ' ' << cart[i].name << " - PKR " << cart[i].price << " x " << cart[i].quantity << " = PKR " << itemTotal << " Address: " << *address << "\n";
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
	cin.ignore();
	getline(cin, address);
	cout << "Proceeding to payment...\n";
	cout << "Thank you for your purchase!\n";
	// Clear the cart after checkout
	updateproducts(products);
	addrecipts(products, &address);
	cartIndex = 0;
}
void fulfillorders(user* currentUser, int* linecount) //line count is total orders for that seller
{
	fstream receiptFile, tempFile;
	receiptFile.open("Receiptsdb.txt", ios::in);
	tempFile.open("TempReceiptsdb.txt", ios::out);
	if (receiptFile.is_open() && tempFile.is_open())
	{
		int tempLineCount;
		cout << "Enter the order line no. you want to fulfill:\n";
		cin.ignore();
		cin >> tempLineCount;
		string line;
		string orderToFulfill;
		while (tempLineCount < 1 || tempLineCount > *linecount || cin.fail())
		{
			cout << "Invalid line number! Please enter a valid line number between 1 and " << *linecount << ":\n";
			cin.ignore();
			cin.clear();
			cin >> tempLineCount;
		}
		int choicecount = 0;
		while (choicecount != tempLineCount)
		{
			getline(receiptFile, line);

			if (line.find(currentUser->phone) != string::npos&& line.find("Delivered") == string::npos) //find returns npos if not found
			{
				orderToFulfill = line;
				choicecount++;
			}
		}
		receiptFile.clear();
		receiptFile.seekg(0, ios::beg);// Reset file pointer to beginning
		bool orderFound = false;
		while (getline(receiptFile, line))
		{
			if (line == orderToFulfill)
			{
				orderFound = true;
				cout << "Order fulfilled: " << line << "\n";
				tempFile << line << " -- Delivered!\n";
				continue; // Skip writing this line to the temp file
			}
			tempFile << line << "\n";
		}
		receiptFile.close();
		tempFile.close();
		if (orderFound) {
			// Replace original file with updated temp file
			remove("Receiptsdb.txt");
			rename("TempReceiptsdb.txt", "Receiptsdb.txt");
		}
		else {
			cout << "Order not found or does not belong to you.\n";
			remove("TempReceiptsdb.txt"); // Clean up temp file
		}
	}
	else {
		cout << "Error opening receipt file!\n";
	}
}

void getordersforsellers(user* currentUser)
{
	fstream receiptFile;
	int linecount = 0;
	receiptFile.open("Receiptsdb.txt", ios::in);
	if (receiptFile.is_open())
	{
		string line;
		cout << "Orders for your products:\n";
		while (getline(receiptFile, line))
		{
			if ((line.find(currentUser->phone) != string::npos)&& line.find("Delivered") == string::npos) //skip delivered
			{
				linecount++;
				cout << linecount << ". " << line << "\n";
			}
		}
		receiptFile.close();
	}
	else {
		cout << "Error opening receipt file!\n";
	}
	cout << "\n";
	if (linecount)
	{
		cout << "Do you want to fulfill any orders? (Y/N): ";
		bool isvalidchoice = false;
		char fulfillChoice;
		fulfillChoice = _getch();
		while (fulfillChoice != 'Y' && fulfillChoice != 'y' && fulfillChoice != 'N' && fulfillChoice != 'n')
		{
			if (!isvalidchoice)
			{
				cout << "Invalid Choice! Try Again! :\n";
				isvalidchoice = true;
			}
			fulfillChoice = _getch();
		}
		if (fulfillChoice == 'Y' || fulfillChoice == 'y')
			fulfillorders(currentUser, &linecount);
	}
	else
		cout << "No Items Ordered from your store, Press any key to continue";
	_getch();
}
void selleritems(user* currentUser)
{
	product products[MAX_PRODUCTS];
	int productCount = 0;
	int localproductCount = 0;
	loadProducts(products, &productCount); // Load all products and update productCount
	cout << "Your Products:\n";
	for (int i = 0; i < productCount; i++) {
		if (products[i].sellerPhone == currentUser->phone) {
			localproductCount++;
			cout << i + 1 << ". " << products[i].name << " - PKR " << fixed << products[i].price << " - Quantity: " << products[i].quantity << "\n";
		}
	}
	cout << "\n";
	cout << "Do you want to update the quantity of any product? (Y/N): ";
	bool isvalidchoice = false;
	char updateChoice;
	updateChoice = _getch();
	while (updateChoice != 'Y' && updateChoice != 'y' && updateChoice != 'N' && updateChoice != 'n')
	{
		if (!isvalidchoice)
		{
			cout << "Invalid Choice! Try Again! :\n";
			isvalidchoice = true;
		}
		updateChoice = _getch();
	}
	if (updateChoice == 'Y' || updateChoice == 'y')
	{
		int choice;
		cout << "Enter the number of the product you want to update: ";
		cin >> choice;
		while (choice < 1 || choice > localproductCount) {
			cout << "Invalid Choice! ";
			cout << "Enter the number of the product you want to update: ";
		}
		localproductCount = 0;
		int newQuantity;
		cout << "Enter the new quantity: ";
		cin >> newQuantity;
		for (int i = 0; i < productCount; i++) {
			if (products[i].sellerPhone == currentUser->phone)
			{
				localproductCount++;
				if (localproductCount == choice)
				{
					products[i].quantity = newQuantity;
					updateproducts(products);
					cout << "Product quantity updated successfully!\n";
					return;
				}
			}
		}
	}
}