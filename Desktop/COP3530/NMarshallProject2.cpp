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

int closerNode(int length[], bool visited[], int realms)
{
	int minimum = 999999999;
	int minimumIndex = 0;
	
	for(int i = 0; i < realms; ++i)
	{
		if(!visited[i] && length[i] <= minimum)
		{
			minimum = length[i];
			minimumIndex = i;
		}
	}
	return minimumIndex;
}


int main()
{
	int realms = 0;
	int magi = 0;
	int cost = 0;
	int changes = 0;
	int temp = 0;
	int curr = 0;
	string input;
	string start;
	string finish;
	vector<string> realmNames;
	
	//take in number of realms
	scanf("%d", &realms);
	vector<int> magiVals [realms];
	vector<int> incantations;
	vector<int> realmSeq [realms];
	vector<int> seqPos [realms];
	int graph[realms][realms];
	int length[realms];
	bool reachable[realms];
	
	
	for(int i = 0; i < realms; ++i)
	{
		cin >> input;
		//put realms in vector array
		realmNames.push_back(input);
		reachable[i] = false;
		scanf("%d", &magi);
		for(int j = 0; j < magi; ++j)
		{
			//put magi costs in order in a 2D array
			scanf("%d", &cost);
			magiVals[i].push_back(cost);
		}
			realmSeq[i] = magiVals[i];
			maxIncantaion(realmSeq[i], seqPos[i]);
			incantations.push_back(seqPos[i].size());
		
	}
	
	cin >> start;
	cin >> finish;
	
	//create directed graph for all realms
	for(int i = 0; i < realms; ++i)
	{
		for(int j = 0; j < realms; ++j)
		{
			if((i != j) && (editDistance(realmNames[i],realmNames[j]) <= incantations[i]))
			{
				//edge weights are edit distance for the names
				//no edge created if cost is higher than # of incantations available
				graph[i][j] = editDistance(realmNames[i], realmNames[j]);
			}
			else
			{
				graph[i][j] = 0;			
			}
		}
	}
	
	int predecessor[realms];
	int predecessor2[realms];
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
	//couldn't find a good way to pass graph[][] to function
	//just put the method in main twice.
	
	for(int i = 0; i < realms; ++i)
	{
		length[i] = 999999;
		predecessor[0] = -1;
	}
	
	length[begin] = 0;
	
	for(int i = 0; i < (realms - 1); ++i)
	{
		int shortest = closerNode(length, reachable, realms);
		reachable[shortest] = true;
		
		for(int j = 0; j < realms; ++j)
		{
			if(!reachable[j] && graph[shortest][j] && length[shortest] != 999999
					&& (length[shortest] + graph[shortest][j] < length[j]))
					{
						length[j] = length[shortest] + graph[shortest][j];
						predecessor[j] = shortest;
					}
		}
	}
	
	//counts path backwards from destination to start
	curr = end;
	cost = 0;
	while(curr != begin && length[end] != 999999)
	{
		//edge weight
		temp = graph[predecessor[curr]][curr];
		for(int i = 0; i < temp; ++i)
		{
			//counts the cost in each realm
			changes = predecessor[curr];
			cost += magiVals[changes][seqPos[changes][i]];
		}
		curr = predecessor[curr];
	}
	
	if(length[end] == 999999)
	{
		cout << "IMPOSSIBLE" << endl;
	}
	else
	{
		cout << length[end] << " " << cost << endl;
	}

	
	
	changes = end;
	end = begin;
	begin = changes;
	//way back
	for(int i = 0; i < realms; ++i)
	{
		length[i] = 999999;
		predecessor2[0] = -1;
		reachable[i] = false;
	}
	
	length[begin] = 0;
	
	for(int i = 0; i < (realms - 1); ++i)
	{
		int shortest = closerNode(length, reachable, realms);
		reachable[shortest] = true;
		
		for(int j = 0; j < realms; ++j)
		{
			if(!reachable[j] && graph[shortest][j] && length[shortest] != 999999
					&& (length[shortest] + graph[shortest][j] < length[j]))
					{
						length[j] = length[shortest] + graph[shortest][j];
						predecessor2[j] = shortest;
					}
		}
	}
	
	//counts path from finish to start
	curr = end;
	cost = 0;
	//while not at destination and if it has any edges at all
	while(curr != begin && length[end] != 999999)
	{
		//edge weight
		temp = graph[predecessor2[curr]][curr];
		for(int i = 0; i < temp; ++i)
		{
			//counts the cost in each realm
			changes = predecessor2[curr];
			cost += magiVals[changes][seqPos[changes][i]];
		}
		curr = predecessor2[curr];
	}
	
	if(length[end] == 999999)
	{
		cout << "IMPOSSIBLE" << endl;
	}
	else
	{
		cout << length[end] << " " << cost << endl;
	}
		
	
	return 0;
}
