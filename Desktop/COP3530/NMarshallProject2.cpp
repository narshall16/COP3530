
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
	int startn, finishn;
	vector<string> realmNames;
	vector<bool> reachable;

	//take in number of realms
	scanf("%d", &realms);
	//cin >> realms;

	vector<int> magiVals [realms]; // magi values
	vector<int> incantations; // compare these
	vector<int> realmSeq [realms]; // realms in order
	vector<int> seqPos [realms]; // realms to visit in order
	int graph[realms][realms];

	for(int i = 0; i < realms; ++i)
	{
		cin >> input;
		//put realms in vector array
		realmNames.push_back(input);
		reachable.push_back(false);

		scanf("%d", &magi);
		//cin >> magi;
		for(int j = 0; j < magi; ++j)
		{
			//put magi costs in order in a 2D array

			scanf("%d", &cost);
			//cin >> cost;
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
			//cout << graph[i][j] << " ";
		}
		//cout << endl;
	}

	int dist[realms];
	int predecessor[realms];
	int begin;
	int end;
	int cost1 = 0;
	int distance = 0;
	for(int i = 0; i < realms; ++i)
	{
	    if(realmNames[i] == start){
            startn = i;
        }
        if(realmNames[i] == finish){
            finishn = i;
        }
		if(realmNames[i].compare(start) == 0)
		{
			begin = i;
			//cout << "start" << i <<endl;
		}
		if(realmNames[i].compare(finish) == 0)
		{
			end = i;
			//cout << "end" << i <<endl;
		}
	}


	//Dijkstras algorithm
	//reachable is bool vector to determine if finish is reachable
	//dist and predecessor are arrays as shown in class lecture
	//need help finishing this algorithms and we're set.


	for(int i = 0; i < realms; ++i)
	{
		//cout << realmNames[i] << endl;
		for(int j = 0; j < magiVals[i].size(); ++j)
		{
			//cout <<  magiVals[i][j] << " ";
		}
		//cout << endl;
		for(int j = 0; j < seqPos[i].size(); ++j)
		{
			//cost1 = cost1 + realmSeq[i][seqPos[i][j]];
		}
		//cout << endl;
		//cout << incantations[i] << endl;
	}
    bool poss = false;
    for(int i = 0; i < realms; i++)
	{
		for(int j = realms-1; j >= 0; j--)
		{
		    if ( i == realms-1){
                poss = true;
                break;
            }
		    //cout << incantations[i] << "and " << graph[i][j] << endl;
		   if(graph[i][j] > 0 && graph[i][j] <= incantations[i]){
            incantations[i] = graph[i][j];
            distance = distance + graph[i][j];

            //cout << "incantations " << incantations[i] << endl;
            //cout << "distance " << distance << endl;
            int temp = incantations[i];
            int z =0;
            //cout << "i is this " << i << endl;
            //cout << "cost before " << cost1 <<endl;
            while(temp>0){
                cost1 = cost1 + realmSeq[i][seqPos[i][z]];
               // cout << "this is seqPos " << realmSeq[i][seqPos[i][z]] << endl;
                //cout << "this is temp" << temp<<endl;
                temp = temp - 1;
                z++;

            }
            //cout << "cost " << cost1 << endl;
            i = j-1;
            //cout <<"j is " << j <<endl;
            //cout <<"i is " << i <<endl;
            break;
		   }
		}
		if (distance == 0){
            //   cout << "IMPOSSIBLE" << endl;
            break;
		}
		if(i == realms-1){

                //cout <<"i is this " << i << endl;
                //cout <<"break" <<endl;
            break;
		}
	}
	if(poss == true){
    cout << distance << " " << cost1 << endl;
	}
	else{
        cout << "IMPOSSIBLE" << endl;
	}
	//cout << editDistance("sunday","saturday");

	return 0;
}



