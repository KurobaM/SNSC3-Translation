# Script Inserter
This program inserts text from the JE Scripts into the untranslated I Scripts.  
It also checks the I Scripts after inserting to check if there is any leftover japanese text.  

# How to Use
First what you need is a copy of the JE Scripts you want to insert and a copy of the untranslated I scripts they will be inserted to.  

Next open the config.ini file. There are 4 fields you can fill out, 2 are optional.  
The iscriptfolder and jescriptfolder fields need to befilled out. Put in the absolute path to where the folder where the relevent
files are stored. You do not need to put quotes around the path. It works with spaces.  

The other 2 fields let you choose where to output the finished I Scripts and errors. They will be output to a default folder
in the same directory as the Inserter by default.  

Then just run the .exe file.

# Error Checking
This program also checkes for errors in the inserting process. When text in the I Script is missing in the JE Script or is untranslated, the untranslated text from the I Script is output into the error directory (Missing_Text by default). This can mean 3 different things. Either the text is missing from the JE Scripts, the SJIS text was copied down wrong in the JE Script, or there is a problem with the JE Script.  

The second error the program checks for is if all text in the JE Script has been inserted. Any text leftover is output to the LeftoverJE directory inside the error directory. There are a few main reasons why this could happen. Either the SJIS was copied wrong into the file, there are unneccessary duplicates of the translations, or there is some other problem with the JE Script.