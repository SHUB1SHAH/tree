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

#include "tree.h" 
int dir_count = 0, file_count = 0;
int Uopt = 0, aopt = 0, ropt = 0, Qopt = 0;
int oopt = 0, nropt = 0, fopt = 0, dopt = 0;
FILE *fp;
stack s;
int main(int argc, char *argv[]) {
	node_t tree;
	init(&s);
	char *filename = NULL;
	char *file_opt;
	int flag = 0, i;
	DIR *dir;
	for(i = 1; i < argc; i++) {
		if(argv[i][0] == '-') {
			if(!(strcmp(argv[i], "-U")) )
				Uopt = 1;
			else if(!(strcmp(argv[i], "-a")) )
				aopt = 1;
			else if(!(strcmp(argv[i], "-r")) ) {
				ropt = 1;
				if(Uopt == 1)
					ropt = 0;
			}
			else if(!(strcmp(argv[i], "-Q")) )
				Qopt = 1;
			else if(!(strcmp(argv[i], "-f")) )
				fopt = 1;
			else if(!(strcmp(argv[i], "-d")) ) 
				dopt = 1;
			else if(!(strcmp(argv[i], "-o")) ) {
				oopt = 1;
				if((i + 1) == argc) {
					printf("\n./tree: invalid argument to -o option\n");
					printf("try './tree --help' for more information\n");
					exit(1);
				}
				else {
					file_opt = argv[++i];
					fp = fopen(file_opt, "w");
				}
			}	
			else if(!(strcmp(argv[i], "--noreport")) )
				nropt = 1;
			else if(!(strcmp(argv[i], "--help")) ) {
				help();
				exit(1);
			}
			else {
				if(!flag) {
					filename = argv[i];
					flag = 1;
				}
				else {
					printf("tree: invalid argument: %s \n", filename);
					printf("try './tree --help' for more information\n");
					exit(1);
				}
			}
		}
		else {
			if(!flag) {
				filename = argv[i];
				flag = 1;
			}
			else {
				printf("tree: invalid argument: %s \n", filename);
				printf("try './tree --help' for more information\n");
				exit(1);
			}
		}
	}	

	if(filename) {
		dir = opendir(filename);

		if(dir == NULL) {
			printf("%s : [error opening directory] \n", filename);

			if(dopt) {
				if(!nropt)
					if(!oopt)
						printf("\n0 directories\n");
					else
						writetofile(0, -1);
			}
			else {
				if(!nropt)
					if(!oopt)
						printf("\n0 directories, 0 files\n");
					else
						writetofile(0, 0);
			}
			printf("try './tree --help' for more information\n");
			exit(1);
		}
		else 
			closedir(dir);
	}

	if(filename) {
		tree = createtree(filename);

		tree.s = (char *)malloc(strlen(filename) + 1);
		if(tree.s == NULL) {
			printf("malloc failed\n");
			exit(1);
		}

		tree.s[0] = '\0';
		strcpy(tree.s, filename);
	}
	else {
		tree = createtree(".");

		tree.s = (char *)malloc(2);
		if(tree.s == NULL) {
			printf("malloc failed\n");
			exit(1);
		}

		tree.s[0] = '\0';
		strcpy(tree.s, ".");
	}

	print_tree(&tree);

	if(dopt) {
		if(!nropt)
			if(!oopt)
				printf("\n%d directories\n", dir_count);
			else
				writetofile(dir_count, -1);
	}
	else {
		if(!nropt)
			if(!oopt)
				printf("\n%d directories, %d files\n", dir_count, file_count);
			else
				writetofile(dir_count, file_count);
	}
	
	if(oopt)
		fclose(fp);
	free_tree(&tree);
	return 0;
}
node_t createtree(char *s) {
	node_t node;
	struct dirent* dent;
	char linkname[128];
	ssize_t r;
	int n, i = 1, k;
	n = num_nodes(s); 

	DIR* sdir = opendir(s);
	if(sdir == NULL) {
		node.child = NULL;
		return node ;
	} 
	
	node.child = (struct node *)malloc((n + 1)*sizeof(node));
 
	if(node.child == NULL) {
		printf("malloc failed\n");
		exit(0);
	} 

	node.child[0].s = (char *)malloc(len_num(n)* sizeof(char)); //store no. of nodes in 1st node
	if(node.child[0].s == NULL) {
		printf("malloc failed\n");
		exit(1);
	}

	node.child[0].child = NULL;
	sprintf(node.child[0].s, "%d", n);

	while((dent = readdir(sdir) ) != NULL) { 
		if(strcmp(dent->d_name, ".") == 0 || strcmp(dent->d_name, "..") == 0)
			continue;	
		else if((dent->d_name[0] == '.') && (!aopt) ) // for hidden files
			continue;
		else if(dent->d_type == DT_LNK) { //symbolic link
			char *p;
			struct stat sb;

			p = (char *)malloc(strlen(dent->d_name) + strlen(s) + 1 );
			if(p == NULL) {
				printf("malloc failed\n");
				exit(1);
			}

			p[0] = '\0';
			strcat(p, s);
			strcat(p, "/");
			strcat(p, dent->d_name); //path for symbolic link

			r = readlink(p, linkname, 127);
			if(r == -1) {
				perror("readlink");
				exit(1);
			}
			linkname[r] = '\0';

			node.child[i].child = (struct node *)malloc(2* (sizeof(node)) );
			if(node.child[i].child == NULL) {
				printf("malloc failed\n");
				exit(1);
			}

			node.child[i].child[0].s = (char *)malloc(3* sizeof(char));
			if(node.child[i].child[0].s == NULL) {
				printf("malloc failed\n");
				exit(1);
			}

			node.child[i].child[0].child = NULL;

			node.child[i].child[1].s = (char *)malloc(strlen(linkname) + 1);
			if(node.child[i].child[1].s == NULL) {
				printf("malloc failed\n");
				exit(1);
			}

			node.child[i].child[1].s[0] = '\0';

			strcpy(node.child[i].child[1].s, linkname);
			node.child[i].child[1].child = NULL;

			if(fopt) 
				node.child[i].s = (char *)malloc(strlen(p) + 1);
			else
				node.child[i].s = (char *)malloc(strlen(dent->d_name) + 1);

			if(node.child[i].s == NULL) {
				printf("malloc failed\n");
				exit(1);
			}

			node.child[i].s[0] = '\0';

			if(!fopt)
				strcpy(node.child[i].s, dent->d_name);
			else 
				strcpy(node.child[i].s, p);
				
			stat(p, &sb);

			if(S_ISDIR(sb.st_mode)) {
				sprintf(node.child[i].child[0].s, "%d", SYBLNKDR);
				dir_count++;
			}
			else {
				if(dopt) {
					free(node.child[i].child[0].s);
					free(node.child[i].child[1].s);
					free(node.child[i].s);
					free(node.child[i].child);
					i--;
				}
				else {
					sprintf(node.child[i].child[0].s, "%d", SYBLNKNR);
					file_count++; 
				}
			}
			free(p);
			i++;
		}	
		else if(dent->d_type == DT_DIR) {
			
			char p[1024];
			p[0] = '\0';
			struct stat sb;
			strcat(p, s);
			strcat(p, "/");
			strcat(p, dent->d_name); //path for opening dir
			dir_count++;

			if(dopt) {
				k = num_nodes(p); 
				if(k)
					node.child[i] = createtree(p);
				else
					node.child[i].child = NULL;
			}
			else
				node.child[i] = createtree(p);

			stat(p, &sb);

			if(fopt) 
				node.child[i].s = (char *)malloc(strlen(p) + 1);
			else
				node.child[i].s = (char *)malloc(strlen(dent->d_name) + 1);

			if(node.child[i].s == NULL) {
				printf("malloc failed\n");
				exit(1);
			}

			node.child[i].s[0] = '\0';

			if(!fopt)
				strcpy(node.child[i].s, dent->d_name);
			else 
				strcpy(node.child[i].s, p);
			
			i++;
			
		}  
		else if(!dopt){ 
			char *p;
			struct stat sb;
			p = (char *)malloc(strlen(dent->d_name) + strlen(s) + 3 );
			if(p == NULL) {
				printf("malloc failed\n");
				exit(1);
			}

			p[0] = '\0';
			strcat(p, s);
			strcat(p, "/");
			strcat(p, dent->d_name);

			stat(p, &sb);
			file_count++;	
			node.child[i].child = NULL;

			if(sb.st_mode & S_IXUSR) {
				node.child[i].child = (struct node *)malloc(2*(sizeof(node)) );
				if(node.child[i].child == NULL) {
					printf("malloc failed\n");
					exit(1);
				}

				node.child[i].child[0].s = (char *)malloc(3* sizeof(char));
				if(node.child[i].child[0].s == NULL) {
					printf("malloc failed\n");
					exit(1);
				}

				node.child[i].child[1].s = (char *)malloc(2* sizeof(char));
				if(node.child[i].child[1].s == NULL) {
					printf("malloc failed\n");
					exit(1);
				}

				node.child[i].child[1].s[0] = '\0';
				strcpy(node.child[i].child[1].s, "z");

				node.child[i].child[0].child = NULL;
				node.child[i].child[1].child = NULL;

				sprintf(node.child[i].child[0].s, "%d", EXCTBL);
			}

			if(fopt) 
				node.child[i].s = (char *)malloc(strlen(p) + 1);
			else
				node.child[i].s = (char *)malloc(strlen(dent->d_name) + 1);

			if(node.child[i].s == NULL) {
				printf("malloc failed\n");
				exit(1);
			}

			node.child[i].s[0] = '\0';

			if(!fopt)
				strcpy(node.child[i].s, dent->d_name);
			else 
				strcpy(node.child[i].s, p);

			i++;
			free(p);
		}
			
	} 
	if(!(Uopt))
		qsort(&node.child[1], n, sizeof(node), cmpstringp);
	closedir(sdir);	
	return node;
}

