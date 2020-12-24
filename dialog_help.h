#ifndef DIALOG_HELP_H
#define DIALOG_HELP_H

#include <QDialog>

namespace Ui {
class dialog_help;
}

class dialog_help : public QDialog
{
	Q_OBJECT

public:
	explicit dialog_help(QWidget *parent = nullptr);
	~dialog_help();

private slots:
	void on_but_sure_clicked();

private:
	Ui::dialog_help *ui;
};

#endif // DIALOG_HELP_H
