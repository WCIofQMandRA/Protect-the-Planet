#ifndef NAMES_H
#define NAMES_H

#include <QDialog>

namespace Ui {
class dialog_names;
}

class dialog_names : public QDialog
{
	Q_OBJECT

public:
	explicit dialog_names(QDialog *parent = nullptr);
	~dialog_names();
	std::u32string player_name;

private slots:
	void on_but_1_clicked();

	void on_lineEdit_editingFinished();

//	void on_listWidget_clicked(const QModelIndex &index);

private:
	Ui::dialog_names *ui;
};

#endif // NAMES_H
