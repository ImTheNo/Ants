#include "AntGuiImpl.hpp"

namespace antgui 
{
    void AntGuiQtImpl::SetAnt(const Ant *ant)
	{
        TestAnt *new_ant = new TestAnt(ant);
		map<Point,const Ant*>::iterator it = antMap.find(new_ant->getPoint());

/*		if (it != antMap.end() && it->second->getTeamId() != new_ant->getTeamId()) 
		{
			throw GUIException("Point is used by another team ants");
		}
  */
		if (it == antMap.end() || *(it->second) < *new_ant) 
		{
			antMap.insert(pair<Point, const Ant*>(new_ant->getPoint(), new_ant));
		}
	}


	struct PaintFood 
	{
		void operator() (const std::map<Point,const Food*>::value_type& p) 
		{
			const Food* food = p.second;
			QFoodPainter foodPainter(*food);
			foodPainter.draw();
			widget->drawObject(food->getPoint(), foodPainter.pixmap());
		}
		PaintFood(AntWidget * widget, AntGuiQtImpl& antGuiImpl) : widget(widget), antGuiImpl(antGuiImpl) {}
	private:
		AntWidget * widget;
		AntGuiQtImpl& antGuiImpl;
	};

	struct PaintAnt 
	{
		void operator() (const map<Point,const Ant*>::value_type& p) 
		{
			const Ant* ant = p.second;
			QAntPainter *antPainter = QAntPainter::getAntPainter(*ant);
			antPainter->draw(antGuiImpl.teamColor[ant->getTeamId()]);
			widget->drawObject(ant->getPoint(), antPainter->pixmap());
			delete antPainter;
		}
		PaintAnt(AntWidget * widget, AntGuiQtImpl& antGuiImpl) : widget(widget), antGuiImpl(antGuiImpl) {}
	private:
		AntWidget * widget;
		AntGuiQtImpl& antGuiImpl;
	};

	void AntGuiQtImpl::Paint()
	{
        paintLock.lock();
        widget->clear();
        PaintFood paintFood(widget, *this);
		PaintAnt paintAnt(widget, *this);
		for_each(foodMap.begin(), foodMap.end(), paintFood);
		for_each(antMap.begin(), antMap.end(), paintAnt);
        paintLock.unlock();
	}

	QAntPainter* QAntPainter::getAntPainter(const Ant& ant)
	{
		QAntPainter *res;
		if (ant.hasFood()) 
		{
			res = new QFoodAntPainter(ant);
		} 
		else if (ant.isFrozen()) 
		{
			res = new QFrozenAntPainter(ant);
		} 
		else 
		{
			res = new QAntPainter(ant);
		}
		return res;
	}
}
