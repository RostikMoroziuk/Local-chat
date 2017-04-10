#ifndef INTERACTSERVER_H
#define INTERACTSERVER_H

#include <QWidget>
#include <QMenu>
#include <QXmlStreamReader>
#include <QXmlStreamAttributes>
#include <QFile>
#include <QSystemTrayIcon>
#include <QTcpServer>
#include "person.h"
#include <QMessageBox>

class interactServer : public QWidget
{
    Q_OBJECT

public:
    interactServer(QWidget *parent = 0);
    ~interactServer();
private:
    QTcpServer *server;
    Teacher teacher;
    QMap<QString, Student> students;
    QSystemTrayIcon* systemTray;
    QAction *exit;

    enum {authorization, addStudent, deleteStudent, showStudents, sendPrivateMessage, sendMessage, sendMark, setSock};
public slots:
    void incommingConnection();
    void listenClient();
};

#endif // INTERACTSERVER_H
