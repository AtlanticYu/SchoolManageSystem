#include "qworkspace.h"


QWorkSpace::QWorkSpace()
{

}
void QWorkSpace::SearchAllCollege()
{
    sqlite3 *pDb;
    if(sqlite3_open("./School.db",&pDb) == SQLITE_OK)
    {
        QString sql = "select * from college order by college_id desc";
        sqlite3_stmt *pStmt;
        if(sqlite3_prepare(pDb,sql.toStdString().c_str(),sql.length(),&pStmt,NULL) == SQLITE_OK)
        {
            sqlite3_close(pDb);
            emit EmitAllCollegepStmt(pStmt);
        }
    }
}
void QWorkSpace::SearchFacultyByCollegeId(QString id)
{
    sqlite3 *pDb;
    if(sqlite3_open("./School.db",&pDb) == SQLITE_OK)
    {
        QString sql = "select * from faculty where college_id = '"+id+"' order by faculty_id desc;";
        sqlite3_stmt *pStmt;
        if(sqlite3_prepare(pDb,sql.toStdString().c_str(),sql.length(),&pStmt,NULL) == SQLITE_OK)
        {
            sqlite3_close(pDb);
            emit EmitSelectedFauclty(pStmt);
        }
    }
}
void QWorkSpace::AddCollege(QString college_name)
{
    sqlite3 *pDb;
    if(sqlite3_open("./School.db",&pDb)!=SQLITE_OK) return;
    QString sql = "insert into college(college_name) values('"+college_name+"')";
    char *errmsg;
    sqlite3_exec(pDb,sql.toStdString().c_str(),NULL,NULL,&errmsg);
    sqlite3_close(pDb);
    //本地更新
    SearchAllCollege();
}
void QWorkSpace::DeleteCollegeById(QString id)
{
    sqlite3 *pDb;
    if(sqlite3_open("./School.db",&pDb) == SQLITE_OK)
    {
        QString sql = "delete from college where college_id = '"+id+"';";
        sqlite3_stmt *pStmt;
        char *errmsg;
        if(sqlite3_exec(pDb,sql.toStdString().c_str(),NULL,NULL,&errmsg)==SQLITE_OK)
        {
            sqlite3_close(pDb);
            //本地更新
            SearchAllCollege();
        }
    }
}
void QWorkSpace::ModifyCollege(QString college_id,QString college_name)
{
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
    //本地更新
    emit EmitRefleshFaculty();
    //本地更新
    SearchAllCollege();

}
void QWorkSpace::AddFaculty(QString faculty_name,QString college_id,QString college_name)
{
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
    //本地更新
    emit EmitRefleshFaculty();
}
void QWorkSpace::DeleteFacultyById(QString faculty_id)
{
    sqlite3 *pDb;
    if(sqlite3_open("./School.db",&pDb)!=SQLITE_OK) return;
    QString sql = "delete from faculty where faculty_id = '"+faculty_id+"';";
    char *errmsg;
    if(sqlite3_exec(pDb,sql.toStdString().c_str(),NULL,NULL,&errmsg)==SQLITE_OK)
    {
//        qDebug() << "delete faculty ok";
    }
    sqlite3_close(pDb);
    //本地更新
    emit EmitRefleshFaculty();
}
void QWorkSpace::ModifyFaculty(QString faculty_id,QString faculty_name)
{
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
    //本地更新
    emit EmitRefleshFaculty();
}
