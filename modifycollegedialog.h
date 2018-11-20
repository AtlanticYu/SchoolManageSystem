#ifndef MODIFYCOLLEGEDIALOG_H
#define MODIFYCOLLEGEDIALOG_H

#include <QDialog>

namespace Ui {
class ModifyCollegeDialog;
}

class ModifyCollegeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ModifyCollegeDialog(QWidget *parent = nullptr);
    ~ModifyCollegeDialog();

private slots:
    void on_BtnCancel_clicked();

    void on_BtnConfirmModify_clicked();

    void GetCollege(QString,QString);

private:
    Ui::ModifyCollegeDialog *ui;
    QString NeedModifyCollegeId;

signals:
    void SendRefleshCollegeTable();
};

#endif // MODIFYCOLLEGEDIALOG_H
