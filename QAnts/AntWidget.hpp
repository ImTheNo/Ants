#ifndef ANT_WIDGET_HPP
#define ANT_WIDGET_HPP

#include <QWidget>
#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QColor>
#include <QPen>
#include <iostream>
#include <string>
#include "IAntGUI.hpp"

class AntWidget : public QWidget 
{
      Q_OBJECT
      
   private: 
      QPixmap pix_;
      QPen pen_;
	  antgui::IAntGui* gui;
   public:
      AntWidget(QWidget *parent=0);
	  void drawObject(const antgui::Point&, const QPixmap&);
      inline QPixmap const& pixmap() const { return pix_;}
      void clear() 
      {
         QPainter painter(this);
     	 painter.setPen(pen_);
//    	 painter.drawRect(0, 0, width(), height());
      }
	  void setGui(antgui::IAntGui* gui)
	  {
		  this->gui = gui;
	  }
   signals:
      void antViewChanged();
   protected:    
      void paintEvent(QPaintEvent* ev);
};

#endif // ANT_WIDGET_HPP
