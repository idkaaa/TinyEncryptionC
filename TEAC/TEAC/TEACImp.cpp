/* For decrypting, the sum needs to be initialized to 1697034457. ie. delta * n*/

#include "TEAC.h"
#include <iostream>
#include <string>
#include <fstream>
#include <bitset>
#include <stdint.h>
using namespace std;

#define BLOCK_SIZE 8


Tea::Tea(unsigned long* lpData, unsigned long* lpKey, bool encrypt, int iterations, unsigned long delta)
{
	c_delta = delta;
	c_numCycles = iterations;
	if (encrypt == true)
	{
		Encrypt(lpData, lpKey);
	}
	else
	{
		Decrypt(lpData, lpKey);
	}
}

Tea::Tea(unsigned long* lpKey, bool encrypt, int iterations, unsigned long delta, std::string fileName)
{

	c_delta = delta;
	c_numCycles = iterations;

	handleFile(lpKey, encrypt, fileName);

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

//		cout << "E" << i << ": sum: " << m_sum << ", d1: " << m_d0 << ", d2: " << m_d1 << "\n";
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
//		cout << "D" << (i-c_numCycles) << ": sum: " << m_sum << ", d1: " << m_d0 << ", d2: " << m_d1 << "\n";
	}

	lpData[0] = m_d0;
	lpData[1] = m_d1;
}


void Tea::verboseEncrypt(unsigned long* lpData, unsigned long* lpKey)
{
	unsigned long m_d0 = lpData[0],
		m_d1 = lpData[1],
		m_sum = 0;

	cout << "Beginning Encyrption..." << endl;

	cout << m_d0 << " = m_d0" << endl;
	cout << m_d1 << " = m_d1" << endl;
	cout << m_sum << " = m_sum" << endl;

	for (int i = 0; i < c_numCycles; ++i)
	{
		unsigned long nullLong = 0;
		unsigned long m_steps[3];

		m_sum += c_delta;


		//m_d0 += ((m_d1 << 4) + lpKey[0]) ^ (m_d1 + m_sum) ^ ((m_d1 >> 5) + lpKey[1]);
		
		fill_n(m_steps, 3, 0); //clear array
		
		m_steps[0] = singleStep(m_d1, lpKey[0],nullLong, 1); //(m_d1 << 4) + lpKey[0])
		m_steps[1] = singleStep(m_d1, nullLong, m_sum, 2); //(m_d1 + m_sum)
		m_steps[2] = singleStep(m_d1, lpKey[1], nullLong, 3); //((m_d1 >> 5) + lpKey[1])

		m_d0 = m_d0 + (m_steps[0] ^ m_steps[1] ^ m_steps[2]);

		

//		m_d1 += ((m_d0 << 4) + lpKey[2]) ^ (m_d0 + m_sum) ^ ((m_d0 >> 5) + lpKey[3]);

		fill_n(m_steps, 3, 0); //clear array
		
		m_steps[0] = singleStep(m_d0, lpKey[2], nullLong, 1); //((m_d0 << 4) + lpKey[2])
		m_steps[1] = singleStep(m_d0, nullLong, m_sum, 2); //(m_d0 + m_sum)
		m_steps[2] = singleStep(m_d0, lpKey[3], nullLong, 3); //((m_d0 >> 5) + lpKey[3])

		m_d1 = m_d1 + (m_steps[0] ^ m_steps[1] ^ m_steps[2]);


		unsigned long tempResults[4] = { m_sum, m_d0, m_d1, i };

		logResults(tempResults);
		
	}

	

	lpData[0] = m_d0;
	lpData[1] = m_d1;
}

void Tea::verboseDecrypt(unsigned long* lpData, unsigned long* lpKey)
{
	unsigned long m_d0 = lpData[0],
		m_d1 = lpData[1],
		m_sum = 0;

	m_sum = c_delta * c_numCycles; //m_sum = c_delta * 32 ie. (<< 2^5)

	cout << "Beginning Decryption..." << endl;

	cout << m_d0 << " = m_d0" << endl;
	cout << m_d1 << " = m_d1" << endl;
	cout << m_sum << " = m_sum" << endl;

	for (int i = 0; i < c_numCycles; ++i)
	{
		unsigned long nullLong = 0;
		unsigned long m_steps[3];


		//m_d1 -= ((m_d0 << 4) + lpKey[2]) ^ (m_d0 + m_sum) ^ ((m_d0 >> 5) + lpKey[3]);

		fill_n(m_steps, 3, 0); //clear array

		m_steps[0] = singleStep(m_d0, lpKey[2], nullLong, 1); //((m_d0 << 4) + lpKey[2])
		m_steps[1] = singleStep(m_d0, nullLong, m_sum, 2); //(m_d0 + m_sum)
		m_steps[2] = singleStep(m_d0, lpKey[3], nullLong, 3); //((m_d0 >> 5) + lpKey[3])

		m_d1 = m_d1 - (m_steps[0] ^ m_steps[1] ^ m_steps[2]);

		
		

		//m_d0 -= ((m_d1 << 4) + lpKey[0]) ^ (m_d1 + m_sum) ^ ((m_d1 >> 5) + lpKey[1]);

		fill_n(m_steps, 3, 0); //clear array

		m_steps[0] = singleStep(m_d1, lpKey[0], nullLong, 1); //(m_d1 << 4) + lpKey[0])
		m_steps[1] = singleStep(m_d1, nullLong, m_sum, 2); //(m_d1 + m_sum)
		m_steps[2] = singleStep(m_d1, lpKey[1], nullLong, 3); //((m_d1 >> 5) + lpKey[1])

		m_d0 = m_d0 - (m_steps[0] ^ m_steps[1] ^ m_steps[2]);



		m_sum -= c_delta;
		


		unsigned long tempResults[4] = { m_sum, m_d0, m_d1, i };

		logResults(tempResults);

	}



	lpData[0] = m_d0;
	lpData[1] = m_d1;
}

unsigned long Tea::singleStep(unsigned long& input, unsigned long& key, unsigned long& sum, int stepNum)
{
	unsigned long output = 0;

	switch (stepNum) {
	case 1:
		output = input << 4;
		output = output + key;
		break;
	case 2:
		output = input + sum;
		break;
	case 3:
		output = input >> 5;
		output = output + key;
	}
	return output;
}

void Tea::printBinary(unsigned long& lpData)
{
	cout << (bitset<32>) lpData;
}

void Tea::logResults(unsigned long* tempResults) 
{
	
	printBinary(tempResults[1]);
	printBinary(tempResults[2]);
	cout << " = Round" << (int)tempResults[3] << endl;
}


void Tea::handleFile(unsigned long* lpKey, bool encrypt, std::string fileName)
{
	fstream file(fileName.c_str(), ios::in | ios::out);

	if (!file)
	{
		cout << "Could not open file";
		return;
	}

	unsigned size;

	file.seekg(0, ios::end);
	size = file.tellg();
	file.seekg(ios::beg);

	file.clear();

	unsigned pos;

	int n_blocks = size / BLOCK_SIZE;
	if (size%BLOCK_SIZE != 0)
		++n_blocks;

	for (int i = 0; i<n_blocks; i++)
	{
		unsigned long data[BLOCK_SIZE];
		pos = file.tellg();


		if (encrypt)
			Encrypt((unsigned long*)data, lpKey);
		else
			Decrypt((unsigned long*)data, lpKey);

		file.seekp(pos);
		file.write((char*)data, BLOCK_SIZE);

		memset(data, 0, BLOCK_SIZE);
	}
	file.close();
}