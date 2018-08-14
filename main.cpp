#include "widget.h"
#include <QApplication>
#include <videoplayer.h>
#include <QObject>

#undef main
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();

    return a.exec();
}
