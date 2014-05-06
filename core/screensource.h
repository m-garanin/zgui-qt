#ifndef SCREENSOURCE_H
#define SCREENSOURCE_H

#include <QThread>
#include <QRect>

class ScreenSource : public QThread
{
    Q_OBJECT
    void run() Q_DECL_OVERRIDE{
        forever{
            if ( QThread::currentThread()->isInterruptionRequested() )
                 return;
            onTimer();
            msleep(40);
        }
    }
public:
    explicit ScreenSource(QObject *parent = 0);
    void init(QRect rect);

signals:
    void  yieldFrame(const QImage&);

public slots:

private:
    QRect m_rect;
    void onTimer();
};

#endif // SCREENSOURCE_H
