#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QObject>
#include <QWidget>
#include <QThread>
#include <QImage>

class VideoPlayer : public QThread
{
    Q_OBJECT

public:
   explicit VideoPlayer();
    ~VideoPlayer();

protected:

    void run();

signals:
    void sig_GetOneFrame(QImage); //没获取到一帧图像 就发送此信号
};

#endif // VIDEOPLAYER_H
