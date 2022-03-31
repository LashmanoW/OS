#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <sstream>
#include <regex>

using namespace std;

regex keywordsRegex("((if)|(for)|(while)|(const)|(int)|(string)|(char)|(bool)|(false)|(true)|(regex)|(return)|(struct)|(map)|(auto)|(break)|(else)|(smatch))");
regex identifierKeywordRegex("[_A-Za-z]+\\w*");

enum class TokenType
{
    KEYWORDS,
    IDENTIFIER,
    OPERATOR,
    ARITHMETIC_OPERATOR,
    LOGIC_OPERATOR,
    ASSIGMENT_OPERATOR,
    BITWISE_OPERATOR,
    COMPARISON_OPERATOR,
    DELIMITER,
    LITERAL, 
    NUMBER,
    STRING, 
    CHAR,
    UNKNOWN,
};

string GetTokenName(TokenType tokenType)
{
    switch (tokenType)
    {
    case TokenType::KEYWORDS:
        return "Keyword";
    case TokenType::IDENTIFIER:
        return "Identifier";
    case TokenType::OPERATOR:
        return "Operator";
    case TokenType::DELIMITER:
        return "Deliminator";
    case TokenType::LITERAL:
        return "Literal";
    case TokenType::NUMBER:
        return "Number";
    case TokenType::STRING:
        return "String";
    case TokenType::CHAR:
        return "Char";
    case TokenType::ARITHMETIC_OPERATOR:
        return "Arithmetic operator";
    case TokenType::LOGIC_OPERATOR:
        return "Logic operator";
    case TokenType::ASSIGMENT_OPERATOR:
        return "Assigment operator";
    case TokenType::BITWISE_OPERATOR:
        return "Bitwise operator";
    case TokenType::COMPARISON_OPERATOR:
        return "Comparison operator";
    }
    return "Unknown";
}

map<std::string, TokenType> tokensMap = {
    {"if", TokenType::KEYWORDS},
    {"for", TokenType::KEYWORDS},
    {"while", TokenType::KEYWORDS},
    {"const", TokenType::KEYWORDS},
    {"int", TokenType::KEYWORDS},
    {"string", TokenType::KEYWORDS},
    {"char", TokenType::KEYWORDS},
    {"struct", TokenType::KEYWORDS},
    {"bool", TokenType::KEYWORDS},
    {"return", TokenType::KEYWORDS},
    {"regex", TokenType::KEYWORDS},
    {"using", TokenType::KEYWORDS},
    {"namespace", TokenType::KEYWORDS},
    {"auto", TokenType::KEYWORDS},
    {"break", TokenType::KEYWORDS},
    {"else", TokenType::KEYWORDS},
    {"map", TokenType::KEYWORDS},
    {"smatch", TokenType::KEYWORDS},

    {"true", TokenType::KEYWORDS},
    {"false", TokenType::KEYWORDS},

    {"(", TokenType::DELIMITER},
    {")", TokenType::DELIMITER},
    {"[", TokenType::DELIMITER},
    {"]", TokenType::DELIMITER},
    {"{", TokenType::DELIMITER},
    {"}", TokenType::DELIMITER},
    {",", TokenType::DELIMITER},
    {";", TokenType::DELIMITER},

    {".", TokenType::OPERATOR},
    {"=", TokenType::ASSIGMENT_OPERATOR},
    {"+", TokenType::ARITHMETIC_OPERATOR},
    {"-", TokenType::ARITHMETIC_OPERATOR},
    {"*", TokenType::ARITHMETIC_OPERATOR},
    {"/", TokenType::ARITHMETIC_OPERATOR},
    {"%", TokenType::ARITHMETIC_OPERATOR},
    {"!", TokenType::LOGIC_OPERATOR},
    {":", TokenType::OPERATOR},
    {"::", TokenType::OPERATOR},
    {"++", TokenType::ARITHMETIC_OPERATOR},
    {"--", TokenType::ARITHMETIC_OPERATOR},
    {"+=", TokenType::ASSIGMENT_OPERATOR},
    {"-=", TokenType::ASSIGMENT_OPERATOR},
    {"==", TokenType::COMPARISON_OPERATOR},
    {"!=", TokenType::COMPARISON_OPERATOR},
    {"<", TokenType::COMPARISON_OPERATOR},
    {">", TokenType::COMPARISON_OPERATOR},
    {">=", TokenType::COMPARISON_OPERATOR},
    {"<=", TokenType::COMPARISON_OPERATOR},
    {"<<", TokenType::BITWISE_OPERATOR},
    {">>", TokenType::BITWISE_OPERATOR},
    {"&", TokenType::BITWISE_OPERATOR},
    {"|", TokenType::BITWISE_OPERATOR},
    {"&&", TokenType::LOGIC_OPERATOR},
    {"||", TokenType::LOGIC_OPERATOR},
};

bool IsWhiteSpace(string c)
{
    return (c == " ") || (c == "\t") || (c == "\f") || (c == "\v");
}

bool IsEndOfLine(string c)
{
    return (c == "\n") || (c == "\r");;
}

bool IsNumeric(string s)
{
    return (s >= "0") && (s <= "9");
}

// позиция токена
struct Token {
    string token;
    TokenType type;
};

struct Tokenizer {
    string input;
    size_t currentIndex = 0;
    string getOneCurrentSymbol() {
        if (currentIndex >= input.size() - 1)
            return "";
        return string(1, input[currentIndex]);
    };
    string getTwoCurrentSymbols() {
        if (currentIndex >= input.size() - 2)
            return "";
        return string(1, input[currentIndex]) + string(1, input[currentIndex + 1]);
    };
};

