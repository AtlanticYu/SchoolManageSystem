#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sqlite3.h"
#include "pthread.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    AddCollegeDlg = NULL;
    ModifyCollegeDlg = NULL;
    AddFacultyDlg = NULL;
    ModityFacultyDlg = NULL;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showEvent (QShowEvent * event)
{
    ShowAllInformation();
}

//刷新学院
void MainWindow::ShowAllInformation()
{
    //先清空，防止数据重复添加
    int Tcount = ui->tableWidgetCollege->rowCount();
    for(int i = 0;i < Tcount;i ++)
    {
        ui->tableWidgetCollege->removeRow(0);
    }
    sqlite3 *pDb;
    if(sqlite3_open("./School.db",&pDb) == SQLITE_OK)
    {
        QString sql = "select * from college order by college_id desc";
        sqlite3_stmt *pStmt;
        if(sqlite3_prepare(pDb,sql.toStdString().c_str(),sql.length(),&pStmt,NULL) == SQLITE_OK)
        {
            while(sqlite3_step(pStmt) == SQLITE_ROW)
            {
                const unsigned char *p_college_id = sqlite3_column_text(pStmt,0);
                const unsigned char *p_college_name = sqlite3_column_text(pStmt,1);
                QTableWidgetItem *pCollegeIdItem = new QTableWidgetItem((char *)p_college_id);
                ui->tableWidgetCollege->insertRow(0);
                ui->tableWidgetCollege->setItem(0,0,pCollegeIdItem);
                QTableWidgetItem *pCollegeNameItem = new QTableWidgetItem((char *)p_college_name);
                ui->tableWidgetCollege->setItem(0,1,pCollegeNameItem);
                //QTableWidget根据内容调整列宽
                ui->tableWidgetCollege->resizeColumnsToContents();
            }
        }
    }
    sqlite3_close(pDb);
}
//添加按钮点击时绑定相应的槽和函数
void MainWindow::on_BtnAddCollege_clicked()
{
    if(AddCollegeDlg == NULL)
    {
        AddCollegeDlg = new AddCollegeDialog(this);
        AddCollegeDlg->setModal(false);
        connect(AddCollegeDlg,SIGNAL(SendAddCollege(QString)),this,SLOT(ReceiveAddCollege(QString)));
    }
    AddCollegeDlg->show();
}
//受到添加窗体的信号
void MainWindow::ReceiveAddCollege(QString college_name)
{
    sqlite3 *pDb;
    if(sqlite3_open("./School.db",&pDb)!=SQLITE_OK) return;
    QString sql = "insert into college(college_name) values('"+college_name+"')";
    char *errmsg;
    sqlite3_exec(pDb,sql.toStdString().c_str(),NULL,NULL,&errmsg);
    sqlite3_close(pDb);
    //本地更新
    ShowAllInformation();
}

//删除学院
void MainWindow::on_BtnDeleteCollege_clicked()
{
    //获取选中行数
    int row = ui->tableWidgetCollege->currentRow();
    if(row >= 0)
    {
        QTableWidgetItem *pIdItem = ui->tableWidgetCollege->item(row,0);
        sqlite3 *pDb;
        QString id = pIdItem->text();
        if(sqlite3_open("./School.db",&pDb)!=SQLITE_OK) return;
        QString sql = "delete from college where college_id = '"+id+"';";
        char *errmsg;
        if(sqlite3_exec(pDb,sql.toStdString().c_str(),NULL,NULL,&errmsg)==SQLITE_OK)
        {
            ui->tableWidgetCollege->removeRow(row);
        }
        sqlite3_close(pDb);
    }
}
//发送信号给修改窗体
void MainWindow::on_BtnModifyCollege_clicked()
{
    if(ModifyCollegeDlg == NULL)
    {
        ModifyCollegeDlg = new ModifyCollegeDialog(this);
        ModifyCollegeDlg->setModal(false);
        connect(this,SIGNAL(SendClickedCollege(QString,QString)),ModifyCollegeDlg,SLOT(GetCollege(QString,QString)));
        connect(ModifyCollegeDlg,SIGNAL(SendRefleshCollegeTable()),this,SLOT(ReceivedRefleshCollege()));
    }
    int CurrentRow = ui->tableWidgetCollege->currentRow();
    if (CurrentRow >= 0)
    {
        emit SendClickedCollege(ui->tableWidgetCollege->item(CurrentRow,0)->text(),ui->tableWidgetCollege->item(CurrentRow,1)->text());
    }
    ModifyCollegeDlg->show();
}
//收到刷新函数
void MainWindow::ReceivedRefleshCollege()
{
    ShowAllInformation();
}

void MainWindow::on_BtnSearchCollege_clicked()
{
    ShowSelectedFaculty();
}

