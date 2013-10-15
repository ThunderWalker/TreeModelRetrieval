#ifndef HITMAPDISPLAYDIALOG_H
#define HITMAPDISPLAYDIALOG_H

#include <QDialog>

class HitMap;
class HitMapDisplayWidget;

class HitMapDisplayDialog : public QDialog
{
    Q_OBJECT
public:
    explicit HitMapDisplayDialog(int height, HitMap& hitmap, QWidget *parent = 0);
    
signals:
    
public slots:

private:
    HitMapDisplayWidget* displayWidget;
};

#endif // HITMAPDISPLAYDIALOG_H
