#ifndef ADDFACULTYDIALOG_H
#define ADDFACULTYDIALOG_H

#include <QDialog>
#include <sqlite3.h>
#include <QMessageBox>
#include <QDebug>

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

protected:
    void showEvent (QShowEvent * event);

private:
    Ui::AddFacultyDialog *ui;
    QString InitStatus;

signals:
    void SendAddFaculty(QString,QString,QString);
    void EmitInitFacultyForCbx();
    void EmitNeedMatchFacultyId(QString);
private slots:
    void on_BtnCancel_clicked();
    void on_BtnComfirmAdd_clicked();
    void on_CbxCollegeName_currentIndexChanged(const QString &arg1);
    void GetAllFacultyFromThread(sqlite3_stmt*);
    void GetMatchFacultyIdFromThread(sqlite3_stmt*);
};

#endif // ADDFACULTYDIALOG_H
