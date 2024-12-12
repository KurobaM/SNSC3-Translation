#include "globals.h"
#include "JEScriptReader.h"
#include "ScriptInserter.h"
#include "SymbolTable.h"
#include "IScriptChecker.h"
using namespace std;

void makeDirectory(filesystem::path dir);
string findMatchingFile(string filename);
static map<string, filesystem::path> readConfig();

SymbolTable ST;

int main()
{
    map<string, filesystem::path> paths = readConfig();

    // Create directory for translated I Scripts
    filesystem::path iScriptOutDir = "./Done_IScripts";
    if (paths.contains("outputiscript"))
        iScriptOutDir = paths["outputiscript"];
    makeDirectory(iScriptOutDir);

    // Create directory for missing translations
    filesystem::path missingOutDir = "./Missing_Text";
    if (paths.contains("outputmissingtext"))
        missingOutDir = paths["outputmissingtext"];
    makeDirectory(missingOutDir);
    makeDirectory(missingOutDir / "LeftoverJE");

    // Generate a table to replace tags like <three_dots>
    ST = SymbolTable();

    // Iterate through every file in a directory for JE Scripts
    for (auto const& i : filesystem::recursive_directory_iterator(paths["jescriptfolder"]))
    {
        filesystem::path filename = i.path().filename();
        // Get matching I script
        string iscriptName = findMatchingFile(filename.stem().string());

        cout << "Starting inserting process for " << filename.string() <<
            " into " << iscriptName << "\n";

        // If there is no matching I script
        if (!filesystem::exists(paths["iscriptfolder"] / iscriptName))
        {
            cerr << "No IScript with the filename: " << iscriptName << "\n\n";
            continue;
        }

        // Read JE script and extract text
        cout << "Reading JE Script" << "\n";
        JEScriptReader jsr(i.path().string());
        jsr.readFile();
        jsr.formatText();

        // Read untranslated I script
        cout << "Reading IScript" << "\n";
        ScriptInserter inserter(paths["iscriptfolder"] / iscriptName, jsr.JEObjects);
        inserter.readFile(missingOutDir);
        cout << "Inserting " << filename.string() << " into " << iscriptName << "\n\n";

        // Write translated I script to a new file
        inserter.replaceFile(iScriptOutDir / iscriptName);


        // Handle unused text from JE Script
        filesystem::path unusedJEFilename = missingOutDir / "LeftoverJE" / filename.filename();
        unusedJEFilename.replace_extension(".xml");
        for (auto const& i : inserter.jObjs)
        {
            if (!i.used)
            {
                ofstream oo(unusedJEFilename, ios::app);
                oo << "<sjis>" << "\n";
                for (auto const& j : i.sjisText)
                {
                    oo << "\t" << j << "<end_line>" << "\n";
                }
                oo << "</sjis>" << "\n";
                oo << "<ascii>" << "\n";
                for (auto const& k : i.asciiText)
                {
                    oo << "\t" << k << "<end_line>" << "\n";
                }
                oo << "</ascii>" << "\n\n";
                oo.close();
            }
        }


        // Handle missing text
        filesystem::path missingTextFilename = missingOutDir / filename.filename();
        missingTextFilename.replace_extension(".xml");
        IScriptChecker checker(iScriptOutDir / iscriptName);
        list<list<string>> remainingSjis = checker.readFile();
        for (auto & i : remainingSjis)
        {
            ofstream oo(missingTextFilename, ios::app);
            for (auto & j : i)
            {
                if (j.find("sjis") == string::npos)
                {
                    ST.replaceTags(j, false, true);
                    j += "<end_line>";
                }
                oo << j << "\n";
            }
            oo << "\n";
            oo.close();
        }
    }
    
    return 0;
}

void makeDirectory(filesystem::path dir)
{
    if (filesystem::is_directory(dir) && filesystem::exists(dir))
    {
        filesystem::remove_all(dir);
    }

    filesystem::create_directory(dir);
}

string findMatchingFile(string filename)
{
    size_t pos = filename.rfind("_");
    return filename.substr(pos + 1) + ".txt";
}

static map<string, filesystem::path> readConfig()
{
    ifstream configFile("config.ini");
    string text;

    list<string> configFields = { "iscriptfolder", "jescriptfolder", "outputiscript", "outputmissingtext" };
    map<string, filesystem::path> paths;

    while (getline(configFile, text))
    {
        size_t pos = text.find("=");
        if (pos == string::npos)
            continue;

        string field = text.substr(0, pos);
        if (find(configFields.begin(), configFields.end(), field) != configFields.end())
        {
            string val = text.substr(pos + 1);
            if (!val.empty())
                paths[field] = filesystem::path(text.substr(pos + 1));
        }
        else
        {
            cout << "Unknown field: " << field << "\n";
            system("pause");
        }
    }

    return paths;
}