#include "boxwidget.h"

#include <QDebug>
#include <QPainter>
#include <QApplication>

CBoxWidget::CBoxWidget(QWidget *parent) :
    PreviewWidget(parent),
    _resizeBegin(false),
    _editMode(true)
{}

void CBoxWidget::paintEvent(QPaintEvent *paint)
{
    PreviewWidget::paintEvent(paint);

    if(!_editMode)
        return;

    QPainter painter(this);
    painter.save();
    QPen pen;
    pen.setColor(Qt::black);
    painter.setPen(pen);
    painter.drawRect(size().width() - 10, size().height() - 10, 9, 9);
    painter.drawRect(rect().adjusted(0,0,-1,-1));
    painter.restore();
}

bool CBoxWidget::event(QEvent *event)
{
    if(!_editMode)
        QWidget::event(event);

    if(event->type() == QEvent::MouseMove)
    {
        qDebug() << "das";
        if(_resizeBegin)
        {
            QPoint cursor_pos = mapFromGlobal(QCursor::pos());
            resize(cursor_pos.x(), cursor_pos.y());
            return true;
        }
    }
    if(event->type() == QEvent::MouseButtonPress)
    {
        QPoint cursor_pos = mapFromGlobal(QCursor::pos());

        if((size().width() - 10) < cursor_pos.x() && (size().height() - 10) < cursor_pos.y())
        {
            _resizeBegin = true;
            return true;
        }
    }
    return QWidget::event(event);
}

void CBoxWidget::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);

    if(!_editMode)
        return;

    QApplication::setOverrideCursor(Qt::OpenHandCursor);
}

void CBoxWidget::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);
    
    if(!_editMode)
        return;
    
    QApplication::restoreOverrideCursor();
}

//void PreviewWidget::mousePressEvent(QMouseEvent *event)
//{

//}

void CBoxWidget::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);
}

void CBoxWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    _resizeBegin = false;
    QWidget::mouseReleaseEvent(event);
}

void CBoxWidget::enableEditMode(bool b)
{
    _editMode = b;
    update();
}


























