/*
    loaddata.h     parses syntax and list files
    Copyright (c) 1999 Mikko Tiihonen (mikko.tiihonen@hut.fi)
    This source code is licensed under the GNU LGPL
  
    Please refer to the file COPYING.LIB contained in the distribution for
    licensing conditions
*/

#include <stdio.h>

// Current line in file being read
extern int current_line;

/* Reads the next line from f. Converts tabs to whitespace.
 * Data is stored in global buffer tempstr */
extern void ReadNextLine(FILE *f, bool strip_comments = false);

/* Loads files in specified directory */
extern void LoadSyntaxis(const char *resource_path);
extern void LoadWordLists(const char *resource_path);

/* Deletes all varibles in syntax's scope */
extern void deleteVars(syntaxlist_t *syntaxlist);
