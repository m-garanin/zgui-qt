#include "airwidget.h"

AirWidget::AirWidget(QWidget *parent) :
    QToolButton(parent)
{
    setStop();
}

void AirWidget::setOnAir(bool test)
{
    setIcon(QIcon(test?":air_test":":air_on"));
    setToolTip(tr("Live") + (test?tr("(test mode)"):""));
}

void AirWidget::setStop()
{
    setIcon(QIcon(":air_off"));
    setToolTip(tr("Press for Live"));

}

