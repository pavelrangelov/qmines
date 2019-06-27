#include "mainwindow.h"

///////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setOrganizationName	(ORG_NAME);
    app.setApplicationName	(APP_NAME);

    MainWindow win;
    win.show();

    return app.exec();
}
