#include "addcollegedialog.h"
#include "ui_addcollegedialog.h"
#include "QMessageBox"

AddCollegeDialog::AddCollegeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddCollegeDialog)
{
    ui->setupUi(this);
}

AddCollegeDialog::~AddCollegeDialog()
{
    delete ui;
}

void AddCollegeDialog::on_BtnCancel_clicked()
{
    this->accept();
}

void AddCollegeDialog::on_BtnConfirmAdd_clicked()
{
    QString CollegeName = ui->EdtCollegeName->text();
    if(CollegeName == NULL)
    {
        QMessageBox::information(this,"错误","没有正确输入相应信息");
        return;
    }
    emit SendAddCollege(CollegeName);
}
