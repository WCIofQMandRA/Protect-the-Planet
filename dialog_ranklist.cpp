//dialog_ranklist: 类dialog_ranklist的实现

//	-* mode: C++		encoding:UTF-8 *-
//	Copyright 2020 张子辰 & 吕航 (GitHub: WCIofQMandRA & LesterLv)
//
//	This file is part of the game 保卫行星
//
//	This game is free software; you can redistribute it and/or modify it
//	under the terms of the GNU Lesser General Public License as published by
//	the Free Software Foundation; either version 3, or (at your option) any
//	later version.
//
//	This game is distributed in the hope that it will be useful, but
//	WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
//	or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
//	License for more details.
//
//	You should have received copies of the GNU Lesser General Public License
//	and the GNU Gerneral Public License along with 保卫行星 .
//	If not, see https://www.gnu.org/licenses/.
#include "dialog_ranklist.h"
#include "ui_dialog_ranklist.h"
#include "save_load.hpp"
#include <QPalette>
dialog_ranklist::dialog_ranklist(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::dialog_ranklist)
{
	ui->setupUi(this);
	setFixedSize(width(),height());
	QPalette pal;
	pal.setColor(QPalette::ButtonText,Qt::white);
	ui->but_back->setPalette(pal);
	ui->but_back->setStyleSheet("background-color:black");
#ifdef _WIN32
	QFont font;
	font.setFamily("Microsoft Yahei");
	font.setBold(true);
	setFont(font);
#endif
	ui->but_normal->setChecked(true);
	ranklist =save_load->get_ranking(1,6);
	//std::cout<<"ppppppppp";
	if(ranklist.size()>=1){
	//std::cout<<"aaaaa";
	std::u32string name_1 = std::get<0>(ranklist[0]);
	uint64_t level_1 = std::get<1>(ranklist[0]);
	uint64_t score_1 = std::get<2>(ranklist[0]);
	ui->name1->setText(QString::fromStdU32String(name_1));
	ui->level1->setText(QString::fromUtf8("%1").arg(level_1));
	ui->score1->setText(QString::fromUtf8("%1").arg(score_1));
	}
	else {
	ui->name1->setText("[Null]");
	ui->level1->setText("[Null]");
	ui->score1->setText("[Null]");
	}
	if(ranklist.size()>=2){
	std::u32string name_2 = std::get<0>(ranklist[1]);
	uint64_t level_2 = std::get<1>(ranklist[1]);
	uint64_t score_2 = std::get<2>(ranklist[1]);
	ui->name2->setText(QString::fromStdU32String(name_2));
	ui->level2->setText(QString::fromUtf8("%1").arg(level_2));
	ui->score2->setText(QString::fromUtf8("%1").arg(score_2));
	}
	else {
	ui->name2->setText("[Null]");
	ui->level2->setText("[Null]");
	ui->score2->setText("[Null]");
	}
	if(ranklist.size()>=3){
	std::u32string name_3 = std::get<0>(ranklist[2]);
	uint64_t level_3 = std::get<1>(ranklist[2]);
	uint64_t score_3 = std::get<2>(ranklist[2]);
	ui->name3->setText(QString::fromStdU32String(name_3));
	ui->level3->setText(QString::fromUtf8("%1").arg(level_3));
	ui->score3->setText(QString::fromUtf8("%1").arg(score_3));
	}
	else {
	ui->name3->setText("[Null]");
	ui->level3->setText("[Null]");
	ui->score3->setText("[Null]");
	}
	if(ranklist.size()>=4){
	std::u32string name_4 = std::get<0>(ranklist[3]);
	uint64_t level_4 = std::get<1>(ranklist[3]);
	uint64_t score_4 = std::get<2>(ranklist[3]);
	ui->name4->setText(QString::fromStdU32String(name_4));
	ui->level4->setText(QString::fromUtf8("%1").arg(level_4));
	ui->score4->setText(QString::fromUtf8("%1").arg(score_4));
	}
	else {
	ui->name4->setText("[Null]");
	ui->level4->setText("[Null]");
	ui->score4->setText("[Null]");
	}
	if(ranklist.size()>=5){
	std::u32string name_5 = std::get<0>(ranklist[4]);
	uint64_t level_5 = std::get<1>(ranklist[4]);
	uint64_t score_5 = std::get<2>(ranklist[4]);
	ui->name5->setText(QString::fromStdU32String(name_5));
	ui->level5->setText(QString::fromUtf8("%1").arg(level_5));
	ui->score5->setText(QString::fromUtf8("%1").arg(score_5));
	}
	else {
	ui->name5->setText("[Null]");
	ui->level5->setText("[Null]");
	ui->score5->setText("[Null]");
	}
	if(ranklist.size()>=6){
	std::u32string name_6 = std::get<0>(ranklist[5]);
	uint64_t level_6 = std::get<1>(ranklist[5]);
	uint64_t score_6 = std::get<2>(ranklist[5]);
	ui->name6->setText(QString::fromStdU32String(name_6));
	ui->level6->setText(QString::fromUtf8("%1").arg(level_6));
	ui->score6->setText(QString::fromUtf8("%1").arg(score_6));
	}
	else {
	ui->name6->setText("[Null]");
	ui->level6->setText("[Null]");
	ui->score6->setText("[Null]");
	}
}
dialog_ranklist::~dialog_ranklist()
{
	delete ui;
}

