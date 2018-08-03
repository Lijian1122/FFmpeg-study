#include "videoplayer.h"
#include "windows.h"
#include "qdebug.h"
#include "qtimer.h"
#include "qeventloop.h"

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

}


VideoPlayer::VideoPlayer()
{

}

void VideoPlayer::run()
{
    char *file_path = "cuc_ieschool.flv";

    AVFormatContext *pFormatCtx;
    AVCodecContext *pCodecCtx;
    AVCodec *pCodec;
    AVFrame *pFrame, *pFrameRGB;
    AVPacket *packet;
    uint8_t *out_buffer;

    static struct SwsContext *img_convert_ctx;
    int videoStream,i,numBytes;
    int ret, got_picture;

    //初始化FFMEG
    av_register_all();

    //分配一个AVFormatContext
    pFormatCtx = avformat_alloc_context();

    //打开视频文件
    if(avformat_open_input(&pFormatCtx,file_path,NULL,NULL) != 0)
    {
        printf("cant't open file");
        return ;
    }

    if (avformat_find_stream_info(pFormatCtx, NULL) < 0)
    {
            printf("Could't find streaminfomation.");
            return;
    }

    videoStream = -1;

    //循环查找视频中包含的流信息，直到找到视频类型的流
    //便将其记录下来 保存到videoStream变量中
    for(i = 0; i < pFormatCtx->nb_streams; i++)
    {
        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            videoStream = i;
        }
    }

    //如果videoStream为-1 说明没有找到视频流
    if (videoStream == -1)
    {
         printf("Didn't find a video stream.");
         return;
    }

    //查找解码器
    pCodecCtx = pFormatCtx->streams[videoStream]->codec;
    pCodec = avcodec_find_decoder(pCodecCtx->codec_id);

    if (pCodec == NULL)
    {
        printf("Codec not found.");
        return ;
    }

    //打开解码器
    if(avcodec_open2(pCodecCtx, pCodec, NULL) < 0)
    {
        printf("Could not open codec.");
        return;
    }

    pFrame = av_frame_alloc();
    pFrameRGB = av_frame_alloc();

    img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height,
               pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height,
               AV_PIX_FMT_RGB32, SWS_BICUBIC, NULL, NULL, NULL);

    //int width = pCodecCtx>width;
    //int height = pCodecCtx>height;
    numBytes =avpicture_get_size(AV_PIX_FMT_RGB32,
                                 pCodecCtx->width,pCodecCtx->height);

    out_buffer = (uint8_t *)av_malloc(numBytes * sizeof(uint8_t));

    avpicture_fill((AVPicture *)pFrameRGB, out_buffer, AV_PIX_FMT_RGB32,
                pCodecCtx->width, pCodecCtx->height);

    //读取视频
    int y_size = pCodecCtx->width * pCodecCtx->height;

    //分配一个packet
    packet = (AVPacket*)malloc(sizeof(AVPacket));
    av_new_packet(packet, y_size); //分配packet的数据

    av_dump_format(pFormatCtx, 0, file_path, 0); //输出视频信息

    //int index = 0;

    while(1)
    {
        if(av_read_frame(pFormatCtx, packet) < 0)
        {
          break; //这里认为视频读取完了
        }
        if(packet->stream_index == videoStream)
        {
             ret = avcodec_decode_video2(pCodecCtx, pFrame,
                                         &got_picture,packet);

             if (ret < 0)
             {
                      printf("decode error.");
             }

             if (got_picture)
             {
                   sws_scale(img_convert_ctx,
                            (uint8_t const * const *) pFrame->data,
                             pFrame->linesize, 0, pCodecCtx->height,
                             pFrameRGB->data,
                             pFrameRGB->linesize);

                            //saveAsBitmap(pFrameRGB, pCodecCtx->width,
                               // pCodecCtx->height,index++); //保存图片

                   //把这个RGB数据 用QImage加载
                   QImage tmpImg((uchar *)out_buffer,pCodecCtx->width,
                                 pCodecCtx->height,QImage::Format_RGB32);

                   QImage image = tmpImg.copy();
                   emit sig_GetOneFrame(image);

                   QEventLoop eventloop;
                   QTimer::singleShot(40, &eventloop, SLOT(quit()));
                   eventloop.exec();

                   qDebug()<<"fa song ..";

              }
        }
        av_free_packet(packet);
    }

    av_free(out_buffer);
    av_free(pFrameRGB);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);

    return;
}

VideoPlayer::~VideoPlayer()
{

}
