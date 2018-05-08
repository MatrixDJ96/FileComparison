#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDragEnterEvent>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QMainWindow>
#include <QProgressBar>
#include <QTableWidget>
#include "../Headers/Reader.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

protected:
	virtual void dragEnterEvent(QDragEnterEvent *event);
	virtual void dropEvent(QDropEvent *event);

private:
	void openFile(const QString& name, int num);
	void cleanTable(int maximum, int num);
	void populateCellRow(const MyBlock& block, int num, const QColor& color = QColor(), int row = -1);

private:
	Ui::MainWindow *ui;

	QGridLayout *gridLayout;
	QPushButton *pushButton;
	QLabel *nameLabel[2];
	QTableWidget *tableWidget[2];
	QProgressBar *progressBar[2];

	Reader *reader[2];

	bool checking;

private slots:
	void progress(const MyBlock& block, int num);
	void done(int num);
	void check();
};

#endif // MAINWINDOW_H
