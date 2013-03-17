#ifndef _AUDIO_PANEL_H_
#define _AUDIO_PANEL_H_

#include "volumewidget.h"

#include <QWidget>
#include <QTableWidget>

class CAudioPanel : public QWidget
{
    Q_OBJECT
public:
    explicit CAudioPanel(QWidget *parent = 0);

    void addVolumeWidget(CVolumeWidget *);

protected:
    void resizeEvent(QResizeEvent *);

private:
    QTableWidget *_tableWidget;
};

#endif // _AUDIO_PANEL_H_

