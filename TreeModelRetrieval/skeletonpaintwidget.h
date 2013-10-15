#ifndef SKELETONPAINTWIDGET_H
#define SKELETONPAINTWIDGET_H

#include <QWidget>

class TreeSkeleton;
class TreeBranch;

class SkeletonPaintWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SkeletonPaintWidget(QWidget *parent = 0);

    void ClearBK(QPainter& painter, QColor& color);
    void DrawSkeleton(QPainter& painter, TreeSkeleton& skeleton);
    void DrawTreeBranch(QPainter& painter, TreeBranch& branch);
signals:

public slots:
    
};

#endif // SKELETONPAINTWIDGET_H
