#ifndef TREEMODELRETRIEVALDIALOG_H
#define TREEMODELRETRIEVALDIALOG_H

#include <QDialog>

class QHBoxLayout;
class UserSketchWidget;
class ResultDisplayWidget;
class QLabel;
class QPushButton;
class QRadioButton;
class QGroupBox;
class HitMap;

class TreeModelRetrievalDialog : public QDialog
{
    Q_OBJECT
    
public:
    enum RuningPhase{readyToRetrieval,skechTrunk,sketchBranch,displayResult};

public:
    TreeModelRetrievalDialog(QWidget *parent = 0);
    ~TreeModelRetrievalDialog();

private:
    float SimilarityComparison(HitMap& ha, HitMap& hb);

signals:
    void PhaseChanged(RuningPhase);
    void DisplayStatueChanged(bool);

private slots:
    void SetRunningPhase(RuningPhase);
    void SketchTrunkDone(){emit PhaseChanged(sketchBranch);}
    void RetrievalButtonClicked();
    void CommitButtonClicked();

private:
    void InitSketchFrame();
    void InitResultFrame();
    void InitWidgetLayout();

    void Retrieval();
    void ClearResultWindow();

private:
    UserSketchWidget* sketchWindow;
    QGroupBox* leftSketchFrame;
    QGroupBox* displayStatueBox;
    QRadioButton* skeletonStatue;
    QRadioButton* picturStatue;
    QLabel* guideLine;
    QPushButton* RetrievalButton;
    QPushButton* CommitButton;
    QPushButton* configDataBaseButton;

    ResultDisplayWidget* resultWindows[9];
    QGroupBox* displayResultBox;

    QHBoxLayout* mainLayout;
    QHBoxLayout* sketchFrame;
    QHBoxLayout* resultFrame;

    bool isRetrieval;

};

#endif // TREEMODELRETRIEVALDIALOG_H
