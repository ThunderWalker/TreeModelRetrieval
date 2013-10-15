#include <QtCore>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QGroupBox>
#include <QMessageBox>
#include <QMap>
#include "treemodelretrievaldialog.h"
#include "usersketchwidget.h"
#include "resultdisplaywidget.h"
#include "hitmapdisplaydialog.h"
#include "hitmap.h"
#include "hitmapedge.h"

TreeModelRetrievalDialog::TreeModelRetrievalDialog(QWidget *parent)
    : QDialog(parent)
{  
    setWindowTitle(tr("TreeModel Retrieval System"));

    InitWidgetLayout();

    connect(this,SIGNAL(PhaseChanged(RuningPhase)),this,SLOT(SetRunningPhase(RuningPhase)));
    connect(sketchWindow,SIGNAL(SketchTrunkDone()),this,SLOT(SketchTrunkDone()));
    connect(RetrievalButton,SIGNAL(clicked()),this,SLOT(RetrievalButtonClicked()));
    connect(CommitButton,SIGNAL(clicked()),this,SLOT(CommitButtonClicked()));
    connect(this,SIGNAL(DisplayStatueChanged(bool)),sketchWindow,SLOT(DisplayConfigure(bool)));

    isRetrieval = false;
    emit PhaseChanged(readyToRetrieval);
}

TreeModelRetrievalDialog::~TreeModelRetrievalDialog()
{
}

void TreeModelRetrievalDialog::InitSketchFrame()
{
    sketchWindow = new UserSketchWidget();
    QHBoxLayout* userSketchLayout = new QHBoxLayout();
    userSketchLayout->addWidget(sketchWindow);
    leftSketchFrame = new QGroupBox(tr("手绘草图区域"));
    leftSketchFrame->setLayout(userSketchLayout);
    leftSketchFrame->setAlignment(Qt::AlignHCenter);

    skeletonStatue = new QRadioButton(tr("显示树木骨架图"));
    picturStatue = new QRadioButton(tr("显示树木渲染图"));
    skeletonStatue->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    picturStatue->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    skeletonStatue->setChecked(true);
    displayStatueBox = new QGroupBox(tr("请选择显示结果的方式："));
    QVBoxLayout* displayStatueBoxLayout = new QVBoxLayout();
    displayStatueBoxLayout->addWidget(skeletonStatue);
    displayStatueBoxLayout->addWidget(picturStatue);
    displayStatueBoxLayout->setAlignment(Qt::AlignHCenter);
    displayStatueBox->setLayout(displayStatueBoxLayout);
    displayStatueBox->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);

    guideLine = new QLabel(tr("提示：请点击开始检索"));
    guideLine->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    QHBoxLayout* guideLayout = new QHBoxLayout();
    guideLayout->addWidget(guideLine);
    guideLayout->setAlignment(Qt::AlignHCenter);

    RetrievalButton = new QPushButton(tr("开始检索"));
    CommitButton = new QPushButton(tr("提交检索"));
    configDataBaseButton = new QPushButton(tr("配置数据库"));
    RetrievalButton->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    CommitButton->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    configDataBaseButton->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    QVBoxLayout* buttonBox = new QVBoxLayout();
    buttonBox->addWidget(CommitButton);
    buttonBox->addWidget(RetrievalButton);  
    buttonBox->addWidget(configDataBaseButton);
    buttonBox->setAlignment(Qt::AlignHCenter);
    CommitButton->setVisible(false);

    QVBoxLayout* rightSketchFrame = new QVBoxLayout();
    rightSketchFrame->addWidget(displayStatueBox);
    rightSketchFrame->addStretch();
    rightSketchFrame->addLayout(guideLayout);
    rightSketchFrame->addStretch();
    rightSketchFrame->addLayout(buttonBox);

    sketchFrame = new QHBoxLayout();
    sketchFrame->addWidget(leftSketchFrame);
    sketchFrame->addLayout(rightSketchFrame);
}

void TreeModelRetrievalDialog::InitResultFrame()
{
    QGridLayout* resultLayout = new QGridLayout();
    int resultWindowHeight = sketchWindow->height()/3;
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            resultWindows[i*3+j] = new ResultDisplayWidget(resultWindowHeight);
            resultLayout->addWidget(resultWindows[i*3+j],i,j);
        }
    }
    resultLayout->setAlignment(Qt::AlignHCenter);
    displayResultBox = new QGroupBox(tr("检索结果"));
    displayResultBox->setLayout(resultLayout);
    displayResultBox->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    displayResultBox->setVisible(false);

    resultFrame = new QHBoxLayout();
    resultFrame->addWidget(displayResultBox);
}

void TreeModelRetrievalDialog::InitWidgetLayout()
{
    InitSketchFrame();
    InitResultFrame();

    mainLayout = new QHBoxLayout();
    mainLayout->addLayout(sketchFrame);
    mainLayout->addStretch();
    mainLayout->addLayout(resultFrame);
    setLayout(mainLayout);
}

void TreeModelRetrievalDialog::RetrievalButtonClicked()
{
    if(!isRetrieval)
    {
        RetrievalButton->setText(tr("重新检索"));
        isRetrieval = true;
        emit PhaseChanged(skechTrunk);
    }
    else
    {  
        if(sketchWindow->EndSketch())
        {
            if(sketchWindow->GetDisplayStatue())
            {
                ClearResultWindow();
                emit DisplayStatueChanged(false);
            }

            RetrievalButton->setText(tr("开始检索"));
            isRetrieval = false;
            emit PhaseChanged(readyToRetrieval);
        }
        else
        {
            exit(0);
        }
    }
}

