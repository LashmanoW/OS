#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <regex>
#include <stack>
#include <set>
#include <map>

using Cell = std::map<std::string, std::set<std::string>>;
using Automaton = std::map<std::string, Cell>;

std::string type;
std::regex Regex("[a-zA-Z]+");

Automaton automaton;
Automaton result;
std::set<std::string> exist;

void ReadR(std::ifstream& input)
{
	std::string line;	
	std::smatch rmatch;

	while (std::getline(input, line))
	{
		std::sregex_iterator it =
			std::sregex_iterator(line.begin(), line.end(), Regex), end;
		
		if (it != end)
		{
			std::smatch match = *it;
			std::string from = match.str();
			
			it++;			
			for (int index = 0; it != end; ++it)
			{
				match = *it;
				std::string match_str = match.str();
				if (match_str.size() == 2)
				{
					std::string simbol;
					simbol = match_str[0];
					std::string to;
					to = match_str[1];

					automaton[from][simbol].insert(to);
				}
				else
				{
					std::string simbol;
					simbol = match_str[0];
					std::string to  = "H";

					automaton[from][simbol].insert(to);
				}				
			}
		}
	}
}

void ReadL(std::ifstream& input)
{
	automaton["H"];

	std::string line;
	Cell h_cell;
	while (std::getline(input, line))
	{
		std::sregex_iterator it =
			std::sregex_iterator(line.begin(), line.end(), Regex), end;		

		if (it != end)
		{
			std::smatch match = *it;
			std::string to = match.str();
			std::string from;
			it++;

			for (int index = 0; it != end; ++it)
			{
				match = *it;
				std::string match_str = match.str();
				if (match_str.size() == 2)
				{
					std::string simbol;
					simbol = match_str[1];					
					from = match_str[0];					

					Automaton::iterator a_it = automaton.find(from);
					if (a_it != automaton.end())
					{
						Cell::iterator c_it = (a_it->second).find(simbol);
						Cell::iterator c_end = (a_it->second).end();
						if (c_it != c_end)
						{
							(c_it->second).insert(to);
						}
						else
						{
							automaton[from][simbol].insert(to);
						}
					}
					else
					{	
						automaton[from][simbol].insert(to);
					}					
				}
				else
				{
					std::string simbol;
					simbol = match_str[0];				

					automaton["H"][simbol].insert(to);
				}
			}
		}
	}
}

void Initialization(std::ifstream& input)
{
	if (type == "R")
	{
		ReadR(input);
	}
	else
	{
		ReadL(input);
	}
}

void WriteAutomaton(std::ofstream& output, Automaton::iterator& it)
{
	if (it != result.end())
	{		
		if (exist.find(it->first) == exist.end())
		{
			output << it->first << " -> ";
			exist.insert(it->first);
			size_t count = 1;
			for (auto elem : it->second)
			{
				auto e_it = elem.second.begin();
				std::string to;
				for (auto str : elem.second)
				{
					to += str;
				}

				if (count < (it->second).size())
				{ 
					output << elem.first << to << " | ";
					count++;
				}
				else
				{
					output << elem.first << to << "\n";
				}				
			}

			for (auto elem : it->second)
			{
				auto e_it = elem.second.begin();
				std::string to;
				for (auto str : elem.second)
				{
					to += str;
				}

				Automaton::iterator next_it = result.find(to);
				WriteAutomaton(output, next_it);
			}
		}
	}
}

void WriteDetermization(std::ofstream& output)
{
	Automaton::iterator it;
	if (type == "R")
	{
		it = result.find("S");		
	}
	else
	{
		it = result.find("H");		
	}

	WriteAutomaton(output, it);
}

void Determinization()
{
	Automaton::iterator it;
	if (type == "R")
	{
		it = automaton.find("S");
		result["S"] = it->second;
	}
	else
	{
		it = automaton.find("H");
		result["H"] = it->second;
	}

	std::stack<std::string> nextState;
	
	for (auto elem : it->second)
	{
		std::string to;
		for (auto str : elem.second)
		{
			to += str;
		}

		nextState.push(to);		
	}	

	while (nextState.size() != 0)
	{
		std::string from = nextState.top();
		nextState.pop();

		if (result.find(from) == result.end())
		{
			for (size_t i = 0; i < from.size(); i++)
			{
				std::string ch;
				ch = from[i];

				Automaton::iterator a_it = automaton.find(ch);
				if (a_it != automaton.end())
				{
					auto c_it = a_it->second;

					for (auto i = c_it.begin(); i != c_it.end(); i++)
					{						
						for (auto elem : i->second)
						{
							result[from][i->first].insert(elem);							
						}
					}

					for (auto elem : result[from])
					{
						std::string to;
						for (auto str : elem.second)
						{
							to += str;
						}
												
						nextState.push(to);
					}
				}
			}
		}		
	}
}

int main()
{
	std::ifstream input;
	input.open("input.txt");
	input >> type;
	Initialization(input);

	Determinization();
	
	std::ofstream output;
	output.open("output.txt");
	WriteDetermization(output);
	return 0;
}

