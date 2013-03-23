#include "clonedwidget.h"
#include "ui_clonedwidget.h"

ClonedWidget::ClonedWidget(qint32 compkey, QGraphicsScene *scene, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClonedWidget)
{
    ui->setupUi(this);

    _sceneWidget = new CSceneWidget(compkey, scene->width(), scene->height(), this);
    _sceneWidget->setGeometry(0, 0, width(), height());
    _sceneWidget->setScene(scene);
}

ClonedWidget::~ClonedWidget()
{
    delete _sceneWidget;
    delete ui;
}

void ClonedWidget::resizeEvent(QResizeEvent *)
{
    _sceneWidget->setGeometry(0, 0, width(), height());
}
