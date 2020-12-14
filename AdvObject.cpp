#include "AdvObject.h"

AdvObject::AdvObject()
{
	locnum=0;
}

AdvObject::AdvObject(string objName, string objDes, int loc)
{
	name=objName;
	descrip=objDes;
	locnum=loc;
}

bool AdvObject::readObject(ifstream &objFile)
{
	// True if able to read name, 
	// description, and initial location.
	// Otherwise returns false.
	
	while(true)
	{
		getline(objFile,name);
		if(objFile.eof())
			return false;
		if(name!="")
			break;
	}
	getline(objFile,descrip);
	objFile>>locnum;
	return true;		
}

string AdvObject::getName()
{
	return name;
}

string AdvObject::getDescription()
{
	return descrip;
}

int AdvObject::getInitialLocation()
{
	return locnum;
}