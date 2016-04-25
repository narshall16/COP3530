/*--------------------------

	Project 2
	COP 3530
	4/28/16
	
---------------------------*/

#include <iostream>
#include <string>
#include <math.h>
#include <vector>
#include <cstdio>
#include <algorithm>

using namespace std;

int minimum(int a, int b, int c)
{
	return min(min(a, b), c);
}

int editDistance(string a, string b)
{
	int dist[a.length()+1][b.length()+1];
	
	//character by character analysis of the two words
	for(int i = 0; i <= a.size(); ++i)
	{
		for(int j = 0; j <= b.size(); ++j)
		{
			if(i == 0)
			{
				//situation where first stsring is empty
				dist[i][j] = j;
			}
			else if(j == 0)
			{
				//situation where second string is empty
				dist[i][j] = i;
			}
			else if(a.at(i-1) == b.at(j-1))
			{
				//situation where the characters are the same
				//no increase to edge count
				dist[i][j] = dist[i-1][j-1];
			}
			else
			{
				//characters being observed are different.
				//put value as 1 more than the least of
				//removing a character, replacing one, or adding one.
				dist[i][j] = 1 + minimum(dist[i][j-1], dist[i-1][j], dist[i-1][j-1]);
			}
		}
	}
	return dist[a.length()][b.length()];
}

//finds longest subsequence of increasing values
void maxIncantaion(vector<int> &seq, vector<int> &pos)
{
	int a;
	int b;
	int c;
	vector<int> longest(seq.size());
	
	if(seq.empty())
	{
		return;
	}
	
	pos.push_back(0);
	
	for(int i = 1; i < seq.size(); ++i)
	{
		if(seq[pos.back()] < seq[i])
		{
			longest[i] = pos.back();
			pos.push_back(i);
			continue;
		}
		
		a = 0;
		b = pos.size() - 1;
		while(a < b)
		{
			c = (a + b) / 2;
			if(seq[pos[c]] < seq[i])
			{
				a = c + 1;
			}
			else
			{
				b = c;
			}
		}
		
		if(seq[i] < seq[pos[a]])
		{
			if(a > 0)
			{
				longest[i] = pos[a - 1];
			}
			pos[a] = i;
		}
	}
	
	for(a = pos.size(), b = pos.back(); a--; b = longest[b])
	{
		pos[a] = b;
	}
}

int main()
{
	int realms = 0;
	int magi = 0;
	int cost = 0;
	int changes = 0;
	int temp = 0;
	string input;
	string start;
	string finish;
	vector<string> realmNames;
	vector<bool> reachable;
	
	//take in number of realms
	scanf("%d", &realms);
	vector<int> magiVals [realms];
	vector<int> incantations;
	vector<int> realmSeq [realms];
	vector<int> seqPos [realms];
	int graph[realms][realms];
	
	for(int i = 0; i < realms; ++i)
	{
		cin >> input;
		//put realms in vector array
		realmNames.push_back(input);
		reachable.push_back(false);
		scanf("%d", &magi);
		for(int j = 0; j < magi; ++j)
		{
			//put magi costs in order in a 2D array
			scanf("%d", &cost);
			magiVals[i].push_back(cost);
		}
		
			int *temp = new int[magi];
			for(int k = 0; k < magi; ++k)
			{
				temp[k] = magiVals[i][k];
			}
			realmSeq[i] = magiVals[i];
			maxIncantaion(realmSeq[i], seqPos[i]);
			incantations.push_back(seqPos[i].size());
			delete [] temp;
		
	}
	
	cin >> start;
	cin >> finish;
	
	//create undirected graph for all realms
	for(int i = 0; i < realms; ++i)
	{
		for(int j = 0; j < realms; ++j)
		{
			//distance from realm to itself is 0
			if(j == i)
			{
				graph[i][j] = 0;
			}
			else if(editDistance(realmNames[i],realmNames[j]) <= incantations[i])
			{
				//edge weights are edit distance for the names
				//no edge created if cost is higher than # of incantations available
				graph[i][j] = editDistance(realmNames[i], realmNames[j]);
			}
			else
			{
				graph[i][j] = -1;
			}
		}
	}
	
	int dist[realms];
	int predecessor[realms];
	int begin;
	int end;
	for(int i = 0; i < realms; ++i)
	{
		if(realmNames[i].compare(start) == 0)
		{
			begin = i;
		}
		if(realmNames[i].compare(finish) == 0)
		{
			end = i;
		}
	}
	//Dijkstras algorithm
	//reachable is bool vector to determine if finish is reachable
	//dist and predecessor are arrays as shown in class lecture
	//need help finishing this algorithms and we're set.
	
	struct edge { int to, length; };
    
int dijkstra(const vector< vector<edge> > &graph, int source, int target) {
    vector<int> min_distance( graph.size(), INT_MAX );
    min_distance[ source ] = 0;
    set< pair<int,int> > active_vertices;
    active_vertices.insert( {0,source} );
        
    while (!active_vertices.empty()) {
        int where = active_vertices.begin()->second;
        if (where == target) return min_distance[where];
        active_vertices.erase( active_vertices.begin() );
        for (auto edge : graph[where]) 
            if (min_distance[edge.to] > min_distance[where] + edge.length) {
                active_vertices.erase( { min_distance[edge.to], edge.to } );
                min_distance[edge.to] = min_distance[where] + edge.length;
                active_vertices.insert( { min_distance[edge.to], edge.to } );
            }
    }
    return INT_MAX;
}
	
	for(int i = 0; i < realms; ++i)
	{
		cout << realmNames[i] << endl;
		for(int j = 0; j < magiVals[i].size(); ++j)
		{
			cout <<  magiVals[i][j] << " ";
		}
		cout << endl;
		for(int j = 0; j < seqPos[i].size(); ++j)
		{
			cout << realmSeq[i][seqPos[i][j]] << " ";
		}
		cout << endl;
		cout << incantations[i] << endl;
	}
	
	
	//cout << editDistance("sunday","saturday");
	
	return 0;
}
