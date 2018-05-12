#include "Histogram.h"
#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/bitmap.h>

#include <iostream>
#include <vector>

Range::Range(unsigned int start, unsigned int end, wxString label) {
	_count = 0;
	_start = start;
	_end = end;
	_label = label;

	if (_label.compare(wxT("")) == 0) {
		if (_start != _end) {
			_label = wxString::Format(wxT("%d-%d"), _start, _end);
		}
		else {
			_label = wxString::Format(wxT("%d"), _start);
		}
	}
}

int Range::isInside(unsigned int x) const { return (_start <= x && x <= _end); }

void Range::inc() { _count++; }

unsigned int Range::getCount() const { return _count; }

wxString Range::getLabel() const { return _label; }

void Range::editLabel(wxString value) { _label = value; }

Histogram::Histogram(wxWindow *parent, wxPoint poz, wxSize size)
	: wxPanel(parent, wxID_ANY, poz, size) {

	_bufor.Create(size.x, size.y);

	draw();

	Connect(wxEVT_PAINT, wxPaintEventHandler(Histogram::onRefresh), NULL, this);

	Refresh();
}

void Histogram::onRefresh(wxPaintEvent &evt) {
	wxPaintDC dc(this);
	dc.DrawBitmap(_bufor, 0, 0);
}

void Histogram::render(wxDC *dc) {
	dc->SetBrush(wxBrush(wxColour(255, 255, 255)));
	dc->SetPen(wxPen(wxColour(0, 0, 0)));

	wxSize size = GetClientSize();

	dc->DrawRectangle(0, 0, size.x, size.y);

	dc->SetBrush(wxBrush(wxColour(0, 0, 255)));
	dc->SetPen(wxPen(wxColour(0, 0, 128)));

	unsigned int maxY = 0;
	int toDisplay = 0;
	for (unsigned int i = 0; i < _ranges.size(); i++) {
		if (maxY < _ranges[i].getCount()) {
			maxY = _ranges[i].getCount();
		}
		if (_ranges[i].getCount() > 0) {
			toDisplay++;
		}
	}

	unsigned int space = (toDisplay + 1) * 5;
	unsigned int width = 0;
	if (toDisplay != 0) {
		width = (size.x - space) / toDisplay;
	}

	int current_x = 5;
	for (unsigned int i = 0; i < _ranges.size(); i++)
		if (_ranges[i].getCount() > 0) {
			unsigned int h = ((size.y - 18) * _ranges[i].getCount()) / maxY;
			dc->DrawRectangle(current_x, size.y - 17 - h, width, h);
			dc->SetTextForeground(wxColour(255, 0, 0));
			dc->DrawText(wxString::Format(wxT("%u"), _ranges[i].getCount()), current_x + 2, size.y - 40);
			dc->SetTextForeground(wxColour(0, 0, 0));
			dc->DrawText(_ranges[i].getLabel(), current_x + 2, size.y - 15);
			current_x += width + 5;
		}
}

void Histogram::draw() {
	wxMemoryDC *dc = new wxMemoryDC(_bufor);
	render(dc);
	delete dc;
}

void Histogram::editLabel(unsigned int index, wxString value) {
	_ranges[index].editLabel(value);
}

void Histogram::sequence(unsigned int startValue,
	unsigned int endValue,
	unsigned int step,
	unsigned int dx) {
	while (startValue <= endValue) {
		unsigned int xx = startValue + dx - 1;
		if (xx > endValue) {
			xx = endValue;
		}
		_ranges.push_back(Range(startValue, xx));
		startValue += step;
	}
}

void Histogram::addValue(unsigned int v) {
	for (unsigned int i = 0; i < _ranges.size(); i++)
		if (_ranges[i].isInside(v))
			_ranges[i].inc();
}

