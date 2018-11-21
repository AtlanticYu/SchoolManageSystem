#include "modifyfacultydialog.h"
#include "ui_modifyfacultydialog.h"

ModifyFacultyDialog::ModifyFacultyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModifyFacultyDialog)
{
    ui->setupUi(this);
}

ModifyFacultyDialog::~ModifyFacultyDialog()
{
    delete ui;
}

void ModifyFacultyDialog::on_BtnCancel_clicked()
{
    this->accept();
}

void ModifyFacultyDialog::GetFaculty(QString faculty_id,QString faculty_name,QString college_id,QString college_name)
{
    ui->EdtFacultyName->setText(faculty_name);
    ui->EdtCollegeId->setText(college_id);
    ui->EdtCollegeName->setText(college_name);
    NeedModifyFacultyId = faculty_id;
 //   qDebug() << NeedModifyFacultyId;
}
//只可以修改系名称
void ModifyFacultyDialog::on_BtnComfirmModify_clicked()
{
    QString faculty_name = ui->EdtFacultyName->text();
    if(faculty_name == NULL)
    {
        QMessageBox::information(this,"错误","没有正确输入相应信息");
        return;
    }
    emit SendNeedModifyFacultyToThread(NeedModifyFacultyId,faculty_name);

//    sqlite3 *pDb;
//    if(sqlite3_open("./School.db",&pDb)!=SQLITE_OK) return;
//    QString sql = "update faculty set faculty_name = '"+faculty_name+"' where faculty_id = '"+NeedModifyFacultyId+"';";
//    char *errmsg;
//    //中文要转成utf8
//    if(sqlite3_exec(pDb,sql.toUtf8().data(),NULL,NULL,&errmsg)==SQLITE_OK)
//    {
//        QMessageBox::information(this,"提示","修改成功");
//    }
//    sqlite3_close(pDb);
}
