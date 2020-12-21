#include "dialog_newgame.h"
#include "ui_dialog_newgame.h"

dialog_newgame::dialog_newgame(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::dialog_newgame)
{
	ui->setupUi(this);
	ui->dif_1->setChecked(true);
	new_dif=0;
}

dialog_newgame::~dialog_newgame()
{
	delete ui;
}

void dialog_newgame::on_nameedit_textEdited(const QString &arg1)
{
	new_name = ui->nameedit->text().toStdU32String();
}


void dialog_newgame::on_dif_1_clicked()
{
	new_dif=0;
}

void dialog_newgame::on_dif_2_clicked()
{
	new_dif=1;
}

void dialog_newgame::on_dif_3_clicked()
{
	new_dif=2;
}

void dialog_newgame::on_but_sure_clicked()
{
	done(0);
}
