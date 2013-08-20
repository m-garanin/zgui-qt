#include "clonedwidget.h"
#include "ui_clonedwidget.h"

ClonedWidget::ClonedWidget(qint32 compkey, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClonedWidget)
{
    ui->setupUi(this);

    _sceneWidget = new CSceneWidget(compkey, true, this);
    _sceneWidget->setGeometry(0, 0, width(), height());
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
