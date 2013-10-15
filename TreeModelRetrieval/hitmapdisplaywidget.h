#ifndef HITMAPDISPLAYWIDGET_H
#define HITMAPDISPLAYWIDGET_H

#include "skeletonpaintwidget.h"

class HitMap;
class HitMapDisplayWidget : public SkeletonPaintWidget
{
public:
    HitMapDisplayWidget(int height, HitMap& hitmap, QWidget* parent = 0);

protected:
    void paintEvent(QPaintEvent *);

private:
    void DrawGrid(QPainter& painter);
    void DrawPixel(QPainter &painter, QColor& color, int row, int col);
    void DrawHitMap(QPainter& painter);

private:
    HitMap* m_hitmap;
};

#endif // HITMAPDISPLAYWIDGET_H
