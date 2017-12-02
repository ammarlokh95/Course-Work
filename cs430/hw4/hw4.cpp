#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include<stdlib.h>
#include<sstream>
#include<math.h>
#include<list>
#include<set>
#define MAX_SIZE 500 
#define MIN_SIZE 0
using namespace std;
void createLine(float,float,float,float, bool);
void putPixel(int,int,string[MAX_SIZE],char);
void writeFile(string []);
void getPoints();
void clipLine(float*,float*,float*,float*);
void drawPolygon(int,int);
void fillPolygon(string[]);
list<pair<int,int>> polygonPoints;
ifstream f;
string line;
string f_name;
// Entry point...
// Reads command line arguments and gets the 
// text from the file indicated
int main(int argc, char *argv[])
{
	string file= "hw1_t.ps";
	char check[] = "-f";
	bool flag = false;
	if(argc>=3)
	{
		if(strcmp(argv[1], "-f")==0)
		{
			string s(argv[2]);
			file = s;
		}
		else {
			cerr<<"Invalid argument flag!";
			exit(1);
		}
	}
	else { 
		cerr<<"Invalid arguments!";
		exit(1);
	}
	f.open(file.c_str());
	f_name = file.substr(0, file.find_last_of("."));
	if(f.is_open()) {
		while(!f.eof())
		{
			getline(f,line);
			if(line == "%%%BEGIN")
			{	
				getline(f, line);
				getPoints();
				break;
			}
		}
	}
	else {
		cerr<< "Unable to open file";
		exit(1);
	}
	if(f.is_open())
		f.close();
	return 0;
}

string trim(string& str, const string& ws = " \t")
{
	const auto strBegin = str.find_first_not_of(ws);
	if(strBegin == string::npos)
		return "";
	const auto strEnd = str.find_last_not_of(ws);
	const auto strRange = strEnd - strBegin +1;
	return str.substr(strBegin,strRange);
}
// Parses through the 'ps' file and gets the points

void getPoints()
{
	int x1,y1,x2,y2;
	int i=0;
	if(!f.eof() && line!="%%%END")
	{
		cout<<line<<endl;
		stringstream ss(line);
		string num;
		getline(ss,num,' ');
		x1 = stoi(trim(num));
		getline(ss,num,' ');
		y1 = stoi(trim(num));
		getline(ss,num,' ');
		if(num=="moveto")
		{
			polygonPoints.clear();
			getline(f,line);
			drawPolygon(x1, y1);
			return;
		}
		else{
		x2 = stoi(num);
		getline(ss,num,' ');
		y2 = stoi(num);
		getline(f,line);
		if(line == "" || line =="\n")
			getline(f,line);
		createLine(x1,y1,x2,y2, false);
		}
	}
}

