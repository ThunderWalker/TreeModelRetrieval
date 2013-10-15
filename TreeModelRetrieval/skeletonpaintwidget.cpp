#include <QPainter>
#include <QPen>
#include "skeletonpaintwidget.h"
#include "treeskeleton.h"

SkeletonPaintWidget::SkeletonPaintWidget(QWidget *parent) :
    QWidget(parent)
{
}

void SkeletonPaintWidget::ClearBK(QPainter& painter, QColor& color)
{
    painter.setBrush(color);
    painter.drawRect(0,0,size().width(),size().height());
}

void SkeletonPaintWidget::DrawSkeleton(QPainter &painter, TreeSkeleton& skeleton)
{
    int branchNum = skeleton.GetBranchNum();
    if(branchNum > 0)
    {
        for(int i=0;i<branchNum;i++)
        {
            DrawTreeBranch(painter,skeleton.GetBranch(i));
        }
    }
}

void SkeletonPaintWidget::DrawTreeBranch(QPainter &painter, TreeBranch &branch)
{
    int sectionNum = branch.GetSectionNum();
    if(sectionNum > 0)
    {
        for(int i=sectionNum-1;i>=0;i--)
        {
            BranchSection& section = branch.GetSection(i);
            int partitionNum = section.GetPartitionNum();
            for(int j=0;j<partitionNum;j++)
            {
                BranchPartition& partition = section.GetPartition(j);
                int priSection = partition.GetPriorSection();
                int priPartition = partition.GetPriorPartition();
                if(priSection!=-1)
                {
                    QPointF startPoint = branch.GetSection(priSection).GetPartition(priPartition).GetCentroid();
                    QPointF endPoint = partition.GetCentroid();
                    painter.drawLine(startPoint,endPoint);
                }
            }
        }
    }
}
