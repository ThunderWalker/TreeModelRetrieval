#include "branchpartition.h"

BranchPartition::BranchPartition(float x, float y, int priSec, int priPar)
{
    m_centroid.setX(x);
    m_centroid.setY(y);
    priorSection = priSec;
    priorPartition = priPar;
}

void BranchPartition::SetCentroid(float x, float y)
{
    m_centroid.setX(x);
    m_centroid.setY(y);
}
