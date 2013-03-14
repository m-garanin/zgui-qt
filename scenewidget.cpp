#include "scenewidget.h"

#include <QDebug>
#include <QMimeData>
#include <QDrag>
#include <QApplication>
#include <QListIterator>
#include <QStringList>

CSceneWidget::CSceneWidget(qint32 compkey, QWidget *parent) :
    PreviewWidget(compkey, parent),
    _enableDragAndDrop(true)
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
    if(!_enableDragAndDrop)
        return;

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
    QPixmap pix;
    pix.convertFromImage(pw->image());
    drag->setPixmap(pix);
    drag->setHotSpot(event->pos() - pw->pos());
    QApplication::setOverrideCursor(Qt::ClosedHandCursor);
    // drag->setDragCursor(); TODO: add pixmap for cursor
    if(drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::MoveAction) == Qt::MoveAction)
    {
    }
    QApplication::restoreOverrideCursor();
}

qint32 CSceneWidget::findPreviewWidget(const QPoint &point)
{
    QRect rect(point, QSize(1,1));

    for(qint32 i = _boxWidgetList.count()-1; i >= 0 ; --i)
    {
        if( _boxWidgetList.at(i)->geometry().intersects(rect) )
            return i;
    }

    return -1;
}

void CSceneWidget::showBox(int layer_id)
{
    Q_UNUSED(layer_id);

    QListIterator<CBoxWidget*> it(_boxWidgetList);

    while(it.hasNext())
    {
        it.next()->enableEditMode(true);
    }

    CBoxWidget *pw = new CBoxWidget(layer_id, this);
    pw->setGeometry(10,10,50,50);
    pw->show();
    _boxWidgetList.push_back(pw);
    _enableDragAndDrop = true;

    qDebug() << "Add PreviewWidget";
}

QStringList CSceneWidget::apply()
{
    QStringList list;

    QListIterator<CBoxWidget*> it(_boxWidgetList);

    while(it.hasNext())
    {
        CBoxWidget* bw = it.next();
        bw->enableEditMode(false);
        list.push_back(QString("%1x%2").arg(bw->pos().x()).arg(bw->pos().y()));
    }
    _enableDragAndDrop = false;

    return list;
}





























