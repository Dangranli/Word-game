#include "AdvRoom.h"

AdvRoom::AdvRoom()
{
	rmNum =0;
	vist=false;
}

bool AdvRoom::readRoom(ifstream &roomFile)
{
	string str;
	desp.clear();
	action.clear();
	advobj.clear();
	getline(roomFile, str);
	
	if(roomFile.eof())
		return false;
	rmNum = stoi(str);
	getline(roomFile,name);
	
	while(true)
	{
		getline(roomFile,str);
		if (str == "-----")
			break;
		desp.push_back(str);
	}
	
	while(true)
	{
		str.clear();
		getline(roomFile,str);
		if (str == "")
			break;
		stringstream ss(str);
		string dir;
		int room;
		string key;
		ss >> dir >> room >> key;
		AdvMotionTableEntry motion(dir,room,key);
		action.push_back(motion);				
	}
	return true;
}

vector<string> AdvRoom::getDescription()
{
	return desp;
}

string AdvRoom::getName()
{
	return name;
}

void AdvRoom::addObject(AdvObject obj)
{
	// This function should add the obj to the room.
	// It however, should not add the object to the room
	// if the room already contains the object.
	if(!containsObject(obj.getName()))
		advobj.push_back(obj);
}

AdvObject AdvRoom::removeObject(string objName)
{
	AdvObject ret;
	// This function should remove the object with objName.
	for (int i = 0; i < advobj.size(); i++)
	{
		if (advobj[i].getName() == objName)
		{
			ret = advobj[i];
			advobj[i] = advobj[advobj.size() - 1];
			advobj.pop_back();
			break;
		}
	}
	return ret;
}

bool AdvRoom::containsObject(string objName)
{
	// Returns true if object with objName is in the room.
	for (int i = 0; i < advobj.size(); i++)
	{
		if (advobj[i].getName() == objName)
			return true;
	}
	return false;
}

int AdvRoom::objectCount()
{
	return advobj.size();
}

AdvObject AdvRoom::getObject(int index)
{
	AdvObject ret;
	if (index >= 0 && index < advobj.size())
		return advobj[index];
	else
		return ret;
}

bool AdvRoom::hasBeenVisited()
{
	return vist;
}

void AdvRoom::setVisited(bool flag)
{
	vist = flag;
}

vector<AdvMotionTableEntry> AdvRoom::getMotionTable()
{
	return action;
}


int AdvRoom::getRoomNumber()
{
	return rmNum;
}
