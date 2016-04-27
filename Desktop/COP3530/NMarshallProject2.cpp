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
	//vector to hold temporary longest sequences while the method is run
	vector<int> longest(seq.size());
	
	//if theres no incantations in the realms (no items in the list)
	if(seq.empty())
	{
		return;
	}
	
	//puts first item in the position vector
	pos.push_back(0);
	
	for(int i = 1; i < seq.size(); ++i)
	{
		//if first element in position vector is less than current element (current element is later in the list)
		if(seq[pos.back()] < seq[i])
		{
			//add current position element to longest array
			//put current element on position array
			longest[i] = pos.back();
			pos.push_back(i);
		}
		
		a = 0;
		b = pos.size() - 1;
		while(a < b)
		{
			//find median value between beginning and end bound for this iteration
			c = (a + b) / 2;
			//if this value is less than the current item
			if(seq[pos[c]] < seq[i])
			{
				//increase lower bound
				a = c + 1;
			}
			else
			{
				//otherwise decrease upper bound
				b = c;
			}
		}
		
		//if current item is less than the lower bound item
		if(seq[i] < seq[pos[a]])
		{
			//and if not the first element
			if(a > 0)
			{
				//add item to longest list
				longest[i] = pos[a - 1];
			}
			//store its position in position vector
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
