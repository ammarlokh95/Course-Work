#include <iostream>
#include "GameFiles.h"
#include <time.h>
#include <stdlib.h>
using namespace std;

clock_t ptime;

void randomWalk(GameState g, int N)
// Performs a random walk of N steps into the game
{
	for(int i=1; i<=N; i++)
	{
		list<Move> moves = getAllMoves(g);
		g.displayState();
	   int random = rand() % (moves.size()-1) + 1;
		list<Move>::iterator it = moves.begin();
		advance(it, random);
		Move m = *it;
	   cout<<endl;
	   m.display();
	   cout<<endl;	
		applyMove(&g,m);
		g.normalize();
		if(g.isSolved())
			break;
	}
}
void traceBackPath(GameState g, int nodes)
// Retraces the GameStates and finds the solution of the Puzzle
// calculates the time of execution and
// displays the number of nodes and length of solution
{
	list<Move> path;
	GameState* next;
	path.push_front(g.getParentMove());
	GameState fin = g.clone();
	next = &g;
	while(next->getParent()!=NULL)
	{
		next = next->getParent();
		path.push_front(next->getParentMove());
	}
	int length =0;
	for(list<Move>::iterator it=path.begin(); it!=path.end(); it++)
	{
		it->display();
		length++;
	}
	cout<<endl;
	fin.displayState();
	ptime = clock() - ptime;
	cout<<endl<<nodes<<" "<<(((float)ptime)/CLOCKS_PER_SEC)<<"s "<<length<<endl;
	
}

void BFS(GameState g)
// Solves the puzzle using bredth first search algorithm starting from 
// a specific GameState
{
	ptime = clock();
	vector<GameState> visited;
	list<GameState> queue; 
	bool isVisited = false;
	list<Move> moves;
	queue.push_back(g);
	while(!queue.empty())
	{
		GameState *t = new GameState();
		*t = queue.front();
		visited.push_back(*t);
		queue.pop_front();
		moves = getAllMoves(*t);
		
	   for(list<Move>::iterator it=moves.begin(); it!=moves.end();it++)
		{
			Move m = *it;
			GameState temp = applyMoveCloning(*t,m);
			temp.normalize();
			isVisited=false;
			for(int i=0;i<visited.size(); i++)
			{
				if(compareState(temp, visited[i]) == true)
				{
					isVisited = true;
					break;				
				}
			}
			if(isVisited==false)
			{	
				temp.setParentMove(m);
				temp.setParent(t);
				//temp.displayState();
				queue.push_back(temp);
				if(temp.isSolved())
				{
					g=temp;
					traceBackPath(temp,visited.size()+queue.size());
					return;
				}
			}
		}

		moves.clear();
	}

}
// Solves the puzzle implementing Depth first search
void DFS(GameState g)
{
	ptime = clock();
	vector<GameState> visited;
	list<GameState> stack;
	bool isVisited = false;
	list<Move> moves;
	stack.push_front(g);
	while(!stack.empty())
	{
		GameState *t = new GameState();
		*t = stack.front();
		stack.pop_front();
		visited.push_back(*t);
		moves = getAllMoves(*t);
		list<GameState>::iterator im= stack.begin();
		for(list<Move>::iterator it=moves.begin(); it!=moves.end(); it++)
		{
			Move m = *it;
			GameState temp = applyMoveCloning(*t, m);
			temp.normalize();
			isVisited = false;
			for(int i=0; i<visited.size(); i++)
			{
				if(compareState(temp, visited[i]) == true)
				{
					isVisited = true;
					break;
				}
			}
			if(isVisited==false)
			{
				temp.setParentMove(m);
				temp.setParent(t);
				stack.insert(im, temp);
				im++;
				if(temp.isSolved())
				{
					g= temp;
					traceBackPath(temp,visited.size()+stack.size());
					return;
				}
			}
		}
		moves.clear();
	}
}
pair<GameState*,int>* depthLimitedSearch(GameState g, int depth)
// Helper funtion to iterative deepening
// performs dpth first search upto a specific depth
// returns the result if found and the nodes explored back to the base funtion
{
	list<Move> moves;
	vector<GameState> visited;
	list<GameState> stack;
	bool isVisited = false;
	stack.push_front(g);
	int noofmoves;
	if(depth!=0)
	{
		while(!stack.empty())
		{
			GameState *t = new GameState();
			*t = stack.front();
			stack.pop_front();
			visited.push_back(*t);
			moves = getAllMoves(*t);
			list<GameState>::iterator im = stack.begin();			
			for(list<Move>::iterator it = moves.begin(); it!=moves.end(); it++)
			{
				Move m = *it;
				GameState temp = applyMoveCloning(*t, m);
				temp.normalize();
				isVisited = false;
				for(int i=0; i<visited.size();i++)
				{
					if(compareState(temp, visited[i]) == true)
					{
						isVisited = true;
						break;
					}
				}
				if(isVisited == false)
				{
					temp.setParentMove(m);
					temp.setParent(t);
					if(depth>0) {
						stack.push_back(temp);
					}
					if(temp.isSolved())
					{
						pair<GameState*,int>* result = new
							pair<GameState*, int>();
						GameState *fin = new GameState;
						*fin = temp;
						result->first = fin;
						result->second = visited.size()+stack.size();
						return result;
					}
				}
			}
			moves.clear();
		}
	}
	if(g.isSolved())
	{
		pair<GameState*,int>* result = new
			pair<GameState*,int>();
		GameState *fin = new GameState();
		*fin = g;
		result->first = fin;
		result->second = visited.size()+stack.size();
		return result;
	}
	return new pair<GameState*,int>(NULL,visited.size()+stack.size());
}
// Base function of iterative deepening depth first search
void iterativeSearch(GameState g)
{
	ptime = clock();
	int nodes=0;
	for(int depth = 0; ;depth++)
	{
		pair<GameState*, int>* result = depthLimitedSearch(g, depth);
		if(result->first!=NULL) {
			nodes += result->second;
			traceBackPath(*result->first, nodes);
			delete result->first;
			delete result;
			return;
		}
		else {nodes += result->second;}
	}
}
int main()
{
	string name = "SBP-level0.txt";
	GameState g;
	g.loadState(name);
	//cout<<"Random walk:\n";
	//randomWalk(g,3);
	name = "SBP-level1.txt";
	g.loadState(name);
	//cout<<"\nBreath First Search:\n";
	//BFS(g);
	//cout<<"\nDepth First Search:\n";
	//DFS(g);	
	cout<<"\nIterative Deepening DFS:\n";
	iterativeSearch(g);
	return 0;
}
