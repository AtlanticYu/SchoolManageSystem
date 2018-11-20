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
    sqlite3 *pDb;
    if(sqlite3_open("./School.db",&pDb)!=SQLITE_OK) return;
    QString sql = "update college set college_name = '"+college_name+"' where college_id = '"+NeedModifyCollegeId+"';";
    char *errmsg;
    //中文要转成utf8
    if(sqlite3_exec(pDb,sql.toUtf8().data(),NULL,NULL,&errmsg)==SQLITE_OK)
    {
        QMessageBox::information(this,"提示","修改成功");
    }
    sqlite3_close(pDb);
    emit SendRefleshCollegeTable();
}
void ModifyCollegeDialog::GetCollege(QString college_id,QString college_name)
{
    ui->EdtCollegeName->setText(college_name);
    NeedModifyCollegeId = college_id;
}
