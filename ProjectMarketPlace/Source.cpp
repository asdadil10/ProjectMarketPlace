#include "Header.h"

int main() {
	user User;
	LoginOrRegister(&User);
	if (User.acctype) // Seller
	{
		cout << "======== Seller Menu =======\n";
		cout << "Do want to :\n1.Add a new item to sell\n2.View Items oredered from your store";
		short choice;
		cin >> choice;
		while (choice < 1 || choice > 2)
		{
			cout << "Invalid choice. Please enter 1 or 2: ";
			cin >> choice;
		}
		if (choice == 1)
			createProduct(&User);
		else if (choice == 2)
			getordersforsellers(&User);
		cout << "Do you want to go home! (yes/no) or exit?: ";
		string homeChoice;
		cin >> homeChoice;
		while (homeChoice != "yes" && homeChoice != "no" && homeChoice != "exit")
		{
			cout << "Invalid choice. Please enter 'yes', 'no' or 'exit': ";
			cin >> homeChoice;
		}
		if (homeChoice == "yes")
			choice = 1; // Loop back to seller menu
		else if (homeChoice == "no")
			choice = 0; // Exit logic
		else if (homeChoice == "exit")
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
				selectproduct(products, &productCount);
			}
			else
				cout << "No products available at the moment!\n";
			cout << "Do you want to continue shopping? (yes/no): ";
			string continueChoice;
			cin >> continueChoice;
			while (continueChoice != "yes" && continueChoice != "no")
			{
				cout << "Invalid choice. Please enter 'yes' or 'no': ";
				cin >> continueChoice;
			}
			if (continueChoice == "no")
				break;
		}
		checkout(products);
	}
	return 0;
}