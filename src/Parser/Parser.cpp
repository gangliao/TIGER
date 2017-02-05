/**
 * Parser
 *
 * @author: Gatech Buzz
 */
#include <cstring>
#include "../Scanner/Scanner.hpp"
#include "../SemanticAnalyzer/SymbolTable.hpp"
#include "../SemanticAnalyzer/SymbolTerminalPair.hpp"
class Parser {
 private:
  Scanner scanner;
  int numErrors;
  std::string globalFileName;
  std::stack<int> parseStack;
  std::map<int, std::string> terminalMapped;
  std::map<SymbolTerminalPair, std::vector<int>> parseTable;

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
    terminalMapped.insert(std::make_pair(Symbol::Terminal::COMMA, ","));
    // Map the other terminals accordingly
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
