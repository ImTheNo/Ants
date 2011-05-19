#if !defined MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include "ui_MainForm.h"
#include <QDialog>
#include "AntManager.hpp"
#include "AntGuiImpl.hpp"

class main_window : public QMainWindow
{
  Q_OBJECT
public:
	AntManager manager;

	QTimer *timer;

	int heightForWidth ( int w ) const
	{
		return w;
	}
	main_window(QWidget * parent = 0);
public slots:
	void nextStep(void);
private:
	int step;
	Ui::MainWindow ui;
	antgui::AntGuiQtImpl *gui;
	AntWidget *ant;
};


#endif //MAIN_WINDOW_HPP
