#include "scenepanel.h"
#include "scenewidget.h"
#include "boxwidget.h"
#include "clonedwidget.h"
#include "rectselectionwidget.h"
#include "effectsdlg.h"
#include "scenepanel.h"
#include "utils.h"
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
#include <QPushButton>

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


CSceneWidget::CSceneWidget(qint32 compkey, bool is_clone, QWidget *parent) :
    PreviewWidget(compkey, true, parent)
{   
    m_Panel = parent;
    m_gridEnabled = false;
    m_zoomFactor = DEFAULT_ZOOM_FACTOR;

    _menu = new QMenu(this);

    QAction *action;

    // раздельное меню для оригинала и клонов
    if(is_clone){
        action = new QAction(tr("Full-Screen"), this);
        action->setCheckable(true);
        connect(action, SIGNAL(triggered(bool)), SLOT(onFullScreenTriggered(bool)));
        _menu->addAction(action);

    }else{

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


        action = new QAction(tr("Effects"), this);
        connect(action, SIGNAL(triggered()), SLOT(onEffectsTriggered()));
        _menu->addAction(action);


        // TODO:
        // save/restore
        CScenePanel* sp = (CScenePanel*)parent;
        action = new QAction(tr("Save scene configuration"), this);
        connect(action, SIGNAL(triggered()), sp, SLOT(onSaveState())) ;
        _menu->addAction(action);

        action = new QAction(tr("Restore scene configuration"), this);
        connect(action, SIGNAL(triggered()), sp, SLOT(onRestoreState()));
        _menu->addAction(action);

        /*
        connect(tbar->addAction(tr("Save")),
                &QAction::triggered, this, &MainWindow::onSaveState);

        connect(tbar->addAction(tr("Restore")),
                &QAction::triggered, this, &MainWindow::onRestoreState);

        */

        setButtonBar();
    }

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), SLOT(onCustomContextMenuRequested(QPoint)));




    //setAcceptDrops(true);


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

void CSceneWidget::onEffectsTriggered()
{
    CEffectsDlg dlg;
    if(dlg.exec() == QDialog::Accepted){
        QString efname = dlg.getEffect();
        if(efname == "CLEAN"){
            global_manager->removeEffects(this->getCompkey());
        }else{
            global_manager->applyEffects(this->getCompkey(), efname.toLocal8Bit().data());
        }
    }
}

void CSceneWidget::onFotoTriggered()
{
    m_pbFoto->setIcon(QIcon(":S_FOTO_ON"));
    makeFoto(this->getCompkey());
    QTimer::singleShot(100, this, SLOT(showFotoIcon()));
}

void CSceneWidget::showFotoIcon()
{
    m_pbFoto->setIcon(QIcon(":S_FOTO"));
}


void CSceneWidget::onFullScreenTriggered(bool check)
{
    if(check){
        this->parentWidget()->window()->showFullScreen();
    }else{
        this->parentWidget()->window()->showNormal();
    }
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
    //qDebug() << "CSceneWidget::resizeEvent: " << event->size();
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
            checkConstructMode();
            return;
        }
    }

    // иначе создаём новый бокс
    CBoxWidget *bw = new CBoxWidget(compkey, this);
    bw->setImageFitMode(PreviewWidget::ImageStretch);
    bw->updatePreview(); // чтобы инициировать получение оригинальной картинки
    QSize s = bw->getOriginalImageSize();
    s.scale(QSize(150, 0), Qt::KeepAspectRatioByExpanding);
    bw->resize(s);
    bw->show();

    _boxWidgetList.push_back(bw);


    checkConstructMode();
}

void CSceneWidget::apply()
{
    CScenePanel* panel = (CScenePanel*)m_Panel;
    QListIterator<CBoxWidget*> it(_boxWidgetList);
    QPoint prv_point = this->getTopLeftPoint();
    QSize prv_size = this->getImageSize();
    int pw = prv_size.width();
    int ph = prv_size.height();

    // TODO: а нужно ли
    // для начала скрываем всё  кроме overlay|background-слоёв
    //panel->hideLayers();
    //

    while(it.hasNext())
    {
        CBoxWidget* bw = it.next();

        if(bw->isHidden())
            continue;

        QPoint bpoint = bw->pos() - prv_point;

        // передаём в ядро геом.параметры бокса в процентах относ. картинки
        global_manager->repositionLayer(bw->getCompkey(),
                                        100.* bpoint.x() / pw,
                                        100.* bpoint.y() / ph,
                                        100.* bw->width() / pw,
                                        100.* bw->height() / ph
                                        );

        // передаём соответствующему LW сигнал что надо перейти в состояние Visible
        CLayerWidget* plw = panel->findLayerWidgetByCompkey(bw->getCompkey());
        if(plw != NULL){
            plw->setVisibleState(true);
        }
    }

    hideBoxes();

    enableBoxButtons(false);
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
    enableBoxButtons(false);
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

    if(m_areaSize == QSize(width, height))
        return;

    m_areaSize = QSize(width, height);
    m_sa->setGeometry(x, y, width + 1, height + 1); // +1 to eliminate scrollbars
    QWidget::setGeometry(x, y, width + 1, height + 1); // +1 to eliminate scrollbars    
}

