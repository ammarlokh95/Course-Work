#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <list>
#include <vector>
using namespace std;

class Move 
// Defines the move of a piece 
{
	private:
	int piece;
	char dir;
	public:
	Move(){}
	Move(int p, char d)
	{
		piece = p;
		dir = d;
	}
	

	int getPiece(){
		return piece;
	}
	char getDir(){
		return dir;
	}
	void display()
	{
		switch(dir)
		{
			case 'u':
				cout<<"("<<piece<<",up)"<<endl;
				break;
			case 'd':
				cout<<"("<<piece<<",down)"<<endl;
				break;
			case 'l':
				cout<<"("<<piece<<",left)"<<endl;
				break;
			case 'r':
				cout<<"("<<piece<<",right)"<<endl;
				break;
		}
	}
};


class GameState {
// Defines the current state of the game
	private:
		vector< vector<int>> state; 
	   int h,w;
		Move parentMove; // Move that led to this state
		GameState* parent; // State from which this state was derived
	public:
		GameState()
		{
			h = -1;
			w = -1;
			parent=NULL;
		}
		~GameState()
		{
		}
		void setParentMove(Move m)
		{
			parentMove = m;
		}
		Move getParentMove()
		{
			return parentMove;
		}
		void setParent(GameState* g)
		{
			parent = g;
		}
		GameState* getParent()
		{
			return parent;
		}
		void loadState(string filename) 
		// Loads the initial gameState from a file
		{
			ifstream file (filename.c_str());
			string line = "";
			state.clear();;
			w = -1;
			h = -1;
			int isFirstLine = 1;
			if (file.is_open())
			{
				int j = 0, k = 0;
				while (getline(file,line)) {
					k = 0;
					string num = "";
					if (isFirstLine == 1)
					{
						for(int i = 0; i< line.length(); i++)
						{
							if(line[i]!=',')
								num = num + line[i];
							else { 
								if(w == -1)
								{
									w = stoi (num);
									num = "";
								}
								else if(h == -1)
								{	
									h = stoi (num);
									num = "";
									break;
								}
							}
						}
						isFirstLine = 0;
						state.resize(h, vector<int>(w, 0));
					}
					else {
						for(int i = 0; i <line.length(); i++)
						{
							if(line[i]!=',')
								num = num + line[i];
							else { 
								state[j][k] = stoi (num);
								k++;
								num = "";
								if(k>=w)
									break;
							}
						}
						j++;
						if(j>=w)
							break;
					}
				}
			}
			file.close();
		}

		void displayState()
		{
			cout<<w<<","<<h<<","<<endl;
			for(int i =0; i<h; i++)
			{
				for(int j=0; j<w; j++)
				{
					cout<<state[i][j]<<",";
				}
				cout<<endl;
			}
		}

		vector<vector<int>> getState()
		{
			return state;
		}
		void setState(vector<vector<int>> newState)
		{
			state = newState;
			w = newState[0].size();
			h = newState.size();
		}
		
		GameState clone()
		{
			GameState g2;
			g2.state = getState();
			g2.w = w;
			g2.h = h;
			return g2;
		}
		bool isSolved()
		{
			for(int i=0;i<h;i++)
			{
				for(int j=0;j<w; j++)
				{
					if(state[i][j] == -1)
						return false;
				}
			}
			return true;
		}

	void swapIdx(int idx1, int idx2) 
	{
		for(int i=0; i<h; i++)
		{
			for(int j=0; j<w; j++)
			{
				if(state[i][j] == idx1) 
				{
					state[i][j] = idx2;
				}
				else if(state[i][j] == idx2) 
				{
					state[i][j] = idx1;
				}
			}
		}
	}
	
	void normalize()
	{
	
		int nextIdx = 3;
		for(int i = 0; i<h;i++) 
		{
			for(int j=0; j<w; j++)
			{
				if(state[i][j] == nextIdx)
				{
					nextIdx++;
				}
				else if(state[i][j] > nextIdx) 
				{
					swapIdx(nextIdx, state[i][j]);
					nextIdx++;
				}
			}
		}
	}
};

list<Move> getMoves(GameState g, int piece)
{
	list<Move> temp;
	if(piece > 1){
	bool pieceFound = false;	
	vector<vector<int>> state = g.getState();
	for(int i=0;i<state.size(); i++)
	{
		for(int j=0;j<state[i].size(); j++)
		{
			if(state[i][j] == piece) 
			{
				int k=j;
				int h=i;
				bool checkFlag = true;
				Move ml (piece,'l'), mr (piece, 'r'), mu(piece,'u'), md(piece,
						'd');		
				temp.push_back(mu);
				while(state[i][k] == piece && k<state[i].size())
				{
					if(piece != 2)
					{	
						if((state[i-1][k] != 0 || i-1<0))
						{
							temp.pop_back();
							k++;
							break;
						}
					}
					else if(state[i-1][k]>0 || i-1<0)
					{
						temp.pop_back();
						k++;
						break;
					}
					k++;
				}
				k--;
				temp.push_back(ml);

				while(state[h][j] == piece && h<state.size())
				{
					if(piece !=2)
					{
						if((state[h][j-1] != 0 || j-1<0))
						{	
							temp.pop_back();
							h++;
							break;
						}
					}
					else if(state[h][j-1]>0 || j-1<0)
					{
						temp.pop_back();
						h++;
						break;
					}
					h++;
				}
				h--;
				temp.push_back(md);
				while(state[h][j] == piece && j<state[h].size())
				{
					if(piece != 2)
					{
						if(state[h+1][j] != 0 || h+1>= state.size())
						{
							temp.pop_back();
							break;
						}
					}
					else if(state[h+1][j]>0 || h+1>= state.size())
					{
						temp.pop_back();
						break;
					}
					j++;
				}
				checkFlag=true;
				temp.push_back(mr);
				while(state[i][k] == piece && i<state.size())
				{
					if(piece != 2)
					{
						if((state[i][k+1] != 0 || k+1>=state[i].size()))
						{
							temp.pop_back();
							break;
						}
					}
					else if(state[i][k+1]>0 || k+1>=state[i].size())
					{
						temp.pop_back();
						break;
					}
					i++;
				}

				return temp;
			}
		}
	}
	}
	return temp;
}

