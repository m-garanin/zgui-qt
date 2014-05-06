#ifndef TESTSOURCE_H
#define TESTSOURCE_H

#include <QThread>
#include <QImage>

class TestSource : public QThread
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
    explicit TestSource(QObject *parent = 0);

    void init(int w, int h);

signals:
    void  yieldFrame(const QImage&);

public slots:

private:
    void onTimer();
    int m_width, m_height;
};

#endif // TESTSOURCE_H
