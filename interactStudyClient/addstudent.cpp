#include "addstudent.h"
#include "ui_addstudent.h"
#include "person.h"

addStudent::addStudent(Teacher* teacher, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addStudent)
{
    ui->setupUi(this);
    t = teacher;
}

addStudent::~addStudent()
{
    delete ui;
}

void addStudent::on_pushButton_clicked()
{
    StudentP student;
    student.name = ui->name->text();
    student.sname = ui->surname->text();
    student.nick = ui->nickName->text();
    student.pass = ui->password->text();
    t->addNewStudent(student);
    emit ok();
    close();
}