int num_nodes(char *s) {
	struct dirent* dent;
	int num_nodes = 0;
	DIR* sdir = opendir(s);
	
	if(sdir == NULL) {
		return num_nodes;
	}

	while((dent = readdir(sdir) ) != NULL) {
		if(strcmp(dent->d_name, ".") == 0 || strcmp(dent->d_name, "..") == 0)
			continue;
		else if( (dent->d_name[0] == '.') && (!aopt) )
			continue;
		else if( (dent->d_type == DT_DIR)) 
			num_nodes++;
		else if(dent->d_type == DT_LNK) { 
			char linkname[128];
			char *p;
			ssize_t r;
			struct stat sb;

			p = (char *)malloc(strlen(dent->d_name) + strlen(s) + 1 );
			if(p == NULL) {
				printf("malloc failed\n");
				exit(1);
			}

			p[0] = '\0';
			strcat(p, s);
			strcat(p, "/");
			strcat(p, dent->d_name); //path for symbolic link

			r = readlink(p, linkname, 127);
			if(r == -1) {
				perror("readlink");
				exit(1);
			}
			linkname[r] = '\0';

			stat(p, &sb);

			if(S_ISDIR(sb.st_mode) && dopt) 
				num_nodes++;
			else if(!dopt)
				num_nodes++;
		}
		else if(!(dopt))
			 num_nodes++;
	}

	closedir(sdir);
	return num_nodes;
}