void TreeModelRetrievalDialog::CommitButtonClicked()
{
    emit DisplayStatueChanged(true);
    emit PhaseChanged(displayResult);    
}

void TreeModelRetrievalDialog::SetRunningPhase(RuningPhase rp)
{
    switch(rp)
    {
    case readyToRetrieval:
        guideLine->setText(tr("提示：请点击开始检索"));
        skeletonStatue->setChecked(true);
        displayResultBox->setVisible(false);
        CommitButton->setVisible(false);

        mainLayout->setSizeConstraint(QLayout::SetFixedSize);
        break;
    case skechTrunk:
        if(sketchWindow->BeginSketch())
        {
            guideLine->setText(tr("   提示：请绘制树干姿态\n\n(点击重新检索开始一次检索)"));
        }
        else
        {
            PhaseChanged(readyToRetrieval);
        }
        break;
    case sketchBranch:
        guideLine->setText(tr("   提示：请绘制次级枝干\n\n(绘制完成请点击提交检索)"));
        CommitButton->setVisible(true);
        break;
    case displayResult:
        guideLine->setText(tr("   提示：请点击查看结果\n\n(点击重新检索开始一次检索)"));
        Retrieval();
        displayResultBox->setVisible(true);
        CommitButton->setVisible(false);

        mainLayout->setSizeConstraint(QLayout::SetFixedSize);
        break;
    default:
        break;
    }
}

float TreeModelRetrievalDialog::SimilarityComparison(HitMap& ha, HitMap& hb)
{
    float score = 0.0;
    int edgeNum = ha.GetEdgeNum();
    for(int i=0;i<edgeNum;i++)
    {
        HitMapEdge edge = ha.GetEdge(i);
        int edgelNum = edge.GetEdgelNum();
        for(int j=0;j<edgelNum;j++)
        {
            HitMapEdgel edgel = edge.GetEdgel(j);
            QPoint edgelPosition = edgel.GetEdgelPosition();
            int orientation = edgel.GetOrientationChannel();
            if(hb.GetHitMapPixel(edgelPosition.x(),edgelPosition.y(),orientation))
            {
                score ++;
            }
            else if(hb.GetHitMapPixel(edgelPosition.x(),edgelPosition.y(),orientation-1) || hb.GetHitMapPixel(edgelPosition.x(),edgelPosition.y(),orientation+1))
            {
                score = score + 0.5;
            }
        }
    }
    score = score/hb.GetTotalEdgelNum();
    return score;
}

void TreeModelRetrievalDialog::Retrieval()
{
    TreeSkeleton& sketchSkelton = sketchWindow->GetSkeleton();
    TreeSkeleton copySketch(sketchSkelton);
    sketchSkelton.ReCalSize();
    copySketch.ReCalSize();
    sketchSkelton.ReSize(sketchWindow->size().width(),sketchWindow->size().height(),TreeSkeleton::TrunkAtCentre);
    update();

    HitMap sketchHitmap(300);
    sketchHitmap.GenerateHitMap(copySketch);
    HitMapDisplayDialog h(600,sketchHitmap);
    h.exec();

    float score;
    int index;
    QStringList files;
    files<<"OC54_3"<<"BL02m"<<"BL02a"<<"AL01y"<<"AL01m"<<"AL01a"<<"BS01y"<<"BL02y"<<"EU22y";
    QMultiMap<float,QString> result;

    //TreeSkeleton* sampleSkelton = new TreeSkeleton();
    //HitMap* sampleHitmap = new HitMap(300);
    foreach (const QString& BaseName, files)
    {
        score = 0.0;
        index = 1;
        for(int i=0;i<4;i++)
        {            
            QString filePath =  QString("%1%2\\%3(%4)%5")
                    .arg(tr("skeleton\\")).arg(BaseName).arg(BaseName).arg(i+1).arg(tr(".skt"));
            TreeSkeleton sampleSkelton(filePath);
            HitMap sampleHitmap(300);
            sampleHitmap.ClearHitArea();
            sampleHitmap.GenerateHitMap(sampleSkelton);
            //HitMapDisplayDialog d(600,sampleHitmap);
            //d.exec();
            float temp_score = sqrtf(SimilarityComparison(sketchHitmap,sampleHitmap)*SimilarityComparison(sampleHitmap,sketchHitmap));
            //sampleSkelton->LoadSkeleton(filePath);
            //sampleHitmap->ClearHitArea();
            //sampleHitmap->GenerateHitMap(*sampleSkelton);
            //HitMapDisplayDialog d(600,*sampleHitmap);
            //d.exec();
           // float temp_score = sqrtf(SimilarityComparison(sketchHitmap,*sampleHitmap)*SimilarityComparison(*sampleHitmap,sketchHitmap));
            if(temp_score > score)
            {
                score = temp_score;
                index = i+1;
            }
        }
        QString choseFile = QString("%1%2\\%3(%4)%5")
                .arg(tr("skeleton\\")).arg(BaseName).arg(BaseName).arg(index).arg(tr(".skt"));
        result.insert(score,choseFile);
    }
    //delete sampleSkelton;
    //delete sampleHitmap;

    int count = 0;
    QMultiMap<float, QString>::const_iterator iterator;
    for(iterator = result.end()-1;iterator!=result.begin()-1;iterator--)
    {
        if(count<9)
        {
            QString filePath = iterator.value();
            resultWindows[count]->DisplaySkeleton(filePath);
        }
        else
        {
            break;
        }
        count++;
    }
}

void TreeModelRetrievalDialog::ClearResultWindow()
{
    for(int i=0;i<9;i++)
    {
        resultWindows[i]->Clear();
    }
}
