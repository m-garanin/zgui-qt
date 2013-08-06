#ifndef IMAGERENDER_H
#define IMAGERENDER_H

#include <QObject>
#include <QImage>

class ImageRender : public QObject
{
    Q_OBJECT
public:
    explicit ImageRender(QString name, QString path, QObject *parent = 0);
    void setFile(QString path);

private:
    QString m_name;
    QImage m_image;    
};

#endif // IMAGERENDER_H
