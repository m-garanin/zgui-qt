#ifndef MENUBARWIDGET_H
#define MENUBARWIDGET_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class MenuBarWidget;
}

class MenuBarWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit MenuBarWidget(QWidget *parent = 0);
    ~MenuBarWidget();
    
private:
    Ui::MenuBarWidget *ui;
    QTimer *air_timer; // таймер для статистики трансляции

signals:
    void startRecordBtnClicked(bool inProgress);
    void startAirBtnClicked(bool inProgress);

public slots:
    void updateAirStat();

private slots:
    void on_startRecordBtn_clicked();
    void on_startAirBtn_clicked();

    void recordStarting();
    void recordStoping();
    void airStarting();
    void airStoping();

    void resizeWidget();
};

#endif // MENUBARWIDGET_H
