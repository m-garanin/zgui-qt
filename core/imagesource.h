#ifndef IMAGESOURCE_H
#define IMAGESOURCE_H

#include <QObject>
#include <QImage>

class ImageSource : public QObject
{
    Q_OBJECT
public:
    explicit ImageSource(QObject *parent = 0);
    void setFile(QString path);

signals:
    void  yieldFrame(const QImage&);

    void newFile(QString);

public slots:
    void switchImage(bool next=true);
    void selectImage();
    // TODO: void onDeleteLayer();

private:
    QString m_fname;
    QImage m_image;
    QSize m_size;
};

#endif // IMAGESOURCE_H
