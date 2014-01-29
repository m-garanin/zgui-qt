#include "rectselectionwidget.h"

#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QAction>
#include <QPushButton>
#include <QHBoxLayout>
#include <QDebug>

namespace {
    const int BORDER_MARGIN = 10; //margin in pixels used to determine resizing state
    const QSize MIN_SIZE = QSize(10, 10);
    const QColor BORDER_COLOR = Qt::red;
    const int BORDER_WIDTH = 4;
    const QColor BRUSH_COLOR = QColor(255, 255, 255, 10);

    qreal distance(const QPoint &p1, const QPoint &p2)
    {
        return QLineF(p1, p2).length();
    }
}

RectSelectionWidget::RectSelectionWidget(QWidget *parent) :
    QWidget(parent), m_windowState(RectSelectionWidget::Idle), m_dragging(false)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    //setAttribute(Qt::WA_NoSystemBackground);
    //setWindowModality(Qt::ApplicationModal);
    setMouseTracking(true);

    //
    addButtons();
}

QRect RectSelectionWidget::grab_geometry()
{
    QRect r = this->geometry();
    return QRect(r.x() + BORDER_WIDTH, r.y() + BORDER_WIDTH, r.width()-2*BORDER_WIDTH, r.height()-2*BORDER_WIDTH);
}

void RectSelectionWidget::paintEvent(QPaintEvent *event)
{    
    Q_UNUSED(event)
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);    
    QPen pen(BORDER_COLOR);
    pen.setWidth(BORDER_WIDTH);
    p.setPen(pen);

    QBrush b;
    b.setColor(QColor(QColor(0,0,0,1)));
    b.setStyle(Qt::SolidPattern);
    p.setBrush(b);

    p.drawRect(event->rect());
}

void RectSelectionWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragPosition = event->pos();
        event->accept();
        m_windowState = windowState(event->pos());
        if (m_windowState == Idle) { //special case
            m_windowState = Dragging;
        }
        updateCursor(m_windowState);
    }
}

void RectSelectionWidget::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);
    QPoint gp = event->globalPos();
    if (event->buttons() & Qt::LeftButton) {
        event->accept();
        QRect r = this->frameGeometry();
        QRect gr = r;
        if (m_windowState == RectSelectionWidget::Dragging) {
            if (!m_dragging) {
                setCursor(Qt::ClosedHandCursor);
                m_dragging = true;
            }
            gr.moveTo(gp - m_dragPosition);
        } else if (m_windowState == ResizingLeft) {
            gr.setLeft(gp.x());
        } else if (m_windowState == ResizingRight) {
            gr.setRight(gp.x());
        } else if (m_windowState == ResizingTop) {
            gr.setTop(gp.y());
        } else if (m_windowState == ResizingBottom) {
            gr.setBottom(gp.y());
        } else if (m_windowState == ResizingTopLeft) {
            gr.setTopLeft(gp);
        } else if (m_windowState == ResizingTopRight) {
            gr.setTopRight(gp);
        } else if (m_windowState == ResizingBottomRight) {
            gr.setBottomRight(gp);
        } else if (m_windowState == ResizingBottonLeft) {
            gr.setBottomLeft(gp);
        }
        if (gr.width() < MIN_SIZE.width()) {
            gr.setWidth(MIN_SIZE.width());
        } if (gr.height() < MIN_SIZE.height()) {
            gr.setHeight(MIN_SIZE.height());
        }
        setGeometry(gr.normalized());
    } else {
        WindowState state = windowState(event->pos());
        updateCursor(state);
    }
}

void RectSelectionWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
    m_dragging = false;
    m_windowState = Idle;
    updateCursor(m_windowState);

}

void RectSelectionWidget::onCancelTriggered()
{
    emit cancelled();
}



RectSelectionWidget::WindowState RectSelectionWidget::windowState(const QPoint &pt)
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

void RectSelectionWidget::updateCursor(RectSelectionWidget::WindowState state)
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

void RectSelectionWidget::addButtons()
{
    QHBoxLayout *layoutBtn = new QHBoxLayout(this);
    //layoutBtn->setSpacing(60);
    //layoutBtn->setContentsMargins(0, 0, 0, 0);

    QPushButton* start_btn = new QPushButton( tr("START CAPTURE"));
    QPushButton* cancel_btn = new QPushButton( tr("CANCEL") );

    layoutBtn->addWidget(start_btn);
    layoutBtn->addWidget(cancel_btn);

    connect(cancel_btn, SIGNAL(clicked()), this, SLOT(onCancelTriggered()));
    connect(start_btn, SIGNAL(clicked()), this, SIGNAL(submitted()));

}


