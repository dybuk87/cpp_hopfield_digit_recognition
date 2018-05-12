#include "main.h"
#include "MainWindow.h"
#include "Matrix.h"
//#include "VLD.h"

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	wxImageHandler *pngHandler = new wxPNGHandler();
	wxImage::AddHandler(pngHandler);

	int data[] = { 0, 1, 2, 7,
					1, 2, 3, 4,
					5, 6, 7, 8,
					-1, 1, -1, 1 };
	Matrix<int> matrix(4, 4, data);
	int det = matrix.det();

	((Matrix<double>)matrix).toFile("matrix.txt");

	Matrix<double> inverted = ((Matrix<double>)matrix).invert();
	inverted.toFile("matrix.txt");

	(((Matrix<double>)matrix) * inverted).toFile("matrix.txt");


	MainWindow *mainWindow = new MainWindow(wxT("MainWindow"));
	mainWindow->Show(true);

	return true;
}

