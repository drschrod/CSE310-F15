#include "Sort.h"
#include <iostream>
#include <vector>
#include <queue>
#include <array>
#include <fstream>
#include <string>

using namespace std;
int A[];
int n = 0;

int main(){
	cout << "hello";
}
int Start(){

}
int End(){


}
/*void toArray(string fileName){
	
	
	ifstream file_(fileName);
	std::string line;
	
	if (file_.is_open())
	{
		while (getline(file_, line))
		{
			A[n] = std::stoi(line);
			n++;
		}file_.close();
	}
	
}*/
void InsertionSort(string fitnessData)
{
	
	int key;
		for (int j = 2; j <= n ;j++)
		{
			key = A[j];
			int i = j - 1;
			while (i > 0 && A[i] > key)
				{
					A[i + 1] = A[i];
					i = i - 1;
				}
			A[i + 1] = key;
		}
		
}


	

/*void MergeSort(int fitData[],int p, int r ){

	
	int q = (p + r) / 2;
	MergeSort(A, p, q);
	MergeSort(A, q + 1, r);
	

}
void parallelMergeSort(int array[]){

}
*/
