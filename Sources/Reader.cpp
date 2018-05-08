#include "../Headers/Reader.h"

Reader::Reader(int num) : num(num)
{
	moveToThread(&thread);
	connect(&thread, SIGNAL(started()), this, SLOT(scan()));
	connect(&file, SIGNAL(progress(MyBlock)), this, SLOT(send(MyBlock)));
	connect(&thread, SIGNAL(finished()), this, SLOT(done()));
}

Reader::~Reader()
{
	thread.terminate();
	thread.wait();
}

bool Reader::openFile(const QString &name)
{
	if (!thread.isRunning()) {
		std::string localName = name.toLocal8Bit().toStdString();

#ifdef _WIN32
		if (localName[0] == '/') {
			localName = localName.substr(1, localName.size());
		}
#endif

		file.open(localName);
		return file.is_open();
	}

	return false;
}

void Reader::start(const uint32_t& size)
{
	if (!thread.isRunning()) {
		if (file.is_open()) {
			this->size = size;
			thread.start();
		}
	}
}

bool Reader::isFinished() const
{
	return thread.isFinished();
}

std::string Reader::getFileName() const
{
	return this->file.getName();
}

uint32_t Reader::getFileSize() const
{
	return this->file.getSize();
}

std::vector<MyBlock> Reader::getFileBlocks() const
{
	return thread.isFinished() ? file.getBlocks() : std::vector<MyBlock>();
}

void Reader::scan()
{
	file.scan(size);
	thread.quit();
}

void Reader::send(const MyBlock& block)
{
	emit progress(block, num);
}

void Reader::done()
{
	emit done(num);
}
