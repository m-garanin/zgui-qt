#ifndef AIRWIDGET_H
#define AIRWIDGET_H

#include <QToolButton>



class AirWidget : public QToolButton
{
    Q_OBJECT
public:
    explicit AirWidget(QWidget *parent = 0);
    void setOnAir(bool test=false);
    void setStop();

signals:
    
public slots:

};

#endif // AIRWIDGET_H
