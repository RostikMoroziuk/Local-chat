#include "ui_start.h"
#include "start.h"
#include <QByteArray>
#include <QMessageBox>
#include <QPair>
#include <QDate>
#include <QMap>

start::start(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::start)
{
    ui->setupUi(this);

    socket = new QTcpSocket(this);
    socket->connectToHost(QHostAddress::LocalHost, 1234);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readAnswer()));
    connect(ui->enter, SIGNAL(clicked(bool)), this, SLOT(send()));
}

start::~start()
{
    socket->close();
    delete ui;
}

void start::readAnswer()
{
    QDataStream reader(socket);
    reader.setVersion(QDataStream::Qt_5_5);
    int command;
    reader>>command;
    switch(command)
    {
    case authorization:
        bool ok, isTeacher;
        reader>>ok>>isTeacher;
        if(ok)
        {
            if(isTeacher)
            {
                this->hide();
                teacher = new Teacher();
                teacher->show();

                connect(this, SIGNAL(destroyed(QObject*)), teacher, SLOT(deleteLater()));
            }
            else
            {
                QString name, sname, nick;
                this->hide();
                QVector<QPair<QString, int> >marks;

                reader>>marks>>name>>sname>>nick;
                student = new Student(marks, name+ " " + sname, nick);
                student->show();

                connect(this, SIGNAL(destroyed(QObject*)), student, SLOT(deleteLater()));
            }
        }
        else
        {
            QMessageBox::critical(NULL,"Error", "Incorrectly login or password");
        }
        break;
    }
}

void start::send()
{
    QByteArray ba;
    QDataStream writer(&ba, QIODevice::WriteOnly);
    writer.setVersion(QDataStream::Qt_5_5);
    writer<<authorization<<ui->login->text()<<ui->password->text();
    socket->write(ba);
}
