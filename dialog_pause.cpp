#include "dialog_pause.hpp"
#include "ui_dialog_pause.h"

dialog_pause::dialog_pause(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::dialog_pause)
{
	ui->setupUi(this);
}

dialog_pause::~dialog_pause()
{
	delete ui;
}

void dialog_pause::on_button_stop_clicked()
{
	done(1);
}

void dialog_pause::on_button_continue_clicked()
{
	done(0);
}
