#include <QPainter>
#include "hitmapdisplaywidget.h"
#include "hitmap.h"
#include "hitmapedge.h"

HitMapDisplayWidget::HitMapDisplayWidget(int height, HitMap& hitmap, QWidget* parent):
    SkeletonPaintWidget(parent)
{
    m_hitmap = &hitmap;
    if(height > m_hitmap->GetResolution())
    {
        setFixedSize(height,height);
    }
    else
    {
        setFixedSize(m_hitmap->GetResolution(),m_hitmap->GetResolution());
    }
}

void HitMapDisplayWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QColor BKColor;
    BKColor.setRgb(255,255,255);
    ClearBK(painter,BKColor);
    DrawGrid(painter);
    DrawHitMap(painter);
}

void HitMapDisplayWidget::DrawGrid(QPainter &painter)
{
    QPen pen(Qt::black);
    pen.setWidth(1.0);
    painter.setPen(pen);

    int length = size().width();
    int unitLength = length/m_hitmap->GetResolution();
    int flag = 0;
    while(flag<=(length+unitLength))
    {
        painter.drawLine(flag,0,flag,length);
        painter.drawLine(0,flag,length,flag);
        flag += unitLength;
    }
}

void HitMapDisplayWidget::DrawPixel(QPainter &painter, QColor& color, int row, int col)
{
    painter.setBrush(color);

    int length = size().width();
    int unitLength = length/m_hitmap->GetResolution();
    int x = row*unitLength;
    int y = col*unitLength;

    painter.drawRect(x,y,unitLength,unitLength);
}

void HitMapDisplayWidget::DrawHitMap(QPainter &painter)
{
    QColor branchColor;
    //trunkColor.setRgb(255,0,0);
    branchColor.setRgb(0,0,0);
    //hitAreaColor.setRgb(255,141,177);
    QColor yellow,yellowOrange,orange, redOrange,red,redViolet, violet,blueViolet,blue, blueGreen,green,greenYellow;
    yellow.setRgb(244,229,0);
    yellowOrange.setRgb(253,198,11);
    orange.setRgb(241,145,1);
    redOrange.setRgb(234,98,31);
    red.setRgb(227,25,24);
    redViolet.setRgb(196,3,125);
    violet.setRgb(109,56,137);
    blueViolet.setRgb(68,78,153);
    blue.setRgb(38,113,178);
    blueGreen.setRgb(6,150,187);
    green.setRgb(0,142,91);
    greenYellow.setRgb(140,187,38);

    int resolution = m_hitmap->GetResolution();
    for(int i=0;i<resolution;i++)
    {
        for(int j=0;j<resolution;j++)
        {
            for(int k=0;k<=12;k++)
            {
                if(m_hitmap->GetHitMapPixel(i,j,k))
                {
                    switch(k)
                    {
                    case 1:
                        DrawPixel(painter,yellow,i,j);
                        break;
                    case 2:
                        DrawPixel(painter,yellowOrange,i,j);
                        break;
                    case 3:
                        DrawPixel(painter,orange,i,j);
                        break;
                    case 4:
                        DrawPixel(painter,redOrange,i,j);
                        break;
                    case 5:
                        DrawPixel(painter,red,i,j);
                        break;
                    case 6:
                        DrawPixel(painter,redViolet,i,j);
                        break;
                    case 7:
                        DrawPixel(painter,violet,i,j);
                        break;
                    case 8:
                        DrawPixel(painter,blueViolet,i,j);
                        break;
                    case 9:
                        DrawPixel(painter,blue,i,j);
                        break;
                    case 10:
                        DrawPixel(painter,blueGreen,i,j);
                        break;
                    case 11:
                        DrawPixel(painter,green,i,j);
                        break;
                    case 12:
                        DrawPixel(painter,greenYellow,i,j);
                        break;
                      default:
                        break;
                    }
                }
            }

            /*bool isHit = m_hitmap->isPixelHit(i,j);
            if(isHit)
            {
                DrawPixel(painter,hitAreaColor,i,j);
            }*/
        }
    }

    int edgeNum = m_hitmap->GetEdgeNum();
    for(int i=0;i<edgeNum;i++)
    {
        HitMapEdge edge = m_hitmap->GetEdge(i);
        int weight = edge.GetWeight();
        int edgelNum = edge.GetEdgelNum();
        for(int j=0;j<edgelNum;j++)
        {
            HitMapEdgel edgel = edge.GetEdgel(j);
            QPoint edgelPosition = edgel.GetEdgelPosition();
            if(weight == 2)
            {
                DrawPixel(painter,branchColor,edgelPosition.x(),edgelPosition.y());
            }
            else
            {
                DrawPixel(painter,branchColor,edgelPosition.x(),edgelPosition.y());
            }
        }
    }
}
