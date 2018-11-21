#ifndef MODIFYFACULTYDIALOG_H
#define MODIFYFACULTYDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QMessageBox>
#include <sqlite3.h>

namespace Ui {
class ModifyFacultyDialog;
}

class ModifyFacultyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ModifyFacultyDialog(QWidget *parent = nullptr);
    ~ModifyFacultyDialog();

private slots:
    void on_BtnCancel_clicked();

    void GetFaculty(QString,QString,QString,QString);

    void on_BtnComfirmModify_clicked();

private:
    Ui::ModifyFacultyDialog *ui;
    QString NeedModifyFacultyId;

signals:
    void SendNeedModifyFacultyToThread(QString,QString);
};

#endif // MODIFYFACULTYDIALOG_H
