#ifndef _VOLUME_WIDGET_H_
#define _VOLUME_WIDGET_H_

#include <QWidget>

namespace Ui {
class VolumeWidget;
}

class QLabel;

class CVolumeWidget : public QWidget
{
    Q_OBJECT
public:

    explicit CVolumeWidget(const QString &sourceKey, QWidget *parent = 0);

    void setVolume(qreal volume);
    qreal volume() const;

    void setMute(bool mute);
    bool getMute() {return _isMute;}

    void setLevelDb(double val);

    QString getPersistentSourceId(){return _sourceKey;}

public slots:
    void onPbMuteClicked();
    void onSliderValueChanged(int);

private:
    Ui::VolumeWidget* ui;
    void init();

private:
    QString _sourceKey;
    qreal _volume;
    QLabel *lSliderName;
    bool _isMute;
};


#endif //_VOLUME_WIDGET_H_
