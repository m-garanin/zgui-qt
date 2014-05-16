#ifndef BKGSOURCE_H
#define BKGSOURCE_H

#include <QThread>
#include <QImage>

class BkgSource : public QThread
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
    explicit BkgSource(QObject *parent = 0);

    void init(int w, int h);
    void setSize(int w, int h) {m_width=w, m_height=h;}
    void setBackground(QString fname);

signals:
    void  yieldFrame(const QImage&);

public slots:

private:
    QImage m_img;
    int m_width, m_height;
    void onTimer();

};

#endif // BKGSOURCE_H
