#include "treebranch.h"
#include "branchsection.h"

TreeBranch::TreeBranch()
{
    sections.clear();
    m_sectionNum = 0;
}

void TreeBranch::AddBranchSection(BranchSection s)
{
    sections.push_back(s);
    m_sectionNum++;
}
