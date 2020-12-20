#include "dialog_load.h"
#include "ui_dialog_load.h"

Dialog_load::Dialog_load(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::Dialog_load)
{
	ui->setupUi(this);
}

Dialog_load::~Dialog_load()
{
	delete ui;
}
