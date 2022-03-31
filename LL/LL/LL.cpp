#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>

struct Rule
{
    Rule(char term,
        std::vector<char> guidSimbol,
        bool shift,
        bool error,
        int ptrNext,
        bool isNextInStack,
        bool isEnd)
    {
        m_term = term;
        m_guidSimbol = guidSimbol;
        m_shift = shift;
        m_error = error;
        m_ptrNext = ptrNext;
        m_isNextInStack = isNextInStack;
        m_isEnd = isEnd;
    }

    char m_term;
    std::vector<char> m_guidSimbol;
    bool m_shift;
    bool m_error;
    int m_ptrNext;
    bool m_isNextInStack;
    bool m_isEnd;
};

void InitializeArr(std::vector<Rule>& arr)
{
    arr.push_back(Rule('S', { '9', 'a', '-', '(' }, false, true, 1, false, false));
    arr.push_back(Rule('E', { '9', 'a', '-', '(' }, false, true, 3, true, false));
    arr.push_back(Rule('\0', { '\0' }, true, true, -1, false, true));
    arr.push_back(Rule('E', { '9', 'a', '-', '(' }, false, true, 4, false, false));
    arr.push_back(Rule('T', { '9', 'a', '-', '(' }, false, true, 14, true, false));
    arr.push_back(Rule('A', { ')', '+', '\0' }, false, true, 6, false, false));
    arr.push_back(Rule('A', { '+' }, false, false, 11, false, false));
    arr.push_back(Rule('A', { ')' }, false, false, 10, false, false));
    arr.push_back(Rule('A', { '\0' }, false, true, 9, false, false));
    arr.push_back(Rule('\0', { '\0' }, false, true, -1, false, false));
    arr.push_back(Rule(')', { ')' }, false, true, -1, false, false));
    arr.push_back(Rule('+', { '+' }, true, true, 12, false, false));
    arr.push_back(Rule('T', { '9', 'a', '-', '(' }, false, true, 14, true, false));
    arr.push_back(Rule('A', { ')', '+', '\0' }, false, true, 6, false, false));
    arr.push_back(Rule('T', { '9', 'a', '-', '(' }, false, true, 15, false, false));
    arr.push_back(Rule('F', { '9', 'a', '-', '(' }, false, true, 27, true, false));
    arr.push_back(Rule('B', { '*', '+', ')', '\0' }, false, true, 17, false, false));
    arr.push_back(Rule('B', { '*' }, false, false, 24, false, false));
    arr.push_back(Rule('B', { '+' }, false, false, 23, false, false));
    arr.push_back(Rule('B', { ')' }, false, false, 22, false, false));
    arr.push_back(Rule('B', { '\0' }, false, true, 21, false, false));
    arr.push_back(Rule('\0', { '\0' }, false, true, -1, false, false));
    arr.push_back(Rule(')', { ')' }, false, true, -1, false, false));
    arr.push_back(Rule('+', { '+' }, false, true, -1, false, false));
    arr.push_back(Rule('*', { '*' }, true, true, 25, false, false));
    arr.push_back(Rule('F', { '9', 'a', '-', '(' }, false, true, 27, true, false));
    arr.push_back(Rule('B', { '*', '+', ')', '\0' }, false, true, 17, false, false));
    arr.push_back(Rule('F', { '9' }, false, false, 31, false, false));
    arr.push_back(Rule('F', { 'a' }, false, false, 32, false, false));
    arr.push_back(Rule('F', { '-' }, false, false, 33, false, false));
    arr.push_back(Rule('F', { '(' }, false, true, 35, false, false));
    arr.push_back(Rule('9', { '9' }, true, true, -1, false, false));
    arr.push_back(Rule('a', { 'a' }, true, true, -1, false, false));
    arr.push_back(Rule('-', { '-' }, true, true, 34, false, false));
    arr.push_back(Rule('F', { '9', 'a', '-', '(' }, false, true, 27, false, false));
    arr.push_back(Rule('(', { '(' }, true, true, 36, false, false));
    arr.push_back(Rule('E', { '9', 'a', '-', '(' }, false, true, 3, true, false));
    arr.push_back(Rule(')', { ')' }, true, true, -1, false, false));
}

bool CompareLine(std::vector<Rule> const& arr, std::string line)
{
    std::stack<int> nextStep;
    bool isSuccess = true;
    int index = 0;
    int ch = 0;

    while (isSuccess)
    {
        char simbol;
        simbol = (ch >= line.size()) ? '\0' : line[ch];

        auto found = std::find(arr[index].m_guidSimbol.begin(), arr[index].m_guidSimbol.end(), simbol);
        if (arr[index].m_error && found == arr[index].m_guidSimbol.end())
        {
            return false;
        }
        if (!arr[index].m_error && found == arr[index].m_guidSimbol.end())
        {
            ++index;
            continue;
        }
        if (arr[index].m_isNextInStack)
        {
            nextStep.push(index + 1);
        }
        if (arr[index].m_shift)
        {
            ++ch;
            simbol = (ch >= line.size()) ? '\0' : line[ch];
        }
        if (arr[index].m_isEnd)
        {
            break;
        }
        if (arr[index].m_ptrNext != -1)
        {
            index = arr[index].m_ptrNext;
        }
        else
        {
            index = nextStep.top();
            nextStep.pop();
        }
    }

    return isSuccess;
}

int main()
{
    std::vector<Rule> arr;
    InitializeArr(arr);
    std::ifstream input;
    std::string line;
    input.open("input.txt");
    while (std::getline(input, line))
    {
        std::cout << line << ":  is ";
        if (CompareLine(arr, line))
        {
            std::cout << "true\n";
        }
        else
        {
            std::cout << "false\n";
        }
    }

    return 0;
}

