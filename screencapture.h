#ifndef SCREENCAPTURE_H
#define SCREENCAPTURE_H

#include <QObject>
#include <QTimer>
#include <QRect>
#include "rectselectionwidget.h"

class ScreenCapture : public QObject
{
    Q_OBJECT
public:
    explicit ScreenCapture(QString name, RectSelectionWidget* widg, QObject *parent = 0);
    
signals:
    
public slots:
    void updateFrame();

private:
    QString m_name;
    //QRect m_rect;
    QTimer m_timer;
    RectSelectionWidget* m_widg;
};

#endif // SCREENCAPTURE_H
