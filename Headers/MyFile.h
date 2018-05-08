#ifndef MYFILE_H
#define MYFILE_H

#include <QObject>
#include <fstream>
#include <vector>
#include "MyBlock.h"

static const uint32_t megaByte = 1048576U;

class MyFile : public QObject
{
	Q_OBJECT

public:
	MyFile();
	~MyFile();

	void open(const std::string& name);
	bool is_open() const;

	void scan(uint32_t size);
	
	const std::string& getName() const;
	const uint32_t& getSize() const;
	const std::vector<MyBlock>& getBlocks() const;

private:
	std::ifstream stream;
	std::string name;
	uint32_t size;
	
	std::vector<MyBlock> blocks;

//private slots:

signals:
	void progress(const MyBlock& block);
};

#endif // MYFILE_H
