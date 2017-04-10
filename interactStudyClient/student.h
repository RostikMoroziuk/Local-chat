#ifndef STUDENT_H
#define STUDENT_H

#include <QMainWindow>
#include <QtNetwork/QTcpSocket>

namespace Ui {
class Student;
}

class Student : public QMainWindow
{
    Q_OBJECT

public:
    explicit Student(QVector<QPair<QString, int> >  marks, QString name, QString nick);
    ~Student();

public slots:
    void readServer();

private slots:
    void on_pushButton_clicked();//відправити повідомлення

private:
    Ui::Student *ui;
    QTcpSocket *socket;//тільки надсилати до сервера
    QString fullName;
    QString nick;

    enum {authorization, addStudent, deleteStudent, showStudents, sendPrivateMessage, sendMessage, sendWork, setSock};
};

#endif // STUDENT_H
