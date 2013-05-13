#ifndef _AUDIO_PANEL_H_
#define _AUDIO_PANEL_H_

#include "volumewidget.h"

#include <QWidget>
#include <QTimer>
#include <QList>
#include <QDebug>

class QVBoxLayout;
class QSpacerItem;

#define LEVELS_UPDATE_PERIOD 100

class CAudioPanel : public QWidget
{
    Q_OBJECT
public:
    explicit CAudioPanel(QWidget *parent = 0);

    void addVolumeWidget(CVolumeWidget *);

public slots:
    void updateLevels();

private:
    void resizeEvent(QResizeEvent * event);
    QVBoxLayout *_mainLayout;
    QSpacerItem *_verticalSpacer;
    QTimer *m_timer; //  таймер обновления dB-уровней
    double levels[21];// для хранение dB-уровней    
    QList<CVolumeWidget*> m_vs;
    CVolumeWidget *m_master;
};

#endif // _AUDIO_PANEL_H_

