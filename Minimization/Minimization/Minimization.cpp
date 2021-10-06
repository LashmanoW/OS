#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <map>

struct Cell
{
    int multiplicity;
    int to;
    int out;
};

using Automaton = std::vector<std::vector<Cell>>;
using EMatrix = std::map<int, std::vector<int>>;
Automaton ResultA;
std::vector<int> ResultI;

void GetEquivalenceMatrix(Automaton& automaton, EMatrix& equivalenceMatrix)
{
    int k = 0;
    equivalenceMatrix[k].push_back(0);

    for (int i = 1; i < automaton[0].size(); i++)
    {
        bool isFound = false;
        EMatrix::iterator it = equivalenceMatrix.begin();

        while (it != equivalenceMatrix.end() && !isFound)
        {
            isFound = true;
            for (int j = 0; j < automaton.size(); j++)
            {
                if (automaton[j][it->second[0]].multiplicity != automaton[j][i].multiplicity)
                {
                    isFound = false;
                    it++;
                    break;
                }
            }
        }

        if (isFound)
        {
            equivalenceMatrix[it->first].push_back(i);
        }
        else
        {
            k++;
            equivalenceMatrix[k].push_back(i);
        }
    }
}

void OverrideAutomaton(Automaton& automaton, EMatrix& equivalenceMatrix)
{
    for (int i = 0; i < automaton.size(); i++)
    {
        for (int j = 0; j < automaton[0].size(); j++)
        {
            EMatrix::iterator it = equivalenceMatrix.begin();

            while (std::find((it->second).begin(), (it->second).end(), automaton[i][j].to) == (it->second).end())
            {
                it++;
            }
            automaton[i][j].multiplicity = it->first;
        }
    }
}

void GetResult(Automaton& automaton, EMatrix& ResultM)
{
    Automaton result(automaton.size(), std::vector<Cell>(ResultM.size()));
    EMatrix::iterator it = ResultM.begin();
    
    for (int i = 0; i < result[0].size(); i++)
    {        
        for (int j = 0; j < result.size(); j++)
        {
            result[j][i] = automaton[j][it->second[0]];  

            EMatrix::iterator n = ResultM.begin();
            while (std::find((n->second).begin(), (n->second).end(), automaton[j][i].to) == (n->second).end())
            {
                n++;
            }
            result[j][i].to = n->second[0];
        }
        ResultI.push_back(it->second[0]);
        it++;        
    }

    ResultA = result;
}

void Minimization(Automaton& automaton, bool& isMinimized, int& N)
{
    EMatrix equivalenceMatrix;
    GetEquivalenceMatrix(automaton, equivalenceMatrix);
    OverrideAutomaton(automaton, equivalenceMatrix);
    if (equivalenceMatrix.size() == N)
    {
        isMinimized = true;
        GetResult(automaton, equivalenceMatrix);
    }

    N = equivalenceMatrix.size();
}

void ReadMilli(Automaton& a, std::ifstream& input)
{
    std::string lineS;
    std::string lineY;
    std::getline(input, lineS);

    for (int i = 0; i < a.size(); i++)
    {
        std::getline(input, lineS);
        for (int k = 0; k < lineS.length(); ++k)
        {
            if (isalpha(lineS[k]))
            {
                lineS[k] = ' ';
            }
        }
        std::getline(input, lineY);
        for (int k = 0; k < lineY.length(); ++k)
        {
            if (isalpha(lineY[k]))
            {
                lineY[k] = ' ';
            }
        }

        std::cout << lineS << std::endl;

        std::stringstream strS(lineS);
        std::stringstream strY(lineY);

        int ar = a[0].size();
        for (int j = 0; j < a[0].size(); j++)
        {
            int numS;
            strS >> numS;
            a[i][j].to = numS - 1;

            int numY;
            strY >> numY;
            a[i][j].multiplicity = numY - 1;
            a[i][j].out = numY - 1;
        }
    }
}

void ReadMurra(Automaton& a, std::ifstream& input)
{
    std::string lineS;
    std::string lineY;
    std::getline(input, lineS);

    std::getline(input, lineY);
    for (int k = 0; k < lineY.length(); ++k)
    {
        if (isalpha(lineY[k]))
        {
            lineY[k] = ' ';
        }
    }
    std::stringstream strY(lineY);

    for (int i = 0; i < a[0].size(); i++)
    {
        int numY;
        strY >> numY;

        for (int j = 0; j < a.size(); j++)
        {
            a[j][i].multiplicity = numY - 1;
            a[j][i].out = numY - 1;
        }
    }

    for (int i = 0; i < a.size(); i++)
    {
        std::getline(input, lineS);
        for (int k = 0; k < lineS.length(); ++k)
        {
            if (isalpha(lineS[k]))
            {
                lineS[k] = ' ';
            }
        }
        std::stringstream strS(lineS);

        for (int j = 0; j < a[0].size(); j++)
        {
            int numS;
            strS >> numS;
            a[i][j].to = numS - 1;
        }
    }
}

void ReadAutomaton(Automaton& a, std::ifstream& input, std::string& type)
{   
    if (type == "Ml")
    {
        ReadMilli(a, input);
    }
    else
    {
        ReadMurra(a, input);
    }
}

void WriteMilli()
{
    std::ofstream output;
    output.open("output.txt");

    for (int i = 0; i < ResultI.size(); i++)
    {
        std::string out = "S" + std::to_string(ResultI[i] + 1);
        output << std::setw(20) << out;
    }
    output << std::endl;

    for (int i = 0; i < ResultA.size(); i++)
    {
        output << "X" << i + 1;
        for (int j = 0; j < ResultA[0].size(); j++)
        {
            std::string out = "[S" + std::to_string(ResultA[i][j].to + 1) + ", Y" + std::to_string(ResultA[i][j].out + 1) + "]";
            output << std::setw(20) << std::right << out;
        }
        output << std::endl;
    }
}

void WriteMurra()
{
    std::ofstream output;
    output.open("output.txt");

    for (int i = 0; i < ResultI.size(); i++)
    {
        std::string out = "Y" + std::to_string(ResultA[0][i].out + 1);
        output << std::setw(20) << out;
    }
    output << std::endl;

    for (int i = 0; i < ResultI.size(); i++)
    {
        std::string out = "S" + std::to_string(ResultI[i] + 1);
        output << std::setw(20) << out;
    }
    output << std::endl;

    for (int i = 0; i < ResultA.size(); i++)
    {
        output << "X" << i + 1;
        for (int j = 0; j < ResultA[0].size(); j++)
        {
            std::string out = "[S" + std::to_string(ResultA[i][j].to + 1) + "]";
            output << std::setw(20) << std::right << out;
        }
        output << std::endl;
    }
}

void WriteAutomaton(std::string& type)
{
    
    if (type == "Ml")
    {                
        WriteMilli();
    }
    else
    {
        WriteMurra();
    }
}


int main()
{
    std::ifstream input;
    input.open("input.txt");
    
    std::string type;
    
    std::getline(input, type);
    int x, y, r;
    input >> y >> x >> r;
    
    Automaton automaton(x, std::vector<Cell>(y));
    ReadAutomaton(automaton, input, type);

    bool isMinimized = false;
    int N = automaton[0].size();

    while (!isMinimized)
    {
        Minimization(automaton, isMinimized, N);
    }

    WriteAutomaton(type);
}