list<Move> getAllMoves(GameState g)
{
	int max = 2;
	vector<vector<int>> state = g.getState();
	list<Move> temp;
	for(int i = 0; i<state.size(); i++)
	{
		for(int j=0; j<state[i].size(); j++)
		{
			if(state[i][j]>max)
				max=state[i][j];
		}
	}

	for(int i=2;i<=max;i++)
		temp.splice(temp.end(), getMoves(g, i));

	return temp;
}

void applyMove(GameState *g, Move m)
{
	vector<vector<int>> temp = g->getState();
	int piece = m.getPiece();
	char c = m.getDir();
	for(int i=0; i< temp.size(); i++)
	{
		for(int j=0; j<temp[i].size(); j++)
		{
			if(temp[i][j]==piece)
			{
				bool shifted = false;
				if(c=='u')
				{
					if(i-1>=0)
					{
						for(int k=i;temp[k][j]==piece ; k++)
						{
							for(int h=j; temp[k][h]==piece;h++)
							{
								if(temp[k-1][h] == 0 || (temp[k][h] == 2 &&
											temp[k-1][h]==-1))
								{
									temp[k-1][h] = piece;
									temp[k][h] = 0;
								}
								else {
									cout<<"Invalid Move! 1 "<<c<<temp[k][h]<<endl; 
									return;
								}
							}
						}
					}
					else { cout<<"Invalid Move! 2"; return;}
				}
				else if(c=='l')
				{
					if(j-1>=0)
					{
						for(int k=i;temp[k][j]==piece ; k++)
						{
							for(int h=j; temp[k][h]==piece;h++)
							{
								if(temp[k][h-1] == 0 || (temp[k][h] == 2 &&
											temp[k][h-1]==-1))
								{
									temp[k][h-1] = piece;
									temp[k][h] = 0;
								}
								else {
									cout<<"Invalid Move! 3"<<c<<temp[k][h]<<endl;
									return;
								}
							}
						}
					}
					else { cout<<"Invalid Move! 4"; return;}
				}
				else if(c=='d')
				{
					int k=i;
					while(temp[k][j] == piece)
						k++;
					k--;
					if(k+1<temp.size())
					{
						for(k;temp[k][j]==piece;k--)
						{
							for(int h=j;temp[k][h]==piece;h++)
							{
								if(temp[k+1][h] == 0 || (temp[k][h] == 2 &&
											temp[k+1][h]==-1))
								{
									temp[k+1][h] = piece;
									temp[k][h] = 0;
								}
								else {cout<<"Invalid Move! 5"<<c<<temp[k][h]<<endl; return;}
							}
						}
					}
					else {cout<<"Invalid Move! 6"; return;}
				}
				else if(c=='r')
				{
					int h=j;
					while(temp[i][h]==piece)
						h++;
					h--;

					if(h+1<temp[i].size())
					{
						for(int k=i;temp[k][j]==piece;k++)
						{
							for(h;temp[k][h]==piece;h--)
							{
								if(temp[k][h+1] == 0 || (temp[k][h] == 2 &&
											temp[k][h+1]==-1))
								{
									temp[k][h+1] = piece;
									temp[k][h] = 0;
								}
								else {cout<<"Invalid Move! 7"<<c<<temp[k][h]<<endl; return;}
							}
						}
					}
					else {cout<<"Invalid Move! 8"; return;}
				}
				else {cout<<"Invalid Move! 9"; return;}
				g->setState(temp);
				return;
			}
		}
	}
}

GameState applyMoveCloning(GameState g, Move m)
{
	GameState gT;
	gT = g.clone();
	applyMove(&gT,m);
	return gT;
}

bool compareState(GameState g1, GameState g2)
{
	vector<vector<int>> v1 = g1.getState();
	vector<vector<int>> v2 = g2.getState();

	if(v1.size() == v2.size())
	{
		for(int i=0;i<v1.size();i++)
		{
			if(v1[i].size() == v2[i].size())
			{
				for(int j=0; j<v1[i].size(); j++)
				{
					if(v1[i][j] != v2[i][j])
						return false;
				}
			}
			else return false;
		}
	}
	else return false;
	return true;
}

