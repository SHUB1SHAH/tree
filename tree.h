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

#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "stack.h"

#define EXCTBL -3
#define SYBLNKDR -2
#define SYBLNKNR -1
#define BLACK "\x1B[30m"
#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1B[33m"
#define BLUE "\x1B[34m"
#define MAGENTA "\x1B[35m"
#define LBLU "\x1B[36m"
#define WHITE "\x1B[37m"

#define RESET "\033[0m"
#define BRIGHT "\033[1m"
#define UNDERLINE "\033[3m"
#define BLINK "\033[5m"

typedef struct node {
	char *s;
	struct node *child;
}node_t;

node_t createtree(char *);
void space(int n, int last);
static int cmpstringp(const void *p1, const void *p2);
void print_tree(node_t *);
int num_nodes(char *s);
int len_num(int n);
void free_tree(node_t *t);
void q_opt(int);
void writetofile(int dir, int fileno);
void help();

