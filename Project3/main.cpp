//
//  main.cpp
//  Project3
//
//  Created by Dylan Schroder on 11/17/15.
//  Copyright © 2015 Dylan Schroder. All rights reserved.
//


#include<list>
#include<iostream>
#include<vector>
#include<stack>
#include<limits>
#include<algorithm>
using namespace std;


void DFS(int v);
void DFSUtil(int v, bool visited[]);
void dijkstra(int source,int target);
void BFS(int nVertex,bool visited[]);
void fwDiameter();
bool isReachable(int s, int d);
bool ccBeenDone(int vertex);
int minDistance(int dist[], bool sptSet[]);
void insertionSort(int arr[], int length);
void printBFS();
int numVertex, numEdges, CCcount, s, t, numComp,cSize,componentSize;
int num=0;
int distance=0;
int *cSizeArray;
list<int> *adj; //creates new list that stores the Vertices // stores the adjacency list
vector<int>check;
vector <int>ccSizes;
vector<int>shortPath;

bool *visited = new bool[numVertex];

int main(){
	//int numVertex, numEdges;
	cin >> numVertex; // read the number of vertex
	cin >> numEdges; // read the number of edges
	adj = new list<int>[numVertex];
	
	//form the adjacency list
	for(int index = 0; index < numEdges; index++){
		//int s, t;
		cin >> s >> t;
		adj[s].push_back(t);
		adj[t].push_back(s);
	}
	
	
	cout << "\n \n \n";
	int numComm;
	cin>>numComm;
	string command;
	for(int i=0;i<numComm;i++){
		cin>>command;
		if(command=="degree-distribution"){
			int* degreeVertex = new int[numVertex];// stores the degree of each vertex
			
			//compute degree of each node and display it in column format
			//column 1 is the node
			//column 2 is the degree of that node
			cout<<"~~~~~~~~~~Degree Distribution~~~~~~~~~~"<<endl;
			cout<<"Node/Vertex |"<<"\t"<<"Degree"<<endl;
			for(int index = 0; index < numVertex; index++){
				degreeVertex[index] = adj[index].size();
				cout << "\t\t"<<index << "\t|\t" << degreeVertex[index] << "\n";//prints out the degree of each node
			}
			cout<<"~~~~~~~~~~End Degree Distribution~~~~~~~~~~"<<endl;
			
		}else if(command=="components"){
			cout << "\n \n \n";
			cout<< "~~~~~~~~~~~Connected Components~~~~~~~~~~"<<endl;
			componentSize = 0;
			cSizeArray = new int[numVertex];
			int sizeCounter =0;
			//print the connected components
			bool *visited = new bool[numVertex];//keep track of visited vertex
			//intialize all vertex visited to false
			for(int v = 0; v < numVertex; v++)
			{
				visited[v] = false;
				cSizeArray[v] = -1;
			}
			
			//compute connected component
			for (int i = 0; i < numVertex; i++)
			{
				if (visited[i] == false)
				{
					componentSize++;
					// print all reachable vertices from vertex
					BFS(i, visited);
					cSizeArray[sizeCounter]= cSize;// cSize is how many nodes connected
					sizeCounter++;
				}
			}
			printBFS();
			cout<< "~~~~~~~~~~~End of Connected Components~~~~~~~~~~"<<endl;
		}else if(command=="diameter"){
			fwDiameter();
		}else if(command=="shortest-path"){
			cin>>s>>t;
			if(isReachable(s, t))
				dijkstra(s, t);
			else
				cout<<"There is no path between "<<s<<" and "<<t<<endl;
			
		}
	}
}
//Recursive DFS function
void DFSUtil(int v, bool visited[])
{
	// Mark the current node as visited and print it
	visited[v] = true;
	CCcount++;
	check.push_back(v);
	//cout << v << " ";
	
	// Recur for all the vertices adjacent to this vertex
	list<int>::iterator i;
	for (i = adj[v].begin(); i != adj[v].end(); ++i)
		if (!visited[*i])
			DFSUtil(*i, visited);
}

// DFS traversal of the vertices reachable from v. It uses recursive DFSUtil()
void DFS(int v)
{
	// Mark all the vertices as not visited
	
	for (int i = 0; i < numVertex; i++)
		visited[i] = false;
 
	// Call the recursive helper function to print DFS traversal
	DFSUtil(v, visited);
	ccSizes.push_back(CCcount);
	//cout<<endl<<"Size: "<<CCcount;
	CCcount=0;
}

