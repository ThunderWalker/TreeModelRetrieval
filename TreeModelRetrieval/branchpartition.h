#ifndef BRANCHPARTITION_H
#define BRANCHPARTITION_H

#include <QPointF>
#include "treeskletonnode.h"

class BranchPartition
{
public:
    BranchPartition(float x = 0.0, float y = 0.0, int priSec = -1, int priPar = -1);

    QPointF GetCentroid(void){return m_centroid;}
    int GetPriorSection(void){return priorSection;}
    int GetPriorPartition(void){return priorPartition;}
    void SetCentroid(float x, float y);
private:
    QPointF m_centroid;

    int priorSection;
    int priorPartition;
};

#endif // BRANCHPARTITION_H
