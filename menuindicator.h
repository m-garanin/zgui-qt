#ifndef MENUINDICATOR_H
#define MENUINDICATOR_H

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
    QIcon * progressIcon;
    QIcon * idleIcon;
    
signals:
    void stateChanged(bool inProgress);

public slots:
    void setState(bool inProgress);
    void changeState();
    
};

#endif // MENUINDICATOR_H
