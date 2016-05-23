/*
    loaddata.cc    parses syntax and list files
    Copyright (c) 1999 Mikko Tiihonen (mikko.tiihonen@hut.fi)
    This source code is licensed under the GNU LGPL
  
    Please refer to the file COPYING.LIB contained in the distribution for
    licensing conditions
*/

#include "intel2gas.h"
#include "loaddata.h"
#include "strhandle.h"

#include <stdlib.h>
#include <string.h>
#ifdef USE_GLOB
#include <glob.h>         //  Find files
#else
#include <errno.h>
#include <dirent.h>
#endif


int current_line; // global variable holds current line number in file


/* Reads the next line from f. 
 * - Converts tabs to whitespace.
 * - Strips comments.
 * Data is stored in global buffer tempstr */
void ReadNextLine(FILE *f, bool strip_comments)
{
    if (feof(f)) {
        tempstr[0] = '\0';
	return;
    }
    char temp[TEMP_SIZE];
    temp[0] = '\0';
    fgets(temp,TEMP_SIZE,f);
    current_line ++;

    char *s1 = temp,
	 *s2 = tempstr;
    while (*s1 != '\0') {
	// convert tabs to spaces
	if (*s1 == '\t') {
	    s1++;
	    do {
	        *s2++ = ' ';
	    } while ((s2-tempstr)%TAB_SIZE != 0);
	    continue;
	}
	if (strip_comments) {
	    if (*s1 == '\\') {
	        if (s1[1] == '#') {
		    *s2++ = '#';
		    s1 += 2;
		    continue;
		}
	    }
	    if (*s1 == '#')
	        break;
	}
	if (*s1 == '\n' || *s1 == '\r')
	    break;
	*s2++ = *s1++;
    }
    *s2 = '\0';
}


/* If str is a valid assignment does it */
static void HandleVarSet(char const * const str)
{
    if (str[0] != '@') return;
    char name[STR_LEN+1], value[STR_LEN+1];
    str_splitstrat(str+1,name,'=',value);
    str_cutat(name,' ');
    setVar(name, str_skipspace(value));
}


/* Allocates a new empty structure for .list file and
 * adds it to linked list called list */
static list_t *newList()
{
    list_t *l = new list_t;
    l->words[0] = NULL;
    l->bit = 0;
    l->assignments = NULL;

    l->next = list;
    list = l;

    return l;
}


/* Allocates a new structure to .syntax file.
 * It's added to linked list called syntaxlist */
static syntaxlist_t *newSyntaxList(char const *name)
{
    syntaxlist_t *s = new syntaxlist_t;
    strcpy(s->name,name);
    s->syntax = NULL;
    s->var = NULL;

    s->next = syntaxlist;
    syntaxlist = s;

    return s;
}


/* Allocates a new empty structure for a single parse syntax.
 * Adds it last to linked list called in current syntaxlist. */
static syntax_t *newSyntax()
{
    syntax_t *s = new syntax_t;
    s->parseline[0] = '\0';
    s->output[0] = '\0';
    s->assignments = NULL;
    s->assignments_after = NULL;

    s->next = NULL;
    if (syntaxlist->syntax == NULL) {
	syntaxlist->syntax = s;
	return s;
    }

    // put last in list
    syntax_t *ss = syntaxlist->syntax;
    while (ss->next)
	ss = ss->next;
    ss->next = s;
    
    return s;
}


/* Deletes all varibles in syntax's scope */
void deleteVars(syntaxlist_t *syntaxlist)
{
    variable_t *v = syntaxlist->var;
    syntaxlist->var = NULL;
    while (v) {
	variable_t *delme = v;
	v = v->next;
	delete delme;
    }
}


static void LoadWordList(char const filename[])
// where filename is listname.nn.list
{
    FILE *f;
    f = fopen(filename,"r");
    current_line = 0;
    
    list_t *l = newList();
    
    // get the nn letters of filename
    int strl = strlen(filename);
    l->bit = 0x10*(filename[strl-7]-'0') +
             0x01*(filename[strl-6]-'0');
    strcpy(l->name,strrchr(filename,'/')+1);
    str_cutat(l->name,'.');

    newSyntaxList("dummy"); // all variables go here
    
    int mode = 0;
    int count = 0;
    while (!feof(f)) {
	ReadNextLine(f, true);
	char *str = tempstr;
       	str = str_skipspace(str);
	// do not allow empty lines (should we?)
	if (str_empty(str)) continue;
	if (mode == 0) {
	    if (!strcmp(str,"-")) {
		// steal the variables from dummy syntax
		l->assignments = syntaxlist->var;
		syntaxlist->var = NULL;
		mode++;
		continue;
	    }
	    if (str[0] == '@') {
	        HandleVarSet(str);	        
		continue;
	    }
	    printf("Error while reading %s: Unknown meaning (%s)\n",filename,str);
	    exit(1);
	}
	if (count == WORDS_PER_FILE-1) {
	    printf("Error while readomg %s: Too many words in file\n",filename);
	    exit(1);
	}
	l->words[count] = strdup(str);
	count++;
    }
    fclose(f);
    l->words[count] = NULL;
    
    // free dummy syntax
    deleteVars(syntaxlist);
    delete syntaxlist;
    syntaxlist = NULL;
}


