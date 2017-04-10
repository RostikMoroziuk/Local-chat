#ifndef ADDSTUDENT_H
#define ADDSTUDENT_H

#include <QDialog>
#include "teacher.h"

class Teacher;

namespace Ui {
class addStudent;
}

class addStudent : public QDialog
{
    Q_OBJECT

public:
    explicit addStudent(Teacher *, QWidget *parent = 0);
    ~addStudent();

private slots:
    void on_pushButton_clicked();

signals:
    void ok();

private:
    Ui::addStudent *ui;
    Teacher* t;
};

#endif // ADDSTUDENT_H
