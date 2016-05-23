/*
    intel2gas.cc   intel to at&t assembly converter
    Copyright (c) 1999-2000 Mikko Tiihonen (mikko.tiihonen@hut.fi)
    This source code is licensed under the GNU GPL

    Please refer to the file COPYING.LIB contained in the distribution for
    licensing conditions
*/
/*

  This code is ulgy.

*/
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "intel2gas.h"
#include "loaddata.h"
#include "strhandle.h"

#define TAG_TEXT    "text"     // text until space or next in parseline
#define TAG_STRING  "string"   // text until eol or next in parseline
#define TAG_MORE    "more"     // restarts parser
#define TAG_SPACE   "space"    // stores whitespace
#define TAG_AT      "@"        // current position in input line
#define MAX_COMMENTS 4

/* Base pointers to linked lists */
list_t	       *list = NULL;
syntaxlist_t   *basesyntaxlist = NULL;
syntaxlist_t   *syntaxlist = NULL; // Current syntax list, variables are stored here

int             basepos; // first character on line

comment_t       comment[MAX_COMMENTS];
int             comments = 0;

int             flags = 0;
bool            in_c_comment = false;// if we're inside a /* .. */ comment
bool            in_asm_block = true; // if in inline mode tells wheather
                                     // we are in asm block or not

#define FLAG_INLINEMODE         1  /* converting inline asm (instead of standalone) */
#define FLAG_DOUBLEPERCENTCHARS 2  /* doubles % chars in syntax files to %% */
#define FLAG_FINDASMBLOCKS      4  /* find inline assembly blocks inside C code */
#define FLAG_CCOMMENTS          8  /* skip text inside /* ... */

// scratch buffer (can be overwritten any time)
char            tempstr[TEMP_SIZE];



syntaxlist_t *findSyntax(char const *name)
{
  syntaxlist_t *l = basesyntaxlist;
  int counter = 0;
  char* prev = 0;
  while (l) {
    if (intptr_t(l->name) > 0x10000000) {
      printf("Error %d, prev=%s\n", counter, prev);
      break;
    }
    if (!strcasecmp(name,l->name))
      return l;
    counter++;
    prev = l->name;
    l = l->next;
  }
  return NULL;
}


void setVar(char const * const varname, char const * const value)
{
  variable_t *v = syntaxlist->var;
  while (v) {
    if (!strcasecmp(v->name, varname)) {
      // value already exists
      strcpy(v->value, value);
      return;
    }
    v = v->next;
  }
  v = new variable_t;
  strcpy(v->name, varname);
  strcpy(v->value, value);
  v->next = NULL;

  if (syntaxlist->var == NULL) {
    syntaxlist->var = v;
    return;
  }

  // put last in list
  variable_t *vv = syntaxlist->var;
  while (vv->next)
    vv = vv->next;

  vv->next = v;
}


static char const *fetchVar(char const *tag)
{
  variable_t *v = syntaxlist->var;
  while (v) {
    if (!strcasecmp(tag,v->name))
      return v->value;
    v = v->next;
  }
  return "";
  //    printf("Variable not found\n");
  //    exit(1);
}


static void expand_string(char const *str, char *result)
{
  int pos = 0;
  while (!str_empty(str)) {
    if (*str == '<') {
      char tag[STR_LEN+1];
      str = str_splitat_close(str+1,tag,'<','>');
      if (tag[0] == '?') { // the mystery equation
	char t1[STR_LEN], *t2;
	const char* values = str_splitat(tag+1,t1,':');
	t2 = str_cutat(t1,'=');

	char T1[STR_LEN], T2[STR_LEN];
	expand_string(t1,T1); expand_string(t2,T2);

	t2 = str_cutat(const_cast<char*>(values),',');
	if (strcmp(T1,T2)) {
	  expand_string(t2,T1);
	} else {
	  expand_string(values,T1);
	}

	strcpy(result,T1);
	result += strlen(T1);
	continue;
      }
      char const *var = fetchVar(tag);
      str_striplastnumber(tag);
      if (!strcasecmp(tag,TAG_AT)) {
	int at = atoi(var);
	if (pos < at) {
	  memset(result,' ',at-pos);
	  result += at-pos;
	  pos = at;
	}
	continue;
      }
      strcpy(result,var);
      result += strlen(var);
      pos += strlen(var);
      continue;
    }
    if (*str == '\\') {
      str++;
      switch (*str) {
      case 'n':
	*result++ = '\n';
	memset(result,' ',basepos);
	result += basepos;
	pos = 0;
	break;
      default:
	*result++ = *str;
	break;
      }
      str++;
      continue;
    }
    if ((flags & FLAG_DOUBLEPERCENTCHARS) && *str == '%')
      *result++ = '%';
    *result++ = *str++;
    pos++;
  }
  *result = '\0';
}


