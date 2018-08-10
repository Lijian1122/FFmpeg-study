#include "widget.h"
#include "ui_widget.h"
#include "qpainter.h"
#include "qpoint.h"
#include "QDebug"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    m_player = new VideoPlayer;

    connect(m_player,SIGNAL(sig_GetOneFrame(QImage)),this,
           SLOT(slotGetOneFrame(QImage)));

    m_player->setFileName("cuc_ieschool.flv");


}

void Widget::run()
{
   m_player->startPlay();
}

void Widget::slotGetOneFrame(QImage img)
{

    qDebug()<<"jieshou...";
    mImage = img;
    update(); //调用update将执行 paintEvent函数
}

void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setBrush(Qt::black);
    painter.drawRect(0, 0, this->width(), this->height()); //先画成黑色

    if (mImage.size().width() <= 0) return;

    ///将图像按比例缩放成和窗口一样大小
    QImage img = mImage.scaled(this->size(),Qt::KeepAspectRatio);

    int x = this->width() - img.width();
    int y = this->height() - img.height();

    x /= 2;
    y /= 2;

    painter.drawImage(QPoint(x,y),img); //画出图像

}
Widget::~Widget()
{
    delete ui;
}
