#pragma once
#include <cstdlib>
#include <fstream>
#include <string>
#include <iostream>
#include <conio.h>
#include <iomanip>
#include <limits>
#include <vector>

struct User 
{
	std::string phone;
	std::string name;
	std::string password;
	bool acctype = false; // true for seller, false for buyer
};

struct Product
{
	int id;
	std::string name;
	std::string description;
	double price;
	int quantity;
	std::string sellerPhone;
};

const int MAX_PRODUCTS = 100;
extern Product cart[MAX_PRODUCTS];
extern int cartIndex;

inline void DisplayHeader()
{
	system("cls");
	std::cout <<  "========= Project Market Place ========="  << std::endl;
}

inline void Welcome(const std::string* userName)
{
	DisplayHeader();
	std::cout << "Welcome to the Market Place, " << *userName << " !, " << "Press any key to continue !" << std::endl;
}

void GetOrdersForSellers(User* currentUser);
void SellerItems(User* currentUser);
void FulfillOrders(User* currentUser, int* lineCount); 
void Checkout(Product* products);
void AddReceipts(Product* products, std::string* address);
void UpdateProducts(Product* products);
void SelectProduct(Product* products, int* productCount, bool isViewOnly);
bool ViewList(Product* products, int* productCount);
void AddToCart(Product* selectedProduct, int quantity);
void LoadProducts(Product* products, int* productCount);
void CreateProduct(User* currentUser);
void LoginOrRegister(User* currentUser);
void WriteToDatabase(User* newUser);
bool CheckDatabaseForUser(const std::string* phone, const std::string* password, std::string* username, bool* accType);
void ViewerOnlyLogic();
void LoginMainLogic();
void CheckDatabase();