#include "branchsection.h"

BranchSection::BranchSection()
{
    partitions.clear();
    m_partitionNum = 0;
}

BranchSection::BranchSection(BranchPartition& p)
{
    partitions.clear();
    m_partitionNum = 0;
    AddPartition(p);

}

void BranchSection::AddPartition(BranchPartition p)
{
    partitions.push_back(p);
    m_partitionNum++;
}
