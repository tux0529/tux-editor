#include "mainwindow.h"
#include "version.h"
#include "common.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName(COMPANY_NAME);
    QCoreApplication::setApplicationName(PRODUCT_NAME);
    QCoreApplication::setApplicationVersion(PRODUCT_VERSION_STR);
    QFile qss(":/Gray.css");
    qss.open(QFile::ReadOnly);
    qApp->setStyleSheet(qss.readAll());
    qss.close();

    MainWindow mw;
    QStringList args = app.arguments();
    if(args.size() > 2){
        mw.open(args.at(1), args.at(2).toInt());
    }
    else if(args.size() > 1){
        mw.open(args.at(1));
    }
    mw.show();

    return app.exec();
}
