#include "FofXYZ.h"



FofXYZ::FofXYZ()
{
}


FofXYZ::~FofXYZ()
{
}

void FofXYZ::removeSpaces()
{
	for (int i = 0, n = m_function.size(); i < n; i++)
	{
		if (m_function[i] == ' ')
		{
			m_function.erase(i, 1);
			--n;
			--i;
		}
	}
}