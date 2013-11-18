#ifndef IMAGERENDER_H
#define IMAGERENDER_H

#include <QObject>
#include <QImage>
#include <QTimer>
#include "IManager.h"

class ImageRender : public QObject
{
    Q_OBJECT
public:
    explicit ImageRender(QString name, QObject *parent = 0);
    void setFile(QString path);

signals:
    void newFile(QString);

public slots:
    void switchImage(bool next=true);
    void updateFrame();
    void onDeleteLayer();

private:
    IExternalSource* m_pOut;
    QString m_name;
    QImage m_image;
    QSize m_size;
    QTimer m_timer;
    QString m_fname;
};

#endif // IMAGERENDER_H
