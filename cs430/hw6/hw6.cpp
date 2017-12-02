#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include<stdlib.h>
#include<math.h>
#include<list>
#include<vector>
#include<sstream>
#include<array>
#define MAX_SIZE 500
#define MIN_SIZE 0
#define front 0.6
#define back -0.6
using namespace std;

void clipLine(float*,float*,float*,float*);
void setArgument(char*, char*);
void createProjectionMatrix();
string trim(string&,string&);
void createViewMatix();
void world2Viewport();
void projectOnto2D();
bool polygonIsInPlane();
void world2CanonicalView();
void createLine(float, float, float, float);
void putPixel(int,int,string[], char);
void writeFile();
void clipPolygon(vector<vector<float>>, int,int,int,int);
bool faceInPlane(float[][2]);
bool drawSomething = true;
ifstream f;
string file;
int vplx=0, vply=0, vpux=499, vpuy=499;
float PRP[3] = {0.0, 0.0, 1.0}, VRP[3] = {0.0, 0.0, 0.0};
float VPN[3] = {0.0, 0.0, -1.0}, VUP[3] = {0.0, 1.0, 0.0};
float VRCumin = -0.7, VRCvmin = -0.7, VRCumax = 0.7, VRCvmax = 0.7;
array<array<float,4>,4> viewMatrix;
array<array<float,4>,4> PMMatrix;
bool isParallel = false;
list<vector<float>> vertices;
list<pair<float,float>> vertices2D;
list<vector<int>> faces;
string content[MAX_SIZE+4]={
			"500 500 2 1",
			"- c #000000",
			"# c #ffffff",
			"+ c #b22222"
		};
