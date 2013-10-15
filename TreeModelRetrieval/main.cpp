#include "treemodelretrievaldialog.h"
#include <QApplication>
#include <QDebug>
#include <QCoreApplication>
#include <QDir>


int main(int argc, char *argv[])
{
    qDebug()<<"current applicationDirPath: "<<QCoreApplication::applicationDirPath();
    qDebug()<<"current currentPath: "<<QDir::currentPath();
    qDebug() <<"start";

    QApplication a(argc, argv);
    TreeModelRetrievalDialog w;
    w.show();
    
    return a.exec();
}