static void setTheseVars(variable_t const *vars)
{
  while (vars) {
    char temp[TEMP_SIZE];
    expand_string(vars->value,temp);
    setVar(vars->name,temp);
    vars = vars->next;
  }
}


static void find_tag_list(char const *source, char const tag[],
                          int bits, char *result)
{
  list_t const *l = list;
  int bestsize = 0;
  char const *bestword = 0;
  list_t const *best = 0;

  *result = '\0';

  while (l) {
    // correct list == tag & bits
    if ((l->bit & bits) && !strcasecmp(l->name, tag)) {
      char * const *word = &l->words[0];
      while (*word != NULL) {
	// match source chars to a word in list
	int size = strlen(*word);
	if (!strncasecmp(source, *word, size)) {
	  if (size > bestsize) {
	    bestsize = size;
	    bestword = *word;
	    best = l;
	  }
	}
	word ++;
      }
    }
    l = l->next;
  }
  if (bestsize) {
    strcpy(result, bestword);
    // set all variables associated with the list
    setTheseVars(best->assignments);
  }
}


/* Copies numbers (of specified type) to ouput.
 * Returns amount characters found */
static int numberparse(char const *tag, char const *input, char *output)
{
  int base;
  if (!strcasecmp(tag,"hex"))
    base = 16;
  else if (!strcasecmp(tag,"dec"))
    base = 10;
  else if (!strcasecmp(tag,"bin"))
    base = 2;
  else if (!strcasecmp(tag,"oct"))
    base = 8;
  else {
    *output = '\0';
    return 0;
  }
  char *ptr;
  strtol(input,&ptr,base);
  const int len = ptr-input;
  strncpy(output,input,len);
  output[len] = '\0';
  return len;
}

static int str_parse(syntaxlist_t *s, const char *parse, char *outline);

static int parsematch(char *text, char *parse)
{
  char const *oldpos = text;
  while (!str_empty(parse)) {
    if (*parse == '<') {
      char str[TEMP_SIZE];   // temporary storage
      char tag[STR_LEN+1];   // tag name
      char number;           // 1..9 tag slot
      int bits;              // selection mask
      int len;               // how much to advance if match found

      // Full tag format
      // <tagx:bb>, f.ex <reg1:20>

      // Strip the tag
      parse = const_cast<char*>(str_splitat(parse+1,tag,'>'));

      // Strip the leading selection bits
      str_splitstrat(tag,tag,':',str);
      if (!str_empty(str)) {
	char *dummy;
	bits = strtol(str,&dummy,16);
	str[0] = '\0';
      } else bits = 0xff;

      // Strip the last number
      number = str_striplastnumber(tag);

      // Try to match tag with syntax file
      syntaxlist_t *l = findSyntax(tag);
      if (l != NULL) {
	char temp[TEMP_SIZE];

	// Try to figure what'll be the ending character
	char stop = *parse;

	/* Could add some more logic later
	 * but my first try didn't work */
	if (stop == ' ') {
	  stop = *str_skipspace(parse);
	  switch (stop) {
	  case '\0':
	    break;
	  case '<':
	    stop = ' ';
	  default:
	    *parse;
	  }
	}


	// Save current context (variables & stuff)
	// and create a clean one
	variable_t *old = syntaxlist->var;
	syntaxlist->var = NULL;

	// Create a new string and start parsing it with
	// the specified syntax file
	str_copyuntil(temp,text,stop);
	len = str_parse(l,temp,str);

	// Restore current context
	syntaxlist->var = old;

	if (len==0) return 0;
      } else {
	// Try to match tag with list file
	// Get the longest match
	find_tag_list(text, tag, bits, str);
	len = strlen(str);

	if (len==0) {

	  // Is tag a numbertag
	  len = numberparse(tag, text, str);

	  if (len == 0) {
	    if (!strcasecmp(tag,TAG_SPACE)) {
	      char *end = str_skipspace(text);
	      len = end-text;
	      strncpy(str, text, len);
	      str[len] = '\0';
	    } else if (!strcasecmp(tag,TAG_TEXT)) {
	      str_copyuntil(str,text,*parse,' ');
	      len = strlen(str);

	    } else if (!strcasecmp(tag,TAG_STRING)) {
	      // TODO: handle escapes in string
	      if (*parse == ' ') // ends in space: assume eol
		strcpy(str,text);
	      else
		str_copyuntil(str,text,*parse);
	      len = strlen(str);
	    } else if (!strcasecmp(tag,TAG_AT)) {
	      sprintf(str,"%d",text-oldpos);
	      len = 0; // don't advance

	    } else if (!strcasecmp(tag,TAG_MORE)) {
	      if (str_empty(str_skipspace(text))) {
		*str = '\0';
		len = 0;
	      } else {
		variable_t *old = syntaxlist->var;
		syntaxlist->var = NULL;
		len = str_parse(findSyntax("main"),text,str);
		deleteVars(syntaxlist);
		syntaxlist->var = old;
		if (len==0) return 0;
	      }
	    } else {
	      // tag not recognized
	      return 0;
	    }
	  }
	}
      }
      str_catchar(tag,number);
      setVar(tag,str);
      text += len;
      continue;
    }
    if (*parse == ' ') {
      parse++;
      text = str_skipspace(text);
      continue;
    }
    if (tolower(*parse) != tolower(*text)) return 0;
    if (str_empty(text)) return 0;
    parse++; text++;
  }
  text = str_skipspace(text);
  if (!str_empty(text)) return 0;

  return text-oldpos;
}


