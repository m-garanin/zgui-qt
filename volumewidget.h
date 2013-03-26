#ifndef _VOLUME_WIDGET_H_
#define _VOLUME_WIDGET_H_

#include <QWidget>

class QLabel;

class CVolumeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CVolumeWidget(const QString &sourceKey, QWidget *parent = 0);
    explicit CVolumeWidget(const QString &sourceKey, qreal volume, QWidget *parent = 0);

    void setText(const QString &);
    void setVolume(qreal volume);
    qreal volume() const;

    void setMute(bool);
    bool isMute() const;

public slots:
    void onPbMuteClicked();
    void onSliderValueChanged(int);

private:
    void init();

private:
    QString _sourceKey;
    qreal _volume;
    QLabel *lSliderName;
    bool _isMute;
};


#endif //_VOLUME_WIDGET_H_
