#include "IScriptChecker.h"


IScriptChecker::IScriptChecker(filesystem::path filename)
{
    IScriptChecker::filename = filename;
}


list<list<string>> IScriptChecker::readFile()
{
    list<list<string>> AllSjisText;

	ifstream inFile(filename);

	string text;

	string beginTxt;
	list<string> sjisText;

	while (getline(inFile, text))
	{
		// Blank Line
		if (text == "")
			continue;
		// Does line contain characters that are not standard english characters?
		if (!regex_match(text, regex(ENRegex2)))
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

			// This line shows up sometimes. It has quotes, but is not dialog.
			if (text.find("strlen") != string::npos)
			{
				continue;
			}

			sjisText.push_back("\t" + weirdTxt);
		}
		// else if there is no dialog on this line
		else
		{
			if (sjisText.empty())
				continue;
			sjisText.push_front("<sjis>");
			sjisText.push_back("</sjis>");
			AllSjisText.push_back(sjisText);
			sjisText.clear();
		}
	}
	inFile.close();

    return AllSjisText;
}