static int str_parse(syntaxlist_t *sl, const char *parse, char *outline)
{
  *outline = '\0';
  parse = str_skipspace(parse);
  if (str_empty(parse))
    return 0;

  syntaxlist_t *old = syntaxlist;
  syntaxlist = sl;
  syntax_t *s = syntaxlist->syntax;

  char tempstr[TEMP_SIZE];
  while (1) {
    if (s == NULL) {
      syntaxlist = old;
      return 0;
    }
    strcpy(tempstr, s->parseline);
    int size = parsematch(const_cast<char*>(parse), tempstr);
    if (size) {
      setTheseVars(s->assignments);
      if (!str_empty(s->output))
	expand_string(s->output, outline);
      setTheseVars(s->assignments_after);
      syntaxlist = old;
      return size;
    }
    s = s->next;
  }
  return 0;
}

static void CommentConvert(char *str, const comment_t &comment)
{
  char tempstr[TEMP_SIZE];
  char *dst = tempstr;
  char *src = str;
  while (!strncmp(src, comment.src, comment.srclen)) {
    strcpy(dst, comment.dst);
    dst += comment.dstlen;
    src += comment.srclen;
  }
  *dst = '\0';
  strcpy(dst, src);

  strcpy(str, tempstr);
}

static void findComment(char const *source, comment_match_t &match)
{
  match.at = NULL;
  for (int c=0; c<comments; c++) {
    const char *pos = strstr(source, comment[c].src);
    if (!match.at || (pos && pos < match.at)) {
      match.at = const_cast<char*>(pos);
      match.data = &comment[c];
    }
  }
}
static void addComment(char const *source, char const *dest)
{
  if (comments == MAX_COMMENTS)
    fprintf(stderr, "WARNING! Too many comment pairs defined\n");
  comment[comments].src = source;
  comment[comments].dst = dest;
  comment[comments].srclen = strlen(source);
  comment[comments].dstlen = strlen(dest);
  comments++;
}

