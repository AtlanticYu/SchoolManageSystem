#include "qworkspace.h"


QWorkSpace::QWorkSpace()
{

}
//查询所有的院信息集，返回给主界面
void QWorkSpace::SearchAllCollege()
{
    mutex.lock();
    sqlite3 *pDb;
    if(sqlite3_open("./School.db",&pDb) == SQLITE_OK)
    {
        QString sql = "select * from college order by college_id desc";
        sqlite3_stmt *pStmt;
        if(sqlite3_prepare(pDb,sql.toStdString().c_str(),sql.length(),&pStmt,NULL) == SQLITE_OK)
        {
            sqlite3_close(pDb);
            mutex.unlock();
            emit EmitAllCollegepStmt(pStmt);
        }
        else
        {
            sqlite3_close(pDb);
            mutex.unlock();
        }
    }
}
//根据院id来进行查找系信息
void QWorkSpace::SearchFacultyByCollegeId(QString id)
{
    mutex.lock();
    sqlite3 *pDb;
    if(sqlite3_open("./School.db",&pDb) == SQLITE_OK)
    {
        QString sql = "select * from faculty where college_id = '"+id+"' order by faculty_id desc;";
        sqlite3_stmt *pStmt;
        if(sqlite3_prepare(pDb,sql.toStdString().c_str(),sql.length(),&pStmt,NULL) == SQLITE_OK)
        {
            sqlite3_close(pDb);
            mutex.unlock();
            emit EmitSelectedFauclty(pStmt);
        }
        else
        {
            sqlite3_close(pDb);
            mutex.unlock();
        }
    }
}
//添加院
void QWorkSpace::AddCollege(QString college_name)
{
    mutex.lock();
    sqlite3 *pDb;
    if(sqlite3_open("./School.db",&pDb)!=SQLITE_OK) return;
    QString sql = "insert into college(college_name) values('"+college_name+"')";
    char *errmsg;
    sqlite3_exec(pDb,sql.toStdString().c_str(),NULL,NULL,&errmsg);
    sqlite3_close(pDb);
    mutex.unlock();
    //本地更新
    SearchAllCollege();
}
//根据院id删除
void QWorkSpace::DeleteCollegeById(QString id)
{
    mutex.lock();
    sqlite3 *pDb;
    if(sqlite3_open("./School.db",&pDb) == SQLITE_OK)
    {
        QString sql = "delete from college where college_id = '"+id+"';";
        sqlite3_stmt *pStmt;
        char *errmsg;
        if(sqlite3_exec(pDb,sql.toStdString().c_str(),NULL,NULL,&errmsg)==SQLITE_OK)
        {
            sqlite3_close(pDb);
            mutex.unlock();
            //本地更新
            SearchAllCollege();
        }
        else
        {
            sqlite3_close(pDb);
            mutex.unlock();
        }
    }
}
//修改院信息
void QWorkSpace::ModifyCollege(QString college_id,QString college_name)
{
    mutex.lock();
    sqlite3 *pDb;
    if(sqlite3_open("./School.db",&pDb)!=SQLITE_OK) return;
    QString sql = "update college set college_name = '"+college_name+"' where college_id = '"+college_id+"';";
    char *errmsg;
    //中文要转成utf8
    if(sqlite3_exec(pDb,sql.toUtf8().data(),NULL,NULL,&errmsg)==SQLITE_OK)
    {
//        qDebug() << "modify ok";
    }
    //级联更新系信息
    sql = "update faculty set college_name = '"+college_name+"' where college_id = '"+college_id+"';";
    if(sqlite3_exec(pDb,sql.toUtf8().data(),NULL,NULL,&errmsg)==SQLITE_OK)
    {
//        qDebug() << "modify ok";
    }
    sqlite3_close(pDb);
    mutex.unlock();
    //本地更新
    emit EmitRefleshFaculty();
    //本地更新
    SearchAllCollege();

}
//根据相应的信息来添加系
void QWorkSpace::AddFaculty(QString faculty_name,QString college_id,QString college_name)
{
    mutex.lock();
    //写入数据库
    sqlite3 *pDb;
    if(sqlite3_open("./School.db",&pDb)!=SQLITE_OK) return;
    QString sql = "insert into faculty(faculty_name,college_id,college_name) values('"+faculty_name+"','"+college_id+"','"+college_name+"')";
    char *errmsg;
    if(sqlite3_exec(pDb,sql.toUtf8().data(),NULL,NULL,&errmsg)==SQLITE_OK)
    {
//        qDebug() << "modify ok";
    }
    sqlite3_close(pDb);
    mutex.unlock();
    //本地更新
    emit EmitRefleshFaculty();
}
//根据系id来进行删除
void QWorkSpace::DeleteFacultyById(QString faculty_id)
{
    mutex.lock();
    sqlite3 *pDb;
    if(sqlite3_open("./School.db",&pDb)!=SQLITE_OK) return;
    QString sql = "delete from faculty where faculty_id = '"+faculty_id+"';";
    char *errmsg;
    if(sqlite3_exec(pDb,sql.toStdString().c_str(),NULL,NULL,&errmsg)==SQLITE_OK)
    {
//        qDebug() << "delete faculty ok";
    }
    sqlite3_close(pDb);
    mutex.unlock();
    //本地更新
    emit EmitRefleshFaculty();
}
//根据收到的系id和name来进行更新操作
void QWorkSpace::ModifyFaculty(QString faculty_id,QString faculty_name)
{
    mutex.lock();
    sqlite3 *pDb;
    if(sqlite3_open("./School.db",&pDb)!=SQLITE_OK) return;
    QString sql = "update faculty set faculty_name = '"+faculty_name+"' where faculty_id = '"+faculty_id+"';";
    char *errmsg;
    //中文要转成utf8
    if(sqlite3_exec(pDb,sql.toUtf8().data(),NULL,NULL,&errmsg)==SQLITE_OK)
    {
//        qDebug() << "modify faculty ok";
    }
    sqlite3_close(pDb);
    mutex.unlock();
    //本地更新
    emit EmitRefleshFaculty();
}
//初始化combobox控件，把结果集发送给添加系窗体
void QWorkSpace::InitFacultyForCbx()
{
    mutex.lock();
//    qDebug() << "2";
    sqlite3 *pDb;
    if(sqlite3_open("./School.db",&pDb) == SQLITE_OK)
    {
        QString sql = "select college_name from college";
        sqlite3_stmt *pStmt;
        if(sqlite3_prepare(pDb,sql.toStdString().c_str(),sql.length(),&pStmt,NULL) == SQLITE_OK)
        {
            sqlite3_close(pDb);
            mutex.unlock();
            emit EmitAllFacultyForCbx(pStmt);
        }
        else
        {
            sqlite3_close(pDb);
            mutex.unlock();
        }
    }
}
//根据组装好的sql语句查询到对应的系id，返回给添加系窗体
void QWorkSpace::SearchMatchFacultyForId(QString sql)
{
    mutex.lock();
    sqlite3 *pDb;
    if(sqlite3_open("./School.db",&pDb) == SQLITE_OK)
    {
        sqlite3_stmt *pStmt;
        //查询中文要转成utf8，其中第二、三个参数都需要做相应修改，不转码会查询失败
        if(sqlite3_prepare(pDb,sql.toUtf8().data(),sql.toUtf8().length(),&pStmt,NULL) == SQLITE_OK)
        {
            sqlite3_close(pDb);
            mutex.unlock();
            emit EmitMatchedFacultyId(pStmt);
        }
        else
        {
            sqlite3_close(pDb);
            mutex.unlock();
        }
    }
}
