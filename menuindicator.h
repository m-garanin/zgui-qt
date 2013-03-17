#ifndef MENUINDICATOR_H
#define MENUINDICATOR_H

#include <QMovie>
#include <QPushButton>

class MenuIndicator : public QPushButton
{
    Q_OBJECT
public:
    explicit MenuIndicator(QWidget *parent = 0);
    ~MenuIndicator();

    enum State {Progress = true, Idle = false};

    bool getState();

private:
    bool _inProgress;
    QMovie * progressIcon;
    QMovie * idleIcon;
    
signals:
    void stateChanged(bool inProgress);

public slots:
    void setState(bool inProgress);
    void changeState();
    void setIndicatorText(QString text);

private slots:
    void setProgressIconFrame(int frame);
    void setIdleIconFrame(int frame);
    
    void setProgressIcon();
    void setIdleIcon();
};

#endif // MENUINDICATOR_H
