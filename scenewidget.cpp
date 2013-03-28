#include "scenewidget.h"

#include <QWheelEvent>
#include <QPushButton>
#include <QLabel>
#include <qmath.h>

#include "IManager.h"
#include "clonedwidget.h"
#include "settingsmanager.h"

#ifndef QT_NO_OPENGL
#include <QtOpenGL>
#endif


namespace {
    const qint32 RESIZE_BOX = 15;
    const qint32 MIN_RESIZE = 50;
}

CSceneWidget::CSceneWidget(qint32 compkey, qint32 width, qint32 height, QWidget *parent) :
    QGraphicsView(parent),
    _compkey(compkey),
    _currentItem(NULL),
    m_currentImage(NULL),
    _resizeBegin(false),
    m_gridEnabled(false),
    m_cellWidth(10),
    _timerId(0),
    _aspectRatioMode(Qt::KeepAspectRatio)
{
    initSceneMenu();
    initItemsMenu();

    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(0,0,width,height);
    setScene(scene);
    setSceneRect(0,0,width,height);
    setTransformationAnchor(AnchorUnderMouse);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setOptimizationFlags(QGraphicsView::DontAdjustForAntialiasing
                                      | QGraphicsView::DontClipPainter
                                      | QGraphicsView::DontSavePainterState);

    setMouseTracking(true);

    CGraphicsItem *background = new CGraphicsItem(_compkey);
    background->setPos(0,0);
    background->setSize(QSize(width,height));
    background->setImageFitMode(CGraphicsItem::ImageStretch/*ImageFit*/);
    scene->addItem(background);
    //scene->addWidget(new QLabel("use +/- for zoming")); // TODO: tempory removed

    SettingsManager setting("Video");
    setEnabledOpenGl(setting.getBoolValue("OpenGL"));
    _timerId = startTimer(1000 / 25);
}

void CSceneWidget::setEnabledOpenGl(bool enable)
{
#ifndef QT_NO_OPENGL
    setViewport(enable?new QGLWidget(QGLFormat(QGL::SampleBuffers | QGL::DirectRendering)):new QWidget);
#endif
}

void CSceneWidget::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);

    fitInView(scene()->sceneRect(), _aspectRatioMode);
}

void CSceneWidget::initSceneMenu()
{
    _sceneMenu = new QMenu(this);

    QAction *action;

    action = new QAction(tr("Apply"), this);
    connect(action, SIGNAL(triggered()), SLOT(onApplyTriggered()));
    _sceneMenu->addAction(action);

    action = new QAction(tr("Hide boxs"), this);
    action->setCheckable(true);
    connect(action, SIGNAL(triggered(bool)), SLOT(onHideBoxsTriggerd(bool)));
    _sceneMenu->addAction(action);

    action = new QAction(tr("Show grid"), this);
    action->setCheckable(true);
    connect(action, SIGNAL(triggered(bool)), SLOT(setGridVisible(bool)));
    _sceneMenu->addAction(action);

    action = new QAction(tr("Clone"), this);
    connect(action, SIGNAL(triggered()), SLOT(onCloneTriggered()));
    _sceneMenu->addAction(action);
}

void CSceneWidget::initItemsMenu()
{
    _itemsMenu = new QMenu(this);

    QAction *action;

    action = _itemsMenu->addAction(tr("Up"));
    connect(action, SIGNAL(triggered()), SLOT(onOrderUpTriggered()));

    action = _itemsMenu->addAction(tr("Down"));
    connect(action, SIGNAL(triggered()), SLOT(onOrderDownTriggered()));

    action = _itemsMenu->addAction(tr("Hide box"));
    connect(action, SIGNAL(triggered()), SLOT(onHideBoxTriggerd()));

    action = _itemsMenu->addAction(tr("Keep Aspect Ratio"));
    action->setCheckable(true);
    connect(action, SIGNAL(triggered(bool)), SLOT(onKeepAspectRatioTriggered(bool)));

    QMenu *orderMenu = _itemsMenu->addMenu(tr("Opacity"));
    QActionGroup *actionGroup = new QActionGroup(this);
    for(qint32 i = 10; i <= 100; i+=10)
    {
        actionGroup->addAction(QString("%1").arg(i));
        actionGroup->actions().last()->setProperty("opacity", qreal(i));
        actionGroup->actions().last()->setCheckable(true);
        connect(actionGroup->actions().last(), SIGNAL(triggered()), SLOT(onOpacityTriggered()));
    }
    actionGroup->actions().last()->setChecked(true);
    orderMenu->addActions(actionGroup->actions());
}

