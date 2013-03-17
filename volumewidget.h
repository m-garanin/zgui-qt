#ifndef _VOLUME_WIDGET_H_
#define _VOLUME_WIDGET_H_

#include <QWidget>

class QLabel;

class CVolumeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CVolumeWidget(QWidget *parent = 0);
    explicit CVolumeWidget(qint32 volume, QWidget *parent = 0);

    void setText(const QString &);
    void setVolume(qint32 volume);
    qint32 volume() const;

    void setMute(bool);
    bool isMute() const;

public slots:
    void onPbMuteClicked();
    void onSliderValueChanged(int);

private:
    void init();

private:
    qint32 _volume;
    QLabel *lSliderName;
    bool _isMute;
};


#endif //_VOLUME_WIDGET_H_