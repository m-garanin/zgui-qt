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

    void startBox();
    void stopBox();

public slots:
    void setGridVisible(bool visible);
    void setCellWidth(quint32);

private slots:
    void onCustomContextMenuRequested(const QPoint &);
    void onApplyTriggered();
    void onHideBoxTriggerd();

protected:
    void dropEvent(QDropEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);

    void mousePressEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *event);

    void resizeEvent(QResizeEvent *);

private:
    qint32 findPreviewWidget(const QPoint &);
    void disableLayers();
    void drawGrid();

private:
    QList<CBoxWidget*> _boxWidgetList;
    bool _enableDragAndDrop;
    QMenu *_menu;
    quint32 m_cellWidth;
    bool    m_gridEnabled;
};

#endif // SCENEWIDGET_H
