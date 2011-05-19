
#include <vector>
#include <QtGui>

#include "AntWidget.hpp"
#include "AntGuiImpl.hpp"

AntWidget::AntWidget(QWidget *parent) 
   : QWidget(parent),
     pen_(QColor(0,0,255,96))
{
	setFixedSize( 500, 500 );
    pen_.setWidth(10);
    pen_.setCapStyle(Qt::RoundCap);
    pen_.setJoinStyle(Qt::RoundJoin);
}

void AntWidget::drawObject(const antgui::Point& point, const QPixmap& pixmap)
{
   QPainter painter(this);
   painter.drawPixmap(point.x * ANT_SIZE, point.y * ANT_SIZE, pixmap);
}

void AntWidget::paintEvent(QPaintEvent*)
{  
    gui->Paint();
}