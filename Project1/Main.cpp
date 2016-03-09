
#include <omp.h>
#include <iostream>
#include <vector>
#include <queue>
#include <array>
#include <fstream>
#include <string>
#include <time.h>
#include <ctime>

using namespace std;
//Global Variable declaration
int *fitData;
int n;
int mComp;
int mSwap;
bool sorted=false;
bool mergeSorted = false;
string name;

//forward declarations of functions
void Start(string fileName);
void End();
void commandParser(string command);
void Select(int number);
void mergesort(int p, int r);
void merge(int p, int q, int r);
void parallelMergeSort(int p, int r);
void Average();
void parallelAverage();
void InsertionSort();
int max();
int min();
int median();
double diffclock(clock_t clock1, clock_t clock2);


//main function
int main(){
	//local variable declaration
	string command="notExit";

	while (command != "Exit")//loops until user inputs Exit
		{
			getline(cin, command);
			commandParser(command);
		}
	cout << "Program Terminating";
}

double diffclock(clock_t clock1, clock_t clock2) { 
	double diffticks = clock1 - clock2;     
	double diffms = (diffticks * 1000) / CLOCKS_PER_SEC;
	return diffms; 
}


//takes in the name of the file, opens it and gets the size of the file
//the size of the file is stored in n and then the array fitData created with size n
void Start(string fileName){
	cout << "Processing fitness data of: " << fileName << "\n";
	
	name = fileName;
	int linecount = 0;
	int tempArray[700];
	int tempInt;
	string temp;
	std::string line;
    fstream file(fileName+".txt");
	getline(file, line);
	while (getline(file, line))
	{		
		file >> temp;
		tempInt = std::stoi(temp);
		tempArray[linecount]=tempInt;
		linecount++;
	}
	n = linecount;

	fitData = new int[n];
	for (int i = 0; i < n; i++)
	{
		fitData[i] = tempArray[i];
	}
	
}
//this function ends the use of the file
//frees the pointer (the array) allowing a new array to be made
//sets the sorted to false as well because there is no array to be sorted
//and the next time there is a new array it will be by default unsorted
void End(){
	cout << "End of processing fitness data for: "<<name<<"\n";
	delete fitData;
	cin.clear();
	cout.clear();
	fflush(stdin);
	sorted = false;
	mergeSorted = false;
}

