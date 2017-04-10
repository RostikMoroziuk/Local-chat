#ifndef PERSON
#define PERSON

#include <QString>
#include <QTcpSocket>
#include <QMap>
#include <QDate>
#include <QVector>

struct Student
{
    QString name, sname, nick, pass;
    QTcpSocket* socket;
    QVector<QPair<QString, int> > marks;
};

struct Teacher
{
    QString name, sname, nick, pass;
    QVector<Student> students;
    QTcpSocket* socket;
};

#endif // PERSON

