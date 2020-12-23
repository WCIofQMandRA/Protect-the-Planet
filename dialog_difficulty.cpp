#include "dialog_difficulty.h"
#include "ui_dialog_difficulty.h"
#include "save_load.hpp"
#include "dialog_start.hpp"
dialog_difficulty::dialog_difficulty(uint16_t dif_,QWidget *parent) :
	QDialog(parent),
	ui(new Ui::dialog_difficulty)
{
	ui->setupUi(this);
	game_entered=false;
	if(save_load.get_userlist().empty()){
		ui->but_dif0->setChecked(true);
	}else{
	   dif = dif_;
	   switch (dif_)
	   {
	   case 0:ui->but_dif0->setChecked(true);break;
	   case 1:ui->but_dif1->setChecked(true);break;
	   case 2:ui->but_dif2->setChecked(true);break;
	   }
	}
}

dialog_difficulty::~dialog_difficulty()
{
	delete ui;
}

void dialog_difficulty::on_but_dif0_clicked()
{
	dif=0;
}

void dialog_difficulty::on_but_dif1_clicked()
{
	dif=1;
}

void dialog_difficulty::on_but_dif2_clicked()
{
	dif=2;
}

void dialog_difficulty::on_but_start_clicked()
{
	game_entered=true;
	done(0);
}

void dialog_difficulty::on_but_back_clicked()
{
	done(0);
}
