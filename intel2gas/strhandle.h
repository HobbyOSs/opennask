/*
    strhandle.cc   string manipulating functions
    Copyright (c) 1999 Mikko Tiihonen (mikko.tiihonen@hut.fi)
    This source code is licensed under the GNU LGPL

    Please refer to the file COPYING.LIB contained in the distribution for
    licensing conditions
*/

#include <ctype.h>
#include <string.h>

/* Cuts the str at first occurance of cut.
 * Returns: pointer following the cut.
 * Modifies: str */
extern char *str_cutat(char *str, char cut);

/* Like str_cutat, but the beginning of str is copied to buffer at left */
extern const char *str_splitat(char const * const str, char *left, char cut);

/* Returns the matching closing parenthesis, using inc=(, dec=) */
extern char const *str_find_close(char const *str, char inc, char dec);

/* Like str_splitat, but splits the string after the matching
 * parenthesis. Again return value points to match+1 and the
 * beginning of str is copied to left */
extern char *str_splitat_close(char const * const str, char *left, char inc, char dec);

/* At first occurance of cut copies beginning of str to buffer left and the
 * rest to right. str is not modified */
extern void str_splitstrat(char const * const str, char *left, char cut, char *right);

/* Returns a pointer to first non-space character in str */
extern char *str_skipspace(const char *str);

/* Just tests if strlen()==0 */
inline int str_empty(char const * const str) { return *str == '\0'; }

/* Appends a character to the string. Make sure buffer is large enough. */
extern void str_catchar(char *str, char ch);

/* Removes a trailing number from str and returns it
 * If str does not end in number returns \0 */
extern char str_striplastnumber(char *str);

/* Copies source to dest until findst end1, end2 */
extern void str_copyuntil(char *dest, char const *source, char end1, char end2 = '\0');

/* Copies source to dest at position pos.
 * If dest is longer a single space is added between
 * else dest is padded with spaces */
extern void str_addatpos(char *dest, char const *source, int pos);
