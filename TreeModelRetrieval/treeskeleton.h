#ifndef TREESKELETON_H
#define TREESKELETON_H

#include <QVector>
#include "treebranch.h"

class TreeSkeleton
{
public:
    enum AlignStyle{TrunkAtCentre,AllAtCentre};
public:
    TreeSkeleton();
    TreeSkeleton(TreeSkeleton& t);
    TreeSkeleton(QString fileName);
    TreeSkeleton& operator=(TreeSkeleton& t);

    void AddBranch(TreeBranch b);
    TreeBranch& GetBranch(int index){return branchs[index];}
    int GetBranchNum(void){return m_branchNum;}
    void Clear(){branchs.clear(); m_branchNum=0;}
    bool LoadSkeleton(QString fileName);
    void ReSize(int width, int height, AlignStyle style);
    void ReCalSize();
    QPointF GetTrunkCentroid(void);

private:
    QVector<TreeBranch> branchs;

    int m_branchNum;
    float min_x,min_y,max_x,max_y;
};

#endif // TREESKELETON_H