//reads the commands the user inputs and processes them.
void commandParser(string command){
	if (command == "InsertionSort"){
		InsertionSort();
	}
	else if (command=="Select max"){
		Select(-1);
	}
	else if (command == "Select min"){
		Select(-2);
	}
	else if (command == "Select median"){
		Select(-3);
	}
	else if (command=="MergeSort"){
		clock_t begin = clock();
		mComp = 0;
		mSwap = 0;
		mergesort(0,n);
		clock_t end = clock();
		cout << "Number of comparisons made by merge sort:" << mComp << endl;
		cout << "Number of swaps made by merge sort:" << mSwap << endl;
		cout << "Time to run merge sort (ms):" << double(diffclock(end, begin)) << endl;
	}
	else if (command == "ParallelAverage"){
		parallelAverage();
	}
	else if (command == "ParallelMergeSort"){
		clock_t begin = clock();
		parallelMergeSort( 0, n);
		clock_t end = clock();
		cout << "Number of comparisons made by merge sort:" << mComp << endl;
		cout << "Number of swaps made by merge sort:" << mSwap << endl;
		cout << "Time to run merge sort (ms):" << double(diffclock(end, begin)) << endl;
	}
	else if (command == "Average"){
		Average();
	}
	else {
		string delim = " ";
		string token = command.substr(0, command.find(delim));
		if (token == "Select"){
			
			size_t pos = command.find(delim);
			token=command.erase(0, pos+delim.length());
			int number= std::stoi(token);
			Select(number);
			
		}
		else if (token == "Start"){
			size_t pos = command.find(delim);
			token = command.erase(0, pos + delim.length());
			
			Start(token);
		}
		else if (token == "End"){
			End();
		}
	}

}
void parallelAverage(){
	int iCPU = omp_get_num_procs();
	#pragma omp_set_num_threads(iCPU)
	clock_t begin = clock();
	int sum = 0;
	int i;
	int count=0;
	if (mergeSorted){
		#pragma omp parallel for
		for (i = 2; i < n - 1; i++){
			sum = sum + fitData[i];
			count++;
		}
	}
	else{
		#pragma omp parallel for
		for (i = 0; i < n; i++){
			sum = sum + fitData[i];
			count++;
		}
	}

	clock_t end = clock();
	cout << "Average number of steps: " << sum / count << endl;
	cout << "Time to run the parallel average (ms):" << double(diffclock(end, begin)) << endl;
}
void mergesort( int p, int r) {
	int q;
	if (p < r) {
		q = (p + r) / 2;
		mergesort(p, q);
		mergesort(q + 1, r);
		merge(p, q, r);
	}
}
void parallelMergeSort(int p, int r)
{
	int iCPU = omp_get_num_procs();
	if (p<r)
	{
		int q = (p + r) / 2;
#pragma omp parallel shared(p,q,r) num_threads(iCPU)
		{
			mergesort(p, q);
			mergesort(q + 1, r);
			merge(p, q, r);	
		} 
	}
}
void merge(int p, int q, int r) {
	int h, i, j, k, tempArray[700];
	h = p;
	i = p;
	j = q + 1;

	while (h <= q && j <= r) {
		if (fitData[h] <= fitData[j]) {
			tempArray[i] = fitData[h];
			h++;
			mSwap++;
		}
		else {
			tempArray[i] = fitData[j];
			j++;
			mSwap++;
		}
		i++;
		mComp++;
	}
	if (h > q) {
		for (k = j; k <= r; k++) {
			tempArray[i] = fitData[k];
			i++;
		}
	}
	else {
		for (k = h; k <= q; k++) {
			tempArray[i] = fitData[k];
			i++;
		}
	}mComp++;

	// Write the final sorted array to our original one
	for (k = p; k < r; k++) {
		if ((tempArray[k] > 0 && tempArray[k-1]!=tempArray[k])){
			
				fitData[k] = tempArray[k];
			}
	}
	sorted = true;
	mergeSorted = true;
}
//Select function, calculates max, min, and median(middle)
//only calculates if the array has already been sorted
void Select(int number ){
	if (sorted){
		if (number>n){
			cout << "Invalid selection." << endl;
		}else if (number == -1){
			cout<<"The max number of steps is: "<<max()<<endl; 
		}
		else if (number == -2){
			cout<<"The min number of steps is: "<<min()<<endl; 
		}
		else if (number == -3){
			cout<<"The median number of steps is: "<<median()<<endl;
		}
		else if (number >= 0){
			clock_t begin = clock();
			cout << "Selecting item: " << fitData[number - 1] << endl;
			clock_t end = clock();
			cout << "Time to run the selection (ms):" << double(diffclock(end, begin)) << endl;
		}
	}
	else{
		cout << "Unable to select from an unsorted array.\n";
		
	}
	
}
//returns max value from array
int max(){
	int max=0;
	for (int i = 0; i < n; i++){
		if (fitData[i]>max)
			max = fitData[i];
	}
	return max;
}
int median(){
	int middle = n / 2;
	return fitData[middle];
}
//returns min value from array
int min(){
	int min = 10000000;
	for (int i = 0; i < n; i++){
		if (fitData[i]<min)
			min = fitData[i];
	}
	return min;
}

void Average(){
	clock_t begin = clock();
	int sum = 0;
	int i;
	if (mergeSorted){
		for (i = 2; i < n-1; i++){
			sum = sum + fitData[i];
		}
	}
	else{
		for (i = 0; i < n; i++){
			sum = sum + fitData[i];
		}
	}
	
	clock_t end = clock();
	cout << "Average number of steps: " << sum/i << endl;
	cout << "Time to run the average (ms):" << double(diffclock(end, begin))<<endl;
}
//sorts the elements in the array
void InsertionSort()
{
	clock_t begin = clock();
	int comparison = 0;
	int swaps = 0;
	
	int key;

	for (int i = 1; i < n; i++)
	{
		for (int j = i; j >= 1; j--)
		{	
			comparison++;
			if (fitData[j] < fitData[j - 1])
			{
				key = fitData[j];
				fitData[j] = fitData[j - 1];
				fitData[j - 1] = key;
				swaps++;
			}
			else
				break;
		}
		

	}
	sorted = true;
	clock_t end = clock();
	cout << "Number of comparisons made by insertion sort: " << comparison << endl;
	cout << "Number of swaps made by insertion sort:  " << swaps << endl;
	cout << "Time to run insertion sort (ms): " << double(diffclock(end, begin)) << endl;
}

