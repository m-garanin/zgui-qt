#include "scenewidget.h"
#include "boxwidget.h"

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
    const quint32 DEFAULT_CELL_WIDTH = 32;
}

CSceneWidget::CSceneWidget(qint32 compkey, QWidget *parent) :
    PreviewWidget(compkey, parent),
    _enableDragAndDrop(true)
{   
    _menu = new QMenu(this);

    QAction *action;
    
    action = new QAction("Apply", this);
    connect(action, SIGNAL(triggered()), SLOT(onApplyTriggered()));
    _menu->addAction(action);

    action = new QAction("Hide boxs", this);
    connect(action, SIGNAL(triggered()), SLOT(onHideBoxTriggerd()));
    _menu->addAction(action);

    m_showGridAction = new QAction(tr("Show grid"), this);
    connect(m_showGridAction, SIGNAL(triggered()), SLOT(onShowGrid()));
    _menu->addAction(m_showGridAction);

    m_hideGridAction = new QAction(tr("Hide grid"), this);
    connect(m_hideGridAction, SIGNAL(triggered()), SLOT(onHideGrid()));
    _menu->addAction(m_hideGridAction);
    m_hideGridAction->setVisible(false);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), SLOT(onCustomContextMenuRequested(QPoint)));

    setAcceptDrops(true);
    m_gridEnabled = false;
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

void CSceneWidget::onShowGrid()
{
    m_showGridAction->setVisible(false);
    m_hideGridAction->setVisible(true);
    showGrid(DEFAULT_CELL_WIDTH);
}

void CSceneWidget::onHideGrid()
{
    m_hideGridAction->setVisible(false);
    m_showGridAction->setVisible(true);
    hideGrid();
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

void CSceneWidget::paintEvent(QPaintEvent *event)
{
    PreviewWidget::paintEvent(event);
    // draw grid;
    if (m_gridEnabled && m_cellWidth > 0) {
        drawGrid();
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

void CSceneWidget::showGrid(quint32 cell_width)
{
    m_cellWidth = cell_width;
    m_gridEnabled = true;
    update();
}

void CSceneWidget::hideGrid()
{
    m_gridEnabled = false;
    update();
}



















