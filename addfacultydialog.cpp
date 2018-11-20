#include "addfacultydialog.h"
#include "ui_addfacultydialog.h"

AddFacultyDialog::AddFacultyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddFacultyDialog)
{
    ui->setupUi(this);
    InitComboBox();
}

AddFacultyDialog::~AddFacultyDialog()
{
    delete ui;
}
//加载候选学院
void AddFacultyDialog::InitComboBox()
{
    sqlite3 *pDb;
    if(sqlite3_open("./School.db",&pDb) == SQLITE_OK)
    {
        QString sql = "select college_name from college";
        sqlite3_stmt *pStmt;
        if(sqlite3_prepare(pDb,sql.toStdString().c_str(),sql.length(),&pStmt,NULL) == SQLITE_OK)
        {
            while(sqlite3_step(pStmt) == SQLITE_ROW)
            {
                const unsigned char *p_college_name = sqlite3_column_text(pStmt,0);
                QString college_name = (char *)p_college_name;
                ui->CbxCollegeName->addItem(college_name);
            }
        }
        sqlite3_close(pDb);
        //解首次进入不匹配ID的问题
        ShowMatchId();
        InitStatus = 1;
    }
}
//快速生成的槽函数，当Cbx改变值时激活
void AddFacultyDialog::on_CbxCollegeName_currentIndexChanged(const QString &arg1)
{
    //因为初始化每次都会激活相应的槽，为了避免数据库的同时打开
    if(InitStatus == 1)
    {
        ShowMatchId();
    }
}
//生成与Cbx控件选取学院匹配的学院编号，为了初始化的方便而做了包装
void AddFacultyDialog::ShowMatchId()
{
    sqlite3 *pDb;
    QString college_name = ui->CbxCollegeName->currentText();
    if(sqlite3_open("./School.db",&pDb) == SQLITE_OK)
    {
        QString sql = "select college_id from college where college_name = '"+college_name+"'";
        sqlite3_stmt *pStmt;
        //查询中文要转成utf8，其中第二、三个参数都需要做相应修改，不转码会查询失败
        if(sqlite3_prepare(pDb,sql.toUtf8().data(),sql.toUtf8().length(),&pStmt,NULL) == SQLITE_OK)
        {
           while(sqlite3_step(pStmt) == SQLITE_ROW)
           {
                const unsigned char *p_college_id = sqlite3_column_text(pStmt,0);
                QString college_id = (char *)p_college_id;
                ui->EdtCollegeId->setText(college_id);
           }
        }
        sqlite3_close(pDb);
    }
}

void AddFacultyDialog::on_BtnCancel_clicked()
{
    this->accept();
}
//发送给主窗体以便于刷新表格
void AddFacultyDialog::on_BtnComfirmAdd_clicked()
{
    emit SendAddFaculty(ui->EdtFacultyName->text(),ui->EdtCollegeId->text(),ui->CbxCollegeName->currentText());
}


