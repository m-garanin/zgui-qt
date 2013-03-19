#ifndef _AUDIO_PANEL_H_
#define _AUDIO_PANEL_H_

#include "volumewidget.h"

#include <QWidget>

class QVBoxLayout;
class QSpacerItem;

class CAudioPanel : public QWidget
{
    Q_OBJECT
public:
    explicit CAudioPanel(QWidget *parent = 0);

    void addVolumeWidget(CVolumeWidget *);

private:
    QVBoxLayout *_mainLayout;
    QSpacerItem *_verticalSpacer;
};

#endif // _AUDIO_PANEL_H_