void mainparse()
{
  char *pos;
  comment_match_t comment;
  char temp[TEMP_SIZE];
  char outline[TEMP_SIZE];
  int  skipline = 0;

  syntaxlist_t *mainsyntax = findSyntax("main");

  pos = tempstr;
  findComment(pos, comment);
  if (comment.at) {
    char quote_char = '"';
    char* quote1 = strchr(pos, quote_char);
    char* quote2;
    while (quote1 && quote1 < comment.at) {
      quote2 = strchr(quote1+1, quote_char);
      if (!quote2) {
	comment.at = 0;
	break;
      }
      if (quote2 > comment.at) {
	findComment(quote2, comment);
	quote1 = strchr(quote2+1, quote_char);
      }
    }
    // cut the comment away so it won't be parsed
    if (comment.at)
      *comment.at = '\0';
  }

  *outline = '\0';

  pos = str_skipspace(pos);
  basepos = pos-tempstr;
  if (!str_empty(pos)) {
    if (!str_parse(mainsyntax, pos, temp)) {
      fprintf(stderr,"WARNING, Line %d: no syntax match: \"%s\"\n",
	      current_line,tempstr);
      printf("MISMATCH: \"%s\"\n",tempstr);
      return;
    }
    if (str_empty(temp))
      skipline = 1;
    else if (flags & FLAG_INLINEMODE) {
      str_addatpos(outline, "\"", basepos);
      strcat(outline, temp);
      str_addatpos(outline, "\\n\\t\"", basepos+29);
    } else
      str_addatpos(outline, temp, basepos);
  }

  if (comment.at) {
    /* Resurrect the first comment character */
    *comment.at = comment.data->src[0];
    /* Convert continuous comment chars */
    CommentConvert(comment.at, *comment.data);
    str_addatpos(outline, comment.at, comment.at-tempstr);
  }

  if (!skipline || comment.at)
    printf("%s\n",outline);
}

static void clean_clobber_list(void)
{
  char *var = (char *)fetchVar("clobber");
  if (str_empty(var)) return;

  static struct {
    const char *reg;
    int   mask;
  } reglist[] = {
    {"bp",0x40000},{"ebp",0xc0000},
    {"sp",0x10000},{"esp",0x20000},
    {"si",0x4000},{"esi",0xc000},
    {"di",0x1000},{"edi",0x3000},
    {"dl",0x200},{"dh",0x400},{"dx",0x600},{"edx",0xe00},
    {"cl",0x040},{"ch",0x080},{"cx",0x0c0},{"ecx",0x1c0},
    {"bl",0x008},{"bh",0x010},{"bx",0x018},{"ebx",0x038},
    {"al",0x001},{"ah",0x002},{"ax",0x003},{"eax",0x007},
  };
  int const reglist_size = sizeof(reglist)/sizeof(reglist[0]);
  int regs=0,mmregs=0;
  char str[STR_LEN+1]; str[0] = '\0';

  /* Go through the list of registers and mark the ones found.
   * al+ah gives automaticly ax and so forth */
  char *reg = var, *next = var;
  while ((next = str_cutat(next, ',')) != NULL) {
    char found = 0;
    if (!strncmp(reg, "mm",2)) {
      mmregs |= 1 << (reg[2]-'0');
      found = 1;
    } else
      for (int i=0; i<reglist_size; i++)
	if (!strcmp(reg, reglist[i].reg)) {
	  regs |= reglist[i].mask;
	  found = 1;
	  break;
	}
    if (!found) {
//      fprintf(stderr,"Warning: Unknown register %s, while creating clobber list\n",reg);
      strcat(str,reg);
      str_catchar(str,',');
    }
    reg = next;
  }

  /* Construct cleaned clobber list */
  for (int i=reglist_size-1; i>=0; i--)
    if ((regs & reglist[i].mask) == reglist[i].mask) {
      regs -= reglist[i].mask;
      strcat(str,reglist[i].reg);
      str_catchar(str,',');
    }
  for (int i=0; i<8; i++)
    if (mmregs & (1<<i)) {
      strcat(str,"mm");
      str_catchar(str, i+'0');
      str_catchar(str,',');
    }

  /* Store the new list back to clobber variable */
  setVar("clobber", str);
}

static void printHelp(char const *infostr)
{
  fprintf(stderr,"%s\n"\
	  "usage: intel2gas [options] [-o outfile] [infile]\n"\
	  "where options include:\n"\
	  "\t-h\t\tthis help\n"\
	  "\t-i\t\tconvert from intel to at&t format (default)\n"\
	  "\t-g\t\tconvert from at&t to intel format\n"\
	  "\t-m -t\t\tconvert from masm/tasm to at&t format\n"\
	  "\t-c\t\tunderstand C style comments\n"\
	  "\t-I\t\tconvert inline assembler (intel to at&t only)\n"\
	  "\t-d\t\toutput all %% chars as %%%%\n"\
	  "\t-V\t\tshow version\n"\
	  "\tinfile and outfile default to stdin/stdout\n"\
	  "\nFor more information see: http://hermes.terminal.at/intel2gas/\n",
	  infostr);
  exit (*infostr != '\0');
}

