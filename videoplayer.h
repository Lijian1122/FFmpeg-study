#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QObject>
#include <QWidget>
#include <QThread>
#include <QImage>

extern "C"
{
   #include "libavcodec/avcodec.h"
   #include "libavformat/avformat.h"
   #include "libswscale/swscale.h"
   #include "libavdevice/avdevice.h"
   #include "libavfilter/avfilter.h"
   #include "libavutil/avutil.h"
   #include "libavutil/pixfmt.h"
   #include "libavutil/avconfig.h"

   #include "libavformat/avformat.h"
   #include "libavutil/log.h"

   #include "SDL.h"
   #include "SDL_audio.h"
   #include "SDL_types.h"
   #include "SDL_name.h"
   #include "SDL_main.h"
   #include "SDL_config.h"

}

typedef struct PacketQueue{
    AVPacketList *first_pkt, *last_pkt;
    int nb_packets;
    int size;
    SDL_mutex *mutex;
    SDL_cond *cond;

}PacketQueue;

#define VIDEO_PICTURE_QUEUE_SIZE 1
#define AVCODEC_MAX_AUDIO_FRAME_SIZE 192000

typedef struct VideoState{
    AVCodecContext *aCodecCtx; //音频解码器

    AVFrame *audioFrame; //解码音频过程中使用的缓存

    PacketQueue *audioq;
    double video_clock;

    AVStream *video_st;

}VideoState;

class VideoPlayer : public QThread
{
    Q_OBJECT

public:
   explicit VideoPlayer();
    ~VideoPlayer();

    void setFileName(QString path){mFileName = path;}
    void startPlay();

protected:

    void run();

signals:
    void sig_GetOneFrame(QImage); //没获取到一帧图像 就发送此信号

private:

    QString mFileName;

    VideoState mVedioSate;//用来传递给SDL音频回调函数的数据
};

#endif // VIDEOPLAYER_H
