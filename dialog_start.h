#ifndef DIALOG_START_H
#define DIALOG_START_H

#include <QDialog>

namespace Ui {
class dialog_start;
}

class dialog_start : public QDialog
{
	Q_OBJECT

public:
	explicit dialog_start(QWidget *parent = nullptr);
	std::u32string name;
	uint16_t difficulty;
	~dialog_start();

private slots:
	void on_button_start_clicked();

	void on_button_exit_clicked();

private:
	Ui::dialog_start *ui;
};

#endif // DIALOG_START_H
