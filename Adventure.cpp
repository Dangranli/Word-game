#include "Adventure.h"

/*
* File: Adventure.cpp
* --------------------
* This program plays the Adventure game.
*/

Adventure::Adventure()
{
	currRoomInd = 0;
}

Adventure::Adventure(string objfile, string rmfile, string cmdfile)
{
	// Setup of the Adventure game using some files.
	LoadSynonyms(cmdfile);
	LoadRooms(rmfile);
	LoadObjects(objfile);
	currRoomInd = 1;
}

void Adventure::Play()
{
	int i;
	ShowRoominfo();
	while (currRoomInd != 0) {
		string cmd, objName;		
		if (CheckForce())
			continue;
		GetUserCmd(cmd, objName);
		if (cmd == "QUIT") {
			QuitCmd();
		}
		else if (cmd == "HELP") {
			HelpCmd();
		}
		else if (cmd == "LOOK")
		{
			LookCmd();
		}
		// The rest of the commands go here.
		else if (cmd == "INVENTORY")
		{
			InventoryCmd();
		}
		else if (cmd == "TAKE")
		{
			TakeCmd(objName);
		}
		else if (cmd == "DROP")
		{
			DropCmd(objName);
		}
		else
		{
			i=MotionCmd(cmd);
			if (i == -1)
				cout << "There is no way to move in that direction." << endl;
			else
			{
				currRoomInd = i;
				ShowRoominfo();
			}
		}
	}

	cout << "Goodbye!" << endl;
}


void Adventure::GetUserCmd(string &verb, string &obj)
{	
	cout << "> ";
	string line;
	getline(cin, line);
	
	// Add code here to figure out the verb and object from the line
	ConvertToUpper(line);
	stringstream ss(line);
	ss >> verb >> obj;
	verb = GetSynonym(verb);
}

int Adventure::MotionCmd(string motion)
{
	// Get the motion table of the room number where the player is currently.
	// For each table entry, check to see if there is an entry that matches
	// "motion".  If the entry matches, check if the motion requires
	// a key and if the player has the has the key. A successful match
	// moves the player into the destination room.
	for (AdvMotionTableEntry entry : Rooms[currRoomInd-1].getMotionTable())
	{
		if (entry.getDirection() == motion)
		{
			if (entry.getKeyName() == "")
				return entry.getDestinationRoom();
			if(HasKey(entry.getKeyName()))
				return entry.getDestinationRoom();
		}
	}
	return -1;
}

void Adventure::LoadObjects(string filename)
{
	// Should load all the objects from a file.
	ifstream ObjFile(filename);
	if (!ObjFile)
		return;
	AdvObject obj;
	while (obj.readObject(ObjFile))
	{
		int loc = obj.getInitialLocation();
		for (AdvRoom& room : Rooms)
		{
			if (room.getRoomNumber() == loc)
			{
				room.addObject(obj);
				break;
			}
		}
	}
	ObjFile.close();
}

void Adventure::LoadRooms(string filename)
{
	// Should load all rooms from a file
	ifstream roomFile(filename);
	if (!roomFile)
	{
		cout << "can't open roomfile: " << filename << endl;
		cout << "quit" << endl;
		exit(0);
	}
	AdvRoom room;
	while (room.readRoom(roomFile))
		Rooms.push_back(room);
	roomFile.close();
}

void Adventure::LoadSynonyms(string filename)
{
	// Should load all synonyms from a file.
	ifstream SynonymsFile(filename);
	if (!SynonymsFile)
	{
		cout << "can't open synonymsfile: " << filename << endl;
		cout << "quit" << endl;
		exit(0);
	}
	Synonym syn;
	while (true)
	{
		SynonymsFile >> syn.word >> syn.synonym;
		if (SynonymsFile.eof())
			break;
		synonyms.push_back(syn);
	}
	SynonymsFile.close();
}

/*
 * Returns a synonym if one exists.  Otherwise returns original word.
 */
string Adventure::GetSynonym(string word)
{
	ConvertToUpper(word);
	for (Synonym syn : synonyms)
	{
		if (syn.word == word)
			return syn.synonym;
	}

	return word;
}


