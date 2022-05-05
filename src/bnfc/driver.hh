/* File generated by the BNF Converter (bnfc 2.9.5). */

#ifndef __DRIVER_H__
#define __DRIVER_H__ 1
#include <string>
#include <cstddef>
#include <istream>
#include <memory>

#include "scanner.hh"
#include "parser.hh"



class  NaskDriver{
public:
    NaskDriver() = default;
    virtual ~ NaskDriver();

    /**
     * parser parsed values defined by bnfc
     */
    int    _int;
    char   _char;
    double _double;
    char*  _string;

    std::shared_ptr<Program> program_;
    std::shared_ptr<ListStatement> liststatement_;
    std::shared_ptr<Statement> statement_;
    std::shared_ptr<ListMnemonicArgs> listmnemonicargs_;
    std::shared_ptr<MnemonicArgs> mnemonicargs_;
    std::shared_ptr<Exp> exp_;
    std::shared_ptr<Factor> factor_;
    std::shared_ptr<ConfigType> configtype_;
    std::shared_ptr<DataType> datatype_;
    std::shared_ptr<Opcode> opcode_;


    std::shared_ptr<Program> pProgram(std::istream &stream);
    std::shared_ptr<ListStatement> pListStatement(std::istream &stream);
    std::shared_ptr<Statement> pStatement(std::istream &stream);
    std::shared_ptr<ListMnemonicArgs> pListMnemonicArgs(std::istream &stream);
    std::shared_ptr<MnemonicArgs> pMnemonicArgs(std::istream &stream);
    std::shared_ptr<Exp> pExp(std::istream &stream);
    std::shared_ptr<Factor> pFactor(std::istream &stream);
    std::shared_ptr<ConfigType> pConfigType(std::istream &stream);
    std::shared_ptr<DataType> pDataType(std::istream &stream);
    std::shared_ptr<Opcode> pOpcode(std::istream &stream);


    /**
     * parse - parse from a file
     * @param filename - valid string with input file
     */
    void parse(const char *filename);
    /**
     * parse - parse from a c++ input stream
     * @param is - std::istream&, valid input stream
     */
    void parse(std::istream &iss);
    /** Error handling with associated line number. This can be modified to output the error. */
    void error(const nask_::NaskParser::location_type& l, const std::string& m);

    std::ostream& print(std::ostream &stream);

    // debug flags
    bool trace_scanning = false;
    bool trace_parsing = false;

    std::unique_ptr<NaskScanner> scanner = nullptr;
    std::unique_ptr<nask_::NaskParser>  parser  = nullptr;

private:
    void parse_helper( std::istream &stream );

};


#endif /* END __DRIVER_H__ */
