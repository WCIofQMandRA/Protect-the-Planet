#include "dialog_newgame.h"
#include "ui_dialog_newgame.h"
#include "save_load.hpp"
#include "dialog_start.hpp"
#include "dialog_help.h"
#include <QPalette>
dialog_newgame::dialog_newgame(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::dialog_newgame)
{
	ui->setupUi(this);
	setFixedSize(width(),height());
#ifdef _WIN32
	QFont font;
	font.setFamily("Microsoft Yahei");
	font.setBold(true);
	setFont(font);
#endif
	if(save_load->get_userlist().empty()) ui->but_back->setDisabled(true);
	else ui->but_back->setEnabled(true);
	clicked_back=false;
	QPalette pal;
	pal.setColor(QPalette::ButtonText,Qt::white);
	ui->but_back->setPalette(pal);
	ui->but_back->setStyleSheet("background-color:rgb(0,0,0)");
	ui->but_sure->setPalette(pal);
	ui->but_sure->setStyleSheet("background-color:rgb(0,0,0)");
	ui->but_help->setPalette(pal);
	ui->but_help->setStyleSheet("background-color:rgb(0,0,0)");
}

dialog_newgame::~dialog_newgame()
{
	delete ui;
}

void dialog_newgame::on_nameedit_textEdited(const QString &)
{
	new_name = ui->nameedit->text().toStdU32String();
}

void dialog_newgame::on_but_sure_clicked()
{
	done(0);
}

void dialog_newgame::on_but_back_clicked()
{
	clicked_back=true;
	done(0);
}

void dialog_newgame::on_but_help_clicked()
{
	dialog_help dia;
	dia.show();
	dia.exec();
}
