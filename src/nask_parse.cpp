#include <iostream>
#include <getopt.h>
#include "driver.hh"

int main (int argc, char *argv[]) {
  int res = 0;
  driver drv;

  int opt, i, option_index;

  struct option long_options[] = {
    { "parse", no_argument, NULL, 'p' },
    { "scan",  no_argument, NULL, 's' },
    { "help",  no_argument, NULL, 'h' },
    { 0, 0, 0, 0 }
  };

  while ((opt = getopt_long(argc, argv, "mes:", long_options, &option_index)) != -1) {

    switch(opt){
    case 'V':
      printf("naskparse \n");
      printf("Copyright (C) 2021 Hiroyuki Nagata.\n"
	     "ライセンス GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.\n"
	     "This is free software: you are free to change and redistribute it.\n"
	     "There is NO WARRANTY, to the extent permitted by law.\n"
	     "\n"
	     "Thank you osask project !\n");
      return 0;
    case 'p':
      drv.trace_parsing = true;
      break;
    case 's':
      drv.trace_scanning = true;
      break;
    case 'h':
      printf("Usage:  [--help | --parse | --scan] source \n");
      return 0;
      // 解析できないオプションが見つかった場合は「?」を返す
      // オプション引数が不足している場合も「?」を返す
    case '?':
      printf("unknown or required argument option -%c\n", optopt);
      printf("Usage:  [--help | --parse | --scan] source \n");
      return 1;   // exit(EXIT_FAILURE);と同等 http://okwave.jp/qa/q794746.html
    }
  }

  if (!drv.parse (argv[optind]))
    std::cout << drv.result << '\n';
  else
    return 1;
}