// Draws the line for the points given
void createLine(float x1, float y1, float x2, float y2, bool isPolygon)
{
	int xp2, yp2;
	if(isPolygon)
	{
		polygonPoints.push_back(make_pair((int)x1,(int)y1));
		polygonPoints.push_back(make_pair((int)x2,(int)y2));
		xp2= x2;
		yp2= y2;
		
	}
	clipLine(&x1,&y1,&x2,&y2);
	static string content[MAX_SIZE+4]={
			"500 500 2 1",
			"- c #000000",
			"# c #ffffff",
			"+ c #b22222"
		};
	static string pixels[MAX_SIZE];
	for(int i=0;i<MAX_SIZE;i++)
	{
		for(int j=0;j<MAX_SIZE;j++)
		{
			pixels[i]= pixels[i]+ "#";
		}
	}
	if(!(x1 == x2 && y1 == y2))
	{	
		float dx = x2 - x1;
		float dy = y2 - y1;
		float m;
		bool isVert = false;
		

		try{
			m = dy/dx;
		} catch(int i){
			isVert = true;
		}
		if(isVert == false){
			if(fabs(m)<1)
			{
				if(dx<0)
					dx = -1;
				else dx = 1;
			    if(dy<0)
					dy = -fabs(m);
				else dy = fabs(m);
				while(((int)x1)!=((int)x2))
				{
					putPixel(x1,y1,pixels,'-');
					x1=x1+dx;
					y1=y1+dy;
				}		
			}	
			else {
				if(dy<0)
						dy = -1;
					else dy = 1;
				if(dx<0)
					dx = -1/(fabs(m));
				else dx = 1/(fabs(m));
				while(((int) y1)!=((int)y2))
				{
					putPixel(x1,y1,pixels,'-');
					x1=x1+dx;
					y1=y1+dy;
				}

			}
		}
		else {
			dx = 0;
			dy = 1;
			while((int)y1!=(int)y2)
			{
				putPixel(x1,y1,pixels,'-');
				x1=x1+dx;
				y1=y1+dy;
			}	
		}

	}
	// Checks to see if there are anymore lines to be drawn
	if(line == "stroke")
	{
		if(isPolygon)
		{
			fillPolygon(pixels);
		}
		polygonPoints.clear();
		getline(f, line);
		if(line == "" || line =="\n")
			getline(f,line);

	}
	if(!f.eof() && line!="%%%END")
	{	
		if(isPolygon)
		{	
			drawPolygon(xp2,yp2);
		}
		else
		getPoints();
	}
	else
	{
		for(int i =0; i<MAX_SIZE;i++)
		{
			content[4+i] =pixels[i];
		}
		writeFile(content);
	}
}
// Puts the pixel in the right place in the pixel matrix
void putPixel(int x, int y, string pixels[MAX_SIZE], char a)
{
	pixels[MAX_SIZE-y-1][x] = a;
}

// Creates and writes the XPM file
void writeFile(string content[])
{
	ofstream f;
	f_name += ".xpm";
	f.open(f_name.c_str(), ios::trunc);
		if(f) {
		f << "\\* XPM *\\\nstatic const char *xpm[] = {\n";
		int i =0;
		for(i=0;i<MAX_SIZE+3;i++)
		{
			f << "\"" + content[i] + "\",\n";
		}
		f << "\"" + content[i] + "\"\n};";
		f.close();
	}
}
int getLocationCode(int x, int y) 
{
	if(x>=MIN_SIZE && x<=MAX_SIZE-1 && y>=MIN_SIZE && y<=MAX_SIZE-1)
	{
		return 0;
	}
	else if(x<MIN_SIZE && y <MIN_SIZE)
	{
		return 5;
	}
	else if(x<MIN_SIZE && (y >= MIN_SIZE && y <= MAX_SIZE-1))
	{
		return 1;
	}
	else if(x<MIN_SIZE && y >MAX_SIZE-1)
	{
		return 9;
	}
	else if(x>=MIN_SIZE && x<=MAX_SIZE-1 && y>MAX_SIZE-1)
	{
		return 8;
	}
	else if(x>MAX_SIZE-1 && y>MAX_SIZE-1)
	{
		return 10;
	}
	else if(x>MAX_SIZE-1 && y>=MIN_SIZE && y<=MAX_SIZE-1)
	{
		return 2;
	}
	else if(x>MAX_SIZE-1 && y<MIN_SIZE)
	{
		return 6;
	}
	else if(x>=MIN_SIZE && x<=MAX_SIZE && y<MIN_SIZE)
	{
		return 4;
	}
}
void clipLine(float *x1, float *y1, float *x2, float *y2)
{
	while(1) {
		int c1 = getLocationCode(*x1,*y1);
		int c2 = getLocationCode(*x2,*y2);
		int c1ORc2 = c1|c2;
		int c1ANDc2 = c1&c2;
		if(c1ORc2 == 0) 
		{
					return;
		}
		if(c1ANDc2 != 0)
		{
			*x1 = *x2;
			*y1 = *y2;
			return;
		}
		float xc,yc;
		if((c1ORc2 >> 0) & 1 == 1)
		{
			xc = MIN_SIZE;
			yc = ((*y2 - *y1)/(*x2-*x1))*(xc - *x1) + *y1;
			if((c1 >> 0) & 1 == 1) {
				*x1 = xc;
				*y1 = yc;
			}
			else {
				*x2 = xc;
				*y2 = yc;
			}
			
			continue;
		}
		if((c1ORc2 >> 1) & 1 == 1)
		{
			xc = MAX_SIZE-1;
			yc = ((*y2-*y1)/(*x2-*x1))*(xc-*x1) + *y1;
			if((c1 >> 1) & 1 == 1) {
				*x1 = xc;
				*y1 = yc;
			}
			else {
				*x2 = xc;
				*y2 = yc;
			}
			continue;
		}
		if((c1ORc2 >> 2) & 1 == 1)
		{
			yc = MIN_SIZE;
			xc = ((*x2-*x1)/(*y2-*y1))*(yc - *y1) + *x1;
				if((c1 >> 2) * 1 == 1) {
				*x1 = xc;
				*y1 = yc;
			}
			else {
				*x2 = xc;
				*y2 = yc;
			}
			continue;
		}
		if((c1ORc2 >> 3) & 1 == 1)
		{
			yc = MAX_SIZE-1;
			xc = ((*x2-*x1)/(*y2-*y1))*(yc-*y1) + *x1;
			if((c1 >> 3) & 1 == 1) {
				*x1 = xc;
				*y1 = yc;
			}
			else {
				*x2 = xc;
				*y2 = yc;
			}
			continue;
		}
	}
}

