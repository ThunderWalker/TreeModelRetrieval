#include "treeskeleton.h"
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QMessageBox>

TreeSkeleton::TreeSkeleton()
{
    branchs.clear();
    m_branchNum = 0;
    min_x = min_y = max_x = max_y = 0.0;
}

TreeSkeleton::TreeSkeleton(TreeSkeleton& t)
{
    for(int i=0;i<t.m_branchNum;i++)
    {
       AddBranch(t.branchs[i]);
    }
    this->m_branchNum = t.m_branchNum;
    this->max_x = t.max_x;
    this->max_y = t.max_y;
    this->min_x = t.min_x;
    this->min_y = t.min_y;
}

TreeSkeleton::TreeSkeleton(QString fileName)
{
    branchs.clear();
    m_branchNum = 0;
    LoadSkeleton(fileName);
}

TreeSkeleton& TreeSkeleton::operator=(TreeSkeleton& t)
{
    for(int i=0;i<t.m_branchNum;i++)
    {
       AddBranch(t.branchs[i]);
    }
    this->m_branchNum = t.m_branchNum;
    this->max_x = t.max_x;
    this->max_y = t.max_y;
    this->min_x = t.min_x;
    this->min_y = t.min_y;
    return *this;
}

void TreeSkeleton::AddBranch(TreeBranch b)
{
    branchs.push_back(b);
    m_branchNum++;
}

bool TreeSkeleton::LoadSkeleton(QString fileName)
{
    if(branchs.size() > 0)
    {
        branchs.clear();
        m_branchNum = 0;
    }

    this->min_x = 0.0;
    this->min_y = 0.0;
    this->max_x = 0.0;
    this->max_y = 0.0;

    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        return false;
    }

    TreeBranch branch;
    float min_width,max_width,min_height,max_height;
    bool isFirst = true;
    int lastSectionIndex = -1;

    QTextStream in(&file);
    while(!in.atEnd())
    {
        QString line = in.readLine();
        QStringList fields = line.split(' ');
        if(fields.at(0) == "b")
        {
            if(branch.GetSectionNum() > 1)
            {
                AddBranch(branch);
                lastSectionIndex = -1;
                branch.Clear();
            }
        }
        else if(fields.at(0) == "v")
        {
            int sectionIndex = fields.at(1).toInt();
            float x = fields.at(2).toFloat();
            float y = -(fields.at(3).toFloat());
            int priSection = fields.at(4).toInt();
            int priPartition = fields.at(5).toInt();

            if(isFirst)
            {
                min_width = max_width = x;
                min_height = max_height = y;
                isFirst = false;
            }
            else
            {
                if(x < min_width)
                {
                    min_width = x;
                }
                if(x > max_width)
                {
                    max_width = x;
                }
                if(y < min_height)
                {
                    min_height = y;
                }
                if(y > max_height)
                {
                    max_height = y;
                }
            }

            BranchPartition p(x,y,priSection,priPartition);

            if(sectionIndex!=lastSectionIndex)
            {
                BranchSection s(p);
                branch.AddBranchSection(s);
                lastSectionIndex = sectionIndex;
            }
            else
            {
               BranchSection& lastSection = branch.GetSection(lastSectionIndex);
               lastSection.AddPartition(p);
            }
        }
    }
    AddBranch(branch);
    branch.Clear();
    this->min_x = min_width;
    this->min_y = min_height;
    this->max_x = max_width;
    this->max_y = max_height;

    return true;
}

QPointF TreeSkeleton::GetTrunkCentroid()
{
    float min_width,max_width,min_height,max_height;
    bool isFirst = true;

    TreeBranch& trunk = this->GetBranch(0);
    int sectionNum = trunk.GetSectionNum();
    for(int i=0;i<sectionNum;i++)
    {
        BranchSection& section = trunk.GetSection(i);
        int partitionNum = section.GetPartitionNum();
        for(int j=0;j<partitionNum;j++)
        {
            BranchPartition& p = section.GetPartition(j);
            QPointF centroid = p.GetCentroid();
            float x = centroid.x();
            float y = centroid.y();

            if(isFirst)
            {
                min_width = max_width = x;
                min_height = max_height = y;
                isFirst = false;
            }
            else
            {
                if(x < min_width)
                {
                    min_width = x;
                }
                if(x > max_width)
                {
                    max_width = x;
                }
                if(y < min_height)
                {
                    min_height = y;
                }
                if(y > max_height)
                {
                    max_height = y;
                }
            }
        }
    }
    float trunk_x = (min_width+max_width)/2.0;
    float trunk_y = (min_height+max_height)/2.0;
    QPointF trunkCentroid(trunk_x,trunk_y);
    return trunkCentroid;
}

