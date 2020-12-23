#ifndef DIALOG_DIFFICULTY_H
#define DIALOG_DIFFICULTY_H

#include <QDialog>

namespace Ui {
class dialog_difficulty;
}

class dialog_difficulty : public QDialog
{
	Q_OBJECT

public:
	explicit dialog_difficulty(uint16_t,QWidget *parent = nullptr);
	~dialog_difficulty();

private:
	Ui::dialog_difficulty *ui;

public:
	uint16_t dif;
	bool game_entered;
private slots:
	void on_but_dif0_clicked();
	void on_but_dif1_clicked();
	void on_but_dif2_clicked();
	void on_but_start_clicked();
	void on_but_back_clicked();
};
#endif // DIALOG_DIFFICULTY_H
