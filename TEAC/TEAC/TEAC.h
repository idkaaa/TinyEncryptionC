#ifndef TEA_H
#define TEA_H

#include <string>

class Tea
{
private:
	unsigned long c_delta = 0;
	int c_numCycles = 0;

	Tea();

	void Encrypt(unsigned long* lpData, unsigned long* lpKey);
	void Decrypt(unsigned long* lpData, unsigned long* lpKey);
	void verboseEncrypt(unsigned long* lpData, unsigned long* lpKey);
	void verboseDecrypt(unsigned long* lpData, unsigned long* lpKey);
	unsigned long singleStep(unsigned long& input, unsigned long& key, unsigned long& sum, int stepNum);
	void printBinary(unsigned long& lpData);
	void logResults(unsigned long* output);
	void handleFile(unsigned long* lpKey, bool encrypt, std::string fileName);

public:
	Tea(unsigned long* lpData, unsigned long* lpKey, bool encrypt, int iterations, unsigned long delta);
	Tea(unsigned long* lpKey, bool encrypt, int iterations, unsigned long delta, std::string fileName);
};

#endif