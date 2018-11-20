#ifndef ADDCOLLEGEDIALOG_H
#define ADDCOLLEGEDIALOG_H

#include <QDialog>

namespace Ui {
class AddCollegeDialog;
}

class AddCollegeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddCollegeDialog(QWidget *parent = nullptr);
    ~AddCollegeDialog();

private slots:
    void on_BtnCancel_clicked();

    void on_BtnConfirmAdd_clicked();

private:
    Ui::AddCollegeDialog *ui;

signals:
    void SendAddCollege(QString);
};

#endif // ADDCOLLEGEDIALOG_H
