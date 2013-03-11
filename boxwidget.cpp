#include "boxwidget.h"

#include <QDebug>
#include <QPainter>

CBoxWidget::CBoxWidget(QWidget *parent) :
    PreviewWidget(parent)
{}

void CBoxWidget::paintEvent(QPaintEvent *paint)
{
    PreviewWidget::paintEvent(paint);

    QPainter painter(this);
    painter.save();
    QPen pen;
    pen.setColor(Qt::black);
    painter.setPen(pen);
    painter.drawRect(size().width() - 10, size().height() - 10, 9, 9);
    painter.drawRect(rect().adjusted(0,0,-1,-1));
    painter.restore();
}





























