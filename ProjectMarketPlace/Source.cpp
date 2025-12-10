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
	home:
		DisplayHeader();
		cout << "======== Seller Menu =======\n";
		cout << "Do want to :\n1.Add a new item to sell\n2.View Items oredered from your store\n3.View your items";
		isvalidchoice = false;
		char choice;
		choice = _getch();
		while (choice < '1' || choice > '3')
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
		cout << "Do you want to go home! (Y) or logout? (N) or exit(x)?: ";
		isvalidchoice = false;
		char homeChoice;
		homeChoice = _getch();
		while (homeChoice != 'Y' && homeChoice != 'y' && homeChoice != 'N' && homeChoice != 'n' && homeChoice != 'X' && homeChoice != 'x')
		{
			if (!isvalidchoice)
			{
				cout << "Invalid Choice! Try Again! :\n";
				isvalidchoice = true;
			}
			homeChoice = _getch();
		}
		if (homeChoice == 'Y' || homeChoice == 'y')
			goto home; // Loop back to seller menu
		else if (homeChoice == 'N' || homeChoice == 'n')
			return; // Exit logic
		else if (homeChoice == 'X' || homeChoice == 'x')
			exit(0);
	}
	else // Buyer
	{
		product products[MAX_PRODUCTS];
		int productCount = 0;
		loadProducts(products, &productCount);
		while (true)
		{
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
	cout << "\nDo you want to login or register to shop or sell (Y)? or go to main menu (N): \n";
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
		cout << "1.Login \n2.View products without login \n0.exit \n";
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