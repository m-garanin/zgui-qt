#include "previewwidget.h"
#include <QPainter>
#include <QFont>
#include <QTimer>

#include <QDebug>

#include "IManager.h"

void myImageCleanupHandler(void *info){
    free(info);
}

PreviewWidget::PreviewWidget(QWidget *parent) :
    QWidget(parent)
{
    this->currentImage = NULL;
    compkey = 0;
}

void PreviewWidget::start(int compkey, int ms)
{
    this->compkey = compkey;
    // заводим таймер
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updatePreview()));
    timer->start(ms);
}

void PreviewWidget::updatePreview()
{
    if(compkey == 0)
        return;
    char* buf = NULL;
    int w,h;
    global_manager->getLastImage(compkey, &buf, &w, &h);
    QImage* pimg = new QImage((uchar*)buf, w, h, QImage::Format_RGB888, &myImageCleanupHandler, buf);
    drawImage(pimg);
}

void PreviewWidget::drawImage(QImage *img)
{
    if(currentImage != NULL)
        delete currentImage;
    currentImage = img;    
    this->update();
}



void PreviewWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    if(currentImage == NULL){
        painter.fillRect(0,0, width(), height(), Qt::red);
        painter.setPen(Qt::blue);
        painter.setFont(QFont("Arial", 30));
        painter.drawText(rect(), Qt::AlignCenter, "NULL");

    }else{
        painter.drawImage(0,0, currentImage->scaled(this->width(), this->height()));
    }

}



