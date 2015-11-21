/*   project_tree "project_tree" is a  recursive directory listing program     	
 *   that  produces  a  depth indented  listing  of  files residing
 *   within specified directories.  
 *       	
 *   Copyright (C) 2015  SHUBHAM SHAH shubhamshah970@gmail.com
 *
 *   project_tree is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   project_tree is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include"stack.h"
void init(stack *s) {
	*s = NULL;
}
int pop(stack *s) {
	if(empty(s))
		return 0;
	node_stack * fr;
	int value;
	value = (*s)->i;
	fr = *s;
	*s = (*s)->prev;

	free(fr);
	return value;
}
void push(stack *s, int value, int n) {
	int i;
	for(i = 0; i < n; i++) {
		node_stack *p;
		p = (node_stack *)malloc(sizeof(node_stack));
		if(*s == NULL) 
			p->prev = NULL;
		else
			p->prev = *s;
		p->i = value;
		*s = p;
	}
}
int empty(stack *s) {
	return *s == NULL; 
}

