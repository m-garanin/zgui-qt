#include "menuindicator.h"

MenuIndicator::MenuIndicator(QWidget *parent) :
    QPushButton(parent)
{
    _inProgress = false;

    progressIcon = new QMovie(":/loader.gif");
    idleIcon = new QMovie(":/loader.gif");

    connect(progressIcon, SIGNAL(frameChanged(int)), this, SLOT(setProgressIconFrame(int)));
    connect(idleIcon, SIGNAL(frameChanged(int)), this, SLOT(setIdleIconFrame(int)));

    if(progressIcon->loopCount() != -1)
        connect(progressIcon, SIGNAL(finished()), progressIcon, SLOT(start()));
    if(idleIcon->loopCount() != -1)
        connect(idleIcon, SIGNAL(finished()), idleIcon, SLOT(start()));

    idleIcon->start();
}

MenuIndicator::~MenuIndicator()
{
    delete progressIcon;
    delete idleIcon;
}

void MenuIndicator::changeState()
{
    setState(!_inProgress);
}

void MenuIndicator::setState(bool inProgress)
{
    _inProgress = inProgress;

    if(_inProgress)
    {
        this->setProgressIcon();
    }
    else
    {
        this->setIdleIcon();
    }

    emit stateChanged(_inProgress);
}

bool MenuIndicator::getState()
{
    return _inProgress;
}

void MenuIndicator::setProgressIconFrame(int)
{
    this->setIcon(QIcon(progressIcon->currentPixmap()));
}

void MenuIndicator::setIdleIconFrame(int)
{
    this->setIcon(QIcon(idleIcon->currentPixmap()));
}

void MenuIndicator::setIndicatorText(QString text)
{
    this->setText(text);
}

void MenuIndicator::setProgressIcon()
{
    idleIcon->stop();
    progressIcon->start();
}

void MenuIndicator::setIdleIcon()
{
    progressIcon->stop();
    idleIcon->start();
}
