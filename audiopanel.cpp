#include "audiopanel.h"

#include <QLayout>

CAudioPanel::CAudioPanel(QWidget *parent) :
    QWidget(parent)
{
    _mainLayout = new QVBoxLayout(this);
    _mainLayout->setSpacing(0);
    _mainLayout->setContentsMargins(11, 11, 11, 11);

    _verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
}

void CAudioPanel::addVolumeWidget(CVolumeWidget *volumeWidget)
{
    _mainLayout->removeItem(_verticalSpacer);
    _mainLayout->addWidget(volumeWidget);
    _mainLayout->addItem(_verticalSpacer);
}
