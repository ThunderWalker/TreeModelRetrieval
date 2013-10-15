#include "hitmap.h"
#include "hitmapdisplaydialog.h"
#include "hitmapdisplaywidget.h"
#include <QHBoxLayout>

HitMapDisplayDialog::HitMapDisplayDialog(int height, HitMap& hitmap, QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("HitMap Display"));

    displayWidget = new HitMapDisplayWidget(height, hitmap);
    QHBoxLayout* mainLayout = new QHBoxLayout();
    mainLayout->addWidget(displayWidget);
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);

    setLayout(mainLayout);
}
