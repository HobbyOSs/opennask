/*
    strhandle.cc   string manipulating functions
    Copyright (c) 1999 Mikko Tiihonen (mikko.tiihonen@hut.fi)
    This source code is licensed under the GNU LGPL

    Please refer to the file COPYING.LIB contained in the distribution for
    licensing conditions
*/

#include "strhandle.h"

/* Cuts the str at first occurance of cut.
 * Returns: pointer following the cut.
 * Modifies: str */
char *str_cutat(char *str, char cut)
{
    char *pos = strchr(str,cut);
    if (!pos) return NULL;
    *pos = '\0';
    return pos+1;
}


/* Like str_cutat, but the beginning of str is copied to buffer at left */
const char *str_splitat(char const * const str, char *left, char cut)
{
    const char *pos = strchr(str,cut);
    if (!pos) {
        strcpy(left,str);
        return NULL;
    }
    strncpy(left,str,pos-str);
    left[pos-str] = '\0';
    return pos+1;
}


/* Used to find the matching close parenthesis, using inc=(, dec=) */
char const *str_find_close(char const *str, char inc, char dec)
{
    int counter = 1;
    char const *pos = str;
    while (counter && *pos) {
        if (*pos == inc) counter++;
        else if (*pos == dec) counter--;
	pos++;
    }
    if (counter)
        return NULL;
    return pos-1;
}

/* Like str_splitat, but splits the string after the matching
 * parenthesis. Again return value points to match+1 and the
 * beginning of str is copied to left */
char *str_splitat_close(char const * const str, char *left, char inc, char dec)
{
    char const *pos = str_find_close(str,inc,dec);
    if (!pos) {
        strcpy(left,str);
	return NULL;
    }
    strncpy(left,str,pos-str);
    left[pos-str] = '\0';
    return (char *)pos+1;
}


/* At first occurance of cut copies beginning of str to buffer left and the
 * rest to right. str is not modified */
void str_splitstrat(char const * const str, char *left, char cut, char *right)
{
    const char *pos = str_splitat(str,left,cut);
    if (!pos) {
	*right = '\0';
	return;
    }
    strcpy(right,pos);
}

/* Returns a pointer to first non-space character in str */
char *str_skipspace(const char *str)
{
    while (*str == ' ') str++;
    return const_cast<char*>(str);
}

/* Appends a character to the string. Make sure buffer is large enough. */
void str_catchar(char *str, char ch)
{
    const int size = strlen(str);
    str[size] = ch;
    str[size+1] = '\0';
}

/* Removes a trailing number from str and returns it
 * If str does not end in number returns \0 */
char str_striplastnumber(char *str)
{
    int size = strlen(str);
    if (size <= 1) return '\0';
    char number = str[size-1];
    if (!isdigit(number)) return '\0';
    str[size-1] = '\0';
    return number;
}

/* Copies source to dest until findst end1, end2 */
void str_copyuntil(char *dest, char const *source, char end1, char end2)
{
    while (tolower(*source) != tolower(end1) &&
	   tolower(*source) != tolower(end2)) {
	*dest = *source;
	if (*source == '\0') return;
	dest++; source++;
    }
    *dest = '\0';
}

/* Copies source to dest at position pos.
 * If dest is longer a single space is added between
 * else dest is padded with spaces */
void str_addatpos(char *dest, char const *source, int pos)
{
    char *str = dest;
    while (*str) {
        if (*str++ == '\n')
            dest = str;
    }
    int size = strlen(dest);
    if (pos > 0 && size >= pos) {
      str_catchar(dest, ' ');
      strcat(dest,source);
      return;
    }
    memset(dest+size,' ',pos-size);
    strcpy(dest+pos,source);
}