static void LoadSyntax(char const *filename)
// where filename is syntaxname.syntax
{
    FILE *f;
    f = fopen(filename,"r");
    current_line = 0;
    
    char name[STR_LEN+1];
    strcpy(name,strrchr(filename,'/')+1);
    str_cutat(name,'.');
    newSyntaxList(name);
    
    int mode = 0;
    syntax_t *s = NULL;

    while (!feof(f)) {
	ReadNextLine(f, true);
	char *str = tempstr;
	str = str_skipspace(str);
	if (str_empty(str)) continue;
	if (!strcmp(tempstr,"-")) {
	    if (s == NULL) continue;
	    // move variables to this syntax
	    s->assignments_after = syntaxlist->var;
	    syntaxlist->var = NULL;

	    s = NULL;
	    mode = 0;
	    continue;
	}
	if (str[0] == '@') {
	    HandleVarSet(str);
	    continue;
	}
	if (mode == 0) {
	    s = newSyntax();
	    strcpy(s->parseline,str);
	    str_catchar(s->parseline,' '); // to allow spaces after parsed line
	    mode = 1;
	    continue;
	}
	if (mode == 2) {
	    printf("In %s(%d): Only one result line allowed\n%s\n",filename,current_line,str);
	    exit(1);
	}
	mode = 2;
	if (!strcmp(str,"SKIPLINE"))
	    s->output[0] = '\0';
	else
	    strcpy(s->output,str);

	// steal variables
	s->assignments = syntaxlist->var;
	syntaxlist->var = NULL;
    }
    fclose(f);
    deleteVars(syntaxlist);
}



/* Loads all .list files in specified directory */
void LoadWordLists(const char *resource_path)
{
#ifdef USE_GLOB
    
    glob_t globbuf;
  
    strcpy(tempstr,resource_path);
    strcat(tempstr,"*.??.list");
    if (glob(tempstr,GLOB_NOSORT,NULL,&globbuf) < 0) {
	perror("glob");
	exit(1);
    }

    for (unsigned i=0;i<globbuf.gl_pathc; i++) {
	LoadWordList(globbuf.gl_pathv[i]);
    }
    globfree(&globbuf);
    
#else // USE_GLOB
    
    DIR *dp;
    dirent *entry;
	
    dp=opendir(resource_path);
    if(errno!=0) {
      perror("opendir");
      exit(1);
    }
    do {
      errno=0;
      if((entry=readdir(dp))!=NULL) {
	if(strstr(entry->d_name,".list"))
	  if(strlen(strstr(entry->d_name,".list"))==strlen(".list")) {
	    strcpy(tempstr,resource_path);
	    strcat(tempstr,entry->d_name);
	    LoadWordList(tempstr);
	  }
      } else if(errno!=0) {
	perror("readdir");
	exit(1);
      }
    } while(entry);
    closedir(dp);
#endif // !USE_GLOB
}


/* Loads all .syntax files in specified directory */
void LoadSyntaxis(const char *resource_path)
{
#ifdef USE_GLOB
    
  glob_t globbuf;
  
  strcpy(tempstr,resource_path);
  strcat(tempstr,"*.syntax");
  if (glob(tempstr,GLOB_NOSORT,NULL,&globbuf) < 0) {
    perror("glob");
    exit(1);
  }

  for (unsigned i=0;i<globbuf.gl_pathc; i++) {
    LoadSyntax(globbuf.gl_pathv[i]);
  }
  globfree(&globbuf);
    
#else // USE_GLOB
    
  DIR *dp;
  dirent *entry;
	
  dp=opendir(resource_path);
  if(errno!=0) {
    perror("opendir");
    exit(1);
  }
  do {
    errno=0;
    if((entry=readdir(dp))!=NULL) {
      if(strstr(entry->d_name,".syntax"))
	if(strlen(strstr(entry->d_name,".syntax"))==strlen(".syntax")) {
	  strcpy(tempstr,resource_path);
	  strcat(tempstr,entry->d_name);
	  LoadSyntax(tempstr);
	}
    } else if(errno!=0) {
      perror("readdir");
      exit(1);
    }
  } while(entry);
  closedir(dp);
    
#endif // !USE_GLOB
}

