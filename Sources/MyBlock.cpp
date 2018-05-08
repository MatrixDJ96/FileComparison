#include <QDebug>
#include "../Headers/MyBlock.h"

MyBlock::MyBlock() : address(0), size(0), hash("")
{
	//qDebug() << "COSTRUTTORE DI DEFAULT";
}

MyBlock::MyBlock(const uint32_t& address, const uint32_t& size, const std::string& hash) : address(address), size(size), hash(hash)
{
	//qDebug() << "COSTRUTTORE CON PARAMETRI";
}

MyBlock::MyBlock(const MyBlock& block) : address(block.address), size(block.size), hash(block.hash)
{
	//qDebug() << "COSTRUTTORE DI COPIA";
}

MyBlock::~MyBlock()
{
	//qDebug() << "DISTRUTTORE";
}
