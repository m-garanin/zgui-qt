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

    void showBox(int compkey);
    QStringList apply();


public slots:
    // proposal :
    // void setGridVisible(bool);
    // void setCellWidth(quint32);
    void showGrid(quint32 cell_width);
    void hideGrid();

private slots:
    void onCustomContextMenuRequested(const QPoint &);
    void onApplyTriggered();
    void onHideBoxTriggerd();
    void onShowGrid();
    void onHideGrid();

protected:
    void dropEvent(QDropEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);

    void mousePressEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *event);

private:
    qint32 findPreviewWidget(const QPoint &);
    void drawGrid();

private:
    QList<CBoxWidget*> _boxWidgetList;
    bool _enableDragAndDrop;
    QMenu *_menu;
    quint32 m_cellWidth;
    bool    m_gridEnabled;
    QAction * m_showGridAction;
    QAction * m_hideGridAction;
};

#endif // SCENEWIDGET_H
