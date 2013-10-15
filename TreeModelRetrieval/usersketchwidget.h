#ifndef USERSKETCHWIDGET_H
#define USERSKETCHWIDGET_H

#include <QVector>
#include "skeletonpaintwidget.h"
#include "treeskeleton.h"

class UserSketchWidget :public SkeletonPaintWidget
{
    Q_OBJECT

public:
    enum SketchPhase{sketchTrunk,sketchBranch};

public:
    UserSketchWidget(QWidget *parent = 0);

    void SetSketchPhase(SketchPhase phase){currentPhase = phase;}
    TreeSkeleton& GetSkeleton(){return *skeleton;}
    bool BeginSketch();
    bool EndSketch();
    bool GetSketchStatue(){return isSketching;}
    bool GetDisplayStatue(){return isDisplayResult;}

signals:
    void SketchTrunkDone();

public slots:
    void DisplayConfigure(bool);

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    void SetSketchStatue(bool isSketch){isSketching = isSketch;}
    void SetDisplayStatue(bool isDisplay){isDisplayResult = isDisplay;}
    void AddPointToBranch(QPoint point);

private:
    bool isSketching;
    bool isDisplayResult;
    bool isMousePress;
    int sectionCount;
    SketchPhase currentPhase;
    TreeBranch* branch;
    TreeSkeleton* skeleton;
};

#endif // USERSKETCHWIDGET_H
