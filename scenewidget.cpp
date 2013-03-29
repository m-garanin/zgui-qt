#include "scenewidget.h"
#include "boxwidget.h"
#include "clonedwidget.h"

#include <QDebug>
#include <QMimeData>
#include <QDrag>
#include <QMenu>
#include <QDropEvent>
#include <QApplication>
#include <QListIterator>
#include <QStringList>
#include <QPainter>
#include <QPen>

namespace {
    const quint32 DEFAULT_CELL_WIDTH = 10;
}
QSize sss;

CSceneWidget::CSceneWidget(qint32 compkey, QWidget *parent) :
    PreviewWidget(compkey, parent),
    _enableDragAndDrop(true)
{   
    _menu = new QMenu(this);

    QAction *action;
    
    action = new QAction(tr("Apply"), this);
    connect(action, SIGNAL(triggered()), SLOT(onApplyTriggered()));
    _menu->addAction(action);

    action = new QAction(tr("Hide boxs"), this);
    connect(action, SIGNAL(triggered()), SLOT(onHideBoxTriggerd()));
    _menu->addAction(action);

    action = new QAction(tr("Show grid"), this);
    action->setCheckable(true);
    connect(action, SIGNAL(triggered(bool)), SLOT(setGridVisible(bool)));
    _menu->addAction(action);

    setCellWidth(DEFAULT_CELL_WIDTH);


    action = new QAction(tr("Clone"), this);
    connect(action, SIGNAL(triggered()), SLOT(onCloneTriggered()));
    _menu->addAction(action);


    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), SLOT(onCustomContextMenuRequested(QPoint)));

    setAcceptDrops(true);
    m_gridEnabled = false;

    sss = size();
}

void CSceneWidget::onCustomContextMenuRequested(const QPoint &point)
{
    Q_UNUSED(point);

    _menu->exec(QCursor::pos());
}

void CSceneWidget::onApplyTriggered()
{
    qDebug() << "TODO: onApplyTriggered()";
    apply();
}

void CSceneWidget::onHideBoxTriggerd()
{
    qDebug() << "TODO: onHideBoxTriggerd()";
    disableLayers();
}

void CSceneWidget::onCloneTriggered()
{
    ClonedWidget * clone = new ClonedWidget(this->getCompkey());
    clone->setAttribute(Qt::WA_DeleteOnClose);
    clone->show();
}

void CSceneWidget::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("widget/x-preview-widget")) {
         QByteArray itemData = event->mimeData()->data("widget/x-preview-widget");
         QDataStream dataStream(&itemData, QIODevice::ReadOnly);

         qint32 index;
         QPoint offset;
         dataStream >> index >> offset;

         _boxWidgetList.at(index)->move(event->pos() - offset);

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
    //QPixmap pix;
    //pix.convertFromImage(pw->image());
    //drag->setPixmap(pix);
    drag->setHotSpot(event->pos() - pw->pos());
    QApplication::setOverrideCursor(Qt::ClosedHandCursor);
    // drag->setDragCursor(); TODO: add pixmap for cursor
    if(drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::MoveAction) == Qt::MoveAction)
    {
    }
    QApplication::restoreOverrideCursor();
}

void CSceneWidget::paintEvent(QPaintEvent *event)
{
    PreviewWidget::paintEvent(event);
    // draw grid;
    if (m_gridEnabled && m_cellWidth > 0) {
        drawGrid();
    }
}

void CSceneWidget::resizeEvent(QResizeEvent *event)
{
    QListIterator<CBoxWidget*> it(_boxWidgetList);

    float scale_w = static_cast<float>(event->size().width()) / event->oldSize().width();
    float scale_h = static_cast<float>(event->size().height()) / event->oldSize().height();

    QPoint pos;
    QSize size;
    while(it.hasNext())
    {
        CBoxWidget *bw = it.next();

        pos.setX((scale_w * bw->x()) + 0.5f);
        pos.setY((scale_h * bw->y()) + 0.5f);
        bw->move(pos);

        size.setWidth((scale_w * bw->width()) + 0.5f);
        size.setHeight((scale_h * bw->height()) + 0.5f);
        bw->resize(size);
    }
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

void CSceneWidget::showBox(int compkey)
{
    _enableDragAndDrop = true;

    QListIterator<CBoxWidget*> it(_boxWidgetList);

    // 
    while(it.hasNext())
    {
        CBoxWidget *bw = it.next();
        if(bw->getCompkey() == compkey)
        {
            bw->enableEditMode(true);
            bw->start();
            bw->show();
            bw->setImageFitMode(PreviewWidget::ImageStretch);
            return;
        }
    }

    it.toFront();
    while(it.hasNext())
    {
        it.next()->enableEditMode(true);
    }

    CBoxWidget *bw = new CBoxWidget(compkey, this);
    bw->setImageFitMode(PreviewWidget::ImageStretch);
    bw->setGeometry(10,10,50,50);
    bw->show();
    _boxWidgetList.push_back(bw);

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

void CSceneWidget::setGridVisible(bool visible)
{
    m_gridEnabled = visible;
    update();
}

void CSceneWidget::setCellWidth(quint32 arg)
{
    m_cellWidth = arg;
}

void CSceneWidget::disableLayers()
{
    QListIterator<CBoxWidget*> it(_boxWidgetList);

    while(it.hasNext())
    {
        CBoxWidget *bw = it.next();
        bw->enableEditMode(true);
        bw->stop();
        bw->hide();
    }
}

void CSceneWidget::startBox()
{
    QListIterator<CBoxWidget*> it(_boxWidgetList);

    while(it.hasNext())
    {
        CBoxWidget *bw = it.next();
        if(!bw->isHidden())
            bw->start();
    }
}

void CSceneWidget::stopBox()
{
    QListIterator<CBoxWidget*> it(_boxWidgetList);

    while(it.hasNext())
    {
        CBoxWidget *bw = it.next();
        if(!bw->isHidden())
            bw->stop();
    }
}

void CSceneWidget::drawGrid()
{
    QPainter painter(this);
    QPen p = painter.pen();
    p.setColor(Qt::gray);
    painter.setPen(p);
    QRect r = this->rect();
    painter.drawRect(r);
    for (int i = m_cellWidth; i < r.width(); i += m_cellWidth) {
        painter.drawLine(i, r.bottom(), i, r.top());
    }
    for (int i = m_cellWidth; i < r.height(); i += m_cellWidth) {
        painter.drawLine(r.left(), i, r.right(), i);
    }

}



















