class Tea
{
private:
	unsigned long c_delta = 0x9e3779b9;
	int c_numCycles = 64;

	Tea();

	void Encrypt(unsigned long* lpData, unsigned long* lpKey);
	void Decrypt(unsigned long* lpData, unsigned long* lpKey);

public:
	Tea(unsigned long* lpData, unsigned long* lpKey, bool encrypt);
};