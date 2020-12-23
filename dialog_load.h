#ifndef DIALOG_LOAD_H
#define DIALOG_LOAD_H

#include <QDialog>

namespace Ui {
class Dialog_load;
}

class Dialog_load : public QDialog
{
	Q_OBJECT

public:
	explicit Dialog_load(QWidget *parent = nullptr);
	~Dialog_load();

private slots:
	void on_but_sure_clicked();

	void on_but_up_clicked();

	void on_but_down_clicked();

	void on_but_1_clicked();

	void on_but_2_clicked();

	void on_but_3_clicked();

	void on_but_4_clicked();

	void on_but_5_clicked();

	void on_but_6_clicked();

	void on_but_back_clicked();

	void on_but_create_clicked();

private:
	Ui::Dialog_load *ui;

public:
	std::u32string name;
	std::vector<std::u32string> name_list;

private:
	int16_t max_down_num;
	int16_t cur_down_num;
	int16_t tot_user_num;
	std::u32string prename;
	QPalette pal_label;
	QPalette pal_but;
};

#endif // DIALOG_LOAD_H