void TreeSkeleton::ReSize(int width, int height, AlignStyle style)
{
    float radio = 0.0;
    float treeWidth = max_x - min_x;
    float treeHeight = max_y - min_y;

    if(treeHeight > treeWidth)
    {
        radio = ((float)height)*0.9/treeHeight;
    }
    else
    {
        radio = ((float)width)*0.9/treeWidth;
    }

    int branchNum = m_branchNum;
    for(int i=0;i<branchNum;i++)
    {
        TreeBranch& branch = this->GetBranch(i);
        int sectionNum = branch.GetSectionNum();
        for(int j=0;j<sectionNum;j++)
        {
            BranchSection& section = branch.GetSection(j);
            int partitionNum = section.GetPartitionNum();
            for(int k=0;k<partitionNum;k++)
            {
                BranchPartition& p = section.GetPartition(k);
                QPointF centroid = p.GetCentroid();
                p.SetCentroid(centroid.x()*radio, centroid.y()*radio);
            }
        }
    }
    max_x = max_x*radio;
    max_y = max_y*radio;
    min_x = min_x*radio;
    min_y = min_y*radio;

    float mid_x = (max_x + min_x)/2.0;
    float mid_y = (max_y + min_y)/2.0;

    float dis_x, dis_y;
    dis_y = mid_y - ((float)height)/2.0;

    if(style == TrunkAtCentre)
    {
        QPointF trunkCentroid = GetTrunkCentroid();
        dis_x = trunkCentroid.x() - ((float)width)/2.0;
    }
    else
    {
        dis_x = mid_x - ((float)width)/2.0;
    }

    for(int i=0;i<branchNum;i++)
    {
        TreeBranch& branch = this->GetBranch(i);
        int sectionNum = branch.GetSectionNum();
        for(int j=0;j<sectionNum;j++)
        {
            BranchSection& section = branch.GetSection(j);
            int partitionNum = section.GetPartitionNum();
            for(int k=0;k<partitionNum;k++)
            {
                BranchPartition& p = section.GetPartition(k);
                QPointF centroid = p.GetCentroid();
                p.SetCentroid(centroid.x()-dis_x, centroid.y()-dis_y);
            }
        }
    }
}

void TreeSkeleton::ReCalSize()
{
    float min_width,max_width,min_height,max_height;
    bool isFirst = true;

    int branchNum = m_branchNum;
    for(int i=0;i<branchNum;i++)
    {
        TreeBranch& branch = this->GetBranch(i);
        int sectionNum = branch.GetSectionNum();
        for(int j=0;j<sectionNum;j++)
        {
            BranchSection& section = branch.GetSection(j);
            int partitionNum = section.GetPartitionNum();
            for(int k=0;k<partitionNum;k++)
            {
                BranchPartition& p = section.GetPartition(k);
                QPointF centroid = p.GetCentroid();

                float x = centroid.x();
                float y = centroid.y();

                if(isFirst)
                {
                    min_width = max_width = x;
                    min_height = max_height = y;
                    isFirst = false;
                }
                else
                {
                    if(x < min_width)
                    {
                        min_width = x;
                    }
                    if(x > max_width)
                    {
                        max_width = x;
                    }
                    if(y < min_height)
                    {
                        min_height = y;
                    }
                    if(y > max_height)
                    {
                        max_height = y;
                    }
                }
            }
        }
    }
    this->min_x = min_width;
    this->min_y = min_height;
    this->max_x = max_width;
    this->max_y = max_height;
}



