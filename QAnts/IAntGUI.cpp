#include "IAntGUI.hpp"
#include "AntWidget.hpp"
#include "AntGuiImpl.hpp"
#include <map>
#include <algorithm>
#include <QPixmap>
#include <QPainter> 

namespace antgui
{
	using namespace std;

	IAntGui* IAntGui::getGUI(antgui::GuiType type)
	{
		switch (type) 
		{
        case (Qt): 
			return new AntGuiQtImpl();
			break;
		case (NCurses):
			return 0;
			break;
		default:
			return 0;
			break;
		}
	}

	bool operator!=(const food_iterator& it1, const food_iterator& it2)
	{
		return it1.iteration != it2.iteration;
	}

    void food_iterator::WakeUp()
    {
		while (!s.empty() && s.top().n > 1)
		{
			FuncParam param = s.top();
			if (param.n > 1)
			{
				s.pop();
				for (int i = 0; i < 5; i++)
				{
					Point p(param.p.x - dx[i] * param.fsize / fractal_den, param.p.y + dy[i] * param.fsize / fractal_den);
					FuncParam new_param(param.n - 1, p, param.count + df[i], param.fsize / fractal_den);
					if (new_param.n > 1 || (p.x >= 0 && p.y >= 0 && p.x < size && p.y < size))
					{
						s.push(new_param);
					}
				}
			}
		}
	}

    food_iterator::food_iterator(int size) : size(size)
	{
		int t = 1;
		int n = 1;
		while (t < size) 
		{
			t *= fractal_den;
			n++;
		}
		s.push(FuncParam(n, Point(size / 2, size / 2), 100, t));
	}

    ConcreteFood food_iterator::operator*()
	{
		WakeUp();
		return ConcreteFood(s.top().p, s.top().count);
	}

    food_iterator& food_iterator::operator++()
	{
		this->operator *();
		s.pop();
		WakeUp();
		if (s.empty())
		{
			iteration = -1;
		}
		return *this;
	}

    food_iterator food_iterator::operator++(int)
	{			
		this->operator *();
		food_iterator tmp = *this;
		++*this;
		return tmp;
	}

    food_iterator::food_iterator(const food_iterator& it) : size(it.size), iteration(it.iteration)
	{
		if (!it.s.empty())
		{
			s.push(it.s.top());
		}
	}


}
