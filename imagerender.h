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

signals:
    void newFile(QString);

public slots:
    void switchImage(bool next=true);
    void updateFrame();

private:
    QString m_name;
    QImage m_image;
    QSize m_size;
    QTimer m_timer;
    QString m_fname;
};

#endif // IMAGERENDER_H
