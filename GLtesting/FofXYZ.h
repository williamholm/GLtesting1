#pragma once
#include <string>
//take in string, read string, compute values for set ranges, make sure that xyz/0 cases don't crash program.
//another class should make the graph


class FofXYZ
{
private:
	std::string m_function;

	void removeSpaces();

public:

	FofXYZ();
	~FofXYZ();
};

