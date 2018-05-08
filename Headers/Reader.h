#ifndef READER_H
#define READER_H

#include <QThread>
#include "MyFile.h"

class Reader : public QObject
{
	Q_OBJECT

public:
	Reader(int num);
	~Reader();

	bool openFile(const QString& name);
	void start(const uint32_t& size);

	bool isFinished() const;

	std::string getFileName() const;
	uint32_t getFileSize() const;

	std::vector<MyBlock> getFileBlocks() const;

private:
	MyFile file;
	QThread thread;
	uint32_t size;
	int num;

	private slots:
	void scan();
	void send(const MyBlock& block);
	void done();

signals:
	void progress(const MyBlock& block, int num);
	void done(int num);
};

#endif // READER_H
