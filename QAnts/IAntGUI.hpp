#ifndef _IANTGUI_H_

#define _IANTGUI_H_

#include <string>
#include <stack>

namespace antgui
{
	using namespace std;

	enum GuiType {Qt, NCurses};

	class GUIException
	{
		string message;
	public:
		GUIException(const string& message) : message(message) {}
		
		const string& getMessage() 
		{
			return message;
		}
	};

	struct Point
	{
		int x, y;
		bool operator< (const Point& p) const
		{
			if (x < p.x) return true;
			if (x > p.x) return false;
			if (y < p.y) return true;
			return false;
		}
		Point(int x, int y) : x(x), y(y) {}
		Point() {}
	};

	class Ant
	{
	public:
		virtual bool hasFood() const = 0;
		virtual bool isFrozen() const = 0;
		virtual Point getPoint() const = 0;
		virtual int getTeamId() const = 0;
		bool operator< (const Ant& other) const
		{
			if (isFrozen()) return false;
			if (other.isFrozen()) return true;
			if (hasFood()) return false;
			if (other.hasFood()) return true;
			return false;
		}
		virtual ~Ant() {}
	};

	class Food
	{
	public:
		virtual Point getPoint() const = 0;
		virtual int getCount() const = 0;
		virtual ~Food() {}
	};

	class ConcreteFood : public Food
	{
		Point p;
		int count;
	public:
		ConcreteFood() {}
		ConcreteFood(const Point &p, int count) : p(p), count(count) {}
		ConcreteFood(const Food *food) : p(food->getPoint()), count(food->getCount()) {}

		virtual Point getPoint() const 
		{
			return p;
		}

		virtual int getCount() const
		{
			return count;
		}
	};

	class IAntGui
	{
	public:
		virtual void Clear() = 0;
		virtual void SetAnt(const Ant *ant) = 0;
		virtual void BeginPaint() = 0;
		virtual void EndPaint() = 0;
		virtual void SetFood(const Food *food) = 0;
		virtual void SetTeamScore(int teamId, int score) = 0;
		virtual void Paint() = 0;
		static IAntGui* getGUI(antgui::GuiType type);
    virtual ~IAntGui() {};
	};

	const int dx[] = {-1, 1,  0, 0, 0};
	const int dy[] = {0,  0, -1, 1, 0};
	const int df[] = {-5,  -5, -5, -5, 0};

	class food_iterator: public std::iterator< std::input_iterator_tag, ConcreteFood> 
	{
		enum {fractal_den = 3};
		struct FuncParam
		{
			int n;
			Point p;
			int count;
			int fsize;
			FuncParam(int n, const Point &p, int count, int fsize) : n(n), p(p), count(count), fsize(fsize) {}
		};
		stack<FuncParam> s;
		int size;
		int iteration;
	public:

		food_iterator() : iteration(-1) {}
		food_iterator(const food_iterator& it);
		food_iterator(int size);

		ConcreteFood operator*();
		food_iterator& operator++();
		food_iterator operator++(int);

	private:
		void WakeUp();
		ConcreteFood GetFood()
		{
			return ConcreteFood(s.top().p, s.top().count);
		}

		friend bool operator!=(const food_iterator& it1, const food_iterator& it2);
	};
}
#endif
