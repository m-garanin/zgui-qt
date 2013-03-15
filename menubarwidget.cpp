#include "menubarwidget.h"
#include "ui_menubarwidget.h"

MenuBarWidget::MenuBarWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MenuBarWidget)
{
    ui->setupUi(this);

    connect(this, SIGNAL(setRecordIndicatorText(QString)), ui->startRecordBtn, SLOT(setIndicatorText(QString)));
    connect(this, SIGNAL(setAirIndicatorText(QString)), ui->startAirBtn, SLOT(setIndicatorText(QString)));

    //connect(this, SIGNAL(setAirIndicatorText(QString)), this, SLOT(resizeWidget()));
}

MenuBarWidget::~MenuBarWidget()
{
    delete ui;
}

void MenuBarWidget::on_startRecordBtn_clicked()
{
    emit startRecordBtnClicked(ui->startRecordBtn->getState());
}

void MenuBarWidget::on_startAirBtn_clicked()
{
    emit startAirBtnClicked(ui->startAirBtn->getState());
}

void MenuBarWidget::recordStarting()
{
    ui->startRecordBtn->setState(MenuIndicator::Progress);
}

void MenuBarWidget::recordStoping()
{
    ui->startRecordBtn->setState(MenuIndicator::Idle);
}

void MenuBarWidget::airStarting()
{
    ui->startAirBtn->setState(MenuIndicator::Progress);
}

void MenuBarWidget::airStoping()
{
    ui->startAirBtn->setState(MenuIndicator::Idle);
}

void MenuBarWidget::resizeWidget()
{
    //this->resize(ui->startRecordBtn->width() + ui->startAirBtn->width(), this->height());
    //this->updateGeometry();
}
