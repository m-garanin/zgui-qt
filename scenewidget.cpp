#include "scenewidget.h"

#include <QDebug>
#include <QMimeData>
#include <QDrag>

CSceneWidget::CSceneWidget(QWidget *parent) :
    QWidget(parent)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), SLOT(onCustomContextMenuRequested(QPoint)));

    _contextMenu = new QMenu(this);
    QAction *action;

    action = new QAction("Add preview widget", this);
    connect(action, SIGNAL(triggered()), this, SLOT(onAddPreviewWidget()));
    _contextMenu->addAction(action);

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

         QRect rect(event->pos() - offset, _prevWidgetList.at(index)->geometry().size());
         _prevWidgetList.at(index)->setGeometry(rect);

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

    PreviewWidget *pw = _prevWidgetList[index];

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

    for(qint32 i = 0; i < _prevWidgetList.count(); ++i)
    {
        if( _prevWidgetList.at(i)->geometry().intersects(rect) )
            return i;
    }

    return -1;
}

void CSceneWidget::onCustomContextMenuRequested(const QPoint &point)
{
    Q_UNUSED(point);

   _contextMenu->exec(QCursor::pos());
}

void CSceneWidget::onAddPreviewWidget()
{
    PreviewWidget *pw = new PreviewWidget(this);
    pw->setGeometry(10,10,50,50);
    pw->show();
    //pw->installEventFilter(this);
    _prevWidgetList.push_back(pw);

    qDebug() << "Add PreviewWidget";
}

void CSceneWidget::showBox(int layer_id)
{
    Q_UNUSED(layer_id);

    onAddPreviewWidget();
}





























