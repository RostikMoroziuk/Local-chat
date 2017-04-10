#include "interactserver.h"
#include "person.h"

interactServer::interactServer(QWidget *parent)
    : QWidget(parent)
{
    systemTray = new QSystemTrayIcon(this);

    QMenu *contextMenu = new QMenu();
    exit = new QAction("Вихід", this);
    contextMenu->addActions(QList<QAction*>()<<exit);
    systemTray->setContextMenu(contextMenu);
    systemTray->setIcon(QIcon(":/img/book_PNG2113.png"));
    systemTray->show();

    //загружаємо інформацію з файла про користувачів
    QXmlStreamReader xmlReader;
    QXmlStreamAttributes attributes;
    QFile file("info.xml");
    if(file.open(QFile::ReadOnly))
    {
        xmlReader.setDevice(&file);

        while(xmlReader.name()!="teachers")
        {
            xmlReader.readNext();
        }
        while(!(xmlReader.name()=="teachers" && xmlReader.isEndElement()))//поки не кінець списку викладачів
        {
            if(xmlReader.name() == "teacher" && !xmlReader.isEndElement())
            {
                //починаємо парсити кожного адміна
                attributes = xmlReader.attributes();
                teacher.nick = attributes.value("nick").toString();
                teacher.pass = attributes.value("pass").toString();
                teacher.sname = attributes.value("sname").toString();
                teacher.name = attributes.value("name").toString();
            }
            xmlReader.readNext();
        }
        //зчитали workers
        while(xmlReader.name()!="students")
        {
            xmlReader.readNext();
        }
        while(!(xmlReader.name()=="students" && xmlReader.isEndElement()))//поки не кінець списку студентів
        {
            if(xmlReader.name() == "student" && !xmlReader.isEndElement())
            {
                Student s;
                //починаємо парсити кожного працівника
                attributes = xmlReader.attributes();
                s.nick = attributes.value("nick").toString();
                s.pass = attributes.value("pass").toString();
                s.sname = attributes.value("sname").toString();
                s.name = attributes.value("name").toString();
                while(xmlReader.name()!="marks")
                {
                    xmlReader.readNext();
                }
                while(!(xmlReader.name()=="marks" && xmlReader.isEndElement()))
                {
                    if(xmlReader.name()=="mark" && !xmlReader.isEndElement())
                    {
                        attributes = xmlReader.attributes();
                        qDebug()<<attributes.value("mark").toString().toInt();
                        s.marks.push_back(QPair<QString, int>(attributes.value("work").toString(), attributes.value("mark").toString().toInt()));
                    }
                    xmlReader.readNext();
                }
                s.socket=NULL;
                students.insert(s.nick, s);
            }
            if(!xmlReader.atEnd())
                xmlReader.readNext();
        }
        file.close();
    }

    //налаштовуємо серверний сокет на слухання клієнтів
    server = new QTcpServer(this);
    if(!server->listen(QHostAddress::Any, 1234))
    {
        QMessageBox::critical(NULL, "Server error", "Unable to start the server"+server->errorString());
        server->close();
        return;
    }

    teacher.socket=NULL;
    connect(server, SIGNAL(newConnection()), this, SLOT(incommingConnection()));
    connect(exit, SIGNAL(triggered(bool)), this, SLOT(close()));
}

interactServer::~interactServer()
{
    server->close();

    QXmlStreamWriter xmlWriter;
    QFile file("info.xml");
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(0, "Error!", "Error opening file");
    }
    else
    {
        xmlWriter.setDevice(&file);
        xmlWriter.setAutoFormatting(true);

        /* Writes a document start with the XML version number.*/
        xmlWriter.writeStartDocument();
        xmlWriter.writeStartElement("db");
            xmlWriter.writeStartElement("teachers");
                    xmlWriter.writeStartElement("teacher");
                    xmlWriter.writeAttribute("nick",teacher.nick);
                    xmlWriter.writeAttribute("pass",teacher.pass);
                    xmlWriter.writeAttribute("sname",teacher.sname);
                    xmlWriter.writeAttribute("name",teacher.name);
                    xmlWriter.writeEndElement();
            xmlWriter.writeEndElement();

            xmlWriter.writeStartElement("students");
            for(QMap<QString, Student>::iterator wrk = students.begin();wrk!=students.end();wrk++)
            {
                xmlWriter.writeStartElement("student");
                xmlWriter.writeAttribute("nick",wrk.value().nick);
                xmlWriter.writeAttribute("pass",wrk.value().pass);
                xmlWriter.writeAttribute("sname",wrk.value().sname);
                xmlWriter.writeAttribute("name",wrk.value().name);
                    xmlWriter.writeStartElement("marks");
                    for(int j=0;j<wrk.value().marks.size();j++)
                    {
                        xmlWriter.writeStartElement("mark");
                        xmlWriter.writeAttribute("work",wrk.value().marks[j].first);
                        xmlWriter.writeAttribute("mark",QString::number(wrk.value().marks[j].second));
                        xmlWriter.writeEndElement();
                    }
                    xmlWriter.writeEndElement();
                xmlWriter.writeEndElement();
            }
            xmlWriter.writeEndElement();
        xmlWriter.writeEndElement();
        xmlWriter.writeEndDocument();
        file.close();
    }

    delete systemTray;
}