string pixels[MAX_SIZE];
string f_name;
int main(int argc, char* argv[])
{
	if (argc >= 3)
	{
		for (int i = 1;i < argc;i += 2)
		{
			if(strcmp(argv[i],"-P")==0)
			{	
				isParallel = true;
				i--;
			}
			else if(strcmp(argv[i], "-ds")==0)
			{
				drawSomething = false;
				i--;
			}
			else {
				setArgument(argv[i], argv[i+1]);
			}
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
	string x,y,z;
	string type;
	if(f.is_open()) {
		while(f >> type >> x >> y >> z)
		{
			if(type == "v")
				vertices.push_back({stof(x), stof(y), stof(z)});
			if(type == "f")
				faces.push_back({stoi(x), stoi(y), stoi(z)});
		}
	}
	f.close();
	createViewMatix();
	createProjectionMatrix();
	world2CanonicalView();
	bool isPlane = polygonIsInPlane();
	if(isPlane || drawSomething) 
	{
		projectOnto2D();
		world2Viewport();
		vector<vector<float>> v2d;
		list<pair<float,float>>::iterator it = vertices2D.begin();
		int i =0;
		v2d.push_back({0,0});
		for(i=0;it!=vertices2D.end();++it,i++)
		{
			float l=0,m=0,p=0,q=0;
			l=it->first;m=it->second;
			v2d.push_back({l,m});
		}
/*
		for(int i=0;i<v2d.size();i++)
		{
			cout<<i<<" = ";
			for(int j=0;j<v2d[i].size();j++)
				cout<<v2d[i][j]<<" ";
			cout<<endl;
		}
		cout<<i;
*/
		list<vector<int>>::iterator it1 = faces.begin();
		for(it1;it1!=faces.end();it1++)
		{
			int i = it1->at(0), j = it1->at(1), k = it1->at(2);
			float v[][2] = {
				{v2d[i][0], v2d[i][1]},
				{v2d[j][0], v2d[j][1]},
				{v2d[k][0], v2d[k][1]}
			};
			if(!faceInPlane(v))
				continue;
			createLine(v2d[i][0], v2d[i][1], v2d[j][0], v2d[j][1]);
			createLine(v2d[j][0], v2d[j][1], v2d[k][0], v2d[k][1]);	
			createLine(v2d[k][0], v2d[k][1], v2d[i][0], v2d[i][1]);
		}
	}
	writeFile();
}
bool faceInPlane(float v[][2])
{
	for(int i=0;i<3;i++)
	{
		if(v[i][0] < vplx || v[i][0] > vpux)
			return false;
		if(v[i][1] < vply || v[i][1] > vpuy)
			return false;
	}
	return true;
}
void world2Viewport()
{
//	cout<<"got here";
	list<pair<float,float>>::iterator it = vertices2D.begin();
	list<pair<float,float>> newPolygon;
	float points[3];
	float sx = (vpux - vplx)/(1 - (-1));
	float sy = (vpuy - vply)/(1 - (-1));

	for(it;it!=vertices2D.end();it++)
	{
		float x = it->first, y = it->second;
		points[0] = (x*sx + sx + vplx);
		points[1] = (y*sy + vply + sy);
		points[2] = 1;
		newPolygon.push_back(make_pair(points[0],points[1]));
	}
	vertices2D.clear();
	vertices2D = newPolygon;
	newPolygon.clear();
}
void projectOnto2D()
{
	list<vector<float>>::iterator it = vertices.begin();
	for(it;it!=vertices.end();it++)
	{
		if(isParallel)
			vertices2D.push_back(make_pair(it->at(0),it->at(1)));
		else
			vertices2D.push_back(make_pair(it->at(0)/(-it->at(2)), (it->at(1))/(-it->at(2))));
	//	cout<<"2D x y:"<<it->at(0)<<" "<<it->at(1)<<endl;
	}	
	vertices.clear();
}

bool polygonIsInPlane()
{
	list<vector<float>>::iterator it = vertices.begin();
	for(it;it!=vertices.end();++it)
	{
		float x,y,z;
		x = it->at(0);
		y = it->at(1);
		z = it->at(2);
		if(isParallel)
		{
			if(y>1 || y<-1 || x>1 ||x<-1 || z<-1 || z>0)
				return false;
		}
		else {
			if(y>-z || y<z || x>-z || x<z || z<-1 || z>((PRP[2] -
						front)/(back - PRP[2])))
				return false;
		}		
	}
	return true;
}

void world2CanonicalView()
{
	float PMxVMatrix[4][4];
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			PMxVMatrix[i][j] = 0;
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			for(int k=0;k<4;k++)
			{
				PMxVMatrix[i][j] += PMMatrix[i][k] * viewMatrix[k][j];
			}
		//	cout<<PMxVMatrix[i][j]<<" ";
		}
	//	cout<<endl;
	}
	list<vector<float>> newVertices;
	list<vector<float>>::iterator it = vertices.begin();
	for(it;it!=vertices.end();++it)
	{
		float x[4];
		for(int i=0;i<4;i++)
		{
			x[i] = PMxVMatrix[i][0] * it->at(0) + PMxVMatrix[i][1] * it->at(1) + PMxVMatrix[i][2] * it->at(2) + PMxVMatrix[i][3] * 1;
		}
		//cout<<"new x, y, z: "<<x[0]<<" "<<x[1]<<" "<<x[2]<<endl;	
		newVertices.push_back({x[0], x[1], x[2]});
	}
    vertices.clear();	
	vertices = newVertices;
	newVertices.clear();
}

