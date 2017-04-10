#include "student.h"
#include "ui_student.h"
#include <QFileDialog>
#include <QHostAddress>

Student::Student(QVector<QPair<QString, int> > marks, QString name, QString nick) :
    QMainWindow(0),
    ui(new Ui::Student)
{
    ui->setupUi(this);
    fullName = name;

    //заповнення журналу оцінок
    for(int i=0;i<marks.size();i++)
    {
        ui->marks->appendPlainText(marks[i].first + " " + QString::number(marks[i].second));
    }

    socket = new QTcpSocket(this);
    socket->connectToHost(QHostAddress::LocalHost, 1234);

    QByteArray ba;
    QDataStream msg(&ba, QIODevice::WriteOnly);
    msg.setVersion(QDataStream::Qt_5_5);
    msg<<setSock<<nick;
    socket->write(ba);
    qDebug()<<socket->socketDescriptor();
    connect(socket, SIGNAL(readyRead()), this, SLOT(readServer()));

    setWindowTitle("Student");
}

Student::~Student()
{
    delete ui;
}

void Student::on_pushButton_clicked()//відправити повідомлення
{
    ui->chat->appendPlainText(fullName + " : " + ui->message->toPlainText());

        QByteArray ba;
        QDataStream msg(&ba, QIODevice::WriteOnly);
        msg.setVersion(QDataStream::Qt_5_5);
        msg<<sendPrivateMessage<<fullName<<ui->message->toPlainText();
        socket->write(ba);
        ui->message->setPlainText(" ");
}

void Student::readServer()
{
    QDataStream reader(socket);
    reader.setVersion(QDataStream::Qt_5_5);
    int command;
    reader>>command;
    if(command==sendMessage)
    {
        QString text;
        reader>>text;
        ui->chat->appendPlainText("Teacher : " + text);
    }
}
