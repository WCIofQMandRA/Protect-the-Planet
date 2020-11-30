#ifndef DIALOG_PAUSE_HPP
#define DIALOG_PAUSE_HPP

#include <QDialog>

namespace Ui {
class dialog_pause;
}

class dialog_pause : public QDialog
{
	Q_OBJECT

public:
	explicit dialog_pause(QWidget *parent = nullptr);
	~dialog_pause();

private slots:
	void on_button_stop_clicked();
	void on_button_continue_clicked();

private:
	Ui::dialog_pause *ui;
};

#endif // DIALOG_PAUSE_HPP
