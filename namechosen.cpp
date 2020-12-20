#include "namechosen.h"
#include "ui_namechosen.h"

namechosen::namechosen(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::namechosen)
{
	ui->setupUi(this);
}

namechosen::~namechosen()
{
	delete ui;
}
