#ifndef BRANCHSECTION_H
#define BRANCHSECTION_H

#include <QVector>
#include "branchpartition.h"

class BranchSection
{
public:
    BranchSection();
    BranchSection(BranchPartition& p);

    void AddPartition(BranchPartition p);
    BranchPartition& GetPartition(int index){return partitions[index];}
    int GetPartitionNum(void){return m_partitionNum;}
private:
    QVector<BranchPartition> partitions;

    int m_partitionNum;
};

#endif // BRANCHSECTION_H
