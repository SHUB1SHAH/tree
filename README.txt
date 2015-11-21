Copyright (C) 2015  SHUBHAM SHAH shubhamshah970@gmail.com

project_tree is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

project_tree is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
--------------------------------------------------------------------------
project_tree

SHUBHAM SHAH 111408054

project_tree is a recursive directory listing program that produces 
a depth indented listing of files residing within specified directories. 

Structure node has two pointers ,one for name of directory and other for 
files of directory(child). First node has its name as number of files in 
that directory. Using this data structure, program stores name of 
files/directories recursively of specified directory according to 
given option.Then finally, tree is listed on screen(except -o option)
with specific colours for different type of files(symlnk, dir, executable)
like ls command and acc. to option(options) specified.

I then implemented following options : 
  -a            All files are listed
  -d            List directories only
  -o filename   Output to file instead of stdout
  -f            Print the full path prefix for each file
  --noreport    Turn off file/directory count at end of tree listing

  -Q            Quote filenames with double quotes

  -r            Sort files in reverse alphanumeric order
  -U            Leave files unsorted
  --help        Print usage and this help message and exit

