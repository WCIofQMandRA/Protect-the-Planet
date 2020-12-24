#include "dialog_load.h"
#include "ui_dialog_load.h"
#include "save_load.hpp"
#include "dialog_newgame.h"
#include "dialog_start.hpp"
#include <QPalette>
Dialog_load::Dialog_load(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::Dialog_load)
{
	ui->setupUi(this);
	setFixedSize(width(),height());
#ifdef _WIN32
	QFont font;
	font.setFamily("Microsoft Yahei");
	font.setBold(true);
	setFont(font);
#endif
	pal_but.setColor(QPalette::ButtonText,Qt::white);
	/*ui->but_1->setPalette(pal_but);
	ui->but_2->setPalette(pal_but);
	ui->but_3->setPalette(pal_but);
	ui->but_4->setPalette(pal_but);
	ui->but_5->setPalette(pal_but);
	ui->but_6->setPalette(pal_but);*/
	//ui->but_changeuser->setStyleSheet("background-color:rgb(0,0,0)");
	ui->but_back->setPalette(pal_but);ui->but_back->setStyleSheet("background-color:rgb(0,0,0)");
	ui->but_sure->setPalette(pal_but);ui->but_sure->setStyleSheet("background-color:rgb(0,0,0)");
	ui->but_create->setPalette(pal_but);ui->but_create->setStyleSheet("background-color:rgb(0,0,0)");
	ui->but_1->setText("[No player]");ui->but_1->setDisabled(true);
	ui->but_2->setText("[No player]");ui->but_2->setDisabled(true);
	ui->but_3->setText("[No player]");ui->but_3->setDisabled(true);
	ui->but_4->setText("[No player]");ui->but_4->setDisabled(true);
	ui->but_5->setText("[No player]");ui->but_5->setDisabled(true);
	ui->but_6->setText("[No player]");ui->but_6->setDisabled(true);
	name_list = save_load->get_userlist();
	tot_user_num= name_list.size();
	if(tot_user_num>=6) ui->but_6->setText(QString::fromStdU32String(name_list[5])),ui->but_6->setEnabled(true);
	if(tot_user_num>=5) ui->but_5->setText(QString::fromStdU32String(name_list[4])),ui->but_5->setEnabled(true);
	if(tot_user_num>=4) ui->but_4->setText(QString::fromStdU32String(name_list[3])),ui->but_4->setEnabled(true);
	if(tot_user_num>=3) ui->but_3->setText(QString::fromStdU32String(name_list[2])),ui->but_3->setEnabled(true);
	if(tot_user_num>=2) ui->but_2->setText(QString::fromStdU32String(name_list[1])),ui->but_2->setEnabled(true);
	if(tot_user_num>=1) ui->but_1->setText(QString::fromStdU32String(name_list[0])),ui->but_1->setEnabled(true);
	ui->but_1->setChecked(true); //默认选中了第一个
	name = name_list[0];
	prename=name;
	max_down_num = tot_user_num - 6;   //最多可向下移动的次数
	if(max_down_num<0) max_down_num=0;
	cur_down_num = 0;         //当前向下移动的次数
	ui->but_up->setEnabled(false);
	if(!max_down_num) ui->but_down->setEnabled(false);
}

Dialog_load::~Dialog_load()
{
	delete ui;
}

void Dialog_load::on_but_sure_clicked()
{
	done(0);
}

void Dialog_load::on_but_up_clicked()
{
	--cur_down_num;
	ui->but_down->setEnabled(true);
	if(!cur_down_num) ui->but_up->setEnabled(false);
	ui->but_6->setText(QString::fromStdU32String(name_list[5+cur_down_num]));
	ui->but_5->setText(QString::fromStdU32String(name_list[4+cur_down_num]));
	ui->but_4->setText(QString::fromStdU32String(name_list[3+cur_down_num]));
	ui->but_3->setText(QString::fromStdU32String(name_list[2+cur_down_num]));
	ui->but_2->setText(QString::fromStdU32String(name_list[1+cur_down_num]));
	ui->but_1->setText(QString::fromStdU32String(name_list[0+cur_down_num]));
}

void Dialog_load::on_but_down_clicked()
{
	++cur_down_num;
	ui->but_up->setEnabled(true);
	if(cur_down_num>=max_down_num) ui->but_down->setEnabled(false);
	ui->but_6->setText(QString::fromStdU32String(name_list[5+cur_down_num]));
	ui->but_5->setText(QString::fromStdU32String(name_list[4+cur_down_num]));
	ui->but_4->setText(QString::fromStdU32String(name_list[3+cur_down_num]));
	ui->but_3->setText(QString::fromStdU32String(name_list[2+cur_down_num]));
	ui->but_2->setText(QString::fromStdU32String(name_list[1+cur_down_num]));
	ui->but_1->setText(QString::fromStdU32String(name_list[0+cur_down_num]));
}

void Dialog_load::on_but_1_clicked()
{
	name = name_list[0+cur_down_num];
}

void Dialog_load::on_but_2_clicked()
{
	name = name_list[1+cur_down_num];
}


void Dialog_load::on_but_3_clicked()
{
	name = name_list[2+cur_down_num];
}

void Dialog_load::on_but_4_clicked()
{
	name = name_list[3+cur_down_num];
}

void Dialog_load::on_but_5_clicked()
{
	name = name_list[4+cur_down_num];
}

void Dialog_load::on_but_6_clicked()
{
	name = name_list[5+cur_down_num];
}

void Dialog_load::on_but_back_clicked()
{
	name = prename;
	done(0);
}

void Dialog_load::on_but_create_clicked()
{
	dialog_newgame dia;
	dia.setWindowTitle("New Player");
	dia.show();
	dia.exec();
	if(dia.clicked_back==false){
		name = dia.new_name;
		save_load->add_user(name);
		done(0);
	}
}
