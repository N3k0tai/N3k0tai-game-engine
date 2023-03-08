#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <dirent.h>

using namespace std;

string welcome = 
"Welcome to the N3k0tai-game-engine editor :)\n"
"Use the \'help\' command for help\n";

string help =
"Possible commands :\n\n"
"\tquit/exit\n"
"\thelp\n\n"
"\ttext_editor [shell command for opening a file in your text editor (without the filename in it)]\n"
"\t\texample : text_editor subl\n"
"\t\tdefault is nano\n\n"
"\tnew_project [project name]\n"
"\topen_project [project name]\n"
"\tdelete_project [project name]\n\n"
"\tnew_scene [scene name]\n"
"\topen_scene [scene name]\n"
"\tdelete_scene [scene name]\n\n"
"\tnew_object [object name]\n"
"\tedit_object [object name]\n"
"\tdelete_object [object name]\n\n";


bool checkIfValidFileName(string& str)
{
	if (str == "")
		return false;

	for (char c : str)
	{
		if (((c != '-') && (c != '.') && (c < '0')) || ((c > '9') && (c < 'A')) || ((c > 'Z') && (c < 'a') && (c != '_')) || (c > 'z'))
			return false;
	}
	return true;
}


int main()
{
	cout << welcome;
	string projectName;
	string sceneName;

	string textEditor;
	ifstream textEditorFileRead("textEditor.txt");
	getline(textEditorFileRead, textEditor);
	textEditorFileRead.close();

	DIR* dir = opendir("../../projects");
	if (dir == nullptr)
	{
		system("mkdir ../../projects");
		cout << "Projects directory created\n";
	}
	else
		closedir(dir);


	while (true)
	{
		if (sceneName != "")
			cout << projectName << "/" << sceneName << "> ";
		else
			cout << projectName << "> ";

		string input;	
		getline(cin, input);

		istringstream iss(input);
		string command;
		iss >> command;

		if (command == "quit" || command == "exit")
			break;

		if (command == "help")
		{
			cout << help;
			continue;
		}

		if (command == "text_editor")
		{
			string shellCommand;
			while (iss)
			{
				string temp;
				iss >> temp;
				temp += " ";
				shellCommand += temp;
			}
			if (shellCommand.size() != 0)
				shellCommand.pop_back();

			ofstream textEditorFileWrite("textEditor.txt");
			textEditorFileWrite << shellCommand;
			textEditorFileWrite.close();
			cout << "Default file editor changed\n";
			continue;
		}

		if (command == "new_project")
		{
			string argument;
			iss >> argument;
			if (!checkIfValidFileName(argument))
			{
				cout << "ERROR : invalid directory name\n";
				continue;
			}

			string shellCommand = "mkdir ../../projects/" + argument;
			dir = opendir(((string)"../../projects/" + argument).c_str());
			if (dir == nullptr)
				system(shellCommand.c_str());
			else
				closedir(dir);

			projectName = argument;
			sceneName = "";

			// add default files

			continue;
		}

		if (command == "open_project")
		{
			string argument;
			iss >> argument;
			if (!checkIfValidFileName(argument))
			{
				cout << "ERROR : invalid directory name\n";
				continue;
			}

			dir = opendir(((string)"../../projects/" + argument).c_str());
			if (dir == nullptr)
			{
				cout << "ERROR : invalid project name\n";
				continue;
			}

			closedir(dir);
			projectName = argument;
			sceneName = "";
			continue;
		}

		if (command == "delete_project")
		{
			string argument;
			iss >> argument;
			if (!checkIfValidFileName(argument))
			{
				cout << "ERROR : invalid directory name\n";
				continue;
			}

			string shellCommand = "rm -r ../../projects/" + argument;
			dir = opendir(((string)"../../projects/" + argument).c_str());
			if (dir == nullptr)
			{
				cout << "ERROR : invalid project name\n";
				continue;
			}
		
			closedir(dir);
			system(shellCommand.c_str());
			projectName = "";
			sceneName = "";

			continue;
		}



	}
	return 0;
}