void drawPolygon(int x1, int y1)
{
	int *x, *y, i=0;
	//polygonPoints.push_back(make_pair(x1,y1));
	int x2,y2;
	i++;
	if(!f.eof() && line!="%%%END")
	{
		cout<<line<<endl;
		stringstream ss(line);
		string num;
		ss >> ws;
		getline(ss, num, ' ');
		x2 = stoi(trim(num));
		getline(ss,num, ' ');
		y2 = stoi(trim(num));
		getline(ss,num, ' ');
		
		if(num=="moveto")
		{
			getline(f,line);
			drawPolygon(x2, y2);
			return;
		}
		getline(f,line);
		if(line == "" || line =="\n")
			getline(f,line);
		createLine(x1,y1,x2,y2, true);
		i++;
	}
}

bool switchFlag(bool f)
{
	if(f)
		return false;
	else return true;
}
// Fills in a particular polygon using scan filling
void fillPolygon(string pixels[])
{
	int polygonYMax,polygonYMin;
	bool first = true;
	for(list<pair<int,int>>::iterator it = polygonPoints.begin(); it!=
			polygonPoints.end();it++)
	{
		if(first)
		{
			polygonYMax = (*it).second;
			polygonYMin = (*it).second;
			first = false;
		}
		if(it->second < polygonYMin)
		{
			polygonYMin = it->second;
		}
		else if (it->second > polygonYMax)
		{
			polygonYMax = it->second;
		}
	}
	bool draw = false;
	set<int> xpoints;
	if(polygonYMin < MIN_SIZE)
		polygonYMin = MIN_SIZE;
	if(polygonYMax > MAX_SIZE-1)
		polygonYMax = MAX_SIZE-1;
	for(float y0=polygonYMin+1;y0<=polygonYMax; y0++)
	{
		draw=false;
		xpoints.clear();
		for(list<pair<int,int>>::iterator it = polygonPoints.begin();
					it != polygonPoints.end(); it++)
		{
			float x1,y1,x2,y2,ymax,ymin;
			
			x1 =(float) (*it).first; 
			y1 =(float) (*it).second;
			++it;
			x2 = (float)(*it).first; 
			y2 = (float)(*it).second;
			
			if(y2>y1) {
			ymax = y2;
			ymin = y1;
			}
			else {
				ymax = y1;
				ymin = y2;
			}
			
			if(y2-y1 == 0)
				continue;
			if(y0 == ymax)
				continue;
				
			if(y0>=ymin && y0<ymax) {
				float x=0;
				if(x2-x1!=0)
				{
					x = ((x2-x1)/(y2-y1)) * (y0 - y1) + x1;
				}
				else x = x1;
				xpoints.insert((int)x);
			}
		}
		for(int x = 0;x<MAX_SIZE;x++)
		{
			if(xpoints.find(x) != xpoints.end() )
			{
				draw = switchFlag(draw);
				continue;
			}
			if(draw)
			{
				putPixel(x,y0,pixels,'+');
			}
		}
	}
}