void MainWindow::ShowSelectedFaculty()
{
    //获取选中行数
    int row = ui->tableWidgetCollege->currentRow();
    if(row >= 0)
    {
        //先清空，防止数据重复添加
        int Tcount = ui->tableWidgetFaculty->rowCount();
        for(int i = 0;i < Tcount;i ++)
        {
            ui->tableWidgetFaculty->removeRow(0);
        }
        sqlite3 *pDb;
        if(sqlite3_open("./School.db",&pDb) == SQLITE_OK)
        {
            QTableWidgetItem *pIdItem = ui->tableWidgetCollege->item(row,0);
            QString id = pIdItem->text();
            QString sql = "select * from faculty where college_id = '"+id+"' order by faculty_id desc;";
            sqlite3_stmt *pStmt;
            if(sqlite3_prepare(pDb,sql.toStdString().c_str(),sql.length(),&pStmt,NULL) == SQLITE_OK)
            {
                while(sqlite3_step(pStmt) == SQLITE_ROW)
                {
                    const unsigned char *p_faculty_id = sqlite3_column_text(pStmt,0);
                    const unsigned char *p_faculty_name = sqlite3_column_text(pStmt,1);
                    const unsigned char *p_college_id = sqlite3_column_text(pStmt,2);
                    const unsigned char *p_college_name = sqlite3_column_text(pStmt,3);
                    QTableWidgetItem *pFacultyIdItem = new QTableWidgetItem((char *)p_faculty_id);
                    ui->tableWidgetFaculty->insertRow(0);
                    ui->tableWidgetFaculty->setItem(0,0,pFacultyIdItem);
                    QTableWidgetItem *pFacultyNameItem = new QTableWidgetItem((char *)p_faculty_name);
                    ui->tableWidgetFaculty->setItem(0,1,pFacultyNameItem);
                    QTableWidgetItem *pCollegeIdItem = new QTableWidgetItem((char *)p_college_id);
                    ui->tableWidgetFaculty->setItem(0,2,pCollegeIdItem);
                    QTableWidgetItem *pCollegeNameItem = new QTableWidgetItem((char *)p_college_name);
                    ui->tableWidgetFaculty->setItem(0,3,pCollegeNameItem);
                    //QTableWidget根据内容调整列宽
                    ui->tableWidgetFaculty->resizeColumnsToContents();
                }
            }
        }
        sqlite3_close(pDb);
    }

}

void MainWindow::on_BtnAddFaculty_clicked()
{
    if(AddFacultyDlg == NULL)
    {
        AddFacultyDlg = new AddFacultyDialog(this);
        AddFacultyDlg->setModal(false);
        connect(AddFacultyDlg,SIGNAL(SendAddFaculty(QString,QString,QString)),this,SLOT(ReceiveAddFaculty(QString,QString,QString)));
    }
    AddFacultyDlg->show();
}

void MainWindow::ReceiveAddFaculty(QString faculty_name,QString college_id,QString college_name)
{
//    qDebug() << faculty_name;
//    qDebug() << college_id;
//    qDebug() << college_name;
    //判断是否为空
    if(faculty_name ==NULL||college_id == NULL||college_name ==NULL)
    {
        QMessageBox::information(this,"错误","没有正确输入相应信息");
        return;
    }
    //写入数据库
    sqlite3 *pDb;
    if(sqlite3_open("./School.db",&pDb)!=SQLITE_OK) return;
    QString sql = "insert into faculty(faculty_name,college_id,college_name) values('"+faculty_name+"','"+college_id+"','"+college_name+"')";
    char *errmsg;
    sqlite3_exec(pDb,sql.toStdString().c_str(),NULL,NULL,&errmsg);
    sqlite3_close(pDb);
    //本地更新
    ShowSelectedFaculty();
}

void MainWindow::on_BtnDeleteFaculty_clicked()
{
    //获取选中行数
    int row = ui->tableWidgetFaculty->currentRow();
    if(row >= 0)
    {
        QTableWidgetItem *pIdItem = ui->tableWidgetFaculty->item(row,0);
        sqlite3 *pDb;
        QString id = pIdItem->text();
        if(sqlite3_open("./School.db",&pDb)!=SQLITE_OK) return;
        QString sql = "delete from faculty where faculty_id = '"+id+"';";
        char *errmsg;
        if(sqlite3_exec(pDb,sql.toStdString().c_str(),NULL,NULL,&errmsg)==SQLITE_OK)
        {
            ui->tableWidgetFaculty->removeRow(row);
        }
        sqlite3_close(pDb);
    }
}
//发送信号给修改窗体
void MainWindow::on_BtnModifyFaculty_clicked()
{
    if(ModityFacultyDlg == NULL)
    {
        ModityFacultyDlg = new ModifyFacultyDialog(this);
        ModityFacultyDlg->setModal(false);
        connect(this,SIGNAL(SendClickedFaculty(QString,QString,QString,QString)),ModityFacultyDlg,SLOT(GetFaculty(QString,QString,QString,QString)));
        connect(ModityFacultyDlg,SIGNAL(SendRefleshFacultyTable()),this,SLOT(ReceivedRefleshFaculty()));
    }
    int CurrentRow = ui->tableWidgetFaculty->currentRow();
    if (CurrentRow >= 0)
    {
        emit SendClickedFaculty(ui->tableWidgetFaculty->item(CurrentRow,0)->text(),ui->tableWidgetFaculty->item(CurrentRow,1)->text(),ui->tableWidgetFaculty->item(CurrentRow,2)->text(),ui->tableWidgetFaculty->item(CurrentRow,3)->text());
    }
    ModityFacultyDlg->show();
}

void MainWindow::ReceivedRefleshFaculty()
{
    ShowSelectedFaculty();
}
