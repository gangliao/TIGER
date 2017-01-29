/**
 * Scanner
 *
 * @author: Gatech Buzz
 */
//#include "../common/Symbol.hpp"
#include "../common/TokenPair.hpp"
#include<vector>
class Scanner {
private:
  std::set<std::string> keywords;
    int currLine;
    std::string oldPrefix ;
    std::string prefix;
    std::string globalFileName;
    int numErrors;
    std::ifstream file;
    std::vector<std::string>keywordsVector ;

public:
   int getNumErrors() {
        return numErrors;
    }

     int getCurrLine() {
        return currLine;
    }

     std::string getPrefix() {
        return prefix;
    }
private:
     void error(std::string message) {
        numErrors++;
	std::cout<<globalFileName << ":" << currLine << ": Lexical error: " << message << "\n";
    }

public:
	Scanner(std::string fileName)   {
		    currLine = 1;
		    oldPrefix = "";
		    prefix = "";
		    numErrors = 0;

		    file.open(fileName);

		    globalFileName = fileName;
	
		    std::string tmparr[] = {
				    "array", "break", "do", "else", "end", "for", "function", "if",
				    "in", "let", "of", "then", "to", "type", "var", "while", "endif",
				    "begin", "enddo", "return", "int", "float"};
		keywords = std::set<std::string>(std::begin(tmparr),std::end(tmparr));
		keywordsVector = std::vector<std::string>(std::begin(tmparr),std::end(tmparr));
         }
private:
    /**
     * Call reader's read
     * Return the read character as an int
     */
     int read()   {
      int readChar = file.get();
        if ((char)readChar == '\n') {
            ++currLine;
            oldPrefix = prefix;
            prefix = "";
        }
        else {
            prefix += (char)readChar;
            oldPrefix += (char)readChar;
        }
        return readChar;
    }

    /**
     * Call reader's unread
     */
     void unread(int unreadChar)   {
        if ((char)unreadChar == '\n') {
            --currLine;
            prefix = oldPrefix;
        }
        else {
            oldPrefix = "";
            for (int i = 0; i < prefix.length() - 1; ++i) {
                oldPrefix += prefix.at(i);
            }
            prefix = oldPrefix;
        }
        file.putback((char)unreadChar);
    }

    /**
     * Return the longest string of consecutive digits
     */
     std::string readDigits()   {
        std::string sNumber = "";
        int currChar = read();
        while (isdigit((char)currChar)) {
            sNumber += (char)currChar;
            currChar = read();
        }

        if (isalpha((char)currChar)) {
            while (isalnum((char)currChar)
                || (char)currChar == '_') {
                sNumber += (char)currChar;
                currChar = read();
            }

            error(sNumber + " is an invalid identifier");
            return std::string();
        }

        unread(currChar);
        return sNumber;
    }