void createProjectionMatrix()
{
	if(isParallel) {
		PMMatrix = {{
			{{2/(VRCumax - VRCumin), 0, ((VRCumax + VRCumin) - 2*PRP[0])/ ((VRCumax - VRCumin) * PRP[2]), -(VRCumax+VRCumin)/(VRCumax - VRCumin)}},
			{{0, 2/(VRCvmax - VRCvmin), ((VRCvmax + VRCvmin) - 2*PRP[1])/ ((VRCvmax - VRCvmin) * PRP[2]), -(VRCvmax+VRCvmin)/(VRCvmax - VRCvmin)}},
			{{0, 0, 1/(front - back), -(front*1.0)/(front-back)}},
			{{0, 0, 0, 1}}
			}};
	}
	else {
		PMMatrix = {{
			{{(2*PRP[2])/((VRCumax - VRCumin)*(PRP[2] - back)), 0, ((VRCumax + VRCumin) - 2*PRP[0])/((VRCumax - VRCumin)*(PRP[2] - back)), (-(VRCumax + VRCumin) * PRP[2])/((VRCumax - VRCumin)*(PRP[2] - back))}},
			{{0, (2*PRP[2])/((VRCvmax - VRCvmin)*(PRP[2] - back)), ((VRCvmax + VRCvmin) - 2* PRP[1])/((VRCvmax-VRCvmin)*(PRP[2]-back)), (-(VRCvmax +VRCvmin)*PRP[2])/((VRCvmax - VRCvmin)*(PRP[2] - back))}},
			{{0, 0, 1/(PRP[2] - back), (-PRP[2])/(PRP[2] - back)}},
			{{0, 0, 0, 1}}
			}};
	}
/*
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<4;j++)
			cout << PMMatrix[i][j] <<" ";
		cout<<endl;
	}
*/
}

