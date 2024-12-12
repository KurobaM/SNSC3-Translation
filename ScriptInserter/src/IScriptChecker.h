#pragma once
#include "globals.h"
using namespace std;


class IScriptChecker
{
public:
	IScriptChecker(filesystem::path filename);
	filesystem::path filename;

	list<list<string>> readFile();
};

