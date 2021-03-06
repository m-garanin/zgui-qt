#include "boxwidget.h"

#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QAction>
#include <QMenu>
#include <QtCore/qmath.h>
#include <QDebug>


namespace {
    const int BORDER_MARGIN = 10; //margin in pixels used to determine resizing state
    const QSize MIN_SIZE = QSize(10, 10);
    const QColor BORDER_COLOR = Qt::green;
    const int BORDER_WIDTH = 4;
    const QColor BRUSH_COLOR = QColor(255, 255, 255, 255);

    qreal distance(const QPoint &p1, const QPoint &p2)
    {
        return QLineF(p1, p2).length();
    }
}


CBoxWidget::CBoxWidget(qint32 compkey, QWidget *parent) :
    PreviewWidget(compkey, parent), m_windowState(CBoxWidget::Idle),
    m_dragging(false), m_keepAspectRatio(true)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setMouseTracking(true);


    setContextMenuPolicy(Qt::DefaultContextMenu);

    m_contextMenu = new QMenu(this);
    QAction * zMoveUp = new QAction(tr("Move Up"), this);
    connect(zMoveUp, SIGNAL(triggered()),
            this, SLOT(zOrderMoveUp()));
    addAction(zMoveUp);
    QAction * zMoveDown = new QAction(tr("Move Down"), this);
    connect(zMoveDown, SIGNAL(triggered()),
            this, SLOT(zOrderMoveDown()));
    addAction(zMoveDown);

    QAction * toggleAspectRatio = new QAction(tr("Keep aspect ratio"), this);
    connect(toggleAspectRatio, SIGNAL(triggered(bool)),
            this, SLOT(toggleAspectRatio(bool)));
    toggleAspectRatio->setCheckable(true);
    toggleAspectRatio->setChecked(m_keepAspectRatio);
    addAction(toggleAspectRatio);

    QAction * hideAction = new QAction(tr("Hide"), this);
    connect(hideAction, SIGNAL(triggered()),
            this, SLOT(onHideTriggered()));
    addAction(hideAction);

    m_contextMenu->addActions(this->actions());
    QActionGroup * transparencyGroup = new QActionGroup(this);
    transparencyGroup->setExclusive(true);

    for (int i = 0; i <= 10; i++) {
        int value = i * 10;
        QAction * t = new QAction(tr("%1 %").arg(value), this);

        t->setCheckable(true);
        t->setProperty("value", value);
        if (transparency() == value)
            t->setChecked(true);
        transparencyGroup->addAction(t);
    }

    connect(transparencyGroup, SIGNAL(triggered(QAction*)),
            this, SLOT(onTransparencyMenuChanged(QAction*)));
    QMenu * transparencyMenu = new QMenu("Transparency", this);
    transparencyMenu->addActions(transparencyGroup->actions());
    m_contextMenu->addMenu(transparencyMenu);

}

void CBoxWidget::show()
{
    this->start();
    QWidget::show();
}

void CBoxWidget::hide()
{
    this->stop();
    setVisible(false);
    qDebug() << "HIDE";
}

int CBoxWidget::transparency() const
{
    return PreviewWidget::transparency();
}

void CBoxWidget::setTransparency(int value)
{
    PreviewWidget::setTransparency(value);
}

void CBoxWidget::paintEvent(QPaintEvent *paint)
{
    PreviewWidget::paintEvent(paint);

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    //p.setBackgroundMode(Qt::OpaqueMode);
    QPen pen(BORDER_COLOR);
    pen.setWidth(BORDER_WIDTH);
    p.setPen(pen);
//    QBrush b;
//    b.setColor(BRUSH_COLOR);
//    b.setStyle(Qt::SolidPattern);
//    p.setBrush(b);
    p.drawRect(this->rect());

    return;
}

void CBoxWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragPosition = event->pos();
        event->accept();
        m_prevSize = this->geometry().size();
        m_windowState = windowState(event->pos());
        if (m_windowState == Idle) { //special case
            m_windowState = Dragging;
        }
        updateCursor(m_windowState);
    }
}