void Adventure::QuitCmd()
{
	// Ask if the uses would like to quit.  Should only take a yes or no.
	while (true) {
		string ans;
		cout << "Are you sure you want to quit now? ";
		getline(cin, ans);
		ConvertToUpper(ans);
		if (ans == "Y" || ans == "YES") {
			currRoomInd = 0;
			break;
		}
		if (ans == "N" || ans == "NO") {
			break;
		}
		cout << "Please answer yes or no." << endl;
	}
}

void Adventure::HelpCmd()
{
	cout << "Welcome to Adventure!" << endl;
	cout << "Somewhere nearby is Colossal Cave, where others have found fortunes in" << endl;
	cout << "treasure and gold, though it is rumored that some who enter are never" << endl;
	cout << "seen again. Magic is said to work in the cave.I will be your eyes" << endl;
	cout << "and hands. Direct me with natural English commands; I don't understand" << endl;
	cout << "all of the English language, but I do a pretty good job." << endl << endl;

	cout << "It's important to remember that cave passages turn a lot, and that" << endl;
	cout << "leaving a room to the north does not guarantee entering the next from" << endl;
	cout << "the south, although it often works out that way. You'd best make yourself" << endl;
	cout << "a map as you go along." << endl << endl;

	cout << "Much of my vocabulary describes places and is used to move you there." << endl;
	cout << "To move, try words like IN, OUT, EAST, WEST, NORTH, SOUTH, UP, or DOWN." << endl;
	cout << "I also know about a number of objects hidden within the cave which you" << endl;
	cout << "can TAKE or DROP.To see what objects you're carrying, say INVENTORY." << endl;
	cout << "To reprint the detailed description of where you are, say LOOK.If you" << endl;
	cout << "want to end your adventure, say QUIT." << endl;
}

bool Adventure::HasKey(string keyname)
{
	for (AdvObject obj : Inventory)
	{
		if (obj.getName() == keyname)
			return true;
	}
	return false;
}

void Adventure::InventoryCmd()
{
	if (Inventory.empty())
		cout << "You are empty-handed." << endl;
	else
	{
		cout << "You are carrying:" << endl;
		for (AdvObject obj : Inventory)
			cout <<"   "<< obj.getName() << endl;
	}
}

void Adventure::LookCmd()
{
	for (auto str : Rooms[currRoomInd - 1].getDescription())
		cout << str << endl;
	ShowRoomObj();
}

void Adventure::TakeCmd(string objname)
{
	if (Rooms[currRoomInd - 1].containsObject(objname))
	{
		AdvObject obj = Rooms[currRoomInd - 1].removeObject(objname);
		Inventory.push_back(obj);
		cout << "Took " << obj.getDescription() << endl;
	}
	else
		cout << "I don't see that here." << endl;
}

void Adventure::DropCmd(string objname)
{
	for (int i = 0; i < Inventory.size(); i++)
	{
		if (Inventory[i].getName() == objname)
		{
			cout << "Dropped " << Inventory[i].getDescription() << endl;
			Rooms[currRoomInd - 1].addObject(Inventory[i]);
			Inventory[i] = Inventory[Inventory.size() - 1];
			Inventory.pop_back();
			return;
		}
	}
	cout << "I don't have " << '"' << objname << '"' << endl;
}

void Adventure::ShowRoominfo()
{
	if (Rooms[currRoomInd - 1].hasBeenVisited())
	{
		cout << Rooms[currRoomInd - 1].getName() << endl;
		ShowRoomObj();
	}
	else
	{
		LookCmd();
		Rooms[currRoomInd - 1].setVisited(true);
	}
}

bool Adventure::CheckForce()
{
	int i=MotionCmd("FORCED");
	if (i != -1)
	{
		currRoomInd = i;
		if(i>0)
			ShowRoominfo();
		return true;
	}
	else
		return false;
}

void Adventure::ShowRoomObj()
{
	for (int i = 0; i < Rooms[currRoomInd - 1].objectCount(); i++)
	{
		cout << "There is "<< Rooms[currRoomInd - 1].getObject(i).getDescription()<<" here." << endl;
	}

}
