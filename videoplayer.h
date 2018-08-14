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
   #include <libavutil/time.h>
   #include "libavutil/pixfmt.h"
   #include "libswscale/swscale.h"
   #include "libswresample/swresample.h"


   #include <SDL.h>
   #include <SDL_audio.h>
   #include <SDL_types.h>
   #include <SDL_name.h>
   #include <SDL_main.h>
   #include <SDL_config.h>

}

typedef struct PacketQueue {
    AVPacketList *first_pkt, *last_pkt;
    int nb_packets;
    int size;
    SDL_mutex *mutex;
    SDL_cond *cond;
} PacketQueue;

#define VIDEO_PICTURE_QUEUE_SIZE 1
#define AVCODEC_MAX_AUDIO_FRAME_SIZE 192000 // 1 second of 48khz 32bit audio

#define MAX_AUDIO_SIZE (25 * 16 * 1024)
#define MAX_VIDEO_SIZE (25 * 256 * 1024)

class VideoPlayer;  //前置声明

typedef struct VideoState {
    AVFormatContext *ic;
    AVCodecContext *aCodecCtx; //音频解码器
    //AVFrame *audioFrame;// 解码音频过程中的使用缓存
    AVFrame *audio_frame;// 解码音频过程中的使用缓存
    PacketQueue audioq;
    AVStream *audio_st; //音频流

    unsigned int audio_buf_size;
    unsigned int audio_buf_index;

    AVPacket audio_pkt;
    uint8_t *audio_pkt_data;
    int audio_pkt_size;
    uint8_t *audio_buf;
    DECLARE_ALIGNED(16,uint8_t,audio_buf2) [AVCODEC_MAX_AUDIO_FRAME_SIZE * 4];

    enum AVSampleFormat audio_src_fmt;
    enum AVSampleFormat audio_tgt_fmt;

    int audio_src_channels;
    int audio_tgt_channels;

    int64_t audio_src_channel_layout;
    int64_t audio_tgt_channel_layout;
    int audio_src_freq;
    int audio_tgt_freq;

    struct SwrContext *swr_ctx; //用于解码后的音频格式转换

    int audio_hw_buf_size;

    double video_clock; ///<pts of last decoded frame / predicted pts of next decoded frame
    double audio_clock;

    AVStream *video_st;
    PacketQueue videoq;

    SDL_Thread *video_tid;
    SDL_AudioDeviceID audioID;

    VideoPlayer *player;

} VideoState;

class VideoPlayer : public QThread
{
    Q_OBJECT

public:
   explicit VideoPlayer();
    ~VideoPlayer();

    void setFileName(QString path){mFileName = path;}
    void startPlay();

    void displayVideo(QImage img);

protected:

    void run();

signals:
    void sig_GetOneFrame(QImage); //没获取到一帧图像 就发送此信号

private:

    QString mFileName;

    VideoState mVideoState;//用来传递给SDL音频回调函数的数据
};

#endif // VIDEOPLAYER_H
