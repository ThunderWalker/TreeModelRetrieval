#ifndef HITMAP_H
#define HITMAP_H

class TreeSkeleton;
class QPointF;

#include <QVector>
#include <QPoint>
#include "hitmapedge.h"

class HitMap
{
public:
    HitMap(int resolution);
    ~HitMap(void){delete hitmap;}
    void SetHitMapPixel(int row, int col, bool isHit, int orientation);
    bool GetHitMapPixel(int row, int col, int orientation);
    bool isPixelHit(int row, int col);
    void GenerateHitMap(TreeSkeleton& skeleton);
    int GetResolution(){return m_resolution;}
    int GetEdgeNum(){return edges.size();}
    int GetTotalEdgelNum(){return totalEdgelNum;}
    HitMapEdge GetEdge(int index){return edges[index];}
    void ClearHitArea();

private:   
    void GenerateEdgel(TreeSkeleton skeleton);
    void CalculateTotalEdgel();
    void CalculateHitArea();
    void CalculatePixelHitArea(int row, int col, int weight, int orientation);

private:
    int m_resolution;
    int totalEdgelNum;
    quint16* hitmap;
    QVector<HitMapEdge> edges;
};

#endif // HITMAP_H