void dialog_ranklist::on_but_back_clicked()
{
	done(0);
}

void dialog_ranklist::on_but_easy_clicked()
{
	ranklist =save_load->get_ranking(0,6);
	if(ranklist.size()>=1){
	std::u32string name_1 = std::get<0>(ranklist[0]);
	uint64_t level_1 = std::get<1>(ranklist[0]);
	uint64_t score_1 = std::get<2>(ranklist[0]);
	ui->name1->setText(QString::fromStdU32String(name_1));
	ui->level1->setText(QString::fromUtf8("%1").arg(level_1));
	ui->score1->setText(QString::fromUtf8("%1").arg(score_1));
	}
	else {
	ui->name1->setText("[Null]");
	ui->level1->setText("[Null]");
	ui->score1->setText("[Null]");
	}
	if(ranklist.size()>=2){
	std::u32string name_2 = std::get<0>(ranklist[1]);
	uint64_t level_2 = std::get<1>(ranklist[1]);
	uint64_t score_2 = std::get<2>(ranklist[1]);
	ui->name2->setText(QString::fromStdU32String(name_2));
	ui->level2->setText(QString::fromUtf8("%1").arg(level_2));
	ui->score2->setText(QString::fromUtf8("%1").arg(score_2));
	}
	else {
	ui->name2->setText("[Null]");
	ui->level2->setText("[Null]");
	ui->score2->setText("[Null]");
	}
	if(ranklist.size()>=3){
	std::u32string name_3 = std::get<0>(ranklist[2]);
	uint64_t level_3 = std::get<1>(ranklist[2]);
	uint64_t score_3 = std::get<2>(ranklist[2]);
	ui->name3->setText(QString::fromStdU32String(name_3));
	ui->level3->setText(QString::fromUtf8("%1").arg(level_3));
	ui->score3->setText(QString::fromUtf8("%1").arg(score_3));
	}
	else {
	ui->name3->setText("[Null]");
	ui->level3->setText("[Null]");
	ui->score3->setText("[Null]");
	}
	if(ranklist.size()>=4){
	std::u32string name_4 = std::get<0>(ranklist[3]);
	uint64_t level_4 = std::get<1>(ranklist[3]);
	uint64_t score_4 = std::get<2>(ranklist[3]);
	ui->name4->setText(QString::fromStdU32String(name_4));
	ui->level4->setText(QString::fromUtf8("%1").arg(level_4));
	ui->score4->setText(QString::fromUtf8("%1").arg(score_4));
	}
	else {
	ui->name4->setText("[Null]");
	ui->level4->setText("[Null]");
	ui->score4->setText("[Null]");
	}
	if(ranklist.size()>=5){
	std::u32string name_5 = std::get<0>(ranklist[4]);
	uint64_t level_5 = std::get<1>(ranklist[4]);
	uint64_t score_5 = std::get<2>(ranklist[4]);
	ui->name5->setText(QString::fromStdU32String(name_5));
	ui->level5->setText(QString::fromUtf8("%1").arg(level_5));
	ui->score5->setText(QString::fromUtf8("%1").arg(score_5));
	}
	else {
	ui->name5->setText("[Null]");
	ui->level5->setText("[Null]");
	ui->score5->setText("[Null]");
	}
	if(ranklist.size()>=6){
	std::u32string name_6 = std::get<0>(ranklist[5]);
	uint64_t level_6 = std::get<1>(ranklist[5]);
	uint64_t score_6 = std::get<2>(ranklist[5]);
	ui->name6->setText(QString::fromStdU32String(name_6));
	ui->level6->setText(QString::fromUtf8("%1").arg(level_6));
	ui->score6->setText(QString::fromUtf8("%1").arg(score_6));
	}
	else {
	ui->name6->setText("[Null]");
	ui->level6->setText("[Null]");
	ui->score6->setText("[Null]");
	}
}