void print_tree(node_t *tree) {
	int i , n, k , level;
	char c = '\n';
	char p[5] = " -> ";
	static int j = 0, flag;
	n = atoi(tree->child[0].s);
	
	if(!n)
		return;
	if(!j) {
		level = 1;
		q_opt(Qopt);

		if(oopt)
			fprintf(fp, "%s", tree->s);
		else
			printf(BRIGHT BLUE "%s" RESET, tree->s);

		q_opt(Qopt);

		if(oopt)
			fprintf(fp, "%c", c);
		else
			printf("\n");
		j++;
		flag = 1;
	}
	else {
		if(flag) {
			level = pop(&s) + 1;
			push(&s, level - 1, 1);
		}
	}
	if(flag)
		push(&s, level, n);
	flag = 1;
	for(i = 1; i <= n; i++) {
		if(i == n) {
			level = pop(&s);
			space(level, 1);
		}
		else{
			level = pop(&s);
			space(level, 0);
		}
		if(tree->child[i].child != NULL) {
			k = atoi(tree->child[i].child[0].s);

			if(k == SYBLNKDR || k == SYBLNKNR) { 
				printf(BRIGHT BLUE);
				q_opt(Qopt);

				if(oopt)
					fprintf(fp, "%s", tree->child[i].s);
				else
					printf(BRIGHT LBLU "%s", tree->child[i].s);

				q_opt(Qopt);

				printf(RESET);
				if(oopt)
					fprintf(fp, "%s", p);
				else
					printf(" -> ");

				if(k == SYBLNKDR) {
					printf(BRIGHT BLUE);
					q_opt(Qopt);

					if(oopt)
						fprintf(fp, "%s", tree->child[i].child[1].s);
					else
						printf(BRIGHT BLUE "%s", tree->child[i].child[1].s);

					q_opt(Qopt);

					printf(RESET);
				}
				else {
					q_opt(Qopt);
					if(oopt)
						fprintf(fp, "%s", tree->child[i].child[1].s);
					else
						printf("%s", tree->child[i].child[1].s);	
					q_opt(Qopt);
				}
				if(oopt)
					fprintf(fp, "%c", c);
				else
					printf("\n");
				
			}				
			else if(k == EXCTBL) {
				printf(BRIGHT GREEN);
				q_opt(Qopt);

				if(oopt)
					fprintf(fp, "%s", tree->child[i].s);
				else
					printf(BRIGHT GREEN "%s", tree->child[i].s);

				q_opt(Qopt);

				printf(RESET);

				if(oopt)
					fprintf(fp, "%c", c);
				else
					printf("\n");
			}
			else {
				if(i == n) {
					push(&s, level + 1, k);
					flag = 0;
				}

				printf(BRIGHT BLUE);
				q_opt(Qopt);

				if(oopt)
					fprintf(fp, "%s", tree->child[i].s);
				else
					printf(BRIGHT BLUE "%s", tree->child[i].s);

				q_opt(Qopt);

				printf(RESET);

				if(oopt)
					fprintf(fp, "%c", c);
				else
					printf("\n");
				print_tree(&(tree->child[i]));
			}
		} 
		else {
			if(dopt)
				printf(BRIGHT BLUE);
			q_opt(Qopt);

			if(oopt)
				fprintf(fp, "%s", tree->child[i].s);
			else
				printf("%s", tree->child[i].s);

			q_opt(Qopt);
			printf(RESET);

			if(oopt)
				fprintf(fp, "%c", c);
			else
				printf("\n");
		}
	}	
	
}

