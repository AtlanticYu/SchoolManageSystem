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
#include <qworkspace.h>

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

private slots:
    void on_BtnAddCollege_clicked();

    void on_BtnDeleteCollege_clicked();

    void on_BtnModifyCollege_clicked();

    void on_BtnSearchCollege_clicked();

    void on_BtnAddFaculty_clicked();

    void on_BtnDeleteFaculty_clicked();

    void on_BtnModifyFaculty_clicked();

    //thread
    void GetAllCollegePstmt(sqlite3_stmt*);
    void GetSelectedFaculty(sqlite3_stmt*);
    void ShowSelectedFaculty();

private:
    Ui::MainWindow *ui;
    AddCollegeDialog *AddCollegeDlg;
    ModifyCollegeDialog *ModifyCollegeDlg;
    AddFacultyDialog *AddFacultyDlg;
    ModifyFacultyDialog *ModityFacultyDlg;
    QWorkSpace *workSpace;
    QThread *thread;


signals:
    void SendClickedCollege(QString,QString);
    void SendClickedFaculty(QString,QString,QString,QString);
    //thread
    void NeedAllCollegePstmt();
    void SendSelectedFacultyId(QString);
    void SendWillDeleteCollegeId(QString);
    void SendWillDeleteFacultyId(QString);
};

#endif // MAINWINDOW_H
