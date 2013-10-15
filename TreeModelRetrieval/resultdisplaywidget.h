#ifndef RESULTDISPLAYWIDGET_H
#define RESULTDISPLAYWIDGET_H

#include "skeletonpaintwidget.h"

class ResultDisplayWidget : public SkeletonPaintWidget
{
public:
    ResultDisplayWidget(int height, QWidget* parent = 0);

    void DisplaySkeleton(QString fileName);
    void Clear(void);

protected:
    void paintEvent(QPaintEvent *);

private:
    void SetDisplayStatue(bool isDisplay){isDisplayResult = isDisplay;}
    bool GetDisplayStatue(void){return isDisplayResult;}

private:
    bool isDisplayResult;
    TreeSkeleton* skeleton;
};

#endif // RESULTDISPLAYWIDGET_H
