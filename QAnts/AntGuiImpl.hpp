#ifndef _ANT_GUI_IMPL_H_
#define _ANT_GUI_IMPL_H_

#include "IAntGUI.hpp"
#include "AntWidget.hpp"
#include <QPainter>
#include <QMutex>
#include <map>
#include <stack>
#include <iostream>
#include <memory>
#include <algorithm>

#define ANT_SIZE 10

using namespace std;

namespace antgui 
{
	class AntGuiQtImpl;

	class QFieldObject
	{
	protected:
		QPixmap pix;
		QFieldObject()  : pix(ANT_SIZE, ANT_SIZE) {}
	public:
		const QPixmap& pixmap() const
		{
			return pix;
		}
	};

	class QFoodPainter : public QFieldObject
	{	
		const Food &food;
	public:
		QFoodPainter(const Food &food) : food(food) {};
		virtual void draw()
		{
			QPainter painter(&pix);
			painter.setBrush(QBrush(QColor(200 - food.getCount(), 200 - food.getCount(), 200 - food.getCount())));
			painter.drawEllipse(0, 0, ANT_SIZE, ANT_SIZE);
		}
	};

	class QAntPainter : public QFieldObject
	{
	protected:
		QAntPainter(const Ant &ant) : ant(ant) {};
		const Ant &ant;
	public:
		virtual void draw(const QColor& color)
		{
			QPainter painter(&pix);
			painter.fillRect(0, 0, ANT_SIZE, ANT_SIZE, color);
		}

		static QAntPainter* getAntPainter(const Ant& ant);
	};

	class QFrozenAntPainter : public QAntPainter
	{
	public:
		QFrozenAntPainter(const Ant &ant) : QAntPainter(ant) {}
		virtual void draw(const QColor& color)
		{
			QAntPainter::draw(color);
			QPainter painter(&pix);
			painter.drawLine(0, 0, ANT_SIZE, ANT_SIZE);
			painter.drawLine(0, ANT_SIZE, ANT_SIZE, 0);
		}

	};

	class QFoodAntPainter : public QAntPainter
	{
	public:
		QFoodAntPainter(const Ant &ant) : QAntPainter(ant) {}
		virtual void draw(const QColor& color)
		{
			QAntPainter::draw(color);
			QPainter painter(&pix);
			painter.setBrush(QBrush(Qt::white));
			painter.drawEllipse(1, 1, 7, 7);
		}
	};

    class deleteMap
    {
    public:
        template <class T> void operator()(T m)
        {
            delete m.second;
        }
    };

    
    class AntGuiQtImpl : public IAntGui
	{
//        bool repaint;
		AntWidget *widget;
        QMutex paintLock;
		map<Point, const Food*> foodMap;
		map<Point, const Ant*> antMap;
	public:
		map<int, QColor> teamColor;

		void SetTeamScore(int teamId, int score) {}

    AntGuiQtImpl()
    {	
		}

		virtual void Clear()
		{
            for_each(antMap.begin(), antMap.end(), deleteMap());
            for_each(foodMap.begin(), foodMap.end(), deleteMap());
			antMap.clear();
			foodMap.clear();
		}

		void init(AntWidget* widget, int size)
		{
			this->widget = widget;
			teamColor.insert(pair<int, QColor>(0, QColor(Qt::red)));
			teamColor.insert(pair<int, QColor>(1, QColor(Qt::green)));
			teamColor.insert(pair<int, QColor>(2, QColor(Qt::blue)));
			teamColor.insert(pair<int, QColor>(3, QColor(Qt::yellow)));
		}

		virtual void SetAnt(const Ant *ant);

		virtual void SetFood(const Food *food)
		{
            ConcreteFood *new_food = new ConcreteFood(food);
			foodMap.insert(pair<Point, const Food*>(new_food->getPoint(), new_food));
		}

		virtual void Paint();

        virtual void BeginPaint()
        {
            paintLock.lock();
           	Clear();
        }

        virtual void EndPaint()
        {
            paintLock.unlock();
        }
	};

	class TestAnt : public Ant
	{
	private:
		bool food;
		bool frozen;
		Point point;
		int teamId;
	public:
		TestAnt(bool food, bool frozen, Point &p, int teamId)
            : food(food), frozen(frozen), point(p), teamId(teamId) {}
        TestAnt(const Ant *ant)
            : food(ant->hasFood()), frozen(ant->isFrozen()), point(ant->getPoint()), teamId(ant->getTeamId()) {}
		bool hasFood() const { return food;}
		bool isFrozen() const { return  frozen;}
		Point getPoint() const {return point;}
		int getTeamId() const { return teamId;}
	};
}

#endif
