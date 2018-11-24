#include "addfacultydialog.h"
#include "ui_addfacultydialog.h"

AddFacultyDialog::AddFacultyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddFacultyDialog)
{
    ui->setupUi(this);
}

AddFacultyDialog::~AddFacultyDialog()
{
    delete ui;
}
//在构造函数中初始化会出现connect()没有绑定的问题
void AddFacultyDialog::showEvent (QShowEvent * event)
{
    if(InitStatus != 1)
    {
        InitComboBox();
    }
}
//加载候选学院
void AddFacultyDialog::InitComboBox()
{
//    qDebug() << "1";
    emit EmitInitFacultyForCbx();
}
void AddFacultyDialog::GetAllFacultyFromThread(sqlite3_stmt* pStmt)
{
    while(sqlite3_step(pStmt) == SQLITE_ROW)
    {
        const unsigned char *p_college_name = sqlite3_column_text(pStmt,0);
        QString college_name = (char *)p_college_name;
        ui->CbxCollegeName->addItem(college_name);
    }
    //解首次进入不匹配ID的问题
    ShowMatchId();
    InitStatus = 1;
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
//生成与Cbx控件选取学院匹配的学院编号，先给线程发送sql语句
void AddFacultyDialog::ShowMatchId()
{
    QString college_name = ui->CbxCollegeName->currentText();
    QString sql = "select college_id from college where college_name = '"+college_name+"'";
    emit EmitNeedMatchFacultyId(sql);
}

void AddFacultyDialog::on_BtnCancel_clicked()
{
    this->accept();
}
//发送给主窗体以便于刷新表格
void AddFacultyDialog::on_BtnComfirmAdd_clicked()
{
    QString faculty_name = ui->EdtFacultyName->text();
    QString college_id = ui->EdtCollegeId->text();
    QString college_name = ui->CbxCollegeName->currentText();
    if(faculty_name == NULL || college_id == NULL || college_name == nullptr)
    {
        QMessageBox::information(this,"错误","没有正确输入相应信息");
        return;
    }
    else
    {
        emit SendAddFaculty(faculty_name,college_id,college_name);
    }

}
//线程中sql语句执行完毕后，返回结果集到此函数，显示到窗口中
void AddFacultyDialog::GetMatchFacultyIdFromThread(sqlite3_stmt* pStmt)
{
   while(sqlite3_step(pStmt) == SQLITE_ROW)
   {
        const unsigned char *p_college_id = sqlite3_column_text(pStmt,0);
        QString college_id = (char *)p_college_id;
        ui->EdtCollegeId->setText(college_id);
   }
}


