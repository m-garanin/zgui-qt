#include "scenewidget.h"
#include "boxwidget.h"
#include "clonedwidget.h"
#include "rectselectionwidget.h"
#include "IManager.h"

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
    PreviewWidget(compkey, parent)
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

    //setAcceptDrops(true);
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
    hideBoxes();
}

void CSceneWidget::onCloneTriggered()
{
    ClonedWidget * clone = new ClonedWidget(this->getCompkey());
    clone->setAttribute(Qt::WA_DeleteOnClose);
    clone->show();
}


void CSceneWidget::paintEvent(QPaintEvent *event)
{
    PreviewWidget::paintEvent(event);
    // draw grid;
    if (m_gridEnabled && m_cellWidth > 0) {
        drawGrid();
    }
}

void CSceneWidget::toggleBox(int compkey)
{    
    QListIterator<CBoxWidget*> it(_boxWidgetList);

    // если бокс уже есть то переключаем его
    while(it.hasNext())
    {
        CBoxWidget *bw = it.next();
        if(bw->getCompkey() == compkey)
        {
            if(bw->isHidden())
                bw->show();
            else
                bw->hide();
            return;
        }
    }

    // иначе создаём новый бокс
    CBoxWidget *bw = new CBoxWidget(compkey, this);
    bw->setImageFitMode(PreviewWidget::ImageStretch);
    bw->setGeometry(10,10,50,50);
    bw->show();

    _boxWidgetList.push_back(bw);
}

void CSceneWidget::apply()
{
    QListIterator<CBoxWidget*> it(_boxWidgetList);
    QPoint prv_point = this->getTopLeftPoint();
    QSize prv_size = this->getImageSize();
    int pw = prv_size.width();
    int ph = prv_size.height();


    while(it.hasNext())
    {
        CBoxWidget* bw = it.next();
        QPoint bpoint = bw->pos() - prv_point;

        // передаём в ядро геом.параметры бокса в процентах относ. картинки
        global_manager->repositionLayer(bw->getCompkey(),
                                        100.* bpoint.x() / pw,
                                        100.* bpoint.y() / ph,
                                        100.* bw->width() / pw,
                                        100.* bw->height() / ph
                                        );
    }

    hideBoxes();
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

void CSceneWidget::hideBoxes()
{
    QListIterator<CBoxWidget*> it(_boxWidgetList);
    while(it.hasNext())
    {
        CBoxWidget *bw = it.next();
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



















