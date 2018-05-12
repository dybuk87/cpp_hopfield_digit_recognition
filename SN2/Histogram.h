#ifndef HISTOGRAM__H
#define HISTOGRAM__H

#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/bitmap.h>

#include <iostream>
#include <vector>

class Range {
public:
	Range(unsigned int start, unsigned int end, wxString label = wxT(""));

	int isInside(unsigned int x) const;

	void inc();

	unsigned int getCount() const;
	wxString getLabel() const;

	void editLabel(wxString value);
private:
	unsigned int _count;
	unsigned int _start;
	unsigned int _end;
	wxString _label;

};

class Histogram : public wxPanel {
public:
	Histogram(wxWindow *parent, wxPoint poz, wxSize size);

	void onRefresh(wxPaintEvent& evt);

	void sequence(unsigned int startValue,
		unsigned int endValue,
		unsigned int step,
		unsigned int dx);

	void addValue(unsigned int v);

	void draw();

	void editLabel(unsigned int index, wxString value);

private:

	void render(wxDC *dc);

	wxBitmap _bufor;

	std::vector<Range> _ranges;
};

#endif
