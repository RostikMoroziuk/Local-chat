#ifndef START_H
#define START_H

#include <QDialog>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QDataStream>
#include "student.h"
#include "teacher.h"
#include "person.h"
#include <QDebug>

namespace Ui {
class start;
}

class start : public QDialog
{
    Q_OBJECT

public:
    explicit start(QWidget *parent = 0);
    ~start();

public slots:
    void readAnswer();
    void send();

private:
    Ui::start *ui;
    QTcpSocket *socket;
    Student* student;
    Teacher *teacher;

    enum {authorization, addStudent, deleteStudent, showStudents, sendPrivateMessage, sendMessage, sendWork, recieveAnswear};
};

#endif // START_H
