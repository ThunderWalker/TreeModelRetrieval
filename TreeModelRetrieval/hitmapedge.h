#ifndef HITMAPEDGE_H
#define HITMAPEDGE_H

#include <QVector>
#include "hitmapedgel.h"

class TreeBranch;

class HitMapEdge
{
public:
    HitMapEdge(TreeBranch& branch, int branchWeight = 0);
    HitMapEdge();

    void AddEdgel(QPointF& start, QPointF& end);
    void GenerateEdge(TreeBranch& branch);
    int GetEdgelNum(){return edgels.size();}
    int GetWeight(){return weight;}
    HitMapEdgel& GetEdgel(int index){return edgels[index];}

private:
    int CalculateOrientationChannel(QPointF& start, QPointF& end);

private:
    QVector<HitMapEdgel> edgels;
    int weight;
};

#endif // HITMAPEDGE_H
