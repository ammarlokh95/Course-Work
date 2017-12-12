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
#include<set>
#include<map>
#include<iomanip>
#define MAX_SIZE 500
#define MIN_SIZE 0

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
void createPolygon();
void clipPolygon(vector<vector<float>>, int,int,int,int);
bool faceInPlane(float[][3]);
void zBuffering(float[][3]);
void fillPolygon();
bool drawSomething = true;
ifstream f;
string file[3]; int noOfFiles =0;int ff=1;
int vplx=0, vply=0, vpux=499, vpuy=499;
float PRP[3] = {0.0, 0.0, 1.0}, VRP[3] = {0.0, 0.0, 0.0};
float VPN[3] = {0.0, 0.0, -1.0}, VUP[3] = {0.0, 1.0, 0.0};
float VRCumin = -0.7, VRCvmin = -0.7, VRCumax = 0.7, VRCvmax = 0.7;
array<array<float,4>,4> viewMatrix;
array<array<float,4>,4> PMMatrix;
float front = 0.6, back = -0.6;
bool isParallel = false;
list<vector<float>> vertices;
list<vector<float>> vertices2D_Z;
list<vector<int>> faces;
string content[MAX_SIZE+62];
map<vector<float>,vector<float>> vptocp;
string pixels[MAX_SIZE];
float zpixels[MAX_SIZE][MAX_SIZE];
string f_name;

std::string int_to_hex( int i )
{
  std::stringstream stream;
  stream  << std::hex << i;
  return stream.str();
}
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
			zpixels[i][j] = back;
		}
	}
	if(noOfFiles == 1)
		content[0]= "500 500 22 1";
	else if(noOfFiles == 2)
		content[0] = "500 500 42 1";
	else if(noOfFiles == 3)
		content[0] = "500 500 62 1";
	content[1]= "# c #000000";
	for(int i=0;i<noOfFiles;i++) {
		faces.clear();
		vertices.clear();
		vertices2D_Z.clear();
		vptocp.clear();
		f.open(file[i].c_str());
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
		for(int j=2;j<22;j++)	
		{
			int color = (j-1)*(255.0/20.0);
			string hex = int_to_hex(color);
			if(ff==1) {
				content[j] = string() + char(j+34) + " c #"+hex+"0000"; }
			else if(ff==2)
				content[j+20] = string() + char(j+(54))+" c #00"+hex+"00";
			else if(ff==3)
				content[j+40] = string() + char(j+(74))+" c #0000" + hex;
		}
		createPolygon();
		ff++;
	}
	ff--;
	writeFile();
}

void createPolygon()
{
	createViewMatix();
	createProjectionMatrix();
	world2CanonicalView();
	bool isPlane = polygonIsInPlane();
	if(isPlane || drawSomething) 
	{
		projectOnto2D();
		world2Viewport();
		vector<vector<float>> v2d;
		list<vector<float>>::iterator it = vertices2D_Z.begin();
		int i =0;
		v2d.push_back({0,0,0});
		for(i=0;it!=vertices2D_Z.end();++it,i++)
		{
			float l=0,m=0;
			l=it->at(0);m=it->at(1);
			v2d.push_back({l,m,it->at(2)});
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
			float v[][3] = {
				{v2d[i][0], v2d[i][1], v2d[i][2]},
				{v2d[j][0], v2d[j][1], v2d[j][2]},
				{v2d[k][0], v2d[k][1], v2d[k][2]}
			};
			if(!faceInPlane(v))
				continue;
			float u[3] = {v2d[j][0] - v2d[i][0], v2d[j][1] - v2d[i][1], v2d[j][2] - v2d[i][2]}; 
			float v1[3] = {v2d[k][0] - v2d[i][0], v2d[k][1] - v2d[i][1], v2d[k][2] - v2d[i][2]};
			float nz = u[0]*v1[1] - u[1]*v1[0];
			if(nz>=0)
				zBuffering(v);
			//createLine(v2d[i][0], v2d[i][1], v2d[j][0], v2d[j][1]);
			//createLine(v2d[j][0], v2d[j][1], v2d[k][0], v2d[k][1]);	
			//createLine(v2d[k][0], v2d[k][1], v2d[i][0], v2d[i][1]);
		}
	}
}

