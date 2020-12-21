#ifndef DIALOG_NEWGAME_H
#define DIALOG_NEWGAME_H

#include <QDialog>

namespace Ui {
class dialog_newgame;
}

class dialog_newgame : public QDialog
{
	Q_OBJECT

public:
	explicit dialog_newgame(QWidget *parent = nullptr);
	~dialog_newgame();

private:
	Ui::dialog_newgame *ui;
public:
	std::u32string new_name;
	int new_dif;
private slots:
	void on_nameedit_textEdited(const QString &arg1);
	void on_dif_1_clicked();
	void on_dif_2_clicked();
	void on_dif_3_clicked();
	void on_but_sure_clicked();
};
#endif // DIALOG_NEWGAME_H