void dialog_ranklist::on_but_normal_clicked()
{
	ranklist =save_load->get_ranking(1,6);
	if(ranklist.size()>=1){
	std::u32string name_1 = std::get<0>(ranklist[0]);
	uint64_t level_1 = std::get<1>(ranklist[0]);
	uint64_t score_1 = std::get<2>(ranklist[0]);
	ui->name1->setText(QString::fromStdU32String(name_1));
	ui->level1->setText(QString::fromUtf8("%1").arg(level_1));
	ui->score1->setText(QString::fromUtf8("%1").arg(score_1));
	}
	else {
	ui->name1->setText("[Null]");
	ui->level1->setText("[Null]");
	ui->score1->setText("[Null]");
	}
	if(ranklist.size()>=2){
	std::u32string name_2 = std::get<0>(ranklist[1]);
	uint64_t level_2 = std::get<1>(ranklist[1]);
	uint64_t score_2 = std::get<2>(ranklist[1]);
	ui->name2->setText(QString::fromStdU32String(name_2));
	ui->level2->setText(QString::fromUtf8("%1").arg(level_2));
	ui->score2->setText(QString::fromUtf8("%1").arg(score_2));
	}
	else {
	ui->name2->setText("[Null]");
	ui->level2->setText("[Null]");
	ui->score2->setText("[Null]");
	}
	if(ranklist.size()>=3){
	std::u32string name_3 = std::get<0>(ranklist[2]);
	uint64_t level_3 = std::get<1>(ranklist[2]);
	uint64_t score_3 = std::get<2>(ranklist[2]);
	ui->name3->setText(QString::fromStdU32String(name_3));
	ui->level3->setText(QString::fromUtf8("%1").arg(level_3));
	ui->score3->setText(QString::fromUtf8("%1").arg(score_3));
	}
	else {
	ui->name3->setText("[Null]");
	ui->level3->setText("[Null]");
	ui->score3->setText("[Null]");
	}
	if(ranklist.size()>=4){
	std::u32string name_4 = std::get<0>(ranklist[3]);
	uint64_t level_4 = std::get<1>(ranklist[3]);
	uint64_t score_4 = std::get<2>(ranklist[3]);
	ui->name4->setText(QString::fromStdU32String(name_4));
	ui->level4->setText(QString::fromUtf8("%1").arg(level_4));
	ui->score4->setText(QString::fromUtf8("%1").arg(score_4));
	}
	else {
	ui->name4->setText("[Null]");
	ui->level4->setText("[Null]");
	ui->score4->setText("[Null]");
	}
	if(ranklist.size()>=5){
	std::u32string name_5 = std::get<0>(ranklist[4]);
	uint64_t level_5 = std::get<1>(ranklist[4]);
	uint64_t score_5 = std::get<2>(ranklist[4]);
	ui->name5->setText(QString::fromStdU32String(name_5));
	ui->level5->setText(QString::fromUtf8("%1").arg(level_5));
	ui->score5->setText(QString::fromUtf8("%1").arg(score_5));
	}
	else {
	ui->name5->setText("[Null]");
	ui->level5->setText("[Null]");
	ui->score5->setText("[Null]");
	}
	if(ranklist.size()>=6){
	std::u32string name_6 = std::get<0>(ranklist[5]);
	uint64_t level_6 = std::get<1>(ranklist[5]);
	uint64_t score_6 = std::get<2>(ranklist[5]);
	ui->name6->setText(QString::fromStdU32String(name_6));
	ui->level6->setText(QString::fromUtf8("%1").arg(level_6));
	ui->score6->setText(QString::fromUtf8("%1").arg(score_6));
	}
	else {
	ui->name6->setText("[Null]");
	ui->level6->setText("[Null]");
	ui->score6->setText("[Null]");
	}
}

