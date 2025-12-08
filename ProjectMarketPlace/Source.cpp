#include "Header.h"
bool isvalidchoice = false;
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
		cout << "Do want to :\n1.Add a new item to sell\n2.View Items oredered from your store";
		isvalidchoice = false;
		char choice;
		choice = _getch();
		while (choice < '1' || choice > '2')
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
	cout << "Do you want to login or register to shop or sell? (Y) or go to main menu (N): \n";
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

		if (loginChoice == 'Y' || loginChoice == 'y')
		{
			Loginmainlogic();
			break;
		}
		else if (loginChoice == 'N' || loginChoice == 'n')
		{
			break;
		}
	}

}
int main() {
	while (true) 
	{
		DisplayHeader();
		cout << "Do, you want to login(Y/y) \nor just view products(N/n) \nor exit(x)? : \n";
		isvalidchoice = false;
		char viewChoice;
		viewChoice = _getch();
		while (viewChoice != 'Y' && viewChoice != 'y' && viewChoice != 'N' && viewChoice != 'n' && viewChoice != 'X' && viewChoice != 'x')
		{
			if (!isvalidchoice)
			{
				cout << "Invalid Choice! Try Again! :";
				isvalidchoice = true;
			}
			viewChoice = _getch();
		}
		if (viewChoice == 'Y' || viewChoice == 'y')
			Loginmainlogic();
		else if (viewChoice == 'N' || viewChoice == 'n')
			Vieweronlylogic();
		else if (viewChoice == 'X' || viewChoice == 'x')
			break;
	}
	return 0;
}