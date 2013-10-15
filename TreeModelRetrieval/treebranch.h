#ifndef TREEBRANCH_H
#define TREEBRANCH_H

#include <QVector>
#include "branchsection.h"

class TreeBranch
{
public:
    TreeBranch();

    void AddBranchSection(BranchSection s);
    BranchSection& GetSection(int index){return sections[index];}
    int GetSectionNum(void){return m_sectionNum;}
    void Clear(void){sections.clear(); m_sectionNum = 0;}
private:
    QVector<BranchSection> sections;

    int m_sectionNum;
};

#endif // TREEBRANCH_H