static void print_inline_asm_regs(int basepos)
{
  char tmp[STR_LEN+1];

  fprintf(stdout, "\n");	// blank line is conventional
  tmp[0] = '\0';
  str_addatpos(tmp, "   : // FIXASM: output regs/vars go here, e.g.:  \"=m\" (memory_var)", basepos);
  fprintf(stdout, "%s\n", tmp);

  fprintf(stdout, "\n");
  tmp[0] = '\0';
  str_addatpos(tmp, "   : // FIXASM: input regs, e.g.:  \"c\" (count), \"S\" (src), \"D\" (dest)", basepos);
  fprintf(stdout, "%s\n", tmp);

  fprintf(stdout, "\n");
  tmp[0] = '\0';
  str_addatpos(tmp, "   : ", basepos);

  // Have we created the clobber list
  char *reg = (char *)fetchVar("clobber");
  if (str_empty(reg)) {
    strcat(tmp, "// FIXASM: clobber list, e.g.:  \"%eax\", \"%ecx\", \"cc\"");
  } else {
    // find each clobber register and print it
    char *next = reg;
    while ((next = str_cutat(next, ',')) != NULL) {
      strcat(tmp, "\"%");
      strcat(tmp, reg);
      strcat(tmp, "\", ");
      reg = next;
    }
    tmp[strlen(tmp)-2] = '\0'; // remove last comma
    strcat(tmp, " // CHECKASM: clobber list");
  }
  fprintf(stdout, "%s\n", tmp);


  tmp[0] = '\0';
  str_addatpos(tmp, ");", basepos);
  fprintf(stdout, "%s\n", tmp);
}

static int parse_parameters(int argc, char *argv[], char modedir[])
{
  int ch;

  while ((ch = getopt(argc, argv, "o:VIhgicxmtd")) != -1) {
    switch(ch) {
    case 'o':
      if (strcmp(*argv,"-"))
	if (!freopen(optarg, "w", stdout)) {
	  fprintf(stderr, "Error: Could not create %s\n",optarg);
	  return 1;
	}
      break;
    case 'i':
      strcpy(modedir, "i2g/");
      break;
    case 'c':
      flags |= FLAG_CCOMMENTS;
      break;
    case 'g':
      strcpy(modedir, "g2i/");
      break;
    case 'I':
      flags |= FLAG_INLINEMODE;
      flags |= FLAG_FINDASMBLOCKS;
      flags |= FLAG_CCOMMENTS;
      in_asm_block = false;
      break;
    case 'd':
      flags |= FLAG_DOUBLEPERCENTCHARS;
      break;
    case 'm':
    case 't':
      strcpy(modedir, "m2g/");
      break;
    case 'V':
      printf("intel2gas 1.3.3\n");
      exit(0);
    default:
      printHelp("Error: unrecognized parameter\n");
      // Fall through
    case 'h':
      printHelp("");
      break;
    }
  }
  argc -= optind;
  argv += optind;

  if (argc > 1)
    printHelp("Error: too many input files");
  if (*argv) {
    if (strcmp(*argv,"-"))
      if (!freopen(*argv, "r", stdin)) {
	fprintf(stderr,"Error: Could not open %s\n",*argv);
	return 1;
      }
  }
  return 0;
}

static int load_syntax_files(char modedir[])
{
  // Override the compiled in path
  const char *resource_path = getenv("I2G_DATA") == NULL ? RESOURCE_PATH : getenv("I2G_DATA");

    // add missing '/'
  char base[STR_LEN+1];
  strcpy(base, resource_path);
  if (base[strlen(base)-1] != '/')
    str_catchar(base,'/');
  strcat(base, modedir);

  // Load the lists & syntaxis
  LoadWordLists(base);
  LoadSyntaxis(base);

    // initialize the base pointers
  basesyntaxlist = syntaxlist;
  syntaxlist = findSyntax("main");

    // check that we actually got some files loaded
  if (basesyntaxlist == NULL || syntaxlist == NULL) {
    printf("fatal error: no %s%smain.syntax found\n"\
	   "If the syntax files are no longer there, you can set the\n"\
	   "I2G_DATA environmental variable to point to the correct directory\n",
	   resource_path,modedir);
    return 1;
  }
  return 0;
}


