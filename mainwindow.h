#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <addcollegedialog.h>
#include <modifycollegedialog.h>
#include <addfacultydialog.h>
#include <modifyfacultydialog.h>
#include <QDebug>
#include <qdebug.h>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void showEvent (QShowEvent * event);
    void ShowAllInformation();
    void ShowSelectedFaculty();

private slots:
    void on_BtnAddCollege_clicked();

    void ReceiveAddCollege(QString);

    void ReceiveAddFaculty(QString,QString,QString);


    void on_BtnDeleteCollege_clicked();

    void on_BtnModifyCollege_clicked();

    void ReceivedRefleshCollege();

    void ReceivedRefleshFaculty();

    void on_BtnSearchCollege_clicked();

    void on_BtnAddFaculty_clicked();

    void on_BtnDeleteFaculty_clicked();

    void on_BtnModifyFaculty_clicked();

private:
    Ui::MainWindow *ui;
    AddCollegeDialog *AddCollegeDlg;
    ModifyCollegeDialog *ModifyCollegeDlg;
    AddFacultyDialog *AddFacultyDlg;
    ModifyFacultyDialog *ModityFacultyDlg;


signals:
    void SendClickedCollege(QString,QString);
    void SendClickedFaculty(QString,QString,QString,QString);
};

#endif // MAINWINDOW_H
