#include "scenewidget.h"

#include <QDebug>
#include <QMimeData>
#include <QDrag>

CSceneWidget::CSceneWidget(QWidget *parent) :
    PreviewWidget(parent)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), SLOT(onCustomContextMenuRequested(QPoint)));

    setAcceptDrops(true);
}


void CSceneWidget::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("widget/x-preview-widget")) {
         QByteArray itemData = event->mimeData()->data("widget/x-preview-widget");
         QDataStream dataStream(&itemData, QIODevice::ReadOnly);

         qint32 index;
         QPoint offset;
         dataStream >> index >> offset;

         QRect rect(event->pos() - offset, _boxWidgetList.at(index)->geometry().size());
         _boxWidgetList.at(index)->setGeometry(rect);

         event->accept();
     } else {
         event->ignore();
     }
}

void CSceneWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("widget/x-preview-widget")) {
        event->accept();
    } else {
        event->ignore();
    }
}

void CSceneWidget::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("widget/x-preview-widget")) {
        event->accept();
    } else {
        event->ignore();
    }
}

void CSceneWidget::mousePressEvent(QMouseEvent *event)
{
    qint32 index = findPreviewWidget(event->pos());

    qDebug() << index;
    if(index == -1)
        return;

    CBoxWidget *pw = _boxWidgetList[index];

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream <<  index << QPoint(event->pos() - pw->pos());

    QMimeData *mimeData = new QMimeData;
    mimeData->setData("widget/x-preview-widget", itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
//    QPixmap pix;
//    pix.convertFromImage(pw->image());
//    drag->setPixmap(pix);
    drag->setHotSpot(event->pos() - pw->pos());

    if(drag->exec(Qt::MoveAction) == Qt::MoveAction)
    {
    }
}

qint32 CSceneWidget::findPreviewWidget(const QPoint &point)
{
    QRect rect(point, QSize(1,1));

    for(qint32 i = 0; i < _boxWidgetList.count(); ++i)
    {
        if( _boxWidgetList.at(i)->geometry().intersects(rect) )
            return i;
    }

    return -1;
}

void CSceneWidget::showBox(int layer_id)
{
    Q_UNUSED(layer_id);

    CBoxWidget *pw = new CBoxWidget(this);
    pw->setGeometry(10,10,50,50);
    pw->show();
    _boxWidgetList.push_back(pw);

    qDebug() << "Add PreviewWidget";
}





























