#ifndef MYBLOCK_H
#define MYBLOCK_H

#include <iostream>

class MyBlock
{
public:
	MyBlock();
	MyBlock(const uint32_t& address, const uint32_t& size, const std::string& hash);
	MyBlock(const MyBlock& block);

	~MyBlock();

public:
	const uint32_t address;
	const uint32_t size;
	const std::string hash;
};


#endif // MYBLOCK_H
