#pragma once
#include <cstdlib>
#include <fstream>
#include <string>
#include <iostream>

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
bool IsValidCNIC(const string* cnic)
{
	if (cnic->length() != 13) return false;
	for (char c : *cnic) {
		if (!isdigit(c)) return false;
	}
	return true;
}
bool CheckDatabaseForUser(const string* phone, const string* password, string* username, bool* acctype)
{
	fstream database;
	database.open("UserDatabase", ios::in);
	if (database.is_open()) {
		string line;
		while (getline(database, line))
		{
			string dbName;
			getline(database, dbName);
			string dbPassword;
			getline(database, dbPassword);
			database >> *acctype;
			if (line == *phone && dbPassword == *password) {
				database.close();
				*username = dbName;
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
	int choice;
	cin >> choice;
	while (choice != 1 && choice != 2)
	{
		DisplayHeader();
		cout << "Invalid choice! Please enter 1 to Register or 2 to Login.\n";
		cin >> choice;
	}
	if (choice == 1)
	{
		cout << "Registering new user...\n";
		cout << "Are you a Seller or Buyer? :";
		string accType;
	reg:
		cin >> accType;
		if (accType == "Seller" || accType == "seller")
			currentUser->acctype = true;
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
	else if (choice == 2)
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