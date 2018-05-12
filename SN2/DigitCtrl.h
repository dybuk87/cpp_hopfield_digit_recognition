#ifndef DIGITCTRL__H
#define DIGITCTRL__H

#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/bitmap.h>
#include <wx/file.h>

#include <iostream>
#include <vector>
#include <string>

class LabelInfo {
public:
	LabelInfo(wxString label, unsigned long percent) : _label(label), _percent(percent) { }
	operator wxString() { return _label; }
	operator unsigned int() { return _percent; }
private:
	wxString _label;
	unsigned int _percent;
};

class LcdDigitCtrl : public wxPanel {
public:
	enum LcdType {
		Dot,
		Rect
	};

	LcdDigitCtrl(wxWindow *parent, wxPoint poz, wxSize size, std::vector<LabelInfo> name,
		int charResX, int charResY, int *data, LcdType typ = Dot, int prev = 1);

	void update(int charResX, int charResY, int *data);

	void draw(wxDC *dc);

	int* getData();

	std::vector<LabelInfo> getLabel();

	~LcdDigitCtrl();
private:
	void render(wxPaintEvent& evt);

	void onMouseEnter(wxMouseEvent &evt);

	int *_data;
	int _charResX;
	int _charResY;

	LcdType _lcdType;

	int _preview;

	std::vector<LabelInfo> _name;

	wxBitmap bufor;
};

class LcdDigitViewer : public wxPanel {
public:
	LcdDigitViewer(wxWindow *parent, wxPoint poz, wxSize size);

	void addDigitCtrl(LcdDigitCtrl *ctrl);

	~LcdDigitViewer();
private:
	void render(wxPaintEvent& evt);
	void onButtonClicked(wxCommandEvent &evt);

	enum ButtonId {
		left = 0,
		right
	};

	unsigned int _startId;
	unsigned int _marker;

	std::vector<LcdDigitCtrl*> _lcdDigitCtrl;

	wxComboBox *_name;
	LcdDigitCtrl *_view;
};

#endif