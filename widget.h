#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QImage>
#include <QPaintEvent>
#include <videoplayer.h>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void run();

protected:
    void paintEvent(QPaintEvent *event);

public slots:

   void slotGetOneFrame(QImage img);

private:
    Ui::Widget *ui;

    QImage mImage;

    VideoPlayer *m_player;
};

#endif // WIDGET_H
