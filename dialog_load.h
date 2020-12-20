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

private:
	Ui::Dialog_load *ui;
};

#endif // DIALOG_LOAD_H
