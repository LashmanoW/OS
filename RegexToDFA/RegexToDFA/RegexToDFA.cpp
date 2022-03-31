#include <sstream> 
#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <fstream>
#include <tuple>
#include <set>
#include <queue>

using namespace std;

const char PLUS_SYMBOL = '+';
const char STAR_SYMBOl = '*';
const char OR_SYMBOL = '|';
const char OPEN_BRACKET_SYMBOL = '(';
const char CLOSE_BRACKET_SYMBOL = ')';
const char EMPTY_SIGNAL = '&';

struct cell 
{
	int fromState = 0;
	int toState = 0;
	char signal = 0;	
};

struct transitionMatrix 
{
	int startState = 0;
	int endState = 0;	
	vector<cell> matrix;
};

int stateCounter;

transitionMatrix buildNFA(string inputRegex, int& currentIndex, transitionMatrix prevBlock, bool& orFound)
{
	transitionMatrix resultMatrix;
	resultMatrix.startState = stateCounter;
	while (currentIndex < inputRegex.size())
	{
		if (inputRegex[currentIndex] == OPEN_BRACKET_SYMBOL)
		{
			currentIndex++;
			prevBlock = buildNFA(inputRegex, currentIndex, prevBlock, orFound);
			resultMatrix.endState = prevBlock.endState;
			if (orFound)
				resultMatrix.startState = prevBlock.startState;
			for (auto cell : prevBlock.matrix)
			{
				resultMatrix.matrix.push_back(cell);
			}
		}
		else if (inputRegex[currentIndex] == CLOSE_BRACKET_SYMBOL)
		{
			currentIndex++;
			return resultMatrix;
		}
		else if (inputRegex[currentIndex] == OR_SYMBOL)
		{
			currentIndex++;
			orFound = true;
			cell newCell;			
			resultMatrix.startState = ++stateCounter;
			newCell.fromState = stateCounter;
			newCell.signal = EMPTY_SIGNAL;
			newCell.toState = prevBlock.startState;
			resultMatrix.matrix.push_back(newCell);

			cell newCell2;
			newCell2.fromState = stateCounter;
			newCell2.signal = EMPTY_SIGNAL;
			newCell2.toState = ++stateCounter;			
			resultMatrix.matrix.push_back(newCell2);
			prevBlock = resultMatrix;
		}
		else if (inputRegex[currentIndex] == STAR_SYMBOl)
		{
			currentIndex++;			
			cell newCell2;
			newCell2.fromState = prevBlock.startState;
			newCell2.signal = EMPTY_SIGNAL;
			newCell2.toState = stateCounter;
			resultMatrix.matrix.push_back(newCell2);

			cell newCell;
			newCell.fromState = prevBlock.endState;
			newCell.signal = EMPTY_SIGNAL;
			newCell.toState = prevBlock.startState;
			resultMatrix.matrix.push_back(newCell);
			
			resultMatrix.endState = prevBlock.endState;
		}
		else if (inputRegex[currentIndex] == PLUS_SYMBOL)
		{
			currentIndex++;
			cell newCell;
			newCell.fromState = prevBlock.endState;
			newCell.signal = EMPTY_SIGNAL;
			newCell.toState = prevBlock.startState;
			resultMatrix.matrix.push_back(newCell);
			resultMatrix.endState = prevBlock.endState;
		}
		else
		{
			cell newCell;
			transitionMatrix prevBlock2;
			prevBlock2.startState = stateCounter;
			newCell.fromState = stateCounter++;
			newCell.signal = inputRegex[currentIndex++];
			newCell.toState = stateCounter;
			resultMatrix.matrix.push_back(newCell);
			prevBlock2.matrix.push_back(newCell);
			resultMatrix.endState = stateCounter;
			prevBlock2.endState = stateCounter;
			prevBlock = prevBlock2;
		}
	}

	return resultMatrix;
}

void printDFA(transitionMatrix matrix)
{
	cout << "start state: " << matrix.startState << endl;
	cout << "end state: " << matrix.endState << endl;
	sort(matrix.matrix.begin(), matrix.matrix.end(), [](cell a, cell b) { return a.fromState < b.fromState; });
	for (auto& cell : matrix.matrix)
	{
		cout << cell.fromState << " -> " << cell.signal << cell.toState << endl;
	}
}

int main()
{
	string input;
	getline(cin, input);
	int i = 0;
	transitionMatrix prevBlock;
	bool orFound = false;
	auto matrix = buildNFA(input, i, prevBlock, orFound);
	printDFA(matrix);
}