static void retrieve_comment_styles()
{
  if (!str_parse(syntaxlist, "General settings:", tempstr))
    return;

  static char src_comment[] = "src_commentX";
  static char dst_comment[] = "dst_commentX";

  for (int i=0; i<MAX_COMMENTS; i++) {
    src_comment[11] = '0'+i;
    dst_comment[11] = '0'+i;
    char const *src = fetchVar(src_comment);
    char const *dst = fetchVar(dst_comment);
    if (!str_empty(src) && !str_empty(dst))
      addComment(src,dst);
  }
}

static bool is_a_define_line(char const *line)
{
  static const char *keywords[] = {
    "if","define","else","endif",NULL,
  };
  for (int i=0; keywords[i]; i++) {
    if (!strncmp(line, keywords[i], strlen(keywords[i])))
      return true;
  }
  return false;
}

static int main_parse_loop()
{
  bool remove_last_comment = false;
  current_line = 0;

  while (!feof(stdin)) {
    ReadNextLine(stdin);

    if (flags & FLAG_CCOMMENTS) {
      // Skip C style comments

      bool just_started = false;
      if (!in_c_comment) {
	if (strstr(tempstr, "/*")) {
	  addComment("/*","/*"); // only temporary
	  remove_last_comment = true;
	  just_started = true;
	  in_c_comment = true;
	}
      }

      if (in_c_comment) {
	char *pos;
	pos = strstr(tempstr, "*/");
	if (pos) {
	  in_c_comment = false;
	  if (!just_started) {
	    *pos = '\0';
	    fprintf(stdout, "%s*/", tempstr);
	    strcpy(tempstr, pos+2);
	  }
	} else {
	  fprintf(stdout, "%s\n", tempstr);
	  goto lineEnd;
	}
      }
    }

    if (flags & FLAG_FINDASMBLOCKS) {
      if (!in_asm_block) {

	/* Search for the asm keyword */
	char *pos = strstr(tempstr, "asm");
	char starts_now = 0;
	if (pos) {
	  if (*str_skipspace(pos+3) == '{') starts_now = 1;
	  else { // maybe {'s on the next line
	    char last_line[TEMP_SIZE];
	    strcpy(last_line, tempstr);
	    ReadNextLine(stdin);
	    if (*str_skipspace(tempstr) == '{') starts_now = 1;
	    // it propably was a just the text asm somewhere
	    else fprintf(stdout, "%s\n", last_line);
	  }
	  if (starts_now) {
	    in_asm_block = true;
	    setVar("clobber",""); // clear clobber list

	    // check for up to two preceding underscores ("__asm")
	    if (pos > tempstr && pos[-1] == '_') --pos;
	    if (pos > tempstr && pos[-1] == '_') --pos;

	    char tmp[STR_LEN+1]; tmp[0] = '\0';
	    str_addatpos(tmp, "__asm__ (", pos-tempstr);
	    fprintf(stdout, "%s\n", tmp);
	    /* skip the rest of the line. Maybe we shouldn't
	     * but hopefully no one notices */
	    goto lineEnd;
	  }
	}
	// Not in assembler block, just copy the line
	fprintf(stdout, "%s\n", tempstr);
	goto lineEnd;
      } else { // in_asm_block

	/* search for the ending bracket, let's hope nobody
	 * uses it in their comments (counting seconds....) */
	char *pos = str_skipspace(tempstr);
	if (*pos == '}') {
	  in_asm_block = false;
	  print_inline_asm_regs(pos-tempstr);
	  goto lineEnd;
	} else if (*pos == '#' && is_a_define_line(pos+1)) {
	  fprintf(stdout, "%s\n", tempstr);
	  goto lineEnd;
	}
      }
    }
    mainparse();
    clean_clobber_list();

  lineEnd:
    if (remove_last_comment) {
      // removes the temporary /* .. */ comment added on this line
      remove_last_comment = false;
      comments--;
    }
  }
  return 0;
}

int main(int argc, char *argv[])
{
  char modedir[STR_LEN] = "i2g/";

  if (parse_parameters(argc, argv, (char *)modedir))
    return 1;

  if (load_syntax_files(modedir))
    return 2;

  retrieve_comment_styles();

  // set comments correctly if converting inline asm
  if (flags & FLAG_INLINEMODE) {
    if (modedir[0] == 'g')
      printHelp("Error: gas -> Intel conversion not supported for inline asm\n");
    addComment("//","//");
  }

  if (comments == 0) {
    fprintf(stderr, "Error: No comments defined\n");
    return 3;
  }

  return main_parse_loop();
}
