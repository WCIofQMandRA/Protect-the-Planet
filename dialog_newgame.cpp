#include "dialog_newgame.h"
#include "ui_dialog_newgame.h"
#include "save_load.hpp"
#include "dialog_start.hpp"
dialog_newgame::dialog_newgame(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::dialog_newgame)
{
	ui->setupUi(this);
	if(save_load.get_userlist().empty()) ui->but_back->setDisabled(true);
	else ui->but_back->setEnabled(true);
	clicked_back=false;
}

dialog_newgame::~dialog_newgame()
{
	delete ui;
}

void dialog_newgame::on_nameedit_textEdited(const QString &arg1)
{
	new_name = ui->nameedit->text().toStdU32String();
}

void dialog_newgame::on_but_sure_clicked()
{
	done(0);
}

void dialog_newgame::on_pushButton_clicked()
{
	;//dialog_help.ui is on the todo list
}

void dialog_newgame::on_but_back_clicked()
{
	clicked_back=true;
	done(0);
}
