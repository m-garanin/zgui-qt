#ifndef SOURCE_H
#define SOURCE_H

#include <QObject>

class Source: public QObject
{
    Q_OBJECT

public:
    explicit Source(QObject *parent = 0);

signals:
    //void  yieldFrame(Frame*);

};

#endif // SOURCE_H
