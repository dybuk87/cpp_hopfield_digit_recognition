#include "MainWindow.h"
#include "DigitCtrl.h"
#include "InputHandler.h"
#include <cmath>
#include "Matrix.h"
#include "Siec.h"
#include "Histogram.h"


MainWindow::MainWindow(const wxString& title)
	: wxFrame(NULL, wxID_ANY, title,
		wxDefaultPosition, wxSize(620, 520))
{
	panel = new wxPanel(this, wxID_ANY);
	panel->Connect(wxEVT_PAINT,
		wxPaintEventHandler(MainWindow::render), NULL, this);

	InputHandler tdata("dane_uczace.txt");
	LcdDigitViewer *lcdViewer = new LcdDigitViewer(panel, wxPoint(10, 150), wxSize(160, 200));
	for (unsigned int y = 0; y < 2; y++)
		for (unsigned int x = 0; x < 5; x++) {
			int *mem = tdata.getDigit(y * 5 + x).get01Data();
			std::vector<LabelInfo> li;
			li.push_back(LabelInfo(wxString::FromAscii(tdata.getDigit(y * 5 + x).getLabel().c_str()), 100));
			lcdViewer->addDigitCtrl(new LcdDigitCtrl(lcdViewer, wxPoint(10 + 14 * x, 100 + y * 18),
				wxSize(12 * 1, 16 * 1),
				li,
				12, 16, mem, LcdDigitCtrl::Rect));

			delete mem;
		}

	Siec *siec = new Siec(192, 10);

	for (unsigned int i = 0; i < 10; i++)
		siec->setSample(tdata.getDigit(i).getData(), i);

	siec->teach();

	Histogram *histogram = new Histogram(panel, wxPoint(350, 10), wxSize(250, 100));
	histogram->sequence(0, 3, 1, 1);
	histogram->editLabel(0, wxT("B³¹d100"));
	histogram->editLabel(1, wxT("B³¹d"));
	histogram->editLabel(2, wxT("Ok100"));
	histogram->editLabel(3, wxT("Ok"));

	Histogram *miejsce = new Histogram(panel, wxPoint(350, 120), wxSize(250, 100));
	miejsce->sequence(1, 10, 1, 1);

	InputHandler tdata2("dane_walidacyjne.txt");
	LcdDigitViewer *lcdViewer2 = new LcdDigitViewer(panel, wxPoint(180, 10), wxSize(160, 200));
	for (unsigned int i = 0; i < 50; i++) {
		std::vector<LabelInfo> li;
		std::vector<Result> result = siec->test(tdata2.getDigit(i).getData());
		if (atoi(tdata2.getDigit(i).getLabel().c_str()) == result[0].getLabelId()) {
			if (result[0].getPercent() == 100)
				histogram->addValue(2);
			else
				histogram->addValue(3);
		}
		else {
			if (result[0].getPercent() == 100)
				histogram->addValue(0);
			else
				histogram->addValue(1);
		}

		for (unsigned int p = 0; p < result.size(); p++) {
			wxString caption = wxString::FromAscii(tdata2.getDigit(i).getLabel().c_str());
			caption += wxString::Format(wxT("-%d(%d%%)"), result[p].getLabelId(), result[p].getPercent());
			li.push_back(LabelInfo(caption, result[p].getPercent()));

			if (result[p].getLabelId() == atoi(tdata2.getDigit(i).getLabel().c_str()))
				miejsce->addValue(p + 1);

		}
		int *mem = tdata2.getDigit(i).get01Data();
		lcdViewer2->addDigitCtrl(new LcdDigitCtrl(lcdViewer2, wxPoint(0, 0),
			wxSize(12 * 1, 16 * 1),
			li,
			12, 16, mem, LcdDigitCtrl::Rect));
		delete mem;
	}
	miejsce->draw();
	miejsce->Refresh();

	histogram->draw();
	histogram->Refresh();
	//	int r = siec->test(tdata3.getDigit(2).getData());

	Histogram *accur = new Histogram(panel, wxPoint(350, 280), wxSize(250, 100));
	accur->sequence(0, 100, 2, 2);
	InputHandler tdata3("dane_testowe.txt");
	LcdDigitViewer *lcdViewer3 = new LcdDigitViewer(panel, wxPoint(180, 280), wxSize(160, 200));
	for (unsigned int i = 0; i < 100; i++) {
		std::vector<LabelInfo> li;
		std::vector<Result> result = siec->test(tdata3.getDigit(i).getData());
		accur->addValue(result[0].getPercent());
		for (unsigned int p = 0; p < result.size(); p++) {
			wxString caption = wxString::Format(wxT("w = %d(%d%%)"), result[p].getLabelId(), result[p].getPercent());
			li.push_back(LabelInfo(caption, result[p].getPercent()));
		}
		int *mem = tdata3.getDigit(i).get01Data();
		lcdViewer3->addDigitCtrl(new LcdDigitCtrl(lcdViewer3, wxPoint(0, 0),
			wxSize(12 * 1, 16 * 1),
			li,
			12, 16, mem, LcdDigitCtrl::Rect));

		delete mem;
	}
	accur->draw();
	accur->Refresh();

	delete siec;

	DoLayout();
	SetDoubleBuffered(true);

	Centre();

}


void MainWindow::render(wxPaintEvent &evt) {
	wxPaintDC dc(panel);

	evt.Skip();
}

MainWindow::~MainWindow() {

}
