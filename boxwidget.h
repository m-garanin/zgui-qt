#ifndef BOXWIDGET_H
#define BOXWIDGET_H

#include "previewwidget.h"

class CBoxWidget : public PreviewWidget
{
    Q_OBJECT
public:
    explicit CBoxWidget(QWidget *parent = 0);
    
protected:
    void paintEvent(QPaintEvent *);
    bool event(QEvent *);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

//    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

private:
    bool _resizeBegin;
};

#endif // BOXWIDGET_H
