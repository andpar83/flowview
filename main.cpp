#include "stdafx.h"
#include <exception>
#include <QApplication>
#include "flow_view.h"

int main(int argc, char *argv[])
{
	try
	{
		QApplication a(argc, argv);
		fv::flow_view w;
		w.show();
		return a.exec();
	}
	catch (const std::exception& e)
	{
		Q_ASSERT(false && "Exception in main");
		return 1;
	}
}