void crossproduct(float u[3],float v[3], float c[3])
{
	c[0] = u[1]*v[2] - v[1]*u[2];
	c[1] = v[0]*u[2] - u[0]*v[2];
	c[2] = u[0]*v[1] - v[0]*u[1];
}
void createViewMatix()
{
	float u[3];
	float n[3];
	float v[3];
	for(int i=0;i<3;i++)
	{	n[i] = VPN[i]/(sqrt(pow(VPN[0], 2.0) + pow(VPN[1], 2.0) + pow(VPN[2], 2.0)));
	}
	crossproduct(VUP,n,u);
	for(int i=0;i<3;i++)
	{
		u[i] = u[i]/(sqrt(pow(u[0],2.0) + pow(u[1],2.0) + pow(u[2], 2.0)));
	}
	crossproduct(n,u,v);
	for(int i=0;i<3;i++)
	{
		v[i] = v[i]/(sqrt(pow(v[0],2.0) + pow(v[1],2.0) + pow(v[2], 2.0)));
	}
	float rotationMatrix[4][4] = {
		{u[0], u[1], u[2], 0},
		{v[0], v[1], v[2], 0},
		{n[0], n[1], n[2], 0},
		{   0,    0,    0, 1}
	};
	float VPNTrans[4][4] = {
		{1, 0, 0, -VRP[0]}, 
		{0, 1, 0, -VRP[1]}, 
		{0, 0, 1, -VRP[2]},
		{0, 0, 0,      1}
	};
	for(int i = 0; i<4;i++)
		for(int j=0; j<4; j++)
			for(int k=0; k<4; k++)
				viewMatrix[i][j] += rotationMatrix[i][k] * VPNTrans[k][j];
/*
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<4;j++)
			cout << viewMatrix[i][j] <<" ";
		cout<<endl;
	}
	cout<<endl;
*/	 				
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
void setArgument(char arg[], char val[])
{
	if (strcmp(arg, "-f") == 0)
	{
		string s(val);
		file = s;
//		cout <<"file: " << file << endl;
	}
	else if (strcmp(arg, "-j") == 0)
	{
		vplx = atoi(val);
//		cout << "-j " << vplx<<endl;
	}
	else if (strcmp(arg, "-k") == 0)
	{
		vply = atoi(val);
//		cout << "-k " << vply << endl;
	}
	else if (strcmp(arg, "-o") == 0)
	{
		vpux = atoi(val);
//		cout << "-o " << vpux << endl;
	}
	else if (strcmp(arg, "-p") == 0)
	{
		vpuy = atoi(val);
//		cout << "-p " << vpuy << endl;
	}
	else if (strcmp(arg, "-x") == 0)
	{
		PRP[0] = atof(val);
//		cout << "-x " << PRP[0] << endl;
	}
	else if (strcmp(arg, "-y") == 0)
	{
		PRP[1] = atof(val);
//		cout << "-y " << PRP[1] << endl;
	}
	else if (strcmp(arg, "-z") == 0)
	{
		PRP[2] = atof(val);
//		cout << "-z " << PRP[2] << endl;
	}
	else if (strcmp(arg, "-X") == 0)
	{
		VRP[0] = atof(val);
//		cout << "-X " << VRP[0] << endl;
	}
	else if (strcmp(arg, "-Y") == 0)
	{
		VRP[1] = atof(val);
//		cout << "-Y " << VRP[1] << endl;
	}
	else if (strcmp(arg, "-Z") == 0)
	{
		VRP[2] = atof(val);
//		cout << "-Z " << VRP[2] << endl;
	}
	else if (strcmp(arg, "-q") == 0)
	{
		VPN[0] = atof(val);
//		cout << "-q " << VPN[0] << endl;
	}
	else if (strcmp(arg, "-r") == 0)
	{
		VPN[1] = atof(val);
//		cout << "-r " << VPN[1] << endl;
	}
	else if (strcmp(arg, "-w") == 0)
	{
		VPN[2] = atof(val);
//		cout << "-w " << VPN[2] << endl;
	}
	else if (strcmp(arg, "-Q") == 0)
	{
		VUP[0] = atof(val);
//		cout << "-Q " << VUP[0] << endl;
	}
	else if (strcmp(arg, "-R") == 0)
	{
		VUP[1] = atof(val);
//		cout << "-R " << VUP[1] << endl;
	}
	else if (strcmp(arg, "-W") == 0)
	{
		VUP[2] = atof(val);
//		cout << "-W " << VUP[2] << endl;
	}
	else if (strcmp(arg, "-u") == 0)
	{
		VRCumin = atof(val);
//		cout << "-u " << VRCumin << endl;
	}
	else if (strcmp(arg, "-v") == 0)
	{
		VRCvmin = atof(val);
//		cout << "-v " << VRCvmin << endl;
	}
	else if (strcmp(arg, "-U") == 0)
	{
		VRCumax = atof(val);
//		cout << "-U " << VRCumax << endl;
	}
	else if (strcmp(arg, "-V") == 0)
	{
		VRCvmax = atof(val);
//		cout << "-V " << VRCvmax << endl;
	}
	else if (strcmp(arg, "-p") == 0)
	{
		isParallel = true;
//		cout << "Parallel" << endl;
	}
}

// Draws the line for the points given
void createLine(float x1, float y1, float x2, float y2)
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
	//ofstream f;
	//f_name += ".xpm";
	//f.open(f_name.c_str(), ios::trunc);
	//	if(f) 
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
void clipPolygon(vector<vector<float>> v, int xmin,int ymin,int xmax, int ymax)
{
	ymax++;xmax++;
	list<pair<int,int>> newPolygon;
	list<pair<int,int>> tempPolygon;
	for(int i =0;i<v.size();i++)
		tempPolygon.push_back(make_pair(v[i][0],v[i][1]));
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
	int i=0;
	for(it;it!=tempPolygon.end();++it,i++)
	{
//	cout<<"x,y: "<<(it->first)<<", "<<(it->second)<<endl;
		//newPolygon.push_back(make_pair(it->first,it->second));
		v[i][0] = it->first;
		v[i][1] = it->second;
	}	
	
}
int getLocationCode(int x, int y) 
{
	if(x>=vplx && x<=vpux && y>=vply && y<=vpuy)
	{
		return 0;
	}
	else if(x<vplx && y <vply)
	{
		return 5;
	}
	else if(x<vplx && (y >= vply && y <= vpuy))
	{
		return 1;
	}
	else if(x<vplx && y >vpuy-1)
	{
		return 9;
	}
	else if(x>=vplx && x<=vpux && y>vpuy)
	{
		return 8;
	}
	else if(x>vpux && y>vpuy)
	{
		return 10;
	}
	else if(x>vpux && y>=vply && y<=vpuy)
	{
		return 2;
	}
	else if(x>vpux && y<vply)
	{
		return 6;
	}
	else if(x>=vplx && x<=vpux && y<vply)
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
			xc = vplx;
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
			xc = vpux;
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
			yc = vply;
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
			yc = vpuy;
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