bool faceInPlane(float v[][3])
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
	list<vector<float>>::iterator it = vertices2D_Z.begin();
	list<vector<float>> newPolygon;
	float points[3];
	float sx = (vpux - vplx)/(1 - (-1));
	float sy = (vpuy - vply)/(1 - (-1));

	for(it;it!=vertices2D_Z.end();it++)
	{
		float x = it->at(0), y = it->at(1);
		points[0] = (x*sx + sx + vplx);
		points[1] = (y*sy + vply + sy);
		points[2] = 1; 
		newPolygon.push_back({points[0],points[1],it->at(2)});
	}
	vertices2D_Z.clear();
	vertices2D_Z = newPolygon;
	newPolygon.clear();
}
float zmin=0,zmax=0;
float mapDepth(float z, float max, float min, float nmin, float nmax)
{
	return ((((z - min)/(max - min)) * (nmax - nmin)) + nmin);
}
void projectOnto2D()
{
	vertices2D_Z.clear();		
	list<vector<float>>::iterator it = vertices.begin();
	for(it;it!=vertices.end();it++)
	{
		float x, y, z;
		if(isParallel) {
			x=it->at(0);
			y=it->at(1);
			z=it->at(2);//mapDepth(it->at(2), zmax,zmin,back,front);
		}
		else {
			x = (it->at(0))/(-it->at(2));
			y = (it->at(1))/(-it->at(2));
			z = it->at(2);//mapDepth(it->at(2), zmax,zmin,back,front);
		}	
		vertices2D_Z.push_back({x, y, z});
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
	zmax=0;
	zmin=0;
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
	bool first=true;
	for(it;it!=vertices.end();++it)
	{
		float x[4];
		for(int i=0;i<4;i++)
		{
			x[i] = PMxVMatrix[i][0] * it->at(0) + PMxVMatrix[i][1] * it->at(1) + PMxVMatrix[i][2] * it->at(2) + PMxVMatrix[i][3] * 1;
			if(first) { zmin = x[2]; zmax = x[2]; first = false;}
			if(x[2]>zmax) zmax = x[2];
			else if (x[2]<zmin) zmin = x[2];
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
			{{0, 0, 1/(front - back), -(front*1)/(front-back)}},
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
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			{viewMatrix[i][j] = 0; PMMatrix[i][j]=0;}
	for(int i = 0; i<4;i++)
		for(int j=0; j<4; j++)
			for(int k=0; k<4; k++)
				viewMatrix[i][j] += rotationMatrix[i][k] * VPNTrans[k][j];	 				
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
		file[noOfFiles] = s;
		noOfFiles++;
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
	else if (strcmp(arg, "-F") == 0)
	{
		front = atof(val);
	}
	else if (strcmp(arg, "-B") == 0)
	{
		back = atof(val);
	}
	else if (strcmp(arg, "-g") == 0)
	{
		string s(val);
		file[noOfFiles] = s;
		noOfFiles++;
	}
	else if (strcmp(arg, "-i") == 0)
	{
		string s(val);
		file[noOfFiles] = s;
		noOfFiles++;
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
					putPixel(x1,y1,pixels,ff);
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
					putPixel(x1,y1,pixels,ff);
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
				putPixel(x1,y1,pixels,ff);
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
		int x = (20*ff + 2) + i;
		content[x] =pixels[i];
	}
	//ofstream f;
	//f_name += ".xpm";
	//f.open(f_name.c_str(), ios::trunc);
	//	if(f) 
		{
		cout << "\\* XPM *\\\nstatic const char *xpm[] = {\n";
		int i =0;
		int x = 20 * ff + 2;
		for(i=0;i<MAX_SIZE+x;i++)
		{
			cout << "\"" + content[i] + "\",\n";
		}
		cout << "\"" + content[i] + "\"\n};";
	//	f.close();
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
bool switchFlag(bool f)
{
	return !f; 
}

template <typename M, typename V> 
void MapToVec( const  M & m, V & v ) {
    for( typename M::const_iterator it = m.begin(); it != m.end(); ++it ) {
        v.push_back( it->second );
    }
}
void zBuffering(float v[][3])
{
	int polygonYMax=v[0][1], polygonYMin=v[0][1];
	int polygonXMax=v[0][0], polygonXMin=v[0][0];
	float maxPixel=0;
	float minPixel=0,f=0,b=-1;
	if(isParallel) { f=0;b=-1;}
	else {f=(PRP[2] - front)/(back - PRP[2]); b = -1;}
	maxPixel = 255* ((2.0)/(f+1));
	minPixel = 255* ((1.0)/(f+1));
//	cout<<zmax<<" "<<zmin<<"\t"<<maxPixel<<" "<<minPixel<<endl;
	int pos = 0;
	for(int i=0; i<3;i++)
	{
		pos = i; 
		for(int j=i;j<3;j++)
		{
			if(v[j][0] < v[pos][0])
				pos = j;
		}
		float temp[3] = {v[pos][0],v[pos][1],v[pos][2]};
		v[pos][0] = v[i][0]; v[pos][1] = v[i][1]; v[pos][2] = v[i][2];
		v[i][0] = temp[0];v[i][1]=temp[1];v[i][2]=temp[2];
	}
	for(int i=0;i<3;i++)
	{
		if((int)v[i][1]>polygonYMax)
			polygonYMax=v[i][1];
		else if((int)v[i][1]<polygonYMin)
			polygonYMin=v[i][1];
		if((int)v[i][0]>polygonXMax)
			polygonXMax=v[i][0];
		else if(polygonXMin>(int)v[i][0])
			polygonXMin = v[i][0];
	}
	bool draw = false;
	set<int> xpoints;
	for(float y0=polygonYMin+1;y0<=polygonYMax;y0++)
	{
		draw=false;
		xpoints.clear();
		for(int i=0;i<3;i++)
		{
			float x1,y1,x2,y2,z1,z2,x3,y3,z3,ymax,ymin;
			x1 = v[i][0];
			y1 = v[i][1];
			z1 = v[i][2];
			int j,k;
			if(i==2) {j=0; k=1;} 
			else {j=i+1; k = (j==2)? 0:j+1;}
			x2 = v[j][0];
 			y2 = v[j][1];
			z2 = v[j][2];
			x3 = v[k][0];
			y3 = v[k][1];
			z3 = v[k][2];
			if(y2>y1) {
				ymax = y2;
				ymin = y1; 
			}
			else { ymax = y1; ymin=y2; }
			if(y2==y1)
				continue;
			if(y0 == ymax)
				continue;
			if(y0>=ymin && y0<ymax) {
				float x=0;
				if(x2-x1!=0)
				{
					x = ((x2 - x1)/(y2 - y1))*(y0 - y1) + x1;
				}
				else x = x1;
				xpoints.insert((int)x);
			}	
		}
		
		float x1,y1,x2,y2,z1,z2,x3,y3,z3,za,zb,xa,xb,ya,yb,z;
	//	float cx1, cy1, cz1, cx1, cy2, cz2, cx3, cy3, cz3; 
		x2 = v[0][0];
		y2 = v[0][1];
		z2 = v[0][2];
		x1 = v[1][0];
 		y1 = v[1][1];
		z1 = v[1][2];
		x3 = v[1][0];
		y3 = v[2][1];
		z3 = v[2][2];
	
		if(y2!=y1)
			xa = ((x2 - x1)/(y2 - y1))*(y0 - y1) + x1;
		else xa = x1;
		if(y3!=y1)
			xb = ((x3-x1)/(y3-y1))*(y0-y1)+x1;
		else xb = x3;
		if(y2!=y1)	
		za = z1 - (z1 - z2)*((y1 - y0)/(y1 - y2));
		else za = z1 ;
		if(y3!=y1)
			zb = z1 - (z1 - z3)*((y1 - y0)/(y1 - y3));
		else zb = z3;
		for(int x = polygonXMin-1;x<=polygonXMax;x++)
		{
			float z=0;

			if(xpoints.find(x) != xpoints.end())
			{
				draw = switchFlag(draw);
			}
			if(draw)
			{
			
				if(xb!=xa)
					z = zb - (zb - za)*((xb-x)/(xb-xa));
				else 
					z = z3;
				if( z<front && z>zpixels[int(y0)][x]) {
					zpixels[(int)y0][x] = z;
					float pixel=0.0;
					float f,b;
					z = mapDepth(z,zmax,zmin,0,1);
					pixel = (255.0 * (z +1)/(f +1));
					pixel = mapDepth(pixel, maxPixel, minPixel, 0, 20);
					int add =0;
					if(ff==1) add =36;
					else if(ff==2) add =56;
					else if(ff==3) add=76;
					putPixel(x,int(y0),pixels,char(pixel + add));
				}

				/*else if(z>zpixels[int(y0)][x]) {
					cout<<x1<<" "<<y1<<" "<<z1<<endl;
					cout<<x2<<" "<<y2<<" "<<z2<<endl;
					cout<<x3<<" "<<y3<<" "<<z3<<endl;
					cout<<xa<<" "<<xb<<endl;
					cout<<x<<" "<<y0<<" "<<z<<endl;
					cout<<za<<" "<<zb<<" "<<zpixels[int(y0)][x]<<endl<<endl;
				}
*/
			}
		}
	}
}
		
