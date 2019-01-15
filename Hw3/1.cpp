// C++ program for implementation of Ford Fulkerson algorithm 
#include <iostream> 
#include <limits.h> 
#include <string.h> 
#include <queue>
#include <vector> 
#include <fstream>
//#define V 6
using namespace std; 

// Number of vertices in given graph 
// Returns true if there is a path from source 's' to sink 't' in 
//residual graph. Also fills parent[] to store the path

bool bfs(int **rGraph, int V, int s, int t, int parent[]) 
{ 
	// Create a visited array and mark all vertices as not visited 
	bool visited[V]; 
	memset(visited, 0, sizeof(visited)); 

	// Create a queue, enqueue source vertex and mark source vertex as visited 
	queue <int> q; 
	q.push(s); 
	visited[s] = true; 
	parent[s] = -1; 

	// Standard BFS Loop 
	while (!q.empty()) 
	{ 
		int u = q.front(); 
		q.pop(); 

		for (int v=0; v<V; v++) 
		{ 
			if (visited[v]==false && rGraph[u][v] > 0) 
			{ 
				q.push(v); 
				parent[v] = u; 
				visited[v] = true; 
			} 
		} 
	} 

	// If we reached sink in BFS starting from source, then return 
	// true, else false 
	return (visited[t] == true); 
} 

// Returns the maximum flow from s to t in the given graph 
int fordFulkerson(int **graph,int V, int s, int t, int team_num) 
{ 
	// Create a residual graph and fill the residual graph with 
	// given capacities in the original graph as residual capacities 
	// in residual graph 
	int **rGraph = new int*[V];//[V][V];
	for(int u=0;u<V;u++){
		rGraph[u] = new int[V];
		for (int v = 0; v < V; v++) 
			rGraph[u][v] = graph[u][v]; 
	}
	
	// Residual graph where rGraph[i][j] indicates 
	// residual capacity of edge from i to j (if there 
	// is an edge. If rGraph[i][j] is 0, then there is not) 

	int parent[V]; // This array is filled by BFS and to store path 

	int max_flow = 0; // There is no flow initially 

	// Augment the flow while tere is path from source to sink 
	while (bfs(rGraph,V, s, t, parent)) 
	{
		int u, v; 
		int path_flow = INT_MAX; 
		// Find minimum residual capacity of the edges along the 
		// path filled by BFS. Or we can say find the maximum flow 
		// through the path found. 
		v=t;
		while(v!=s){
			u = parent[v];
			path_flow = min(path_flow, rGraph[u][v]);
			v = parent[v];
		}
		// update residual capacities of the edges and reverse edges 
		// along the path 
		v=t;
		while(v!=s){
			u = parent[v];
			rGraph[u][v] -= path_flow; 
			rGraph[v][u] += path_flow; 
			v = parent[v];
		}
		
		// Add path flow to overall flow 
		max_flow += path_flow; 
	}
	for(int i=team_num;i<s;i++)
		if(rGraph[s][i]>0){
			//cout<<"eliminated"<<endl;
			return -1;
			break;
		} 
	
	return 1;
	// Return the overall flow 
	//return max_flow; 
} 
int checkElimination(int team, int team_num, int *win, int* lose, int* rest, int** games){
	vector<int> nodes, games_nodes, team_nodes;
	int s_node, t_node, nodes_num=team_num;
	int **graph;
	
	//count number of nodes
	for(int i=0;i<team_num;i++){
		for(int j=i;j<team_num;j++){
			if(i==team || j==team || games[i][j]==0)
				continue;
			//cout<<i<<" vs "<<j<<": "<<nodes_num<<endl;
			games_nodes.push_back(nodes_num);
			team_nodes.push_back(i);
			team_nodes.push_back(j);
			nodes_num++;
		}
	}
	s_node = nodes_num;
	nodes_num++;
	t_node = nodes_num;
	nodes_num++;
	
	//initial directed graph
	graph = new int*[nodes_num];
	for(int i=0;i<nodes_num;i++){
		graph[i] = new int[nodes_num];
		for(int j=0;j<nodes_num;j++){
			graph[i][j] = 0;
		}
	}
	
	//s -> games_nodes, games_nodes -> team_nodes
	for(int i=0;i<games_nodes.size();i++){
		int t1 = team_nodes[2*i], t2 = team_nodes[2*i+1];
		int game_node = games_nodes[i];
		graph[s_node][game_node] = games[t1][t2];
		graph[game_node][t1] = INT_MAX;
		graph[game_node][t2] = INT_MAX;
	}
	//team_nodes -> t
	for(int i=0;i<team_num;i++){
		graph[i][t_node] = win[team]+rest[team]-win[i];
	}
	
	//cout << "The maximum possible flow is " << fordFulkerson(graph, nodes_num, s_node, t_node, team_num) <<endl;
	if(fordFulkerson(graph, nodes_num, s_node, t_node, team_num)>0)
		return team;
	else
		return -1;

}
int main(int argc, char* argv[]){
	// Let us create a graph shown in the above example 
	/*
	int graph[V][V] = { {0, 16, 13, 0, 0, 0}, 
			    {0, 0, 10, 12, 0, 0}, 
			    {0, 4, 0, 0, 14, 0}, 
			    {0, 0, 9, 0, 0, 20}, 
			    {0, 0, 0, 7, 0, 4}, 
			    {0, 0, 0, 0, 0, 0} }; 
	*/
	int team_num, num;
	int *win, *lose, *rest, **games;
	vector<int> team_champion;
	ifstream fin;
	ofstream fout;

	//Input
	fin.open(argv[1]);
	fin >> team_num;
	win = new int[team_num];
	lose = new int[team_num];
	rest = new int[team_num];
	games = new int *[team_num];
	for(int i=0;i<team_num;i++){
		fin >> num;
		fin >> win[i];
		fin >> lose[i];
		fin >> rest[i];
		//cout<<win[i]<<" "<<lose[i]<<" "<<rest[i]<<endl;
	}
	for(int i=0;i<team_num;i++){
		games[i] = new int[team_num];
		for(int j=0;j<team_num;j++){
			fin >> num;
			games[i][j] = num;
			//cout<<games[i][j]<<" ";
		}
		//cout<<endl;
	}
	fin.close();
	//cout<<"-----------------------------------"<<endl;
	//Construct max flow graph for each team
	for(int team=0;team<team_num;team++){
		num = checkElimination(team,team_num,win,lose,rest,games);
		if(num != -1)
			team_champion.push_back(num);
	}
	
	fout.open(argv[2], ofstream::out);
	for(int i=0;i<team_champion.size();i++)
		fout<<team_champion[i]<<endl;
	fout.close();

	return 0; 
} 
