#ifndef QWORKSPACE_H
#define QWORKSPACE_H
#include <QString>
#include <QThread>
#include <sqlite3.h>
#include <QDebug>


class QWorkSpace : public QThread
{
        Q_OBJECT
public:
    QWorkSpace();

signals:
    void EmitAllCollegepStmt(sqlite3_stmt*);
    void EmitSelectedFauclty(sqlite3_stmt*);
    void EmitRefleshFaculty();

public slots:
    void SearchAllCollege();
    void SearchFacultyByCollegeId(QString);
    void AddCollege(QString);
    void DeleteCollegeById(QString);
    void ModifyCollege(QString,QString);
    void AddFaculty(QString,QString,QString);
    void DeleteFacultyById(QString);
    void ModifyFaculty(QString,QString);
};

#endif // QWORKSPACE_H