void CSceneWidget::showBox(qint32 compkey)
{
    QListIterator<QGraphicsItem*> it(scene()->items());
    while(it.hasNext())
    {
        CGraphicsItem *gi = qgraphicsitem_cast<CGraphicsItem*>(it.next());
        if(gi->getCompkey() == compkey)
        {
            gi->setEditMode(true);
            gi->setImageFitMode(CGraphicsItem::ImageStretch);
            return;
        }
    }

    CGraphicsItem *item = new CGraphicsItem(compkey);
    item->setEditMode(true);
    item->setPos(scene()->items().count() * 10,scene()->items().count() * 10);
    item->setImageFitMode(CGraphicsItem::ImageStretch);
    item->setZValue(100.0);
    scene()->addItem(item);
}

qint32 CSceneWidget::getCompkey() const
{
    return _compkey;
}

void CSceneWidget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    QListIterator<QGraphicsItem*> it(scene()->items());
    while(it.hasNext())
    {
        it.next()->update();
    }
}

void CSceneWidget::paintEvent(QPaintEvent *event)
{
    QGraphicsView::paintEvent(event);
}

void CSceneWidget::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->fillRect(rect, Qt::black);
    QGraphicsView::drawBackground(painter, rect);
}

void CSceneWidget::drawForeground(QPainter *painter, const QRectF &rect)
{
    if (m_gridEnabled && m_cellWidth > 0) {
        drawGrid(painter);
    }
    QGraphicsView::drawForeground(painter, rect);
}

void CSceneWidget::mouseMoveEvent(QMouseEvent * event)
{
    if(_currentItem != 0)
    {
        if(_resizeBegin)
        {
            QPointF p = mapToScene(event->pos()) - _currentItem->pos();
            if(_currentItem->imageFitMode() == CGraphicsItem::ImageFit)
            {
                QSizeF ds = QSizeF(p.x(),p.y()) - _currentItem->boundingRect().size();
                qreal aspect = qMin(ds.width(), ds.height());
                _currentItem->setSize(QSize(_currentItem->imageSize().width() + aspect, _currentItem->imageSize().height() + aspect));
            }
            else
            {
                if(p.x() < MIN_RESIZE && p.y() > MIN_RESIZE)
                    _currentItem->setSize(QSize(50, p.y()));

                if(p.x() > MIN_RESIZE && p.y() < MIN_RESIZE)
                    _currentItem->setSize(QSize(p.x(), MIN_RESIZE));

                if(p.x() > MIN_RESIZE && p.y() > MIN_RESIZE)
                    _currentItem->setSize(QSize(p.x(), p.y()));
            }
        }
        else
            _currentItem->setPos(event->pos() - _offsetMove);
    }
    QGraphicsView::mouseMoveEvent(event);
}

void CSceneWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() != Qt::LeftButton)
    {
        QGraphicsView::mousePressEvent(event);
        return;
    }

    if(QGraphicsItem *item = itemAt(event->pos()))
    {
        _currentItem = qgraphicsitem_cast<CGraphicsItem*>(item);

        if(_currentItem != 0 && !_currentItem->isEditMode())
        {
            _currentItem = 0;
            return;
        }

        QRectF rect = _currentItem->sceneBoundingRect();
        rect.adjust(0,0,rect.x() -RESIZE_BOX, rect.y() -RESIZE_BOX);
        QPoint point = mapFromScene(rect.width(),rect.height());

        if(point.x()  < event->pos().x() && point.y() < event->pos().y())
        {
            _resizeBegin = true;
        }

        _offsetMove = event->pos() - item->pos();
    }
    QGraphicsView::mousePressEvent(event);
}

void CSceneWidget::mouseReleaseEvent ( QMouseEvent * event )
{
    _resizeBegin = false;
    _currentItem = 0;
    QGraphicsView::mouseReleaseEvent(event);
}

void CSceneWidget::contextMenuEvent(QContextMenuEvent *event)
{
    if(QGraphicsItem *item = itemAt(event->pos()))
    {
        _currentItem = qgraphicsitem_cast<CGraphicsItem *>(item);
        if(!qFuzzyCompare(item->zValue(), qreal(0.0))) // ignore first item, first item is background
        {
            QListIterator<QAction *> it(_itemsMenu->actions());
            while(it.hasNext())
            {
                QAction *action = it.next();
                if(action->property("action").toString().compare("hidebox") == 0)
                {
                    action->setChecked(!_currentItem->isEditMode());
                    action->setText(_currentItem->isEditMode()?tr("Hide box"):tr("Show box"));
                    break;
                }
            }
            _itemsMenu->exec(event->globalPos());
        }
        else
        {
            _sceneMenu->exec(event->globalPos());
        }
    }

    _currentItem = 0;
    update();
}

void CSceneWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_Plus:
            onZoomIn();
        break;
        case Qt::Key_Minus:
            onZoomOut();
        break;
        default:
            QGraphicsView::keyPressEvent(event);
    }
}

void CSceneWidget::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
}

void CSceneWidget::onZoomIn()
{
    scaleView(qreal(1.2));
}

void CSceneWidget::onZoomOut()
{
    scaleView(1 / qreal(1.2));
}


