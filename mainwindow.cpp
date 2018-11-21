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
    workSpace = new QWorkSpace();
    connect(this,SIGNAL(NeedAllCollegePstmt()),workSpace,SLOT(SearchAllCollege()));
    connect(workSpace,SIGNAL(EmitAllCollegepStmt(sqlite3_stmt*)),this,SLOT(GetAllCollegePstmt(sqlite3_stmt*)));
    connect(this,SIGNAL(SendSelectedFacultyId(QString)),workSpace,SLOT(SearchFacultyByCollegeId(QString)));
    connect(workSpace,SIGNAL(EmitSelectedFauclty(sqlite3_stmt*)),this,SLOT(GetSelectedFaculty(sqlite3_stmt*)));
    connect(this,SIGNAL(SendWillDeleteCollegeId(QString)),workSpace,SLOT(DeleteCollegeById(QString)));
    connect(workSpace,SIGNAL(EmitRefleshFaculty()),this,SLOT(ShowSelectedFaculty()));
    connect(this,SIGNAL(SendWillDeleteFacultyId(QString)),workSpace,SLOT(DeleteFacultyById(QString)));
    thread = new QThread(this);
    workSpace->moveToThread(thread);
    thread->start();
}

MainWindow::~MainWindow()
{
    delete ui;
    workSpace->quit();
    workSpace->wait();
    thread->quit();
    thread->wait();
}

void MainWindow::showEvent (QShowEvent * event)
{
    ShowAllInformation();
}

//刷新学院
void MainWindow::ShowAllInformation()
{
    NeedAllCollegePstmt();
}
//添加按钮点击时绑定相应的槽和函数
void MainWindow::on_BtnAddCollege_clicked()
{
    if(AddCollegeDlg == NULL)
    {
        AddCollegeDlg = new AddCollegeDialog(this);
        AddCollegeDlg->setModal(false);
        connect(AddCollegeDlg,SIGNAL(SendAddCollege(QString)),workSpace,SLOT(AddCollege(QString)));
    }
    AddCollegeDlg->show();
}

//删除学院
void MainWindow::on_BtnDeleteCollege_clicked()
{
    //获取选中行数
    int row = ui->tableWidgetCollege->currentRow();
    if (row >= 0)
    {
        QTableWidgetItem *pIdItem = ui->tableWidgetCollege->item(row,0);
        QString id = pIdItem->text();
        emit SendWillDeleteCollegeId(id);
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
        connect(ModifyCollegeDlg,SIGNAL(SendNeedModifyCollegeToThread(QString,QString)),workSpace,SLOT(ModifyCollege(QString,QString)));
    }
    int CurrentRow = ui->tableWidgetCollege->currentRow();
    if (CurrentRow >= 0)
    {
        emit SendClickedCollege(ui->tableWidgetCollege->item(CurrentRow,0)->text(),ui->tableWidgetCollege->item(CurrentRow,1)->text());
    }
    ModifyCollegeDlg->show();
}
//查看所选院的系
void MainWindow::on_BtnSearchCollege_clicked()
{
    ShowSelectedFaculty();
}
//把id发送给线程
void MainWindow::ShowSelectedFaculty()
{
    //获取选中行数
    int row = ui->tableWidgetCollege->currentRow();
    if(row >= 0)
    {
        QTableWidgetItem *pIdItem = ui->tableWidgetCollege->item(row,0);
        QString id = pIdItem->text();
        emit SendSelectedFacultyId(id);
    }
}
//添加系
void MainWindow::on_BtnAddFaculty_clicked()
{
    if(AddFacultyDlg == NULL)
    {
        AddFacultyDlg = new AddFacultyDialog(this);
        AddFacultyDlg->setModal(false);
        connect(AddFacultyDlg,SIGNAL(SendAddFaculty(QString,QString,QString)),workSpace,SLOT(AddFaculty(QString,QString,QString)));
    }
    AddFacultyDlg->show();
}
//删除系
void MainWindow::on_BtnDeleteFaculty_clicked()
{
    //获取选中行数
    int row = ui->tableWidgetFaculty->currentRow();
    if(row >= 0)
    {
        QTableWidgetItem *pIdItem = ui->tableWidgetFaculty->item(row,0);
        QString id = pIdItem->text();
        emit SendWillDeleteFacultyId(id);
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
        connect(ModityFacultyDlg,SIGNAL(SendNeedModifyFacultyToThread(QString,QString)),workSpace,SLOT(ModifyFaculty(QString,QString)));
    }
    int CurrentRow = ui->tableWidgetFaculty->currentRow();
    if (CurrentRow >= 0)
    {
        emit SendClickedFaculty(ui->tableWidgetFaculty->item(CurrentRow,0)->text(),ui->tableWidgetFaculty->item(CurrentRow,1)->text(),ui->tableWidgetFaculty->item(CurrentRow,2)->text(),ui->tableWidgetFaculty->item(CurrentRow,3)->text());
    }
    ModityFacultyDlg->show();
}

//读取所有的院信息
void MainWindow::GetAllCollegePstmt(sqlite3_stmt* pStmt)
{
    //先清空，防止数据重复添加
    int Tcount = ui->tableWidgetCollege->rowCount();
    for(int i = 0;i < Tcount;i ++)
    {
        ui->tableWidgetCollege->removeRow(0);
    }
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
//读取所选院的系
void MainWindow::GetSelectedFaculty(sqlite3_stmt* pStmt)
{
    //先清空，防止数据重复添加
    int Tcount = ui->tableWidgetFaculty->rowCount();
    for(int i = 0;i < Tcount;i ++)
    {
        ui->tableWidgetFaculty->removeRow(0);
    }
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
