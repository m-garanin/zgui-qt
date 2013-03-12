#include "menuindicator.h"

MenuIndicator::MenuIndicator(QWidget *parent) :
    QPushButton(parent)
{
    _inProgress = false;

    progressIcon = new QIcon(":/red.ico");
    idleIcon = new QIcon(":/green.ico");
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
        this->setIcon(*progressIcon);
    }
    else
    {
        this->setIcon(*idleIcon);
    }

    emit stateChanged(_inProgress);
}

bool MenuIndicator::getState()
{
    return _inProgress;
}
