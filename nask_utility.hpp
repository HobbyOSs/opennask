#include <fstream>
#include "ParaTokenizer.hh"
#include "ParaOperator.hh"
#include "ParaExpression.hh"
#include "ParaSymbolTable.hh"
#include "ParaMathLibrary.hh"

namespace nask_utility {

     // デフォルトのトークンテーブル
     TParaCxxTokenTable token_table;

     std::ifstream::pos_type filesize(const char* filename) {
	  std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
	  return in.tellg();
     }

     bool is_comment_line(TParaCxxTokenTable& token_table, TParaToken& token) {
	  return token_table.IsCommentLimiter(token.AsString());
     }

     // 簡単なDB命令の実装
     int process_token_DB(TParaTokenizer& tokenizer, std::vector<uint8_t>& binout_container) {
	  for (TParaToken token = tokenizer.Next(); ; token = tokenizer.Next()) {
	       if (is_comment_line(token_table, token)) {
		    break;
	       } else if (token.Is(",")) {
		    continue;
	       } else if (token.IsQuote()) {
		    std::string str = token.AsString();
		    if (str.front() == '"' && str.back() == '"') {
			 str.erase( 0, 1 );
			 str.erase( str.size() - 1 );
			 for ( uint8_t b : str ) {
			      binout_container.push_back(b);
			 }
		    } else {
			 std::cerr << "NASK : DB not quoted correctly' " << std::endl;
			 return 17;
		    }
	       } else {
		    binout_container.push_back(token.AsLong());
	       }
	  }
     }

     // 簡単なRESB命令の実装
     int process_token_RESB(TParaTokenizer& tokenizer, std::vector<uint8_t>& binout_container) {
	  for (TParaToken token = tokenizer.Next(); ; token = tokenizer.Next()) {
	       if (is_comment_line(token_table, token)) {
		    break;
	       } else if (token.Is(",")) {
		    continue;
	       } else {
		    for (ulong l = 0; l < token.AsLong(); l++) {
			 binout_container.push_back(0x00);
		    }
	       }
	  }
     }
}
