#ifndef SCENEWIDGET_H
#define SCENEWIDGET_H

#include "previewwidget.h"

#include <QWidget>
#include <QDropEvent>
#include <QMenu>

class CSceneWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CSceneWidget(QWidget *parent = 0);

    void showBox(int layer_id);

protected:
    void dropEvent(QDropEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);

    void mousePressEvent(QMouseEvent *);
    
public slots:
    void onCustomContextMenuRequested(const QPoint &);
    void onAddPreviewWidget();

private:
    qint32 findPreviewWidget(const QPoint &);

private:
    QMenu *_contextMenu;
    QList<PreviewWidget*> _prevWidgetList;
};

#endif // SCENEWIDGET_H
