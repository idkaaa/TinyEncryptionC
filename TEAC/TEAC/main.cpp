#include "TEAC.h"
#include <iostream>
#include <string>
using namespace std;

void main()
{
	unsigned long m_128BitKey[] = { 1111, 3333, 5555, 7777 };

	unsigned long m_32BitData[] = { 5552, 13234 };

	cout << "Raw: data 1: " << m_32BitData[0] << " data 2: " << m_32BitData[1] << "\n";

	bool bEncrypt = true;

	Tea code = Tea(m_32BitData, m_128BitKey, bEncrypt);

	cout << "Encrypted: data 1: " << m_32BitData[0] << " data 2: " << m_32BitData[1] << "\n";

	bEncrypt = false;
	
	Tea decode = Tea(m_32BitData, m_128BitKey, bEncrypt);

	cout << "Decrypted: data 1: " << m_32BitData[0] << " data 2: " << m_32BitData[1] << "\n";

	getchar();
}