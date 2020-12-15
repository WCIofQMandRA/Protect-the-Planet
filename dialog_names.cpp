//用户此前存档名字的选择与新名字的键入
#include "dialog_names.hpp"
#include "ui_dialog_names.h"
#include <QListWidgetItem>
#include <QListWidget>
dialog_names::dialog_names(QDialog *parent) :
	QDialog(parent),
	ui(new Ui::dialog_names)
{
	ui->setupUi(this);
#ifdef _WIN32
	QFont font;
	font.setFamily("Microsoft Yahei");
	setFont(font);
#endif
	QListWidgetItem *nitem1 = new QListWidgetItem(ui->name_list);
	ui->name_list->addItem(nitem1);
	ui->name_list->setItemWidget(nitem1,ui->but_1);
	QListWidgetItem *nitem2 = new QListWidgetItem(ui->name_list);
	ui->name_list->addItem(nitem2);
	ui->name_list->setItemWidget(nitem2,ui->but_2);
	QListWidgetItem *nitem3 = new QListWidgetItem(ui->name_list);
	ui->name_list->addItem(nitem3);
	ui->name_list->setItemWidget(nitem3,ui->but_3);
}

dialog_names::~dialog_names()
{
	delete ui;
}
void dialog_names::on_but_1_clicked()
{
  ;   //todo
}

void dialog_names::on_lineEdit_editingFinished()
{
  ;//player_name =
}

//void names::on_listWidget_clicked(const QModelIndex &index)
//{
//	;
//}
