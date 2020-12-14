#ifndef ADVOBJECT
#define ADVOBJECT

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
using namespace std;

class AdvObject
{
public:
	AdvObject();
	AdvObject(string objName, string objDes, int loc);
	bool readObject(ifstream &objFile);

	string getName();
	string getDescription();
	int getInitialLocation();

private:
	// Add your private variables here...
	string name;
	string descrip;
	int locnum;
};

#endif