void SkipWhiteSpacesAndComments(Tokenizer& tokenizer)
{
    while (IsWhiteSpace(tokenizer.getOneCurrentSymbol()) || IsEndOfLine(tokenizer.getOneCurrentSymbol()))
        tokenizer.currentIndex++;
    if (tokenizer.getTwoCurrentSymbols() == "//")
    {
        while (!IsEndOfLine(tokenizer.getOneCurrentSymbol()))
        {
            tokenizer.currentIndex++;
        }
        tokenizer.currentIndex++;
    }
    else if (tokenizer.getTwoCurrentSymbols() == "/*")
    {
        while (tokenizer.getTwoCurrentSymbols() == "*/")
        {
            tokenizer.currentIndex++;
            if (tokenizer.currentIndex >= tokenizer.input.size())
            {
                throw exception("Invalid input");
            }
        }
        tokenizer.currentIndex += 2;
    }
}

bool MatchKeyword(Tokenizer& tokenizer, Token& token)
{
    smatch match;
    auto endIndex = tokenizer.input.find("\n", tokenizer.currentIndex);
    if (regex_search(tokenizer.input.cbegin() + tokenizer.currentIndex, tokenizer.input.cbegin() + endIndex, match, keywordsRegex))
    {
        if (match.prefix().str().size() != 0)
            return false;
        token.token = match[0].str();
        token.type = tokensMap[token.token];
        tokenizer.currentIndex += token.token.size();
        return true;
    }
    return false;
}

bool MatchIdentifier(Tokenizer& tokenizer, Token& token)
{
    smatch match;
    auto endIndex = tokenizer.input.find("\n", tokenizer.currentIndex);
    if (regex_search(tokenizer.input.cbegin() + tokenizer.currentIndex, tokenizer.input.cbegin() + endIndex, match, identifierKeywordRegex))
    {
        if (match.prefix().str().size() != 0)
            return false;
        token.token = match[0].str();
        token.type = TokenType::IDENTIFIER;
        tokenizer.currentIndex += token.token.size();
        return true;
    }
    return false;
}

Token GetToken(Tokenizer& tokenizer)
{
    Token token;
    SkipWhiteSpacesAndComments(tokenizer);

    if (tokensMap.find(tokenizer.getTwoCurrentSymbols()) != tokensMap.end())
    {
        token.token = tokenizer.getTwoCurrentSymbols();
        token.type = tokensMap[token.token];
        tokenizer.currentIndex += 2;
        return token;
    }

    if (tokensMap.find(tokenizer.getOneCurrentSymbol()) != tokensMap.end())
    {
        token.token = tokenizer.getOneCurrentSymbol();
        token.type = tokensMap[token.token];
        tokenizer.currentIndex++;
        return token;
    }

    if (MatchKeyword(tokenizer, token))
    {
        return token;
    }

    if (tokenizer.getOneCurrentSymbol() == "\"")
    {
        token.type = TokenType::STRING;
        string lastSymbol = tokenizer.getOneCurrentSymbol();
        token.token += lastSymbol;
        tokenizer.currentIndex++;
        while (lastSymbol + tokenizer.getOneCurrentSymbol() == "\\\"" || (tokenizer.getOneCurrentSymbol() != "\"" && tokenizer.getOneCurrentSymbol() != ""))
        {
            lastSymbol = tokenizer.getOneCurrentSymbol();
            token.token += lastSymbol;
            tokenizer.currentIndex++;
        }
        if (tokenizer.getOneCurrentSymbol() == "")
        {
            token.type = TokenType::UNKNOWN;
        }
        tokenizer.currentIndex++;
        token.token += "\"";
        return token;
    }

    if (tokenizer.getOneCurrentSymbol() == "'")
    {
        token.type = TokenType::CHAR;
        token.token += tokenizer.getOneCurrentSymbol();
        tokenizer.currentIndex++;
        token.token += tokenizer.getOneCurrentSymbol();
        if (tokenizer.getOneCurrentSymbol() == "")
        {
            token.type = TokenType::UNKNOWN;
        }
        tokenizer.currentIndex++;
        token.token += "'";
        return token;
    }

    if (IsNumeric(tokenizer.getOneCurrentSymbol()))
    {
        token.type = TokenType::NUMBER;
        token.token += tokenizer.getOneCurrentSymbol();
        tokenizer.currentIndex++;
        while (IsNumeric(tokenizer.getOneCurrentSymbol()))
        {
            // добавить ограничения по размеру
            token.token += tokenizer.getOneCurrentSymbol();
            tokenizer.currentIndex++;
        }
        return token;
    }
    if (MatchIdentifier(tokenizer, token))
    {
        return token;
    }
    token.type = TokenType::UNKNOWN;
    return token;
}

string readFile(string inputFile)
{
    ifstream in;
    in.open(inputFile);
    string line;
    string result;
    while (getline(in, line))
        result += line + '\n';
    return result;
}

int main()
{    
    //string outputFileName = argv[2];    
    string inputLine = readFile("input.txt");
    
    //ofstream output(outputFileName);
    Tokenizer tokenizer;
    tokenizer.input = inputLine;
    while (tokenizer.currentIndex < tokenizer.input.size())
    {
        auto token = GetToken(tokenizer);
        if (token.type == TokenType::UNKNOWN)
            break;
        cout << "[" + token.token + "\t\t" + 
            GetTokenName(token.type) + "]" << endl;
    }
}
