/* For decrypting, the sum needs to be initialized to 1697034457. ie. delta * n*/

#include "TEAC.h"
#include <iostream>
using namespace std;

Tea::Tea(unsigned long* lpData, unsigned long* lpKey, bool encrypt)
{
	if (encrypt == true)
	{
		Encrypt(lpData, lpKey);
	}
	else
	{
		Decrypt(lpData, lpKey);
	}
}

void Tea::Encrypt(unsigned long* lpData, unsigned long* lpKey)
{
	unsigned long m_d0 = lpData[0], 
		m_d1 = lpData[1], 
		m_sum = 0;

	for (int i = 0; i < c_numCycles; ++i)
	{
		m_sum += c_delta;

		m_d0 += ((m_d1 << 4) + lpKey[0]) ^ (m_d1 + m_sum) ^ ((m_d1 >> 5) + lpKey[1]);

		m_d1 += ((m_d0 << 4) + lpKey[2]) ^ (m_d0 + m_sum) ^ ((m_d0 >> 5) + lpKey[3]);

		cout << "E" << i << ": sum: " << m_sum << ", d1: " << m_d0 << ", d2: " << m_d1 << "\n";
	}

	lpData[0] = m_d0;
	lpData[1] = m_d1;
}

void Tea::Decrypt(unsigned long* lpData, unsigned long* lpKey)
{
	unsigned long m_d0 = lpData[0],
		m_d1 = lpData[1],
		m_sum = 0;

	m_sum = c_delta * c_numCycles; //m_sum = c_delta * 32 ie. (<< 2^5)


	

	for (int i = 0; i < c_numCycles; ++i)
	{
		
		m_d1 -= ((m_d0 << 4) + lpKey[2]) ^ (m_d0 + m_sum) ^ ((m_d0 >> 5) + lpKey[3]);

		m_d0 -= ((m_d1 << 4) + lpKey[0]) ^ (m_d1 + m_sum) ^ ((m_d1 >> 5) + lpKey[1]);

		m_sum -= c_delta;
		cout << "D" << (i-c_numCycles) << ": sum: " << m_sum << ", d1: " << m_d0 << ", d2: " << m_d1 << "\n";
	}

	lpData[0] = m_d0;
	lpData[1] = m_d1;
}