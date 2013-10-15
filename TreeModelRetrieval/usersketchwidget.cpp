#include "usersketchwidget.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QPainter>
#include <QMouseEvent>

UserSketchWidget::UserSketchWidget(QWidget *parent):
    SkeletonPaintWidget(parent)
{
    int currentScreenHeight = QApplication::desktop()->height();
    int widgetHeight = currentScreenHeight*0.5;
    setFixedSize(widgetHeight,widgetHeight);

    isSketching = false;
    isMousePress = false;
    isDisplayResult = false;
    currentPhase = sketchTrunk;
    branch = 0;
    skeleton = 0;
}

void UserSketchWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QColor BKColor;
    QPen pen(Qt::black);
    pen.setWidth(3);

    if(isSketching)
    {
        switch(currentPhase)
        {
        case sketchTrunk:
            BKColor.setRgb(255,255,255);
            ClearBK(painter,BKColor);
            painter.setPen(pen);
            DrawTreeBranch(painter,*branch);
            break;
        case sketchBranch:
            BKColor.setRgb(255,255,255);
            ClearBK(painter,BKColor);
            painter.setPen(pen);
            DrawSkeleton(painter,*skeleton);
            DrawTreeBranch(painter,*branch);
            break;
        default:
            break;
        }
    }
    else
    {
        if(isDisplayResult)
        {
            BKColor.setRgb(255,255,255);
            ClearBK(painter,BKColor);
            painter.setPen(pen);
            DrawSkeleton(painter,*skeleton);
        }
        else
        {
            BKColor.setRgb(100,100,100);
            ClearBK(painter,BKColor);
        }
    }
}

void UserSketchWidget::AddPointToBranch(QPoint point)
{
    double x = (float)point.x();
    double y = (float)point.y();
    BranchPartition p(x,y,sectionCount,0);
    BranchSection s(p);
    branch->AddBranchSection(s);
    sectionCount++;
}

void UserSketchWidget::mousePressEvent(QMouseEvent *event)
{
    if(isSketching)
    {
        isMousePress = true;
        sectionCount = -1;
        AddPointToBranch(event->pos());
        update();
    }
}

void UserSketchWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(isSketching)
    {
        isMousePress = false;
        AddPointToBranch(event->pos());
        if(branch->GetSectionNum()>1)
        {
            skeleton->AddBranch(*branch);
        }
        branch->Clear();
        update();
        if(currentPhase == sketchTrunk)
        {
            emit SketchTrunkDone();
            currentPhase = sketchBranch;
            update();
        }
    }
}

void UserSketchWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(isMousePress)
    {
        AddPointToBranch(event->pos());
        update();
    }
}

bool UserSketchWidget::BeginSketch()
{
    if(!GetSketchStatue())
    {
        branch = new TreeBranch();
        skeleton = new TreeSkeleton();
        if(branch==0 || skeleton==0)
        {
            return false;
        }
        else
        {
            SetSketchStatue(true);
            currentPhase = sketchTrunk;
            update();
            return true;
        }
    }
    else
    {
        return false;
    }
}

bool UserSketchWidget::EndSketch()
{
    if(GetSketchStatue() || GetDisplayStatue())
    {
        delete branch;
        delete skeleton;
        branch = 0;
        skeleton = 0;
        SetSketchStatue(false);
        update();
        return true;
    }
    else
    {
        return false;
    }
}

void UserSketchWidget::DisplayConfigure(bool isDisplay)
{
    if(isDisplay)
    {
        SetSketchStatue(false);
        SetDisplayStatue(true);
    }
    else
    {
        SetDisplayStatue(false);
    }
}
