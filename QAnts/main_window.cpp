#include "main_window.hpp"
#include <QLabel>
#include <QPixmap>
#include <QWidget>
#include "AntWidget.hpp"
#include <QPainter>
#include <QGridLayout>
#include <QPushButton>
#include <QTimer>

main_window::main_window(QWidget *parent)
  : QMainWindow(parent), step(0), manager(40, 40, 2)
{
	ui.setupUi(this);
	QLabel *label1 = new QLabel("One");
	QLabel *label2 = new QLabel("Two");
	QLabel *label3 = new QLabel("Three");
	QLabel *label4 = new QLabel("Four");
	QPushButton *button = new QPushButton("step");
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(nextStep()));
	ant = new AntWidget();

	QGridLayout *layout = new QGridLayout;
	layout->setRowStretch(1, 100);
	layout->addWidget(label1, 0, 0, Qt::AlignLeft);
	layout->addWidget(label2, 0, 2, Qt::AlignRight);
	layout->addWidget(label3, 2, 0, Qt::AlignLeft);
	layout->addWidget(label4, 2, 2, Qt::AlignRight);
	layout->addWidget(ant, 1, 0, 1, 3, Qt::AlignCenter);
	layout->addWidget(button, 3, 0, 1, 3, Qt::AlignCenter);

	connect(button, SIGNAL(clicked()), this, SLOT(nextStep()));

	using namespace antgui;

	gui = dynamic_cast<AntGuiQtImpl *>(IAntGui::getGUI(antgui::Qt));
	if (gui)
	{
		gui->init(ant, 21);
	}
	ant->setGui(gui);
	manager.setGui(gui);

	food_iterator *it = new food_iterator(21);
	manager.setFoodGeneretor(it);

	this->centralWidget()->setLayout(layout);	
	timer->start(20);
}

void main_window::nextStep()
{
	ant->setUpdatesEnabled(false);
	manager.step(step++);
	ant->setUpdatesEnabled(true);
	ant->repaint();
}
