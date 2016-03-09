#include<stdio.h>
#include "stdafx.h"
#include<iostream>
#include <vector>
#include <string>
using namespace std;


#define	CAT_NAME_LEN	25
#define	APP_NAME_LEN	50
#define	VERSION_LEN	10
#define	UNIT_SIZE	3

struct hash_table_entry *hashTable;
int hTSize;


//~~~~~~~~~~~~~~Structs~~~~~~~~~~~~~~~~~~~~~~~~~~
struct categories{
	string category; // Name of category
	struct tree *root;  // Pointer to root of search tree for this category
};

/*struct tree{ // A binary search tree
	struct app_info *info; // Information about the application
	struct tree *left;  // Pointer to the left subtree
	struct tree *right;  // Pointer to the right subtree
};*/

struct app_info{
	string category; // Name of category
	string app_name; // Name of the application
	string version; // Version number
	float size; // Size of the application
	string units; // GB or MB
	float price; // Price in $ of the application
};

struct hash_table_entry{
	string app_name; // Name of the application
	struct tree *app_node; // Pointer to node in tree containing the application information
};



// This definition of the tree is to be used for the full project deadline.

struct tree{ // A 2-3 tree
struct app_info* info; // Information about the application
struct tree *left;  // Pointer to the left subtree
struct tree *mid; // Pointer to the middle subtree
struct tree *right;  // Pointer to the right subtree
};



//**************Tree Functions and constructors************************

tree* catTree(app_info* aInfo){
	tree* newNode = new tree();
	newNode-> info=aInfo;
	newNode->left = NULL;
	newNode->mid = NULL;
	newNode->right = NULL;
	return newNode;
}
void Insert(tree* root, tree* aInfo) {
	if (root == NULL) { // empty tree
		
		root=aInfo;
		return ;
	}
	// if data to be inserted is lesser, insert in left subtree. 
	else if (root->left==NULL) {
		//root->left->info=aInfo;
		Insert(root->left, aInfo);
		return;
	}
	 
	else if(root->mid==NULL){
		Insert(root->mid, aInfo);
		return;
	}
	else{
		//root->left->info = aInfo;
		Insert(root->right, aInfo);
		return;
	}
	//return root;
}
//2-3 Tree Functions

//`````````````````````````````````````````
void printAppData(tree* root);
void range(string s);
void removal(string s);
void find(string s);
int asciiSum(string name);
int linProbe(int pos, int size, hash_table_entry *table);
void addHash(int pos, int size, hash_table_entry *table, tree *app);
int getHash(int num, int size);
int findPrime(int num);
bool isPrime(int num);
void SearchApp(tree *node, string name);
void Insert(tree* root, tree* aInfo);

