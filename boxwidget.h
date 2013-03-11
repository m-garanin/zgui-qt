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
};

#endif // BOXWIDGET_H
