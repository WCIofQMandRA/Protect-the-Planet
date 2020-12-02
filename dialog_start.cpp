#include "dialog_start.hpp"
#include "ui_dialog_start.h"

dialog_start::dialog_start(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::dialog_start)
{
	ui->setupUi(this);
}

dialog_start::~dialog_start()
{
	delete ui;
}

void dialog_start::on_button_start_clicked()
{
	name=ui->editor_player_name->toPlainText().toStdU32String();
	difficulty=1;//TODO!!!
	//difficulty=static_cast<uint16_t>(ui->diffculty->checkedId());
	done(1);
}

void dialog_start::on_button_exit_clicked()
{
	done(0);
}
