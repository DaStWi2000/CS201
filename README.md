# CS 201 Portfolio Project
by Daniel Williams
#Food Database Diary Tracker
To use this program, you must have gcc, with ncurses installed, and the food database from https://www.ars.usda.gov/ARSUserFiles/80400525/Data/BFPDB/BFPD_db_07132018.zip
A makefile has been provided for your convenience. Making sure the database is extracted from the zip archive, you can compile the code with the 'make' command.
When the program is launched, you are prompted to enter your user name. The program only accepts Alphanumeric characters as input.
#User Interface Information
Once the user name has been typed, you can use the command line to type your diary commands. If you wish to search for a diary entry that has punctuation, simply type in the term with the punctuation being represented as a space. For example, if you want to look up something by Fresh&Easy, you can type in 'fresh easy' as the search arguments. The search command will find items that contain both 'fresh' and 'easy' in them and return them. The search command is NOT case sensitive.
For more information on commands, you can type the 'help' command in the program to display the syntax and brief description of each command. Command arguments are not case sensitive, but commands ARE case sensitive.
#Brief Command Summary
The search command displays items with the product id, name, and manufacturer being displayed in a table in that order. The diary view/page functions include the quantity to the right of the manufacturer.
The 'diary write/delete' functions provide 'CUD' access to the diary. Whereas the 'diary page/view' functions provide 'R' access to the diary. To update a quantity, use the 'diary write' command.
The info command provides the actual nutrition of the item given.

Please note that this program works best in an 80x24 sized terminal.
