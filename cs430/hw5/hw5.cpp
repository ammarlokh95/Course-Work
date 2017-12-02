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
#define PI 3.14159265
using namespace std;
void createLine(float,float,float,float, bool);
void putPixel(int,int,string[MAX_SIZE],char);
void writeFile();
void getPoints();
void clipLine(float*,float*,float*,float*);
void drawPolygon(int,int);
void fillPolygon();
bool createPolygonPoints(int,int);
void transformPolygon();
void ww2Screen();
void world2Viewport();
string content[MAX_SIZE+4]={
			"500 500 2 1",
			"- c #000000",
			"# c #ffffff",
			"+ c #b22222"
		};
string pixels[MAX_SIZE];
string file= "hw5_1.ps";
float scalingF = 1.0;
bool fillPoly = true;
int degOfRotation = 0, xTrans=0, yTrans=0,wwlx =0, wwly=0, wwux =499, wwuy=499, vplx=0,vply=0,vpux=499,vpuy=499;
list<pair<int,int>> polygonPoints;
ifstream f;
string line;
string f_name;
void setArgument(char arg[], char val[])
{
	if(strcmp(arg, "-f")==0)
	{
		string s(val);
		file = s;
	}
	else if(strcmp(arg, "-s")==0)
	{
		scalingF= atof(val);
	}
	else if(strcmp(arg, "-r")==0)
	{
		degOfRotation = atoi(val);
	}
	else if(strcmp(arg, "-m")==0)
	{
		xTrans = atoi(val);
	}
	else if(strcmp(arg, "-n")==0)
	{
		yTrans = atoi(val);
	}
	else if(strcmp(arg, "-a")==0)
	{
		wwlx = atoi(val);
	}
	else if(strcmp(arg, "-b")==0)
	{
		wwly = atoi(val);
	}
	else if(strcmp(arg, "-c")==0)
	{
		wwux = atoi(val);
	}
	else if(strcmp(arg, "-d")==0)
	{
		wwuy = atoi(val);
	}
	else if(strcmp(arg, "-j")==0)
	{
		vplx = atoi(val);
	}
	else if(strcmp(arg, "-k")==0)
	{
		vply = atoi(val);
	}
	else if(strcmp(arg, "-o")==0)
	{
	    vpux = atoi(val);
	}
	else if(strcmp(arg, "-p")==0)
	{
		vpuy = atoi(val);
	}			
	else if(strcmp(arg, "-fp")==0)
	{
		if(strcmp(val,"y")==0)
			 fillPoly=true;
		else fillPoly=false;
	}
}
// Entry point...
// Reads command line arguments and gets the 
// text from the file indicated
int main(int argc, char *argv[])
{	
	if(argc>=3)
	{
		for(int i=1;i<argc;i+=2)
		{
			setArgument(argv[i],argv[i+1]);	
		}
	}
	for(int i=0;i<MAX_SIZE;i++)
	{
		for(int j=0;j<MAX_SIZE;j++)
		{
			pixels[i]= pixels[i]+ "#";
		}
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
				if(line=="" || line=="\n")
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
	while(!f.eof() && line!="%%%END")
	{
		//cout<<line<<endl;
		stringstream ss(line);
		string num;	
		getline(ss,num,' ');
		if(trim(num) == "")
			getline(ss,num,' ');
		x1 = stoi(trim(num));
		getline(ss,num,' ');
		if(trim(num) == "")
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
		if(trim(num) == "")
			getline(ss,num,' ');
		x2 = stoi(num);
		getline(ss,num,' ');
		if(trim(num) == "")
			getline(ss,num,' ');
		y2 = stoi(num);
		getline(f,line);
		while(line == "" || line =="\n")
			getline(f,line);
		createLine(x1,y1,x2,y2, false);
	}
	writeFile();
}

// Draws the line for the points given
void createLine(float x1, float y1, float x2, float y2, bool isPolygon)
{	
	clipLine(&x1,&y1,&x2,&y2);	
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
}
// Puts the pixel in the right place in the pixel matrix
void putPixel(int x, int y, string pixels[MAX_SIZE], char a)
{
	pixels[MAX_SIZE-y-1][x] = a;
}

// Creates and writes the XPM file
void writeFile()
{

	for(int i =0; i<MAX_SIZE;i++)
	{
		content[4+i] =pixels[i];
	}
	ofstream f;
//	f_name += ".xpm";
//	f.open(f_name.c_str(), ios::trunc);
//		if(f) 
		{
		cout << "\\* XPM *\\\nstatic const char *xpm[] = {\n";
		int i =0;
		for(i=0;i<MAX_SIZE+3;i++)
		{
			cout << "\"" + content[i] + "\",\n";
		}
		cout << "\"" + content[i] + "\"\n};";
	//	f.close();
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
void clipPolygon(int xmin,int ymin,int xmax, int ymax)
{
	ymax++;xmax++;
	list<pair<int,int>> newPolygon;
	list<pair<int,int>> tempPolygon = polygonPoints;
	//left edge
	{
//		cout<<"left Edge"<<endl;
		newPolygon.clear();
		list<pair<int,int>>::iterator it = tempPolygon.begin();
		for(it;it!=prev(tempPolygon.end());)
		{
			float x1 = it->first, y1 = it->second;
			++it;
			float x2 = it->first, y2 = it->second;
//			cout<<"x1 y1: "<<x1<<","<<y1<<"  x2 y2: "<<x2<<","<<y2<<endl;
			
			if(x1>=xmin && x2 >=xmin)
			{
				newPolygon.push_back(make_pair(x1,y1));
				//newPolygon.push_back(make_pair(x2,y2));
			}
			else if(x1>=xmin && x2<xmin)
			{
//				cout<<"Clipping "<<x2<<" "<<y2<<endl;
				newPolygon.push_back(make_pair(x1,y1));
				float xc = xmin;
				float yc = ((y2-y1)/(x2-x1))*(xc-x1) + y1;
				newPolygon.push_back(make_pair(xc,yc));

				//newPolygon.push_back(make_pair(xc,yc));
			}
			else if(x1<xmin && x2<xmin)
			{
			}
			else if(x1<xmin && x2>=xmin)
			{
//				cout<<"Clipping "<<x1<<" "<<y1<<endl;
				float xc = xmin;
				float yc = ((y2-y1)/(x2-x1))*(xc-x1) + y1;
				newPolygon.push_back(make_pair(xc,yc));
				//it->first = xc;
				//it->second = yc;
				//newPolygon.push_back(make_pair(x2,y2));
			}
		}
		newPolygon.push_back(make_pair(newPolygon.begin()->first,newPolygon.begin()->second));
	}
	tempPolygon=newPolygon;
	list<pair<int,int>>::iterator it;
	it = tempPolygon.begin();
//	for(it;it!=tempPolygon.end();++it)
	{
//		cout<<"x,y: "<<(it->first)<<", "<<(it->second)<<endl;
		//newPolygon.push_back(make_pair(it->first,it->second));
	}	
	
	newPolygon.clear();
	//bottom edge
	{
		//cout<<"bottom Edge"<<endl;
		list<pair<int,int>>::iterator it = tempPolygon.begin();
		for(it;it!=prev(tempPolygon.end());)
		{
			float x1 = it->first, y1 = it->second;
			++it;
			float x2 = it->first, y2 = it->second;
//			cout<<"x1 y1: "<<x1<<","<<y1<<"  x2 y2: "<<x2<<","<<y2<<endl;
			if(y1>=ymin && y2 >= ymin)
			{
				newPolygon.push_back(make_pair(x1,y1));
				//newPolygon.push_back(make_pair(x2,y2));
			}
			else if(y1>=ymin && y2<ymin)
			{
//				cout<<"Clipping "<<x2<<" "<<y2<<endl;
				newPolygon.push_back(make_pair(x1,y1));
				float yc = ymin;
				float xc = ((x2-x1)/(y2-y1))*(yc-y1) + x1;
				newPolygon.push_back(make_pair(xc,yc));

				//it->first = xc;
			    //it->second = yc;	
				//newPolygon.push_back(make_pair(xc,yc));
			}
			else if(y1<ymin && y2<ymin)
			{
			}
			else if(y1<ymin && y2>=ymin)
			{
//				cout<<"Clipping "<<x1<<" "<<y1<<endl;	
				float yc = ymin;
				float xc = ((x2-x1)/(y2-y1))*(yc-y1) + x1;
				newPolygon.push_back(make_pair(xc,yc));
				//it->first = xc;
			    //it->second = yc;			
				//newPolygon.push_back(make_pair(x2,y2));
			}
		}
	newPolygon.push_back(make_pair(newPolygon.begin()->first,newPolygon.begin()->second));
	}
	tempPolygon.clear();
	tempPolygon=newPolygon;
	it = tempPolygon.begin();
//	for(it;it!=tempPolygon.end();++it)
	{
//		cout<<"x,y: "<<(it->first)<<", "<<(it->second)<<endl;
		//newPolygon.push_back(make_pair(it->first,it->second));
	}
	newPolygon.clear();
	//right edge
	{
		//cout<<"right Edge"<<endl;
		list<pair<int,int>>::iterator it = tempPolygon.begin();
		for(it;it!=prev(tempPolygon.end());)
		{
			float x1 = it->first, y1 = it->second;
			++it;
			float x2 = it->first, y2 = it->second;
//			cout<<"x1 y1: "<<x1<<","<<y1<<"  x2 y2: "<<x2<<","<<y2<<endl;
			if(x1<xmax && x2 <xmax)
			{
				newPolygon.push_back(make_pair(x1,y1));
				//newPolygon.push_back(make_pair(x2,y2));
			}
			else if(x1<xmax && x2>=xmax)
			{
//				cout<<"Clipping "<<x2<<" "<<y2<<endl;	
				newPolygon.push_back(make_pair(x1,y1));
				float xc = xmax-1;
				float yc = ((y2-y1)/(x2-x1))*(xc-x1) + y1;
				newPolygon.push_back(make_pair(xc,yc));
				//newPolygon.push_back(make_pair(xc,yc));
			}
			else if(x1>=xmax && x2>=xmax)
			{
			}
			else if(x1>=xmax && x2<xmax)
			{
//				cout<<"Clipping "<<x1<<" "<<y1<<endl;	
				float xc = xmax;
				float yc = ((y2-y1)/(x2-x1))*(xc-x1) + y1;
				newPolygon.push_back(make_pair(xc,yc));
				//it->first = xc;
			    //it->second = yc;			
				//newPolygon.push_back(make_pair(x2,y2));
			}
		}
		newPolygon.push_back(make_pair(newPolygon.begin()->first,newPolygon.begin()->second));
	}
	tempPolygon=newPolygon;
	it = tempPolygon.begin();
//	for(it;it!=tempPolygon.end();++it)
//	{
//		cout<<"x,y: "<<(it->first)<<", "<<(it->second)<<endl;
		//newPolygon.push_back(make_pair(it->first,it->second));
//	}
	
	newPolygon.clear();

	//top edge
	{
		//cout<<"top Edge"<<endl;
		list<pair<int,int>>::iterator it = tempPolygon.begin();
		for(it;it!=prev(tempPolygon.end());)
		{
			float x1 = it->first, y1 = it->second;
			++it;
			float x2 = it->first, y2 = it->second;
//			cout<<"x1 y1: "<<x1<<","<<y1<<"  x2 y2: "<<x2<<","<<y2<<endl;
			if(y1<ymax && y2 <ymax)
			{
					newPolygon.push_back(make_pair(x1,y1));
			}
			else if(y1<ymax && y2>=ymax)
			{
//				cout<<"Clipping "<<x2<<" "<<y2<<endl;	
				newPolygon.push_back(make_pair(x1,y1));
				float yc = ymax-1;
				float xc = ((x2-x1)/(y2-y1))*(yc-y1) + x1;
				newPolygon.push_back(make_pair(xc,yc));
			}
			else if(y1>=ymax && y2>=ymax)
			{
			}
			else if(y1>=ymax && y2<ymax)
			{
//				cout<<"Clipping "<<x1<<" "<<y1<<endl;
				float yc = ymax-1;
				float xc = ((x2-x1)/(y2-y1))*(yc-y1) + x1;
				newPolygon.push_back(make_pair(xc,yc));
				//newPolygon.push_back(make_pair(x2,y2));
			}
		}
		newPolygon.push_back(make_pair(newPolygon.begin()->first,newPolygon.begin()->second));
	}
	tempPolygon = newPolygon;
	newPolygon.clear();
	it = tempPolygon.begin();
	for(it;it!=tempPolygon.end();++it)
	{
//	cout<<"x,y: "<<(it->first)<<", "<<(it->second)<<endl;
		//newPolygon.push_back(make_pair(it->first,it->second));
	}	
	polygonPoints = tempPolygon;
}
bool createPolygonPoints(int x,int y)
{
	polygonPoints.push_back(make_pair(x,y));
}
void drawPolygon(int x, int y)
{
	polygonPoints.push_back(make_pair(x,y));
	int x1,y1,x2,y2;
	while(!f.eof() && line!="%%%END")
	{
		//cout<<line<<endl;
		stringstream ss(line);
		string num;
		ss >> ws;
		getline(ss, num, ' ');
		if(trim(num) == "")
			getline(ss,num,' ');
		x = stoi(trim(num));
		getline(ss,num, ' ');
		if(trim(num) == "")
			getline(ss,num,' ');
		y = stoi(trim(num));
		getline(f,line);
		while(line == " " || line =="\n")
			getline(f,line);	
		createPolygonPoints(x,y);
		if(line=="stroke")
		{	
			int l,m,p,q;
			transformPolygon();
			clipPolygon(wwlx,wwly,wwux,wwuy);
		//	ww2Screen();
			world2Viewport();
//			clipPolygon(vplx,vply,vpux,vpuy);
			if(!fillPoly) {
			list<pair<int,int>>::iterator it = polygonPoints.begin();
			for(;it!=prev(polygonPoints.end());)
			{
				int l=0,m=0,p=0,q=0;
				l=it->first;m=it->second;
				++it;
				p=it->first;q=it->second;
//				cout<<"x y: "<<l<<" "<<m<<"   p,q: "<<p<<" "<<q<<endl;
				createLine(l,m,p,q,true);
			}
			}
			else fillPolygon();
			polygonPoints.clear();
			getline(f,line);
			while(line=="" || line=="\n")
				getline(f,line);
		}
	}
 	writeFile();
}

bool switchFlag(bool f)
{
	if(f)
		return false;
	else return true;
}

void ww2Screen()
{
	list<pair<int,int>>::iterator it = polygonPoints.begin();
	list<pair<int,int>> newPolygon;
	int points[3];
	float max = MAX_SIZE-1, min = MIN_SIZE;
	float x1t = wwlx, y1t = wwly, x2t = wwux, y2t = wwuy;
	float sx = (max)/(x2t - x1t);
	float sy =(max)/(y2t - y1t);
//	cout<<"sx:"<<sx<<" sy:"<<sy<<" xt:"<<x1t<<" yt:"<<y1t<<endl;
	for(it;it!=polygonPoints.end();++it)
	{
		float x = it->first, y = it->second;
		points[0] =  (x*sx) - (sx * x1t);
		points[1] =  (sy * y - sy*y1t);
		points[2] = 1;
		newPolygon.push_back(make_pair(points[0],points[1]));

//		cout<<"x, y: "<<x<<", "<<y<<" to  xt,yt:"<<points[0]<<","<<points[1]<<endl;
	}
	polygonPoints.clear();
	polygonPoints = newPolygon;
}
void world2Viewport()
{
	list<pair<int,int>>::iterator it = polygonPoints.begin();
	list<pair<int,int>> newPolygon;
	int points[3];
	float ut1 = vplx, vt1 = vply, ut2 = vpux, vt2 = vpuy;
	float xt1 = wwlx, yt1 = wwly, xt2 = wwux, yt2 = wwuy;
	float sx = (ut2 - ut1)/(xt2 - xt1);
	float sy = (vt2 - vt1)/(yt2 - yt1);
//	cout<<"sx:"<<sx<<" sy:"<<sy<<endl;
	for(it;it!=polygonPoints.end();it++)
	{
		int x = it->first, y = it->second;
		points[0] = (int) (x*sx - sx*xt1 + ut1);
		points[1] = (int) (y*sy + vt1 - sy*yt1);
		points[2] = 1;
		newPolygon.push_back(make_pair(points[0],points[1]));
//		cout<<"x, y: "<<x<<", "<<y<<" to  xt,yt:"<<points[0]<<","<<points[1]<<endl;
	
	}
	polygonPoints.clear();
	polygonPoints = newPolygon;
}
// Fills in a particular polygon using scan filling
void fillPolygon()
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
					it != prev(polygonPoints.end());)
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

void transformPolygon()
{
	list<pair<int,int>>::iterator it = polygonPoints.begin();
	list<pair<int,int>> newPolygon;
	int points[3];
	float sf = scalingF;
	float coso = cos (degOfRotation * (PI/180));
	float sino = sin (degOfRotation * (PI/180));
	for(it;it!=polygonPoints.end();++it)
	{
		int x = it->first, y =it->second;
		points[0]=(int)(x*sf*coso) - (y*sf*sino) + xTrans;
		points[1]=(int)(x*sf*sino) + (y*sf*coso) + yTrans;
		points[2]=1;
		newPolygon.push_back(make_pair(points[0],points[1]));
	}
	polygonPoints.clear();
	polygonPoints = newPolygon;
}
