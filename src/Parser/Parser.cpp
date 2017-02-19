/**
 * Parser
 *
 * @author: Gatech Buzz
 */

#include <cstring>
#include <unordered_map>

#include "../Scanner/Scanner.hpp"
#include "../SemanticAnalyzer/SymbolTable.hpp"
#include "../SemanticAnalyzer/SymbolTerminalPair.hpp"

class Parser {
private:
  Scanner scanner;
  int numErrors;
  std::string globalFileName;
  std::stack<int> parseStack;
  std::unordered_map<int, std::string> terminalMapped;
  std::unordered_map<SymbolTerminalPair, std::vector<int>> parseTable;

public:
  std::ofstream outFile;
  bool printDebug;
  Parser(std::string fileName) : scanner(fileName) {
    parseStack.push(Symbol::Terminal::EOFF);
    parseStack.push(Symbol::Nonterminal::TIGER_PROGRAM);

    initializeTerminalMapped();

    globalFileName = fileName;
    int tmparr[] = {Symbol::Action::FinalizeScope,
                    Symbol::Terminal::END,
                    Symbol::Nonterminal::STAT_SEQ,
                    Symbol::Action::MakeMainLabel,
                    Symbol::Terminal::IN,
                    Symbol::Nonterminal::DECLARATION_SEGMENT,
                    Symbol::Action::InitializeScope,
                    Symbol::Terminal::LET};
    SymbolTerminalPair stp(Symbol::Nonterminal::TIGER_PROGRAM,
                           Symbol::Terminal::LET);
    std::vector<int> rhsVec = {std::begin(tmparr), std::end(tmparr)};
    // Insert the pair stp and rhsVec inside parseTable
    // Fill in the remaining of the parseTable as above.
    // Recommended- work out the parseTable by hand and then code it here
  }

  void error(std::string message) {
    numErrors++;
    std::cout << globalFileName << ":" << scanner.getCurrLine()
              << ": error: " << message << "\n";
  }

  void parse() {
    TokenPair *word = scanner.getToken();
    std::string parseStackTemp = "";
    Symbol focus;

    /* Used for the DEBUG FLAG */
    int currLine = scanner.getCurrLine();
    int lastLine = scanner.getCurrLine();

    // while (true) {
    // get the token and parse
    // complete parsing before going to semantic analysis
    // then embed the semantic actions in the parsing table and
    // accordingly take actions and generate the IR.
    // Output the IR, only if there are no errors in the input.
    //}
  }

private:
  void initializeTerminalMapped() {
    terminalMapped[Symbol::Terminal::ARRAY] = "array";
    terminalMapped[Symbol::Terminal::BREAK] = "break";
    terminalMapped[Symbol::Terminal::DO] = "do";
    terminalMapped[Symbol::Terminal::ELSE] = "else";
    terminalMapped[Symbol::Terminal::END] = "end";
    terminalMapped[Symbol::Terminal::FOR] = "for";
    terminalMapped[Symbol::Terminal::FUNCTION] = "function";
    terminalMapped[Symbol::Terminal::IF] = "if";
    terminalMapped[Symbol::Terminal::IN] = "in";
    terminalMapped[Symbol::Terminal::LET] = "let";
    terminalMapped[Symbol::Terminal::OF] = "of";
    terminalMapped[Symbol::Terminal::THEN] = "then";
    terminalMapped[Symbol::Terminal::TO] = "to";
    terminalMapped[Symbol::Terminal::TYPE] = "type";
    terminalMapped[Symbol::Terminal::VAR] = "var";
    terminalMapped[Symbol::Terminal::WHILE] = "while";
    terminalMapped[Symbol::Terminal::ENDIF] = "endif";
    terminalMapped[Symbol::Terminal::BEGIN] = "begin";
    terminalMapped[Symbol::Terminal::ENDDO] = "enddo";
    terminalMapped[Symbol::Terminal::RETURN] = "return";
    terminalMapped[Symbol::Terminal::INT] = "int";
    terminalMapped[Symbol::Terminal::FLOAT] = "float";
    terminalMapped[Symbol::Terminal::COMMA] = ",";
    terminalMapped[Symbol::Terminal::COLON] = ":";
    terminalMapped[Symbol::Terminal::SEMI] = ";";
    terminalMapped[Symbol::Terminal::LPAREN] = "(";
    terminalMapped[Symbol::Terminal::RPAREN] = ")";
    terminalMapped[Symbol::Terminal::LBRACK] = "[";
    terminalMapped[Symbol::Terminal::RBRACK] = "]";
    terminalMapped[Symbol::Terminal::LBRACE] = "{";
    terminalMapped[Symbol::Terminal::RBRACE] = "}";
    terminalMapped[Symbol::Terminal::PERIOD] = ".";
    terminalMapped[Symbol::Terminal::PLUS] = "+";
    terminalMapped[Symbol::Terminal::MINUS] = "-";
    terminalMapped[Symbol::Terminal::MULT] = "*";
    terminalMapped[Symbol::Terminal::DIV] = "/";
    terminalMapped[Symbol::Terminal::EQ] = "=";
    terminalMapped[Symbol::Terminal::NEQ] = "!=";
    terminalMapped[Symbol::Terminal::LESSER] = "<";
    terminalMapped[Symbol::Terminal::GREATER] = ">";
    terminalMapped[Symbol::Terminal::LESSEREQ] = "<=";
    terminalMapped[Symbol::Terminal::GREATEREQ] = ">=";
    terminalMapped[Symbol::Terminal::AND] = "&";
    terminalMapped[Symbol::Terminal::OR] = "|";
    terminalMapped[Symbol::Terminal::ASSIGN] = ":=";
    terminalMapped[Symbol::Terminal::ID] = "id";
    terminalMapped[Symbol::Terminal::INTLIT] = "intlit";
    terminalMapped[Symbol::Terminal::FLOATLIT] = "floatlit";
    terminalMapped[Symbol::Terminal::NULLL] = "epsilon";
    terminalMapped[Symbol::Terminal::EOFF] = "";
  }
};

/**
 * Prints helpful information for the user if they try to run the
 * program with a bad number of arguments
 */
void printHelp() {
  std::cout << "\n"
            << "You have entered an incorrect number of arguments."
            << "\n"
            << "\n"
            << "Please specify the file you wish to parse and"
            << "\n"
            << "optionally whether you want to flag for debugging."
            << "\n"
            << "\n"
            << "> parser <filename> -d"
            << "\n";
}
/**
     * There are two arguments: one mandatory and one optional
     *
     * The mandatory argument is the file to perform a parse on while the
     * optional flag -d indicates that the user wants to print debug info.,
     * namely the sequences of Tokens as the Scanner reads them
     */
int main(int argc, char **argv) {
  // The user has given us a bad number of args
  if (argc > 3 || argc < 1) {
    printHelp();
    return 0;
  }

  // Initialize the Parser with the given filename
  Parser parser(argv[1]);

  // Print debug info. if flagged
  if (argc > 2 && strcmp(argv[2], "-d") == 0) parser.printDebug = true;

  if (argc > 2 && strcmp(argv[2], "-d") != 0)
    std::cout << "\nPlease use \"-d\" as flag for debugging.\n";

  // Pass
  parser.parse();

  // Close all open files like
  parser.outFile.close();
  return 0;
}
