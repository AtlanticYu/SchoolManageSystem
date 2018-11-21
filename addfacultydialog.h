#ifndef ADDFACULTYDIALOG_H
#define ADDFACULTYDIALOG_H

#include <QDialog>
#include <sqlite3.h>
#include <QMessageBox>

namespace Ui {
class AddFacultyDialog;
}

class AddFacultyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddFacultyDialog(QWidget *parent = nullptr);
    ~AddFacultyDialog();
    void InitComboBox();
    void ShowMatchId();

private:
    Ui::AddFacultyDialog *ui;
    QString InitStatus;

signals:
    void SendAddFaculty(QString,QString,QString);
private slots:
    void on_BtnCancel_clicked();
    void on_BtnComfirmAdd_clicked();
    void on_CbxCollegeName_currentIndexChanged(const QString &arg1);
};

#endif // ADDFACULTYDIALOG_H