//std::vector<categories> catArray;
categories *catArray;
int main()
{
	int n=0;
	int arraySize;
	
	string category;
	printf("Input number of Categories\n");
	cin >> n;
	catArray = new categories[n];
	for (int i = 0; i < n; i++){
		//takes in all the categories and stores them into a vector array
		cin >> category;
		categories temp;
		temp.category= category;
		temp.root = NULL;
		catArray[i].category=temp.category;
		catArray[i].root = temp.root;
		arraySize = n;
		// creates a binary tree for these categories
							//~~~~~~~~~~~~~~~~~~debug~~~~~~~~~~~~~~~
							//cout << "myvector stores " << int(catArray.size()) << " numbers.\n";
							//string tem = catArray[i].category;
							//cout <<tem ;
	}
	printf("Input number of apps\n");
	cin >> n;
	hTSize =findPrime( n*2);
	hashTable = new hash_table_entry[hTSize];
	for (int i = 0; i < n; i++){
		tree * app = new tree();
		app_info *info = new app_info;
		printf("Input category\n");
		cin >> info->category;
		printf("Input app name\n");
		cin >> info->app_name;
		printf("Input version\n");
		cin >> info->version;
		printf("Input size\n");
		cin >> info->size;
		printf("Input unit\n");
		cin >> info->units;
		printf("Input price\n");
		cin >> info->price;
		app->info = info;
		int pos = getHash(asciiSum(info->app_name), hTSize);
		addHash(pos, hTSize, hashTable, app);
		
		for (int j = 0; j < arraySize; j++)
		{
			if (catArray[j].category == app->info->category){
				printf("Adding!\n");
				Insert(catArray[j].root, app);			
			}
		}

	}
	printf("Input number of commands\n");
	cin >> n;
	char command[100];
	string comm;
	
	for (int i = 0; i < n; i++){
		cout << "Put in your command" << endl;
		cin.ignore();
		cin.getline(command,100);
		comm = command;
		cout << comm<<endl;
		if (comm.find("find")!=std::string::npos){
			find(comm);
		}
		else if (comm.find("range") != std::string::npos){
			
			range(comm);
		}
		else if (comm.find("delete") != std::string::npos){
			
			removal(comm);
		}


	}
	
}
//Hash Table Functions~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void addHash(int pos, int size, hash_table_entry *table, tree *app){
	if (table[pos].app_name == "")
	{
		//cout << app->info->app_name;
		table[pos].app_name = app->info->app_name;
		table[pos].app_node = app;
	}
	else{
		int newPos = linProbe(pos, size, table);
		table[newPos].app_name = app->info->app_name;
		table[newPos].app_node = app;
	}

}
int linProbe(int pos, int size, hash_table_entry *table){
	bool rFull = false;
	bool lFull = false;
	for (int i = pos; i < size; i++){
		if (table[i].app_name == "")
			return i;
		rFull = true;
	}
	for (int i = 0; i < pos; i++){
		if (table[i].app_name == "")
			return i;
		lFull = true;
	}
	if (lFull && rFull){
		cout << "Hash table full";
	}


}
int getHash(int num, int size){
	return num%size;
}
//number will be used to calc the location to put the app
int asciiSum(string name){
	int sum = 0;
	for (int i = 0; i < name.size(); i++) {
		sum += name[i];
	}
	return sum;
}
int findPrime(int num){
	for (; !isPrime(num); num++);
	return num;
}
bool isPrime(int num){
	if ((num % 2) == 0){
		return true;
	}else return false;
}
//End Hash Table Functions~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void find(string s){
	
	string delim = "\"";
	size_t pos1 = s.find(delim);
	size_t pos2 = s.find(delim, pos1 + 1);
	string query = s.substr(pos1+1, pos2-1);
	bool found = false;
	if (s.find("app") != std::string::npos){
		
		
		for (int j = 0; j < hTSize; j++)
		{
			if ((hashTable[j].app_name+"\"") == query){
				cout << hashTable[j].app_node->info->app_name << "\n";
				cout << hashTable[j].app_node->info->category << "\n";
				cout << hashTable[j].app_node->info->price << "\n";
				cout << hashTable[j].app_node->info->size;
				cout << hashTable[j].app_node->info->units << "\n";
				cout << hashTable[j].app_node->info->version << "\n";
				found = true;
			}
			if (!found)cout << "App Not Found" << endl;
		}
		
	}
	else if (s.find("category") != std::string::npos){
		for (int j = 0; j < hTSize; j++)
		{
			if ((hashTable[j].app_node->info->category+"\"") == query){
				cout << hashTable[j].app_node->info->app_name << "\n";
				cout << hashTable[j].app_node->info->category << "\n";
				cout << hashTable[j].app_node->info->price << "\n";
				cout << hashTable[j].app_node->info->size;
				cout << hashTable[j].app_node->info->units << "\n";
				cout << hashTable[j].app_node->info->version << "\n";
				found = true;
			}
		}
		if (!found)cout << "Category Not Found" << endl;
	}
	else if (s.find("price") != std::string::npos){
		for (int j = 0; j < hTSize; j++)
		{
			if ((hashTable[j].app_node->info->price)==0){
				cout << hashTable[j].app_node->info->app_name << "\n";
				cout << hashTable[j].app_node->info->category << "\n";
				cout << hashTable[j].app_node->info->price << "\n";
				cout << hashTable[j].app_node->info->size;
				cout << hashTable[j].app_node->info->units << "\n";
				cout << hashTable[j].app_node->info->version << "\n";
			}
		}
	}

}
void range(string s){
	string delim = "\"";
	size_t pos1 = s.find(delim);
	size_t pos2 = s.find(delim, pos1 + 1);
	string query = s.substr(pos1 + 1, pos2 - 1);
	int lowPrice = std::stoi(s.substr(pos2 + 2, pos2 + 5));
	int highPrice = std::stoi(s.substr(pos2 + 6, s.length()-1));
	if (s.find("price") != std::string::npos){
		app_info *arr = new app_info[hTSize];
		for (int i = 0; i < hTSize; i++){
			if ((hashTable[i].app_node->info->price)<highPrice && (hashTable[i].app_node->info->price)>lowPrice && (hashTable[i].app_node->info->category+"\"")==query){
				cout << hashTable[i].app_node->info->app_name << "\n";
				cout << hashTable[i].app_node->info->category << "\n";
				cout << hashTable[i].app_node->info->price << "\n";
				cout << hashTable[i].app_node->info->size;
				cout << hashTable[i].app_node->info->units << "\n";
				cout << hashTable[i].app_node->info->version << "\n";
			}


		}
	}
	else if (s.find("app") != std::string::npos){

	}
}
void removal(string s){
	string delim = " ";
	string token = s.substr(0, s.find(delim));
}
void SearchApp(tree *node, string name) {

	if (node->info->app_name == name) {
		printAppData(node);
	}
}
void printAppData(tree* root){
	cout << root->info->app_name << "\n";
	cout << root->info->category << "\n";
	cout << root->info->price << "\n";
	cout << root->info->size;
	cout << root->info->units << "\n";
	cout << root->info->version << "\n";
}

