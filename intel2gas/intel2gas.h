/*
    intel2gas.h    intel to at&t assembly converter
    Copyright (c) 1999 Mikko Tiihonen (mikko.tiihonen@hut.fi)
    This source code is licensed under the GNU LGPL

    Please refer to the file COPYING.LIB contained in the distribution for
    licensing conditions
*/

#warning "FIXME: Create config.h with cmake"
//#include "config.h"

#define TAB_SIZE	  8
#define STR_LEN 	  509
#define SYNTAX_LINE_LEN   505
#define TEMP_SIZE         512
#define WORDS_PER_FILE    256

/* Holds runtime variable values (strings) */
struct variable_t {
    char	name[STR_LEN+1];
    char	value[STR_LEN+1];
    variable_t *next;
};

/* The parsed .list files are in this struct.
 * Each .list file can also do assignments, which are executed
 * whenever it's member is matched */
struct list_t {
    char	name[STR_LEN+1];
    int         bit;
    char	*words[WORDS_PER_FILE];
    variable_t *assignments;
    list_t     *next;
};

/* The parsed in/out syntaxes are stored here.
 * Assignments are executed after the parse is matched, but
 * before the output is parsed */
struct syntax_t {
    char        parseline[SYNTAX_LINE_LEN+1];
    char        output[SYNTAX_LINE_LEN+1];
    variable_t *assignments;
    variable_t *assignments_after;
    syntax_t   *next;
};

/* List of syntax files found.
 * Each file has it's own variable space, which makes
 * recursive syntaxes possible */
struct syntaxlist_t {
    char          name[STR_LEN+1];
    syntax_t     *syntax;
    variable_t   *var;
    syntaxlist_t *next;
};

/* List of different comment types to recoginze
 * and their sizes for speed */
struct comment_t {
  char const *src;
  int         srclen;
  char const *dst;
  int         dstlen;
};

// Comments found in input are stored in this struct
struct comment_match_t {
  comment_t const *data;  // which comment was found
  char            *at;    // where
};


/* Base pointers to linked lists */
extern list_t	      *list;
extern syntaxlist_t   *basesyntaxlist;
// current active syntax
extern syntaxlist_t   *syntaxlist;

// scratch buffer (can be overwritten any time)
extern char            tempstr[TEMP_SIZE];


extern void setVar(char const * const varname, char const * const value);
extern syntaxlist_t *findSyntax(char const *name);
