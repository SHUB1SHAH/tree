
   # 	project_tree "project_tree" is a  recursive directory listing program     	
   #	that  produces  a  depth indented  listing  of  files residing
   #     within specified directories.  
   # 	
   # 	Copyright (C) 2015  SHUBHAM SHAH shubhamshah970@gmail.com
   #
   # 	project_tree is free software; you can redistribute it and/or modify
   # 	it under the terms of the GNU General Public License as published by
   # 	the Free Software Foundation, either version 3 of the License, or
   #	(at your option) any later version.
   #
   #	project_tree is distributed in the hope that it will be useful,
   # 	but WITHOUT ANY WARRANTY; without even the implied warranty of
   # 	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   #	GNU General Public License for more details.

   #	You should have received a copy of the GNU General Public License
   #	along with this program.  If not, see <http;//www.gnu.org/licenses/>.

project: tree.o stack.o  
	cc tree.o stack.o -o project
tree.o: tree.c tree.h
	cc -c tree.c
stack.o: stack.c stack.h
	 cc -c stack.c