int len_num(int n) {
	int m = 1;
	while(n != 0) {
		n = n / 10;
		m++;
	}
	return m;
}

void space(int n, int last) {
	int i;
	char s3[5] = "|   ";
	char s1[5] = "`-- ";
	char s2[5] = "|-- ";

	for(i = 0; i < n - 1; i++) {

		if(oopt)
			fprintf(fp, "%s", s3);
		else
			printf("%s", s3);
	}

	if(last) {
		if(oopt)
			fprintf(fp, "%s", s1);
		else
			printf("%s", s1);
	}
	else {
		if(oopt)
			fprintf(fp, "%s", s2);
		else
			printf("%s", s2);
	}
}

static int cmpstringp(const void *p1, const void *p2) {	
	char c,  a[128], b[128], e[128], d[128];
	int i = 0, j = 0, k = 0;
	a[0] = '\0';e[0] = '\0';

	while( (c = (const char)(( (const node_t *)p1)->s[i]))  != '\0') {
		if( (c > 64) && (c < 91) ) {
			e[j] = c;
			c = c + 32;
			a[j++] = c;
		}
		else if( (  (c > 96) && (c < 123) )  ) {
			e[j] = c;
			a[j++] = c;
		}
		else if( (  (c > 47) && (c < 58) )  )  {
			e[j] = c;
			a[j++] = c;
		}
		i++;
		k++;
	}
	a[j] = '\0';e[j] = '\0';
	i = 0;
	j = 0;
	b[0] = '\0';d[0] = '\0';

	while((c = (const char)((((const node_t *)p2)->s)[i]))  != '\0') {
		if( (c > 64) && (c < 91) ) {
			d[j] = c;
			c = c + 32;
			b[j++] = c;
		}
		else if( (  (c > 96) && (c < 123) )  ) {
			d[j] = c;
			b[j++] = c;
		}
		else if( (  (c > 47) && (c < 58) )  ) {
			d[j] = c;
			b[j++] = c;
		}
		i++;
	}
	b[j] = '\0';d[j] = '\0';

	if(strcmp(a, b) == 0) {
		if(strcmp(e, d) == 0) {
			if(ropt)
				return i - k;
			return k - i;
		}
		else if(strcmp(e, d) < 0) {
			if(ropt)
				return -1;
			return 1;
		}
		else {
			if(ropt)
				return 1;
			return -1;
		}
	}

	if(ropt)
        	return ( (-1)*(strcmp(a, b)) );

	return (strcmp(a, b));
}

