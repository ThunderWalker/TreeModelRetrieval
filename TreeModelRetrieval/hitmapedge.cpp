#include "hitmapedge.h"
#include "treebranch.h"

HitMapEdge::HitMapEdge(TreeBranch& branch, int branchWeight)
{
    edgels.clear();
    GenerateEdge(branch);
    weight = branchWeight;
}

HitMapEdge::HitMapEdge()
{
}

void HitMapEdge::AddEdgel(QPointF &start, QPointF &end)
{
    int orientation = CalculateOrientationChannel(start,end);

    int start_x,start_y,end_x,end_y;
    start_x = (int)start.x();
    start_y = (int)start.y();
    end_x = (int)end.x();
    end_y = (int)end.y();

    if(start_x == end_x)
    {
        if(start_y > end_y)
        {
            int temp;
            temp = end_y;
            end_y = start_y;
            start_y = temp;
        }
        for(int y=start_y;y<=end_y;y++)
        {
            HitMapEdgel pixel(start_x,y,orientation);
            edgels.push_back(pixel);
        }
    }
    else if(start_y == end_y)
    {
        if(start_x > end_x)
        {
            int temp;
            temp = end_x;
            end_x = start_x;
            start_x = temp;
        }
        for(int x=start_x;x<=end_x;x++)
        {
            HitMapEdgel pixel(x,start_y,orientation);
            edgels.push_back(pixel);
        }
    }
    else
    {
        if(start_x > end_x) //确保dx大于0
        {
            int temp;
            temp = end_x;
            end_x = start_x;
            start_x = temp;
            temp = end_y;
            end_y = start_y;
            start_y = temp;
         }

        int dx = end_x -start_x;
        int dy = end_y - start_y;
        int d = 0;
        int c,m;
        if(abs(dx) > abs(dy)) //斜率小于1，x递增
        {
            m=2*dy;
            c=2*dx;
            if(dy>= 0) //向上
            {
                int y = start_y;
                for(int x=start_x;x<=end_x;x++)
                {
                    HitMapEdgel pixel(x,y,orientation);
                    edgels.push_back(pixel);
                    d += m;
                    if(d>=dx)
                    {
                        y++;
                        d-=c;
                    }
                }
            }
            else
            {
                m = -m;
                int y = start_y;
                for(int x=start_x;x<=end_x;x++)
                {
                    HitMapEdgel pixel(x,y,orientation);
                    edgels.push_back(pixel);
                    d += m;
                    if(d>=dx)
                    {
                        y--;
                        d-=c;
                    }
                }
            }
        }
        else  //斜率大于1,y递增
        {
            m=2*dx;
            c=2*dy;
            if(dy>0)  //向上
            {
                int x = start_x;
                for(int y=start_y;y<=end_y;y++)
                {
                    HitMapEdgel pixel(x,y,orientation);
                    edgels.push_back(pixel);
                    d += m;
                    if(d>=dy)
                    {
                        x++;
                        d-=c;
                    }
                }
            }
            else
            {
                c = -c;
                dy = -dy;
                int x = start_x;
                for(int y=start_y;y>=end_y;y--)
                {
                    HitMapEdgel pixel(x,y,orientation);
                    edgels.push_back(pixel);
                    d += m;
                    if(d>=dy)
                    {
                        x++;
                        d-=c;
                    }
                }
            }
        }
    }
}

void HitMapEdge::GenerateEdge(TreeBranch &branch)
{
    int sectionNum = branch.GetSectionNum();
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
                AddEdgel(startPoint,endPoint);
            }
        }
    }
}

int HitMapEdge::CalculateOrientationChannel(QPointF &start, QPointF &end)
{
    if(start.x() == end.x())
    {
        if(start.y() < end.y())
        {
            return 10;
        }
        else if(start.y() > end.y())
        {
            return 4;
        }
        else
        {
            return 0;
        }
    }
    else if(start.y() == end.y())
    {
        if(start.x() < end.x())
        {
            return 1;
        }
        else
        {
            return 7;
        }
    }
    else
    {
        float dif_x = end.x() - start.x();
        float dif_y = end.y() - start.y();
        float slope;

        if(dif_x > 0)  //一 四象限
        {
            if(dif_y < 0) //第一象限
            {
               dif_y = -dif_y;
               slope = dif_y/dif_x;
               if(slope < 0.57735 )
               {return 1;}
               else if(slope >= 0.57735  && slope < 1.7321)
               {return 2;}
               else
               {return 3;}
            }
            else    //第四象限
            {              
               slope = dif_y/dif_x;
               if(slope <= 0.57735 )
               {return 12;}
               else if(slope > 0.57735  && slope <= 1.7321)
               {return 11;}
               else
               {return 10;}
            }
        }
        else      //二 三象限
        {
            if(dif_y < 0) //第二象限
            {
                slope = dif_y/dif_x;
                if(slope <= 0.57735 )
                {return 6;}
                else if(slope > 0.57735  && slope <= 1.7321)
                {return 5;}
                else
                {return 4;}
            }
            else   //第三象限
            {
                dif_x = -dif_x;
                slope = dif_y/dif_x;
                if(slope < 0.57735 )
                {return 7;}
                else if(slope >= 0.57735  && slope < 1.7321)
                {return 8;}
                else
                {return 9;}
            }
        }
    }
}
