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
#include <QScrollArea>

namespace {
    const quint32 DEFAULT_CELL_WIDTH = 10;
    const qreal   DEFAULT_ZOOM_FACTOR = 1.1;


    void zoomWidget(QWidget * w, qreal zoomFactor)
    {
        QRect r = w->geometry();
        QSize s = r.size();
        QPoint c = r.center();
        s *= zoomFactor;
        r.setSize(s);
        r.moveCenter(c * zoomFactor);
        w->setGeometry(r);
    }

}


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

    m_zoomFactor = DEFAULT_ZOOM_FACTOR;
    m_sa = new QScrollArea(parent);
    m_sa->setWidget(this);
    m_sa->setAlignment(Qt::AlignCenter);
    m_sa->setFrameShape(QFrame::NoFrame);
    m_sa->setContentsMargins(0, 0, 0, 0);
    setFocusPolicy(Qt::StrongFocus);
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

void CSceneWidget::resizeEvent(QResizeEvent *event)
{
    qDebug() << "CSceneWidget::resizeEvent: " << event->size();
    PreviewWidget::resizeEvent(event);
}

void CSceneWidget::keyPressEvent(QKeyEvent *event)
{
    QWidget::keyPressEvent(event);
    if (event->key() == Qt::Key_Plus) {
        qDebug() << "Zooming in";
        zoomIn();
    } else if (event->key() == Qt::Key_Minus ) {
        qDebug() << "Zooming out";
        zoomOut();
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

void CSceneWidget::setGeometry(int x, int y, int width, int height)
{
    qDebug() << "CSceneWidget::setGeometry()";
    if (m_initialSize.isEmpty()) {
        m_initialSize = this->size();
        qDebug() << "Initial size: " << m_initialSize;
    }
    m_sa->setGeometry(x, y, width + 1, height + 1); // +1 to eliminate scrollbars
    if (width > m_initialSize.width() && height > m_initialSize.height())
        m_sa->widget()->setGeometry(x, y, width, height);
}

void CSceneWidget::zoomIn()
{
    zoom(m_zoomFactor);
}

void CSceneWidget::zoomOut()
{
    zoom(1 / m_zoomFactor);
}

void CSceneWidget::setZoomFactor(qreal zoomFactor)
{
    if (zoomFactor > 1.0) {
        m_zoomFactor = zoomFactor;
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

void CSceneWidget::zoom(qreal zoomFactor)
{
    QWidget * w = this;
    if (w->size().width() * zoomFactor < m_initialSize.width()
            && w->size().height() * zoomFactor < m_initialSize.height())
        return;

    zoomWidget(w, zoomFactor);
    foreach (CBoxWidget * bw,  _boxWidgetList) {
        zoomWidget(bw, zoomFactor);
    }
}



