void free_tree(node_t *tree) {
	int i, n;
	free(tree->s);

	if(tree->child) {
		n = atoi(tree->child[0].s);

		if((n == SYBLNKDR) || (n == SYBLNKNR) || (n == EXCTBL))
			n = 1;

		for(i = 0; i <= n; i++) {			
			if(tree->child[i].child)
				free_tree(&(tree->child[i]));
			else
				free(tree->child[i].s);
		}
		free(tree->child);
	}
	else {
		free(tree);
	}
}

void q_opt(int n) {

	if(n == 1) {
		if(oopt)
			fprintf(fp, "%c", '"' );
		else
			printf("\"");
	}
}

void help() {
	printf("\nBy default tree prints listing of files in current directory\n");

	printf("  ------- Listing options -------\n");
	printf("  -a            All files are listed.\n");
	printf("  -d            List directories only.\n");
	printf("  -o filename   Output to file instead of stdout.\n");
	printf("  -f            Print the full path prefix for each file.\n");
	printf("  --noreport    Turn off file/directory count at end of tree listing.\n");

	printf("  -------- File options ---------\n");
	printf("  -Q            Quote filenames with double quotes.\n\n");

	printf("  ------- Sorting options -------\n");
	printf("  -r            Sort files in reverse alphanumeric order.\n");
	printf("  -U            Leave files unsorted.\n");

	printf("  ---- Miscellaneous options ----\n");
	printf("  --help        Print usage and this help message and exit.\n");

}	

void writetofile(int dir, int fileno) {
	int d = dir, f = fileno, n1 = 1, n2 = 1;
	while(f != 0) {
		n1++;
		f = f / 10;
	}
	while(d != 0) {
		n2++;
		d = d / 10;
	}
	if(fileno == -1) {
		char c[n2];	
		fprintf(fp, "%c", '\n');
		sprintf(c, "%d", dir);
		fprintf(fp, "%s", c);
		fprintf(fp, "%s", " directories");
	}
	else {
		char c1[n1], c2[n2];
		fprintf(fp, "%c", '\n');
		sprintf(c1, "%d", dir);
		fprintf(fp, "%s", c1);
		fprintf(fp, "%s", " directories, ");
		sprintf(c2, "%d", fileno);
		fprintf(fp, "%s", c2);
		fprintf(fp, "%s", " files");
		fprintf(fp, "%c", '\n');
	}

}


