#include "TEAC.h"
#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
#include <time.h>
#include <chrono>
using namespace std;



void main()
{
	int iterations = 500;
	unsigned long delta = 0x9e3779b9;


	std::string fileName = "sandbox.binary";
	
	unsigned long m_128BitKey[] = { 0x30bcf5f82684f19c, 0xa7f9bc424fd3eb9 };

	auto t1 = chrono::high_resolution_clock::now();

	bool bEncrypt = true;

	Tea code = Tea(m_128BitKey, bEncrypt, iterations, delta, fileName);


	Tea decode = Tea(m_128BitKey, bEncrypt, iterations, delta, fileName);
	



	//unsigned long m_64BitData[] = { 123456, 78910 };

	//cout << "Raw: data 1: " << m_64BitData[0] << " data 2: " << m_64BitData[1] << "\n";

	//bool bEncrypt = true;

	//Tea code = Tea(m_64BitData, m_128BitKey, bEncrypt, iterations, delta);

	//cout << "Encrypted: data 1: " << m_64BitData[0] << " data 2: " << m_64BitData[1] << "\n";

	//bEncrypt = false;

	//Tea decode = Tea(m_64BitData, m_128BitKey, bEncrypt, iterations, delta);

	//cout << "Decrypted: data 1: " << m_64BitData[0] << " data 2: " << m_64BitData[1] << "\n";

	

	auto t2 = std::chrono::high_resolution_clock::now();
	std::cout << "test function took "
		<< std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
		<< " milliseconds\n";

	ofstream myfile;
	myfile.open("log.txt", ios::app);
	myfile << iterations << " rounds took " << 
		chrono::duration_cast<chrono::milliseconds>(t2 - t1).count()
		<< " milliseconds using default implementation\n";
	myfile.close();


}

