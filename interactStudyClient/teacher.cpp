#include "teacher.h"
#include "ui_teacher.h"
#include <QHostAddress>

Teacher::Teacher(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Teacher)
{
    ui->setupUi(this);

    socket = new QTcpSocket(this);
    socket->connectToHost(QHostAddress::LocalHost, 1234);

    QByteArray ba;
    QDataStream msg(&ba, QIODevice::WriteOnly);
    msg.setVersion(QDataStream::Qt_5_5);
    msg<<showStudents;
    socket->write(ba);
    setWindowTitle("Teacher");

    connect(socket, SIGNAL(readyRead()), this, SLOT(readServer()));
}

Teacher::~Teacher()
{
    socket->close();
    delete ui;
}

void Teacher::addNewStudent(StudentP student)
{
    students.push_back(student);
    ui->students->clear();
    for(int i=0;i<students.size();i++)
        ui->students->addItem(students[i].nick);
}

void Teacher::readServer()
{
    QDataStream reader(socket);
    reader.setVersion(QDataStream::Qt_5_5);
    int command;
    reader>>command;
    switch(command)
    {
    case showStudents:
        students.clear();
        ui->students->clear();
        while(!reader.atEnd())
        {
            StudentP student;
            reader>>student.sname>>student.name>>student.nick;
            students.push_back(student);
            ui->students->addItem(student.nick);
        }
        break;
    case sendPrivateMessage:
        QString name, text;
        reader>>name;
        reader>>text;
        ui->chat->appendPlainText(name + " : " + text);
    }
}

void Teacher::on_pushButton_2_clicked()
{
    QByteArray ba;
    QDataStream msg(&ba, QIODevice::WriteOnly);
    msg.setVersion(QDataStream::Qt_5_5);
    msg<<sendMark<<ui->students->currentText()<<ui->workName->text()<<ui->spinBox->value();
    socket->write(ba);

    ui->chat->appendPlainText("Teacher take " + QString::number(ui->spinBox->value()) + " for " + ui->students->currentText());
}

void Teacher::on_pushButton_clicked()
{
    QByteArray ba;
    QDataStream msg(&ba, QIODevice::WriteOnly);
    msg.setVersion(QDataStream::Qt_5_5);
    msg<<sendMessage<<ui->message->toPlainText();
    socket->write(ba);

    ui->chat->appendPlainText("Teacher : " + ui->message->toPlainText());
    ui->message->setPlainText("");
}

void Teacher::on_pushButton_5_clicked()
{
    students.remove(ui->students->currentIndex());
    QByteArray ba;
    QDataStream msg(&ba, QIODevice::WriteOnly);
    msg.setVersion(QDataStream::Qt_5_5);
    msg<<deleteStudent<<ui->students->currentText();
    socket->write(ba);
    ui->students->removeItem(ui->students->currentIndex());
}

void Teacher::on_pushButton_4_clicked()
{
    addStudent form(this);
    form.show();
    form.exec();

    QByteArray ba;
    QDataStream msg(&ba, QIODevice::WriteOnly);
    msg.setVersion(QDataStream::Qt_5_5);
    msg<<addNStudent;
    msg<<students.last().nick<<students.last().pass<<students.last().sname<<students.last().name;
    socket->write(ba);
}
