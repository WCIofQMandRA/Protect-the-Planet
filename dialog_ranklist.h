#ifndef DIALOG_RANKLIST_H
#define DIALOG_RANKLIST_H

#include <QDialog>

namespace Ui {
class dialog_ranklist;
}

class dialog_ranklist : public QDialog
{
	Q_OBJECT

public:
	explicit dialog_ranklist(QWidget *parent = nullptr);
	~dialog_ranklist();

private slots:
	void on_but_back_clicked();

	void on_but_easy_clicked();

	void on_but_normal_clicked();

	void on_but_hard_clicked();

private:
	Ui::dialog_ranklist *ui;

public:
	std::vector<std::tuple<std::u32string,uint64_t,uint64_t>> ranklist;
};

#endif // DIALOG_RANKLIST_H