//checks if the connected components have been
bool ccBeenDone(int vertex){
	for(int i=0;i<check.size();i++){
		if(vertex==check.at(i))
			return false;
	}
	return true;
}
bool isReachable(int s, int d)
{
	// Base case
	if (s == d)
		return true;
 
	// Mark all the vertices as not visited
	bool *visited = new bool[numVertex];
	for (int i = 0; i < numVertex; i++)
		visited[i] = false;
 
	// Create a queue for BFS
	list<int> queue;
 
	// Mark the current node as visited and enqueue it
	visited[s] = true;
	queue.push_back(s);
 
	// it will be used to get all adjacent vertices of a vertex
	list<int>::iterator i;
 
	while (!queue.empty())
	{
		// Dequeue a vertex from queue and print it
		s = queue.front();
		queue.pop_front();
		
		// Get all adjacent vertices of the dequeued vertex s
		// If a adjacent has not been visited, then mark it visited
		// and enqueue it
		for (i = adj[s].begin(); i != adj[s].end(); ++i)
		{
			// If this adjacent node is the destination node, then
			// return true
			if (*i == d)
				return true;
			
			// Else, continue to do BFS
			if (!visited[*i])
			{
				visited[*i] = true;
				queue.push_back(*i);
			}
		}
	}
	
	// If BFS is complete without visiting d
	return false;
}
//prints out the shortest path
void dijkstra(int source,int target)
{
	//create an array to hold previous nodes
	int *previous=new int[numVertex];
	vector<int>final;
	// Mark all the vertices as not visited
	bool *visited = new bool[numVertex];
	for (int i = 0; i < numVertex; i++)
	{
		visited[i] = false;
		previous[i]=-1;
	}
	// Create a queue for BFS
	list<int> Q;
 
	// Mark the source node as visited and enqueue it
	visited[s] = true;
	Q.push_back(s);
 
	// it will be used to get all adjacent vertices of a vertex
	list<int>::iterator i;
	
	//slowly empty the queue
	while (!Q.empty())
	{
		// Dequeue a vertex from queue and print it
		s = Q.front();
		Q.pop_front();
		
		for (i = adj[s].begin(); i != adj[s].end(); ++i)
		{
			//if the node has not been visited then:
			if (!visited[*i]) {
				visited[*i]=true;	//mark it as visited
				previous[*i]=s;		//add the previous connected node to the list
				Q.push_back(*i);	//push back queue
			}
		}
	}
	int v=target;
	int pos=0;
	//add the array to a vector so it can be printed in the correct order
	//if you print the array it would print the path in reverse
	while(previous[v]!=-1){
		//cout<<v<<"->";
		final.push_back(v);
		v=previous[v];
		pos++;
	}
	reverse(final.begin(), final.end());
	cout<<source<<"->";
	for (int k=0; k<pos; k++) {
		cout<<final.at(k);
		if(k+1!=pos){
			cout<<"->";
		}
	}
	cout<<endl;
}
void fwDiameter(){
	
	/* dist[][] will be the output matrix that will finally have the shortest
	 distances between every pair of vertices */
	int dist[numVertex][numVertex], i, j, k;
	int m;
	/* Initialize the solution matrix same as input graph matrix. Or
	 we can say the initial values of shortest distances are based
	 on shortest paths considering no intermediate vertex. */
	list<int> Q;
	while(!Q.empty()){
		m = Q.front();
		Q.pop_front();
		for (list<int>::iterator i = adj->begin(); i != adj->end(); ++i)
		{
			for (list<int>::iterator j = adj->begin(); j != adj->end(); ++j)
				if (*i>0) {
					dist[*i][*j]=1;
					Q.push_back(*i);
				}
				else{
					dist[*i][*j]=0;
					Q.push_back(*i);
				}
		}
 }
	/* Add all vertices one by one to the set of intermediate vertices.
	 ---> Before start of a iteration, we have shortest distances between all
	 pairs of vertices such that the shortest distances consider only the
	 vertices in set {0, 1, 2, .. k-1} as intermediate vertices.
	 ----> After the end of a iteration, vertex no. k is added to the set of
	 intermediate vertices and the set becomes {0, 1, 2, .. k} */
	for (k = 0; k < numVertex; k++)
	{
		// Pick all vertices as source one by one
		for (i = 0; i < numVertex; i++)
		{
			// Pick all vertices as destination for the
			// above picked source
			for (j = 0; j < numVertex; j++)
			{
				// If vertex k is on the shortest path from
				// i to j, then update the value of dist[i][j]
				if (dist[i][k] + dist[k][j] < dist[i][j])
					dist[i][j] = dist[i][k] + dist[k][j];
			}
		}
	}
	
			cout<<dist[numVertex][numVertex]<<endl;
	
	printf("\n");

}
void BFS(int s, bool visited[])
{
	
	cSize = 1;//connected size
	list<int> q;
	
	list<int>::iterator i;
	
	visited[s] = true;
	
	q.push_back(s);
	
	while (!q.empty())
	{
		
		s = q.front();
		
		q.pop_front();
		
		for(i = adj[s].begin(); i != adj[s].end(); ++i)
		{
			
			if(!visited[*i])
			{
				visited[*i] = true;
				cSize++;
				q.push_back(*i);
			}
		}//end for
		
	}//end while
}

void insertionSort(int arr[], int length)
{
	
	int i,j,temp;
	
	//t1 = clock();
	for( i=1; i<length; i++)
	{//compare
		j=i;
		while(j > 0 && arr[j-1] > arr[j])
		{//swap
			temp = arr[j];
			arr[j] = arr[j-1];
			arr[j-1] = temp;
			j--;
		}// end while
		
	}
	
}
void printBFS()
{
	cout<<componentSize<<endl;
	insertionSort(cSizeArray, componentSize);
	for(int i=0; i < componentSize; i++)
	{
		if(cSizeArray[i] != 0)
			cout<<cSizeArray[i]<<endl;
	}
}
