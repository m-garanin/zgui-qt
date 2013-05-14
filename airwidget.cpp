#include "airwidget.h"

AirWidget::AirWidget(QWidget *parent) :
    QToolButton(parent)
{
    setStop();
}

void AirWidget::setOnAir(bool test)
{
    setIcon(QIcon(test?":onair_test":":onair"));
    setToolTip(tr("Live") + (test?tr("(test mode)"):""));
}

void AirWidget::setStop()
{
    setIcon(QIcon(":onair_wait"));
    setToolTip(tr("Press for Live"));

}

