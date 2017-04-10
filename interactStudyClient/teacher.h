#ifndef TEACHER_H
#define TEACHER_H

#include <QMainWindow>
#include "person.h"
#include "addstudent.h"

namespace Ui {
class Teacher;
}

class Teacher : public QMainWindow
{
    Q_OBJECT

public:
    explicit Teacher(QWidget *parent = 0);
    ~Teacher();
    void addNewStudent(StudentP student);

public slots:
    void readServer();

private slots:
    void on_pushButton_2_clicked();//оцінити студента

    void on_pushButton_clicked();//відправити повідомлення студентам

    void on_pushButton_5_clicked();//видалити студента

    void on_pushButton_4_clicked();//додати студента

private:
    Ui::Teacher *ui;
    QVector<StudentP> students;
    QTcpSocket *socket;
    QDate date;


    enum {authorization, addNStudent, deleteStudent, showStudents, sendPrivateMessage, sendMessage, sendMark, recieveAnswear};
};

#endif // TEACHER_H
