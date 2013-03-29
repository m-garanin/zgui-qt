#ifndef BOXWIDGET_H
#define BOXWIDGET_H

#include "previewwidget.h"

class CBoxWidget : public PreviewWidget
{
    Q_OBJECT
public:
    explicit CBoxWidget(qint32 compkey, QWidget *parent = 0);
    void enableEditMode(bool);
    
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
    bool _editMode;
};

#endif // BOXWIDGET_H
