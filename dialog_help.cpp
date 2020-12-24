#include "dialog_help.h"
#include "ui_dialog_help.h"
#include <QPalette>
dialog_help::dialog_help(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::dialog_help)
{
	ui->setupUi(this);
	setFixedSize(width(),height());
#ifdef _WIN32
	QFont font;
	font.setFamily("Microsoft Yahei");
	font.setBold(true);
	setFont(font);
	QPalette pal_label,pal_but;
	pal_but.setColor(QPalette::ButtonText,Qt::white);
	ui->but_sure->setPalette(pal_but);
	ui->but_sure->setStyleSheet("background-color:black");
	//ui->listWidget->setStyleSheet("background-color:black");
#endif
}

dialog_help::~dialog_help()
{
	delete ui;
}

void dialog_help::on_but_sure_clicked()
{
	done(0);
}