void CBoxWidget::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);


    if (event->buttons() & Qt::LeftButton) {
        QPoint gp = this->mapToParent(event->pos());
        QRect rect = this->geometry();
        QRect newRect = rect;
        QPoint delta = gp - m_prevPos;

        Qt::AspectRatioMode arMode;
        if (m_windowState == CBoxWidget::Dragging) {

            if (!m_dragging) {
                setCursor(Qt::ClosedHandCursor);
                m_dragging = true;
            }
            newRect.moveTo(gp - m_dragPosition);

        } else if (m_windowState == ResizingLeft) {
            newRect.setLeft(gp.x());
            if (m_keepAspectRatio) {
                arMode = delta.x() < 0 ? Qt::KeepAspectRatioByExpanding : Qt::KeepAspectRatio;
                newRect.setHeight(m_prevSize.scaled(newRect.size(), arMode).height());
            }
        } else if (m_windowState == ResizingRight) {
            newRect.setRight(gp.x());
            if (m_keepAspectRatio) {
                arMode = delta.x() > 0 ? Qt::KeepAspectRatioByExpanding : Qt::KeepAspectRatio;
                newRect.setHeight(m_prevSize.scaled(newRect.size(), arMode).height());
            }
        } else if (m_windowState == ResizingTop) {
            newRect.setTop(gp.y());
            if (m_keepAspectRatio) {
                arMode = delta.y() < 0 ? Qt::KeepAspectRatioByExpanding : Qt::KeepAspectRatio;
                newRect.setWidth(m_prevSize.scaled(newRect.size(), arMode).width());
            }
        } else if (m_windowState == ResizingBottom) {
            newRect.setBottom(gp.y());
            if (m_keepAspectRatio) {
                arMode = delta.y() > 0 ? Qt::KeepAspectRatioByExpanding : Qt::KeepAspectRatio;
                newRect.setWidth(m_prevSize.scaled(newRect.size(), arMode).width());
            }
        } else if (m_windowState == ResizingTopLeft) {
            newRect.setTopLeft(gp);

            if (m_keepAspectRatio) {
                arMode = (delta.x() < 0 && delta.y() < 0) ? Qt::KeepAspectRatioByExpanding
                                                          : Qt::KeepAspectRatio;
                newRect.setSize(m_prevSize.scaled(newRect.size(), arMode));
            }


        } else if (m_windowState == ResizingTopRight) {
            newRect.setTopRight(gp);
            if (m_keepAspectRatio) {
                arMode = (delta.x() > 0 && delta.y() < 0) ? Qt::KeepAspectRatioByExpanding
                                                          : Qt::KeepAspectRatio;
                newRect.setSize(m_prevSize.scaled(newRect.size(), arMode));
            }

        } else if (m_windowState == ResizingBottomRight) {
            newRect.setBottomRight(gp);
            if (m_keepAspectRatio) {
                arMode = (delta.x() < 0 && delta.y() > 0) ? Qt::KeepAspectRatioByExpanding
                                                          : Qt::KeepAspectRatio;
                newRect.setSize(m_prevSize.scaled(newRect.size(), arMode));
            }
        } else if (m_windowState == ResizingBottonLeft) {
            newRect.setBottomLeft(gp);
            if (m_keepAspectRatio) {
                arMode = (delta.x() > 0 && delta.y() > 0) ? Qt::KeepAspectRatioByExpanding
                                                          : Qt::KeepAspectRatio;
                newRect.setSize(m_prevSize.scaled(newRect.size(), arMode));
            }
        }

        if (newRect.width() < MIN_SIZE.width()) {
            newRect.setWidth(MIN_SIZE.width());
        } if (newRect.height() < MIN_SIZE.height()) {
            newRect.setHeight(MIN_SIZE.height());
        }

        event->accept();

        setGeometry(newRect);
        m_prevPos = gp;

    } else {
        WindowState state = windowState(event->pos());
        updateCursor(state);
    }
}

void CBoxWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
    m_dragging = false;
    m_windowState = Idle;
    m_prevSize = this->geometry().size();
    updateCursor(m_windowState);
}

void CBoxWidget::contextMenuEvent(QContextMenuEvent *event)
{
    m_contextMenu->exec(event->globalPos());
}

void CBoxWidget::zOrderMoveUp()
{
    qDebug() << "z-order move up";
    if (parentWidget()) {
        foreach (QObject *o, parentWidget()->children()) {
            QWidget * w = qobject_cast<CBoxWidget *>(o);
            if (w && w != this && w->geometry().intersects(this->geometry())) {
                w->stackUnder(this);
            }
        }
    }
    qDebug() << "raising()";
    this->raise();
}

void CBoxWidget::zOrderMoveDown()
{
    qDebug() << "z-order move down";
    if (parentWidget()) {
        foreach (QObject *o, parentWidget()->children()) {
            QWidget * w = qobject_cast<CBoxWidget *>(o);
            if (w) {
                qDebug() << "sibling: " << w;
            }
            if (w && w != this && w->geometry().intersects(this->geometry())) {
                this->stackUnder(w);
            }
        }
    }
    qDebug() << "lowering()";
    this->lower();
}

void CBoxWidget::toggleAspectRatio(bool arg)
{
    m_keepAspectRatio = arg;
    update();
}

void CBoxWidget::onTransparencyMenuChanged(QAction *action)
{
    int value = action->property("value").toInt();
    this->setTransparency(value);
}

void CBoxWidget::onHideTriggered()
{
    this->hide();
}

CBoxWidget::WindowState CBoxWidget::windowState(const QPoint &pt)
{
    WindowState state = Idle;
    QRect r = this->rect();
    if (distance(r.topLeft(), pt) <= BORDER_MARGIN) {
        state = ResizingTopLeft;
    } else if (distance(r.topRight(), pt) <= BORDER_MARGIN) {
        state = ResizingTopRight;
    } else if (distance(r.bottomRight(), pt) <= BORDER_MARGIN) {
        state = ResizingBottomRight;
    } else if (distance(r.bottomLeft(), pt) <= BORDER_MARGIN) {
        state = ResizingBottonLeft;
    } else if (pt.x() - r.left() <= BORDER_MARGIN) {
        state = ResizingLeft;
    } else if (pt.y() - r.top() <= BORDER_MARGIN) {
        state = ResizingTop;
    } else if (r.right() - pt.x() <= BORDER_MARGIN) {
        state = ResizingRight;
    } else if (r.bottom() - pt.y() <= BORDER_MARGIN) {
        state = ResizingBottom;
    }
    return state;
}

void CBoxWidget::updateCursor(CBoxWidget::WindowState state)
{
    QCursor cursor = Qt::ArrowCursor;
    switch (state) {
    case Dragging:
        cursor.setShape(Qt::OpenHandCursor);
        break;
    case ResizingTopLeft:
    case ResizingBottomRight:
        cursor.setShape(Qt::SizeFDiagCursor);
        break;
    case ResizingTop:
    case ResizingBottom:
        cursor.setShape(Qt::SizeVerCursor);
        break;
    case ResizingTopRight:
    case ResizingBottonLeft:
        cursor.setShape(Qt::SizeBDiagCursor);
        break;
    case ResizingRight:
    case ResizingLeft:
        cursor.setShape(Qt::SizeHorCursor);
        break;
    default:
        cursor.setShape(Qt::ArrowCursor);
    }
    setCursor(cursor);
}



























