/* File generated by the BNF Converter (bnfc 2.9.5). */

#include <cctype>
#include <fstream>
#include <cassert>

#include "driver.hh"


NaskDriver::~NaskDriver()
{
}

void
NaskDriver::parse( const char * const filename )
{
    /**
     * Remember, if you want to have checks in release mode
     * then this needs to be an if statement
     */
    assert( filename != nullptr );
    std::ifstream in_file( filename );
    if( ! in_file.good() )
    {
        exit( EXIT_FAILURE );
    }
    parse_helper( in_file );
    return;
}

void
NaskDriver::parse( std::istream &stream )
{
    if( ! stream.good()  && stream.eof() ) {
        return;
    }
    parse_helper( stream );
    return;
}

void
NaskDriver::error( const nask_::NaskParser::location_type& l, const std::string& m )
{
    std::cerr << "error: "
              << scanner->loc->begin.line << "," << scanner->loc->begin.column
              << ": "
              << m
              << " at " << std::string(scanner->YYText())
              << std::endl;
}

void
NaskDriver::parse_helper( std::istream &stream )
{

    scanner.reset();
    try {
        scanner = std::make_unique<NaskScanner>( &stream );
        scanner->set_debug(trace_scanning);
    } catch( std::bad_alloc &ba ) {
        std::cerr << "Failed to allocate scanner: ("
                  << ba.what()
                  << "), exiting!!\n";
        exit( EXIT_FAILURE );
    }

    parser.reset();
    try {
        parser = std::make_unique<nask_::NaskParser>((*scanner), (*this));
    } catch( std::bad_alloc &ba ) {
        std::cerr << "Failed to allocate parser: ("
                  << ba.what()
                  << "), exiting!!\n";
        exit( EXIT_FAILURE );
    }
    const int accept( 0 );

    parser->set_debug_level (trace_parsing);
    if( parser->parse() != accept ) {
        exit( EXIT_FAILURE );
    }
    return;
}

std::shared_ptr<Program>
NaskDriver::pProgram(std::istream &stream)
{
    parse_helper( stream );

    return this->program_;
}

std::shared_ptr<ListStatement>
NaskDriver::pListStatement(std::istream &stream)
{
    parse_helper( stream );
    this->liststatement_->reverse();
    return this->liststatement_;
}

std::shared_ptr<Statement>
NaskDriver::pStatement(std::istream &stream)
{
    parse_helper( stream );

    return this->statement_;
}

std::shared_ptr<ListFactor>
NaskDriver::pListFactor(std::istream &stream)
{
    parse_helper( stream );
    this->listfactor_->reverse();
    return this->listfactor_;
}

std::shared_ptr<ListMnemonicArgs>
NaskDriver::pListMnemonicArgs(std::istream &stream)
{
    parse_helper( stream );
    this->listmnemonicargs_->reverse();
    return this->listmnemonicargs_;
}

std::shared_ptr<MnemonicArgs>
NaskDriver::pMnemonicArgs(std::istream &stream)
{
    parse_helper( stream );

    return this->mnemonicargs_;
}

std::shared_ptr<Exp>
NaskDriver::pExp(std::istream &stream)
{
    parse_helper( stream );

    return this->exp_;
}

std::shared_ptr<Exp>
NaskDriver::pExp1(std::istream &stream)
{
    parse_helper( stream );

    return this->exp_;
}

std::shared_ptr<Exp>
NaskDriver::pExp2(std::istream &stream)
{
    parse_helper( stream );

    return this->exp_;
}

std::shared_ptr<MemoryAddr>
NaskDriver::pMemoryAddr(std::istream &stream)
{
    parse_helper( stream );

    return this->memoryaddr_;
}

std::shared_ptr<IndexExp>
NaskDriver::pIndexExp(std::istream &stream)
{
    parse_helper( stream );

    return this->indexexp_;
}

std::shared_ptr<Factor>
NaskDriver::pFactor(std::istream &stream)
{
    parse_helper( stream );

    return this->factor_;
}

std::shared_ptr<JumpDir>
NaskDriver::pJumpDir(std::istream &stream)
{
    parse_helper( stream );

    return this->jumpdir_;
}

std::shared_ptr<ConfigType>
NaskDriver::pConfigType(std::istream &stream)
{
    parse_helper( stream );

    return this->configtype_;
}

std::shared_ptr<DataType>
NaskDriver::pDataType(std::istream &stream)
{
    parse_helper( stream );

    return this->datatype_;
}

std::shared_ptr<Opcode>
NaskDriver::pOpcode(std::istream &stream)
{
    parse_helper( stream );

    return this->opcode_;
}



