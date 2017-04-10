#ifndef PERSON
#define PERSON

#include <QString>
#include <QTcpSocket>
#include <QMap>
#include <QDate>
#include <QVector>

struct StudentP
{
    QString name, sname, nick, pass;
    QTcpSocket* socket;
    QVector<QPair<QString, int> > marks;
};

struct TeacherP
{
    QString name, sname, nick, pass;
    QVector<StudentP> students;
    QTcpSocket* socket;
};

#endif // PERSON

