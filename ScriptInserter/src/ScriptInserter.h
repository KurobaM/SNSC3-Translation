#pragma once
#include "globals.h"
#include "JEObject.h"
#include "SymbolTable.h"
using namespace std;


class ScriptInserter
{
public:
	ScriptInserter(filesystem::path filename, list<JEObject> jObjs);
	filesystem::path filename;
	filesystem::path missingFileDir;
	list<JEObject> jObjs;
	list<string> newFileStrings;

	void readFile(filesystem::path missingDir);
	void replaceFile(filesystem::path outFilename);

private:
	JEObject findMatchingJEObject(list<string> &sjisText);
	bool handleEdgeCases(string text, string beginTxt, string weirdTxt);
	void insertAsciiText(string beginTxt, string sjisText);
	void insertAsciiText(string beginTxt, list<string>& sjisText);
};