void interactServer::incommingConnection()
{
    //створюємо сокет для керування клієнтом
    QTcpSocket* client = server->nextPendingConnection();
    connect(client, SIGNAL(readyRead()), this, SLOT(listenClient()));//сигнал кидається коли є готові, доступні для читання дані
    connect(client, SIGNAL(disconnected()), client, SLOT(deleteLater()));
}

void interactServer::listenClient()
{
    QTcpSocket *socket = (QTcpSocket*)sender();//sender вертає вказівник на об'єкт, який кинув сигнал
    QDataStream reader(socket);
    int command;
    reader>>command;
    switch(command)
    {
    case authorization:
    {
        QString nickName, password;
        reader>>nickName>>password;
        QMap<QString, Student>::iterator s = students.find(nickName);
        if(teacher.nick == nickName)//якшо таке ім'я було знайдено
        {
            if(teacher.pass==password)//якшо пароль правильний
            {
                QByteArray ba;
                QDataStream msg(&ba, QIODevice::WriteOnly);
                msg.setVersion(QDataStream::Qt_5_5);
                msg<<authorization<<true;
                msg<<true;//викладач
                socket->write(ba);
                break;//зробить вихід
            }
        }
        else if(s!=students.end())
        {
            if(s.value().pass==password)//якшо пароль правильний
            {
                QByteArray ba;
                QDataStream msg(&ba, QIODevice::WriteOnly);
                msg.setVersion(QDataStream::Qt_5_5);
                msg<<authorization<<true;
                msg<<false;//students
                msg<<s.value().marks<<s.value().name<<s.value().sname<<s.value().nick;
                socket->write(ba);
                break;//зробить вихід
            }
        }
        //авторизація неуспішна
        else
        {
            QByteArray ba;
            QDataStream msg(&ba, QIODevice::WriteOnly);
            msg.setVersion(QDataStream::Qt_5_5);
            msg<<authorization<<false;
            socket->write(ba);
            break;
        }
    }
    case addStudent:
    {
        //записуємо нового працівника
        Student s;
        reader>>s.nick>>s.pass>>s.sname>>s.name;
        students.insert(s.nick, s);
        break;
    }
    case deleteStudent:
    {
        QString nickName;
        reader>>nickName;
        students.remove(nickName);
        break;
    }
    case showStudents:
    {
        QByteArray ba;
        QDataStream msg(&ba, QIODevice::WriteOnly);
        msg.setVersion(QDataStream::Qt_5_5);
        msg<<showStudents;
        teacher.socket = socket;
        for(QMap<QString, Student>::iterator it = students.begin();it!=students.end();it++)
        {
            msg<<it.value().sname<<it.value().name<<it.value().nick;
        }
        teacher.socket->write(ba);
        break;
    }
    case sendPrivateMessage://студент відправляє повідомлення викладачу
    {
        QByteArray ba;
        QString fullName, text;
        reader>>fullName>>text;
        QDataStream msg(&ba, QIODevice::WriteOnly);
        msg.setVersion(QDataStream::Qt_5_5);
        msg<<sendPrivateMessage;
        msg<<fullName<<text;
        if(teacher.socket!=NULL)
            teacher.socket->write(ba);
        break;
    }
    case sendMessage://викладач відсилає повідомлення всім студентам
    {
        QByteArray ba;
        QString  text;
        reader>>text;
        QDataStream msg(&ba, QIODevice::WriteOnly);
        msg.setVersion(QDataStream::Qt_5_5);
        msg<<sendMessage;
        msg<<text;
        for(QMap<QString, Student>::iterator it = students.begin(); it!=students.end(); it++)
        {
            if(it.value().socket!=NULL)
                it.value().socket->write(ba);
        }
        break;
    }
    case sendMark:
    {
        QString nick, text;
        int mark;
        reader>>nick>>text>>mark;
        QMap<QString, Student>::iterator it = students.find(nick);
        it.value().marks.push_back(QPair<QString, int>(text, mark));
        break;
    }
    case setSock:
    {
        QString nick;
        reader>>nick;
        QMap<QString, Student>::iterator it = students.find(nick);
        qDebug()<<it.value().nick;
        it.value().socket = socket;
    }
   }
}
