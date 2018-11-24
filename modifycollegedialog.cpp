#include "modifycollegedialog.h"
#include "ui_modifycollegedialog.h"
#include "QMessageBox"
#include "sqlite3.h"

ModifyCollegeDialog::ModifyCollegeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModifyCollegeDialog)
{
    ui->setupUi(this);
}

ModifyCollegeDialog::~ModifyCollegeDialog()
{
    delete ui;
}

void ModifyCollegeDialog::on_BtnCancel_clicked()
{
    this->accept();
}

void ModifyCollegeDialog::on_BtnConfirmModify_clicked()
{
    QString college_name = ui->EdtCollegeName->text();
    if(college_name == NULL)
    {
        QMessageBox::information(this,"错误","没有正确输入相应信息");
        return;
    }
    else
    {
        emit SendNeedModifyCollegeToThread(NeedModifyCollegeId,college_name);
    }
}
void ModifyCollegeDialog::GetCollege(QString college_id,QString college_name)
{
    ui->EdtCollegeName->setText(college_name);
    NeedModifyCollegeId = college_id;
}
