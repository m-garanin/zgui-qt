#ifndef MENUBARWIDGET_H
#define MENUBARWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QPushButton>

#include "IManager.h"
/*
namespace Ui {
class MenuBarWidget;
}
*/
class MenuBarWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit MenuBarWidget(QWidget *parent = 0);
    ~MenuBarWidget();

private slots:    
    void on_startAirBtn_clicked();
    void updateAirStat();

private:
    QPushButton* m_startAirBtn;
    QPushButton* m_startRecordBtn;

    QTimer *air_timer; // таймер для статистики трансляции
    uint64 m_total_bytes, m_total_frames;

};

#endif // MENUBARWIDGET_H
