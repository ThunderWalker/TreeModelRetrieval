#ifndef HITMAPEDGEL_H
#define HITMAPEDGEL_H

#include <QPoint>

class HitMapEdgel
{
public:
    HitMapEdgel(int x = 0, int y = 0, int orientation = 0);

    QPoint GetEdgelPosition(){return edgel;}
    int GetOrientationChannel(){return orientationChannel;}

private:
    QPoint edgel;
    int orientationChannel;
};

#endif // HITMAPEDGEL_H
