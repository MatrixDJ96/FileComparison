#include <QFileDialog>
#include <QMimeData>
#include "../Headers/MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	gridLayout = new QGridLayout();

	pushButton = new QPushButton(ui->centralWidget);
	pushButton->setText("Start comparision");
	pushButton->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

	gridLayout->addWidget(pushButton, 0, 0, 1, 1);

	nameLabel[0] = new QLabel(ui->centralWidget);
	nameLabel[1] = new QLabel(ui->centralWidget);

	tableWidget[0] = new QTableWidget(ui->centralWidget);
	tableWidget[1] = new QTableWidget(ui->centralWidget);

	progressBar[0] = new QProgressBar(ui->centralWidget);
	progressBar[1] = new QProgressBar(ui->centralWidget);
	progressBar[0]->setValue(0);
	progressBar[1]->setValue(0);

	ui->gridLayout->addLayout(gridLayout, 0, 0, 1, 2);

	ui->gridLayout->addWidget(nameLabel[0], 1, 0, 1, 1);
	ui->gridLayout->addWidget(tableWidget[0], 2, 0, 1, 1);
	ui->gridLayout->addWidget(progressBar[0], 3, 0, 1, 1);

	ui->gridLayout->addWidget(nameLabel[1], 1, 1, 1, 1);
	ui->gridLayout->addWidget(tableWidget[1], 2, 1, 1, 1);
	ui->gridLayout->addWidget(progressBar[1], 3, 1, 1, 1);

	reader[0] = new Reader(0);
	reader[1] = new Reader(1);

	connect(pushButton, SIGNAL(clicked()), this, SLOT(check()));

	connect(reader[0], SIGNAL(progress(MyBlock, int)), this, SLOT(progress(MyBlock, int)));
	connect(reader[1], SIGNAL(progress(MyBlock, int)), this, SLOT(progress(MyBlock, int)));
	connect(reader[0], SIGNAL(done(int)), this, SLOT(done(int)));
	connect(reader[1], SIGNAL(done(int)), this, SLOT(done(int)));

	setAcceptDrops(true);

	// Checking
	checking = false;
}

MainWindow::~MainWindow()
{
	delete ui;
	delete gridLayout;
	delete pushButton;
	delete nameLabel[0];
	delete nameLabel[1];
	delete tableWidget[0];
	delete tableWidget[1];
	delete progressBar[0];
	delete progressBar[1];
	delete reader[0];
	delete reader[1];
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
	if (!checking && event->mimeData()->urls().size() <= 2) {
		event->acceptProposedAction();
	}
}

void MainWindow::dropEvent(QDropEvent *event)
{
	if (!checking) {
		QList<QUrl> urls = event->mimeData()->urls();
		if (urls.size() == 1) {
			for (int i = 0; i < 2; i++) {
				QRect rect = QRect(tableWidget[i]->pos(), tableWidget[i]->size());
				if (rect.contains(event->pos(), true)) {
					openFile(urls[0].path(), i);
				}
			}
		}
		else if (urls.size() == 2) {
			openFile(urls[0].path(), 0);
			openFile(urls[1].path(), 1);
		}
	}
}

void MainWindow::openFile(const QString& name, int num)
{
	if (!checking && (num == 0 || num == 1)) {
		if (reader[num]->openFile(name)) {
			nameLabel[num]->setText(reader[num]->getFileName().c_str());

			cleanTable(reader[num]->getFileSize(), num);

			reader[num]->start(megaByte);
		}
	}
}

void MainWindow::cleanTable(int maximum, int num)
{
	if (num == 0 || num == 1) {
		QStringList header({ "Address", "Size", "Hash" });

		tableWidget[num]->clear();
		tableWidget[num]->setRowCount(0);
		tableWidget[num]->setColumnCount(header.size());
		tableWidget[num]->setHorizontalHeaderLabels(header);
		progressBar[num]->setValue(0);
		progressBar[num]->setMaximum(maximum);


		tableWidget[num]->setAlternatingRowColors(true);
	}
}

void MainWindow::populateCellRow(const MyBlock& block, int num, const QColor& color, int row)
{
	if (num == 0 || num == 1) {
		if (row < 0) {
			row = tableWidget[num]->rowCount();
		}

		tableWidget[num]->insertRow(row);

		QTableWidgetItem* item = new QTableWidgetItem(QString::number(block.address) + " - " + QString::number(block.address + block.size - 1));
		tableWidget[num]->setItem(row, 0, item);

		item = new QTableWidgetItem(QString::number(block.size));
		tableWidget[num]->setItem(row, 1, item);

		item = new QTableWidgetItem(block.hash.c_str());

		if (color != QColor()) {
			item->setBackgroundColor(color);
		}

		tableWidget[num]->setItem(row, 2, item);
	}
}

void MainWindow::progress(const MyBlock& block, int num)
{
	if (num == 0 || num == 1) {
		progressBar[num]->setValue(block.address);
		populateCellRow(block, num);
	}
}

void MainWindow::done(int num)
{
	if (num == 0 || num == 1) {
		progressBar[num]->setValue(progressBar[num]->maximum());
		tableWidget[num]->resizeColumnsToContents();
	}

}

void MainWindow::check()
{
	if (!checking && reader[0]->isFinished() && reader[1]->isFinished()) {
		checking = true;

		//pushButton->setEnabled(false);

		std::vector<MyBlock> blocks[2];
		blocks[0] = reader[0]->getFileBlocks();
		blocks[1] = reader[1]->getFileBlocks();

                uint32_t size[2];
		size[0] = blocks[0].size();
		size[1] = blocks[1].size();

		uint32_t min = std::min(size[0], size[1]);
		uint32_t max = std::max(size[0], size[1]);

		int num = size[0] > size[1] ? 0 : 1;

		cleanTable(num == 0 ? size[0] : size[1], num);
		cleanTable(num != 0 ? size[0] : size[1], num != 0 ? 0 : 1);

		QCoreApplication::processEvents();

		for (uint32_t i = 0; i < min; i++) {
			if (blocks[0][i].hash != blocks[1][i].hash || blocks[0][i].size != blocks[1][i].size) {
				populateCellRow(blocks[0][i], 0, QColor(230, 25, 25));
				populateCellRow(blocks[1][i], 1, QColor(230, 25, 25));

				progressBar[0]->setValue(i);
				progressBar[1]->setValue(i);

				QCoreApplication::processEvents();
			}
		}

		done(num != 0 ? 0 : 1);

		for (uint32_t i = min; i < max; i++) {
			populateCellRow(blocks[num][i], num, QColor(250, 220, 80));
			progressBar[num]->setValue(i);

			QCoreApplication::processEvents();
		}

		done(num);

		checking = false;
	}
}
