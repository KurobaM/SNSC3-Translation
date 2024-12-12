#include "ScriptInserter.h"


ScriptInserter::ScriptInserter(filesystem::path filename, list<JEObject> jObjs)
{
	ScriptInserter::filename = filename;
	ScriptInserter::jObjs = jObjs;
}

void ScriptInserter::readFile(filesystem::path missingDir)
{
	// Reads and processes IScript file

	missingFileDir = missingDir;
	ifstream inFile(filename);

	string text;

	string beginTxt;
	list<string> sjisText;

	while (getline(inFile, text))
	{
		// Blank Line
		if (text == "")
			newFileStrings.push_back("\n");
		// Does line contain characters that are not standard english characters?
		if (!regex_match(text, regex(ENRegex)))
		{
			// position of first quote
			size_t quote1 = text.find("\"");
			if (quote1 == string::npos)
				continue;
			beginTxt = text.substr(0, quote1);
			// position of second quote
			size_t quote2 = text.find("\"", quote1 + 1);
			// text in between quotes
			string weirdTxt = text.substr(quote1 + 1, quote2 - quote1 - 1);

			if (handleEdgeCases(text, beginTxt, weirdTxt))
				continue;

			sjisText.push_back(weirdTxt);
		}
		// else if there is no dialog on this line
		else
		{
			// Inserts ascii text before copying over the non-dialog lines
			insertAsciiText(beginTxt, sjisText);

			newFileStrings.push_back(text + "\n");
		}
	}
	inFile.close();
}

bool ScriptInserter::handleEdgeCases(string text, string beginTxt, string weirdTxt)
{
	// This line shows up sometimes. It has quotes, but is not dialog.
	if (text.find("strlen") != string::npos)
	{
		newFileStrings.push_back(text + "\n");
		return true;
	}
	// similar probelm to strlen. Has quotes but no dialog.
	if (text.find("setname") != string::npos)
	{
		newFileStrings.push_back(text + "\n");
		return true;
	}

	// This program reads text so that consecutive lines are added to a singular JEObject
	// "choicetxt" lines are consecutive, but should be their own JEObjects
	if (text.find("choicetxt") != string::npos)
	{
		insertAsciiText(beginTxt, weirdTxt);
		return true;
	}
	// "menutitle" and "menutxt" have same problem as choicetxt
	if (text.find("menu") != string::npos)
	{
		insertAsciiText(beginTxt, weirdTxt);
		return true;
	}
	return false;
}

//Insert ascii one liners
void ScriptInserter::insertAsciiText(string beginTxt, string sjisText)
{
	list<string> lstStr = {sjisText};
	insertAsciiText(beginTxt, lstStr);
}

// Insert list of text
void ScriptInserter::insertAsciiText(string beginTxt, list<string> &sjisText)
{
	if (!sjisText.empty())
	{
		JEObject jObj = findMatchingJEObject(sjisText);
		list<string> textToUse = jObj.asciiText;
		if (jObj.asciiText.empty())
			textToUse = jObj.sjisText;
		for (auto const& i : textToUse)
		{
			newFileStrings.push_back(beginTxt + "\"" + i + "\"" + "\n");
		}
		sjisText.clear();
	}
}

// Makes new file with translated text
void ScriptInserter::replaceFile(filesystem::path outFilename)
{
	ofstream outFile(outFilename);
	for (auto const& i : newFileStrings)
	{
		outFile << i;
	}
	outFile.close();
}

// Matches text based on sjis
JEObject ScriptInserter::findMatchingJEObject(list<string>& sjisText)
{
	for (auto& i : sjisText)
	{
		ST.replaceSjisOnlyTags(i);
	}

	for (auto & i: jObjs)
	{
		if (!i.used && i.sjisText == sjisText)
		{
			i.used = true;
			return i;
		}
	}
	// In case there is no match Handle elsewhere
	/*
	cout << "Found a no match\n";
	filesystem::path missingTextFilename = missingFileDir / filename.filename();
	missingTextFilename.replace_extension(".xml");
	ofstream oo(missingTextFilename, ios::app);
	oo << "<sjis>" << "\n";
	for (auto& i : sjisText)
	{
		ST.replaceTags(i, false, true);
		oo << "\t" << i << "<end_line>" << "\n";
	}
	oo << "</sjis>" << "\n\n";
	oo.close();
	*/

	JEObject defaultJEObj(sjisText);
	defaultJEObj.asciiText = sjisText;
	return defaultJEObj;
}
