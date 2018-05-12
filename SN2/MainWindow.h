#ifndef MAIN_WINDOW__H
#define MAIN_WINDOW__H

#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/bitmap.h>
#include <wx/file.h>

class MainWindow : public wxFrame {
public:
	MainWindow(const wxString& title);

	void render(wxPaintEvent& evt);

	~MainWindow();

private:
	wxPanel *panel;

	enum ButtonId {
		StartId = 0,
		StopId,
		ButtonNo
	};

	wxButton *button[ButtonNo];
};

#endif