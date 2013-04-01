#ifndef SCENEWIDGET_H
#define SCENEWIDGET_H

#include "previewwidget.h"

class QPaintEvent;
class CBoxWidget;
class QMenu;
class QDropEvent;
class QAction;

class CSceneWidget : public PreviewWidget
{
    Q_OBJECT
public:
    explicit CSceneWidget(qint32 compkey, QWidget *parent = 0);

    void toggleBox(int compkey); // показывает либо скрывает бокс
    void apply();

    void startBox();
    void stopBox();

public slots:
    void setGridVisible(bool visible);
    void setCellWidth(quint32);

private slots:
    void onCustomContextMenuRequested(const QPoint &);
    void onApplyTriggered();
    void onHideBoxTriggerd();
    void onCloneTriggered();

protected:
    void paintEvent(QPaintEvent *event);


private:
    void hideBoxes();
    void drawGrid();

private:
    QList<CBoxWidget*> _boxWidgetList;

    QMenu *_menu;
    quint32 m_cellWidth;
    bool    m_gridEnabled;
};

#endif // SCENEWIDGET_H
