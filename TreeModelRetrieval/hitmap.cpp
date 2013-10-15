#include <QtCore>
#include "hitmap.h"
#include "treeskeleton.h"
#include "hitmapedge.h"

HitMap::HitMap(int resolution)
{
    m_resolution = resolution;
    totalEdgelNum = 0;
    hitmap = new quint16[resolution*resolution];
    ClearHitArea();
}

void HitMap::ClearHitArea()
{
    for(int i=0;i<m_resolution;i++)
    {
        for(int j=0;j<m_resolution;j++)
        {
            SetHitMapPixel(i,j,false,0);
        }
    }
    edges.clear();
}

void HitMap::SetHitMapPixel(int row, int col, bool isHit, int orientation)
{
    if(row>=0 && row<=m_resolution && col>=0 && col<=m_resolution)
    {
        int index = row*m_resolution+col;
        if(isHit)
        {
            quint16 hitBit = 1;
            hitBit = hitBit<<15;
            quint16 channelBit = 1;
            if(orientation!=0)
            {
                channelBit = channelBit<<(orientation-1);
            }
            else
            {
                channelBit = 8191;
            }

            quint16 finalBit = hitBit | channelBit;
            hitmap[index] = hitmap[index] | finalBit;
        }
        else
        {
            hitmap[index] = hitmap[index] & 0;
        }
    }
}

bool HitMap::GetHitMapPixel(int row, int col, int orientation)
{
    if(row>=0 && row<=m_resolution && col>=0 && col<=m_resolution)
    {
        int index = row*m_resolution+col;
        quint16 hitBit = 1;
        hitBit = hitBit<<15;
        quint16 isHit = hitmap[index] & hitBit;

        if(isHit == 0)
        {
            return false;
        }
        else
        {
            quint16 channelBit = 1;
            channelBit = channelBit<<(orientation-1);
            quint16 isChannelHit = hitmap[index] & channelBit;
            if(isChannelHit == 0)
            {
                return false;
            }
            else
            {
                return true;
            }
        }
    }
    else
    {
        return false;
    }
}

bool HitMap::isPixelHit(int row, int col)
{
    int index = row*m_resolution+col;
    quint16 hitBit = 1;
    hitBit = hitBit<<15;
    quint16 isHit = hitmap[index] & hitBit;

    if(isHit == 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void HitMap::GenerateHitMap(TreeSkeleton& skeleton)
{
    skeleton.ReSize(m_resolution,m_resolution,TreeSkeleton::TrunkAtCentre);
    GenerateEdgel(skeleton);
    CalculateHitArea();
}


void HitMap::GenerateEdgel(TreeSkeleton skeleton)
{
    int branchNum = skeleton.GetBranchNum();
    for(int i=0;i<branchNum;i++)
    {
        TreeBranch& branch = skeleton.GetBranch(i);
        int weight;
        if(i == 0)
        {
            weight = 3;
        }
        else
        {
            weight = 2;
        }
        HitMapEdge edge(branch,weight);
        edges.push_back(edge);
    }
    CalculateTotalEdgel();
}

void HitMap::CalculateTotalEdgel()
{
    int edgelCount = 0;
    int edgeNum = GetEdgeNum();
    for(int i=0;i<edgeNum;i++)
    {
        HitMapEdge edge = GetEdge(i);
        int edgelNum = edge.GetEdgelNum();
        edgelCount += edgelNum;
    }
    totalEdgelNum = edgelCount;
}

void HitMap::CalculateHitArea()
{
    int edgeNum = edges.size();
    for(int i=0;i<edgeNum;i++)
    {
        HitMapEdge edge = GetEdge(i);
        int edgelNum = edge.GetEdgelNum();
        int weight = edge.GetWeight();

        for(int j=0;j<edgelNum;j++)
        {
            HitMapEdgel edgel = edge.GetEdgel(j);
            QPoint edgelPosition = edgel.GetEdgelPosition();
            int x = edgelPosition.x();
            int y = edgelPosition.y();
            int orientation = edgel.GetOrientationChannel();
            CalculatePixelHitArea(x,y,weight,orientation);
        }
    }
}

void HitMap::CalculatePixelHitArea(int row, int col, int weight, int orientation)
{
    for(int i=weight;i>=0;i--)
    {
        int upRow = row - i;
        int downRow = row + i;
        for(int j=0;j<=(weight -i);j++)
        {
            int leftCol = col - j;
            int rightCol = col + j;
            if(upRow >= 0)
            {
                if(leftCol >= 0)
                {
                    SetHitMapPixel(upRow,leftCol,true,orientation);
                }
                if(rightCol <= m_resolution)
                {
                    SetHitMapPixel(upRow,rightCol,true,orientation);
                }
            }

            if(downRow <= m_resolution)
            {
                if(leftCol >= 0)
                {
                    SetHitMapPixel(downRow,leftCol,true,orientation);
                }
                if(rightCol <= m_resolution)
                {
                    SetHitMapPixel(downRow,rightCol,true,orientation);
                }
            }
        }
    }
    /*for(int i=1;i<=3;i++)
    {
        int left_x = row -i;
        int right_x = row + i;
        int up_y = col -i;
        int down_y = col + i;

        if(left_x >= 0)
        {
            SetHitMapPixel(left_x,col,true);
        }
        if(right_x <= m_resolution)
        {
            SetHitMapPixel(right_x,col,true);
        }
        if(up_y >= 0)
        {
            SetHitMapPixel(row,up_y,true);
        }
        if(down_y <= m_resolution)
        {
            SetHitMapPixel(row,down_y,true);
        }
    }

    for(int i=1;i<=2;i++)
    {
        int up_y = col - i;
        int down_y = col + i;

        if(up_y >= 0)
        {
            for(int j=1;j<=2;j++)
            {
                int left_x = row -j;
                int right_x = row + j;

                if(left_x >= 0)
                {
                    SetHitMapPixel(left_x,up_y,true);
                }
                if(right_x <= m_resolution)
                {
                    SetHitMapPixel(right_x,up_y,true);
                }
            }
        }
        if(down_y <= m_resolution)
        {
            for(int j=1;j<=2;j++)
            {
                int left_x = row -j;
                int right_x = row + j;

                if(left_x >= 0)
                {
                    SetHitMapPixel(left_x,down_y,true);
                }
                if(right_x <= m_resolution)
                {
                    SetHitMapPixel(right_x,down_y,true);
                }
            }
        }
    }*/
}
