#pragma once
#include <cstdlib>
#include <fstream>
#include <string>
#include <iostream>
#include <conio.h>
#include <iomanip>
#include <limits>

struct User 
{
	std::string phone;
	std::string name;
	std::string password;
	bool acctype = false; // true for seller, false for buyer
};

struct product
{
	int id;
	std::string name;
	std::string description;
	double price;
	int quantity;
	std::string sellerPhone;
};

const int MAX_PRODUCTS = 100;
product cart[MAX_PRODUCTS];
int cartIndex = 0;
using namespace std;

inline void DisplayHeader()
{
	system("cls");
	cout <<  "========= Project Market Place ========="  << endl;
}
inline void Welcome(string* UserName)
{
	DisplayHeader();
	cout << "Welcome to the Market Place, " << *UserName << " !, " << "Press any key to continue !" << endl;
}
void getordersforsellers(User* currentUser);
void selleritems(User* currentUser);
void fulfillorders(User* currentUser, int* linecount); //line count is total orders for that seller
void checkout(product* products);
void addrecipts(product* products, string* address);
void updateproducts(product* products);
void selectproduct(product* products, int* productCount, bool isviewonly);
bool viewlist(product* products, int* productCount);
void addtocart(product* selectedProduct, int quantity);
void loadProducts(product* products, int* productCount);
void createProduct(User* currentUser);
void LoginOrRegister(User* currentUser);
inline void WritetoDatabase(User* newUser);
bool CheckDatabaseForUser(const string* phone, const string* password, string* Username, bool* acctype);
void Vieweronlylogic();
void Loginmainlogic();
void checkdb(); //check if db files exist else create them