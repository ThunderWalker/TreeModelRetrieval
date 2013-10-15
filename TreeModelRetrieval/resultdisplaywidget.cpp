#include <QPainter>
#include "resultdisplaywidget.h"
#include "treeskeleton.h"

ResultDisplayWidget::ResultDisplayWidget(int height,QWidget* parent):
    SkeletonPaintWidget(parent)
{
    setFixedSize(height,height);
    SetDisplayStatue(false);
}

void ResultDisplayWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QColor Bkcolor;
    Bkcolor.setRgb(255,255,255);
    QPen pen(Qt::black);
    pen.setWidth(1);

    ClearBK(painter,Bkcolor);

    if(GetDisplayStatue())
    {
        painter.setPen(pen);
        DrawSkeleton(painter,*skeleton);
    }
}

void ResultDisplayWidget::Clear()
{
    delete skeleton;
    SetDisplayStatue(false);
}

void ResultDisplayWidget::DisplaySkeleton(QString fileName)
{
    skeleton = new TreeSkeleton(fileName);
    skeleton->ReSize(size().width(),size().height(),TreeSkeleton::AllAtCentre);
    SetDisplayStatue(true);
    update();
}
