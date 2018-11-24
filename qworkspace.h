#ifndef QWORKSPACE_H
#define QWORKSPACE_H
#include <QString>
#include <QThread>
#include <sqlite3.h>
#include <QDebug>
#include <QMutex>


class QWorkSpace : public QThread
{
        Q_OBJECT
public:
    QWorkSpace();

signals:
    void EmitAllCollegepStmt(sqlite3_stmt*);
    void EmitSelectedFauclty(sqlite3_stmt*);
    void EmitRefleshFaculty();
    void EmitAllFacultyForCbx(sqlite3_stmt*);
    void EmitMatchedFacultyId(sqlite3_stmt*);

public slots:
    void SearchAllCollege();
    void SearchFacultyByCollegeId(QString);
    void AddCollege(QString);
    void DeleteCollegeById(QString);
    void ModifyCollege(QString,QString);
    void AddFaculty(QString,QString,QString);
    void DeleteFacultyById(QString);
    void ModifyFaculty(QString,QString);
    void InitFacultyForCbx();
    void SearchMatchFacultyForId(QString);

private:
    QMutex mutex;
};

#endif // QWORKSPACE_H
