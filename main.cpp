#include "widget.h"
#include <QApplication>
#include <videoplayer.h>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();

    w.run();

    return a.exec();
}