void CSceneWidget::zoomIn()
{
    zoom(m_zoomFactor);
}

void CSceneWidget::zoomOut()
{
    zoom(1 / m_zoomFactor);
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
    if (w->size().width() * zoomFactor < m_areaSize.width()
            && w->size().height() * zoomFactor < m_areaSize.height())
        return;

    zoomWidget(w, zoomFactor);
    foreach (CBoxWidget * bw,  _boxWidgetList) {
        zoomWidget(bw, zoomFactor);
    }

}

void CSceneWidget::setButtonBar()
{
    CScenePanel* sp = (CScenePanel*)m_Panel;

    //////////////////////////////////////////
    QVBoxLayout *layoutBtn = new QVBoxLayout(this);
    layoutBtn->setSpacing(6);
    layoutBtn->setContentsMargins(0, 0, 0, 0);
    layoutBtn->addItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));

    QFrame *frame = new QFrame(this);
    frame->setObjectName(QStringLiteral("frame"));
    frame->setStyleSheet(QStringLiteral("#frame {background: rgba(0, 0, 0, 128)}"));
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setFrameShadow(QFrame::Raised);

    QHBoxLayout *horizontalLayout = new QHBoxLayout(frame);
    horizontalLayout->setSpacing(6);
    horizontalLayout->setContentsMargins(0, 0, 0, 0);

    horizontalLayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    QSize icon_size = QSize(32,32);

    QPushButton* pb;

    pb = new QPushButton(frame);
    m_pbFoto = pb;
    pb->setIconSize(icon_size);
    pb->setIcon(QIcon(":S_FOTO"));
    pb->setMaximumSize(icon_size);
    pb->setToolTip(tr("Foto"));    
    connect(pb, SIGNAL(clicked()), SLOT(onFotoTriggered()));
    horizontalLayout->addWidget(pb);

    pb = new QPushButton(frame);
    pb->setIconSize(icon_size);
    pb->setIcon(QIcon(":S_EFFECTS"));
    pb->setMaximumSize(icon_size);
    pb->setToolTip(tr("Effects"));
    connect(pb, SIGNAL(clicked()), SLOT(onEffectsTriggered()));
    horizontalLayout->addWidget(pb);

    pb = new QPushButton(frame);
    pb->setIconSize(icon_size);
    pb->setIcon(QIcon(":S_CLONE"));
    pb->setMaximumSize(icon_size);
    pb->setToolTip(tr("Clone"));
    connect(pb, SIGNAL(clicked()), SLOT(onCloneTriggered()));
    horizontalLayout->addWidget(pb);


    pb = new QPushButton(frame);
    pb->setIconSize(icon_size);
    pb->setIcon(QIcon(":S_RESTORE"));
    pb->setMaximumSize(icon_size);
    pb->setToolTip(tr("Restore configuration"));
    connect(pb, SIGNAL(clicked()), sp, SLOT(onRestoreState()));
    horizontalLayout->addWidget(pb);

    pb = new QPushButton(frame);
    pb->setIconSize(icon_size);
    pb->setIcon(QIcon(":S_SAVE"));
    pb->setMaximumSize(icon_size);
    pb->setToolTip(tr("Save configuration"));
    connect(pb, SIGNAL(clicked()), sp, SLOT(onSaveState()));
    horizontalLayout->addWidget(pb);


    m_pbClean = new QPushButton(frame);
    pb = m_pbClean;
    pb->setIconSize(icon_size);
    pb->setIcon(QIcon(":S_CLEAN"));
    pb->setMaximumSize(icon_size);
    pb->setToolTip(tr("Hide boxes"));
    connect(pb, SIGNAL(clicked()), SLOT(onHideBoxTriggerd()));
    horizontalLayout->addWidget(pb);

    m_pbApply = new QPushButton(frame);
    pb = m_pbApply;
    pb->setIconSize(icon_size);
    pb->setIcon(QIcon(":S_APPLY"));
    pb->setMaximumSize(icon_size);
    pb->setToolTip(tr("apply"));
    connect(pb, SIGNAL(clicked()), SLOT(onApplyTriggered()));
    horizontalLayout->addWidget(pb);

    horizontalLayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    layoutBtn->addWidget(frame);

    enableBoxButtons(false);
}


void CSceneWidget::enableBoxButtons(bool flag)
{

    m_pbClean->setVisible(flag);
    m_pbApply->setVisible(flag);
}

void CSceneWidget::checkConstructMode()
{
    QListIterator<CBoxWidget*> it(_boxWidgetList);
    bool flag = false;
    while(it.hasNext())
    {
        CBoxWidget *bw = it.next();
        if(!bw->isHidden()){
            flag = true;
            break;
        }

    }

    enableBoxButtons(flag);
}