    /**
     * Return the longest string of consecutive alphanumerics and '_'
     */
     std::string readWordChars()   {
        std::string word = "";
        int currChar = read();
        while (isalnum((char)currChar)
            || (char)currChar == '_') {
            word += (char)currChar;
            currChar = read();
        }
        unread(currChar);
        return word;
    }

public:
  TokenPair* getToken()   
  {
        while (true) {
            int currChar = read();
            int nextChar = 0;

            // If EOFF, return nothing
            if (currChar == -1) {
                return new TokenPair(Symbol::Terminal::EOFF, "");
            }
            // Skip whitespace characters
            if (isspace((char)currChar)) {
                continue;
            }
            // Read a word
            if (isalpha((char)currChar)) {
                std::string word = (char)currChar + readWordChars();

                // Check if it is a keyword
                if (keywords.count(word)) {
		  int terminalEnum = -1,index =0;
		  for(auto kword : keywordsVector)
		    {
			if(word.compare(kword) == 0){
			    terminalEnum = index;
			    break;
			}
			index++;
		    }
		  if(terminalEnum == -1){
		    error("No such Keyword \n");
		    continue;
		  }
                    return new
                        TokenPair(terminalEnum, word);
                }
                // Else it is an identifier
                return new TokenPair(Symbol::Terminal::ID, word);
            }
            // Read an int or float
            if (isdigit((char)currChar)) {
                unread(currChar);
                std::string sNumber = readDigits();

                // For the case with a bad identifier
                if (sNumber == std::string()) {
                    continue;
                }

                // If the first character of the integer is a '0' and the
                // integer is not exactly "0", we have an error
                if (sNumber.length() > 1 && sNumber.at(0) == '0') {
                    error("Invalid token" + sNumber);
                    continue;
                }
                currChar = read();

                // An int followed by a "." must be a float
                if ((char)currChar == '.') {
                    std::string sDecimals = readDigits();
                    return new TokenPair(Symbol::Terminal::FLOATLIT,
                                         sNumber + "." + sDecimals);
                }
                // Else it is just an int
                unread(currChar);
                return new TokenPair(Symbol::Terminal::INTLIT, sNumber);
            }
            // Handle symbols and operators
            // Invalid tokens will be found here
            switch ((char)currChar) {
                // Single-character symbols
                case ',': return new TokenPair(Symbol::Terminal::COMMA,  ",");
                case ';': return new TokenPair(Symbol::Terminal::SEMI,   ";");
                case '(': return new TokenPair(Symbol::Terminal::LPAREN, "(");
                case ')': return new TokenPair(Symbol::Terminal::RPAREN, ")");
                case '[': return new TokenPair(Symbol::Terminal::LBRACK, "[");
                case ']': return new TokenPair(Symbol::Terminal::RBRACK, "]");
                case '{': return new TokenPair(Symbol::Terminal::LBRACE, "{");
                case '}': return new TokenPair(Symbol::Terminal::RBRACE, "}");
                case '.': return new TokenPair(Symbol::Terminal::PERIOD, ".");
                case '+': return new TokenPair(Symbol::Terminal::PLUS,   "+");
                case '-': return new TokenPair(Symbol::Terminal::MINUS,  "-");
                case '*': return new TokenPair(Symbol::Terminal::MULT,   "*");
                case '&': return new TokenPair(Symbol::Terminal::AND,    "&");
                case '|': return new TokenPair(Symbol::Terminal::OR,     "|");
                case '=': return new TokenPair(Symbol::Terminal::EQ,     "=");
                case '_':
                    unread(currChar);
                    error(readWordChars() + " is not a valid identifier");
                    continue;
                // Two-character symbols
                case ':':
                    nextChar = read();
                    if ((char)nextChar == '=') {
                        return new TokenPair(Symbol::Terminal::ASSIGN, ":=");
                    }
                    unread(nextChar);
                    return new TokenPair(Symbol::Terminal::COLON, ":");
                case '<':
                    nextChar = read();
                    switch ((char)nextChar) {
                        case '>':
                            return new TokenPair(Symbol::Terminal::NEQ, "<>");
                        case '=':
                            return new TokenPair(Symbol::Terminal::LESSEREQ, "<=");
                        default:
                            unread(nextChar);
                            return new TokenPair(Symbol::Terminal::LESSER, "<");
                    }
                case '>':
                    nextChar = read();
                    if ((char)nextChar == '=') {
                        return new TokenPair(Symbol::Terminal::GREATEREQ, ">=");
                    }
                    unread(nextChar);
                    return new TokenPair(Symbol::Terminal::GREATER, ">");
                case '/':
                    nextChar = read();
                    if ((char)nextChar == '*') {
                        int currLineNumber = currLine;
                        std::string comment = "Unclosed comment: /*";
                        currChar = read();
                        if (currChar == -1) {
                            error(comment);
                            return new TokenPair(Symbol::Terminal::EOFF, "");
                        }
                        comment += (char)currChar;
                        nextChar = read();
                        if (nextChar == -1) {
                            error(comment);
                            return new TokenPair(Symbol::Terminal::EOFF, "");
                        }
                        comment += (char)nextChar;
                        while ((char)currChar != '*' || (char)nextChar != '/') {
                            currChar = nextChar;
                            nextChar = read();
                            if (nextChar == -1) {
                                error(comment);
                                return new TokenPair(Symbol::Terminal::EOFF, "");
                            }
                            comment += (char)nextChar;
                        }

                        // Ignore the comment
                        continue;
                    }
                    unread(nextChar);
                    return new TokenPair(Symbol::Terminal::DIV, "/");
                default:
		  error("Invalid symbol " + std::to_string(currChar));
            }
        }
    }
};