void dialog_ranklist::on_but_hard_clicked()
{
	ranklist =save_load->get_ranking(2,6);
	if(ranklist.size()>=1){
	std::u32string name_1 = std::get<0>(ranklist[0]);
	uint64_t level_1 = std::get<1>(ranklist[0]);
	uint64_t score_1 = std::get<2>(ranklist[0]);
	ui->name1->setText(QString::fromStdU32String(name_1));
	ui->level1->setText(QString::fromUtf8("%1").arg(level_1));
	ui->score1->setText(QString::fromUtf8("%1").arg(score_1));
	}
	else {
	ui->name1->setText("[Null]");
	ui->level1->setText("[Null]");
	ui->score1->setText("[Null]");
	}
	if(ranklist.size()>=2){
	std::u32string name_2 = std::get<0>(ranklist[1]);
	uint64_t level_2 = std::get<1>(ranklist[1]);
	uint64_t score_2 = std::get<2>(ranklist[1]);
	ui->name2->setText(QString::fromStdU32String(name_2));
	ui->level2->setText(QString::fromUtf8("%1").arg(level_2));
	ui->score2->setText(QString::fromUtf8("%1").arg(score_2));
	}
	else {
	ui->name2->setText("[Null]");
	ui->level2->setText("[Null]");
	ui->score2->setText("[Null]");
	}
	if(ranklist.size()>=3){
	std::u32string name_3 = std::get<0>(ranklist[2]);
	uint64_t level_3 = std::get<1>(ranklist[2]);
	uint64_t score_3 = std::get<2>(ranklist[2]);
	ui->name3->setText(QString::fromStdU32String(name_3));
	ui->level3->setText(QString::fromUtf8("%1").arg(level_3));
	ui->score3->setText(QString::fromUtf8("%1").arg(score_3));
	}
	else {
	ui->name3->setText("[Null]");
	ui->level3->setText("[Null]");
	ui->score3->setText("[Null]");
	}
	if(ranklist.size()>=4){
	std::u32string name_4 = std::get<0>(ranklist[3]);
	uint64_t level_4 = std::get<1>(ranklist[3]);
	uint64_t score_4 = std::get<2>(ranklist[3]);
	ui->name4->setText(QString::fromStdU32String(name_4));
	ui->level4->setText(QString::fromUtf8("%1").arg(level_4));
	ui->score4->setText(QString::fromUtf8("%1").arg(score_4));
	}
	else {
	ui->name4->setText("[Null]");
	ui->level4->setText("[Null]");
	ui->score4->setText("[Null]");
	}
	if(ranklist.size()>=5){
	std::u32string name_5 = std::get<0>(ranklist[4]);
	uint64_t level_5 = std::get<1>(ranklist[4]);
	uint64_t score_5 = std::get<2>(ranklist[4]);
	ui->name5->setText(QString::fromStdU32String(name_5));
	ui->level5->setText(QString::fromUtf8("%1").arg(level_5));
	ui->score5->setText(QString::fromUtf8("%1").arg(score_5));
	}
	else {
	ui->name5->setText("[Null]");
	ui->level5->setText("[Null]");
	ui->score5->setText("[Null]");
	}
	if(ranklist.size()>=6){
	std::u32string name_6 = std::get<0>(ranklist[5]);
	uint64_t level_6 = std::get<1>(ranklist[5]);
	uint64_t score_6 = std::get<2>(ranklist[5]);
	ui->name6->setText(QString::fromStdU32String(name_6));
	ui->level6->setText(QString::fromUtf8("%1").arg(level_6));
	ui->score6->setText(QString::fromUtf8("%1").arg(score_6));
	}
	else {
	ui->name6->setText("[Null]");
	ui->level6->setText("[Null]");
	ui->score6->setText("[Null]");
	}
}