void CSceneWidget::wheelEvent(QWheelEvent *event)
{
//    qreal factor = qPow(1.2, event->delta() / 240.0);
//    scale(factor, factor);
//    event->accept();
    QGraphicsView::wheelEvent(event);
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

void CSceneWidget::onApplyTriggered()
{
    qDebug() << apply();
}

void CSceneWidget::onHideBoxsTriggerd(bool triggerd)
{
    QListIterator<QGraphicsItem*> it(scene()->items());
    while(it.hasNext())
    {
        CGraphicsItem *gi = qgraphicsitem_cast<CGraphicsItem*>(it.next());
        if(!qFuzzyCompare(gi->zValue(), qreal(0.0)))
        {
            gi->setEditMode(!triggerd);
            gi->setVisible(!triggerd);
        }
    }

    if(QAction *action = qobject_cast<QAction*>(sender()))
    {
        if(triggerd)
            action->setText(tr("Show boxs"));
        else
            action->setText(tr("Hide boxs"));
    }
    update();
}

void CSceneWidget::drawGrid(QPainter *painter)
{
    painter->setRenderHint(QPainter::Antialiasing, true);

    qint32 m_cellWidth = 10;
    QPen p = painter->pen();
    p.setColor(Qt::gray);
    painter->setPen(p);
    QRectF r = sceneRect();
    painter->drawRect(r);
    for (int i = m_cellWidth; i < r.width(); i += m_cellWidth) {
        painter->drawLine(i, r.bottom(), i, r.top());
    }
    for (int i = m_cellWidth; i < r.height(); i += m_cellWidth) {
        painter->drawLine(r.left(), i, r.right(), i);
    }

    painter->setRenderHint(QPainter::Antialiasing, false);
}

void CSceneWidget::onCloneTriggered()
{
    qDebug() << "Clone";

    ClonedWidget * clone = new ClonedWidget(_compkey, scene());
    clone->resize(480, 360);
    clone->setAttribute(Qt::WA_DeleteOnClose);
    clone->show();
}

void CSceneWidget::onOrderUpTriggered()
{
    if(_currentItem != 0)
    {
        _currentItem->setZValue(_currentItem->zValue() + 0.001);
        qDebug() << "ZOreder: " << _currentItem->zValue();
    }
}

void CSceneWidget::onOrderDownTriggered()
{
    if(_currentItem != 0)
    {
        _currentItem->setZValue(_currentItem->zValue() - 0.001);
        qDebug() << "ZOreder: " << _currentItem->zValue();
    }
}

void CSceneWidget::onHideBoxTriggerd()
{
    _currentItem->setEditMode(false);
    _currentItem->hide();
}

void CSceneWidget::onKeepAspectRatioTriggered(bool triggerd)
{
    _currentItem->setImageFitMode(triggerd?CGraphicsItem::ImageFit:CGraphicsItem::ImageStretch);
    _currentItem->setSize(_currentItem->imageSize());
}

void CSceneWidget::onOpacityTriggered()
{
    QAction *action = qobject_cast<QAction*>(sender());
    if(_currentItem != 0 && action != 0)
    {
        qreal opacity = action->property("opacity").toReal();
        _currentItem->setOpacity(opacity/qreal(100.0));
        qDebug() << "opacity: " << opacity/qreal(100.0);
    }
}

QStringList CSceneWidget::apply()
{
    QStringList list;

    QListIterator<QGraphicsItem*> it(scene()->items());

    while(it.hasNext())
    {
        CGraphicsItem *gi = qgraphicsitem_cast<CGraphicsItem*>(it.next());
        if(!qFuzzyCompare(gi->zValue(), qreal(0.0))) // ignore first item, first item is background
        {
            gi->setEditMode(false);
            list.push_back(QString("x=%1,y=%2,w=%3,h=%4").
                           arg(gi->pos().x()).
                           arg(gi->pos().y()).
                           arg(gi->imageSize().width()).
                           arg(gi->imageSize().height()));
        }
    }

    return list;
}

void CSceneWidget::start()
{
    // only one timer
    if(_timerId == 0)
        startTimer(1000 / 25);
}

void CSceneWidget::stop()
{
    killTimer(_timerId);
    qDebug() << "_timerId " << _timerId;
}

void CSceneWidget::startBox()
{
    QListIterator<QGraphicsItem*> it(scene()->items());

    while(it.hasNext())
    {
        CGraphicsItem *gi = qgraphicsitem_cast<CGraphicsItem*>(it.next());
        gi->setEditMode(true);
    }
}

void CSceneWidget::stopBox()
{
    QListIterator<QGraphicsItem*> it(scene()->items());

    while(it.hasNext())
    {
        CGraphicsItem *gi = qgraphicsitem_cast<CGraphicsItem*>(it.next());
        gi->setEditMode(false);
    }
}

void CSceneWidget::setAspectRatioMode(Qt::AspectRatioMode mode)
{
    _aspectRatioMode = mode;
}















