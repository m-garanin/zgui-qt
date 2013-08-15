#ifndef IMAGERENDER_H
#define IMAGERENDER_H

#include <QObject>
#include <QImage>
#include <QTimer>

class ImageRender : public QObject
{
    Q_OBJECT
public:
    explicit ImageRender(QString name, QString path, QObject *parent = 0);
    void setFile(QString path);


public slots:
    void updateFrame();

private:
    QString m_name;
    QImage m_image;
    QTimer m_timer;
};

#endif // IMAGERENDER_H
