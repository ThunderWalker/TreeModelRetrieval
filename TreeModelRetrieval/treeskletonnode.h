#ifndef TREESKLETONNODE_H
#define TREESKLETONNODE_H

class TreeSkletonNode
{
public:
    TreeSkletonNode(double x = 0.0, double y = 0.0);

    void SetNodePosition(double x, double y);
private:
    double m_x,m_y;
};

#endif // TREESKLETONNODE_H
