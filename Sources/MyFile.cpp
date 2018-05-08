#include <QCryptographicHash>
#include "../Headers/MyFile.h"

MyFile::MyFile() : name(""), size(0)
{}

MyFile::~MyFile()
{
	if (this->stream.is_open()) {
		this->stream.close();
	}
}

void MyFile::open(const std::string& name)
{
	if (this->stream.is_open()) {
		this->stream.close();
	}
	this->stream.clear();

	this->stream.open(name, std::ios::binary);
	if (this->stream.is_open()) {
		this->name = name;
		this->stream.seekg(0, std::ios::end);
		this->size = this->stream.tellg();
		this->stream.seekg(0, std::ios::beg);
	}
	else {
		this->name = "";
		this->size = 0;
	}
	this->blocks.clear();
}

bool MyFile::is_open() const
{
	return this->stream.is_open();
}

void MyFile::scan(uint32_t size)
{
	QByteArray bytes;
	char *content = NULL;

	uint32_t address = stream.tellg();

	while (!this->stream.eof()) {
		address = stream.tellg();

		if (address == this->size) {
			break;
		}

		if (size + address > this->size) {
			size = this->size - address;
			if (content != NULL) {
				delete[] content;
				content = NULL;
			}
		}

		if (content == NULL) {
			content = new char[size];
		}

		this->stream.read(content, size);
		bytes = QByteArray::fromRawData(content, size);
		QByteArray hash = QCryptographicHash::hash(bytes, QCryptographicHash::Keccak_256).toHex();

		blocks.push_back(MyBlock(address, size, hash.toStdString()));
		emit progress(blocks[blocks.size() - 1]);
	}

	delete[] content;
}

const std::string& MyFile::getName() const
{
	return this->name;
}

const uint32_t& MyFile::getSize() const
{
	return this->size;
}

const std::vector<MyBlock>& MyFile::getBlocks() const
{
	return this->blocks;
}
