#include "DigitCtrl.h"

unsigned int min(unsigned int a, unsigned int b) { return (a < b ? a : b); }
unsigned int max(unsigned int a, unsigned int b) { return (a > b ? a : b); }

LcdDigitCtrl::LcdDigitCtrl(wxWindow *parent, wxPoint poz, wxSize size, std::vector<LabelInfo> name,
	int charResX, int charResY, int *data, LcdType typ, int prev)
	: wxPanel(parent, wxID_ANY, poz, size) {
	_charResX = charResX;
	_charResY = charResY;

	_lcdType = typ;

	_name = name;

	_preview = prev;

	_data = new int[_charResX * _charResY];
	memcpy(_data, data, _charResX * charResY * sizeof(int));

	bufor.Create(size.x, size.y);
	wxMemoryDC *mem = new wxMemoryDC(bufor);
	draw(mem);
	delete mem;

	Connect(wxEVT_PAINT, wxPaintEventHandler(LcdDigitCtrl::render),
		NULL, this);

	Connect(wxEVT_ENTER_WINDOW, (wxObjectEventFunction)
		(wxEventFunction)(wxMouseEventFunction)&LcdDigitCtrl::onMouseEnter, NULL, this);

}

std::vector<LabelInfo> LcdDigitCtrl::getLabel() { return _name; }

void LcdDigitCtrl::onMouseEnter(wxMouseEvent &evt) {
	evt.Skip();
}

void LcdDigitCtrl::draw(wxDC *dc) {
	wxSize size = dc->GetSize();

	float cellX = size.x / _charResX;
	float cellY = size.y / _charResY;

	unsigned int offset = 0;
	for (int i = 0; i < _charResY; i++)
		for (int j = 0; j < _charResX; j++, offset++) {

			if (_lcdType == Dot) {
				dc->SetPen(wxPen(wxColour(0, 0, 0)));
				dc->SetBrush(wxBrush(wxColour(0, 0, 0)));
				dc->DrawRectangle(j*cellX, i*cellY,
					cellX, cellY);
				if (_data[offset])
					dc->SetBrush(wxBrush(wxColour(200, 200, 0)));
				else
					dc->SetBrush(wxBrush(wxColour(128, 128, 128)));
				dc->DrawEllipse(j * cellX + 1, i*cellY + 1, cellX - 1, cellY - 1);
			}

			if (_lcdType == Rect) {
				if (_data[offset]) {
					dc->SetPen(wxPen(wxColour(200, 200, 0)));
					dc->SetBrush(wxBrush(wxColour(200, 200, 0)));
				}
				else {
					dc->SetBrush(wxBrush(wxColour(0, 0, 0)));
					dc->SetPen(wxPen(wxColour(0, 0, 0)));
				}
				dc->DrawRectangle(j*cellX, i*cellY,
					cellX + 1, cellY + 1);
			}
		}
}

void LcdDigitCtrl::update(int charResX, int charResY, int *data) {
	_charResX = charResX;
	_charResY = charResY;

	if (_data) {
		delete _data;
	}

	_data = new int[_charResX * _charResY];
	memcpy(_data, data, _charResX * charResY * sizeof(int));

	bufor.Create(GetSize().x, GetSize().y);
	wxMemoryDC *mem = new wxMemoryDC(bufor);
	draw(mem);
	delete mem;

	Refresh(false);
}

void LcdDigitCtrl::render(wxPaintEvent& evt) {
	wxPaintDC dc(this);

	dc.DrawBitmap(bufor, 0, 0);

	evt.Skip();
}

int *LcdDigitCtrl::getData() { return _data; }

LcdDigitCtrl::~LcdDigitCtrl() {
	if (_data) {
		delete _data;
	}
	_data = 0;
}


LcdDigitViewer::LcdDigitViewer(wxWindow *parent, wxPoint poz, wxSize size)
	: wxPanel(parent, wxID_ANY, poz, size) {

	Connect(wxEVT_PAINT, wxPaintEventHandler(LcdDigitViewer::render),
		NULL, this);

	(new wxButton(this, left, wxT("<"),
		wxPoint(1, 1), wxSize(28, size.y - 50)))->Connect(
			wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LcdDigitViewer::onButtonClicked), NULL, this);
	(new wxButton(this, right, wxT(">"), wxPoint(size.x - 30, 1), wxSize(28, size.y - 50)))->Connect(
		wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LcdDigitViewer::onButtonClicked), NULL, this);

	int n = 1;
	const wxString tmp[1] = { wxT("noname") };
	_name = new wxComboBox(this, wxID_ANY, wxT(""), wxPoint(size.x / 2 - 48, 2), wxSize(96, 30), n, tmp);

	int mem[192];
	memset(mem, 0, sizeof(int) * 192);

	std::vector<LabelInfo> li;
	li.push_back(LabelInfo(wxT("noname"), 100));

	_view = new LcdDigitCtrl(this, wxPoint(30, 21), wxSize(size.x - 60, size.y - 70), li, 12, 16, mem);
	//	_name = (wxString)(_view->getLabel()[0]);

	_startId = 0;

	_marker = 0;
}

void LcdDigitViewer::render(wxPaintEvent& evt) {
	wxPaintDC dc(this);

	dc.SetBrush(wxBrush(wxColour(255, 255, 255)));
	dc.SetPen(wxPen(wxColour(0, 0, 0)));

	dc.DrawRectangle(0, 0, GetClientSize().x, GetClientSize().y);

	//	dc.SetTextForeground(wxColour(0,0,0));
	//	dc.DrawText(_name, (GetClientSize().x - dc.GetTextExtent(_name).x)/2, 2);						

	wxBitmap tmp;

	if (_lcdDigitCtrl.size() > 0) {
		tmp.Create(_lcdDigitCtrl[0]->GetSize().x,
			_lcdDigitCtrl[0]->GetSize().y);
		for (unsigned int i = _startId; i < min(_lcdDigitCtrl.size(), _startId + 6); i++) {
			wxMemoryDC *mdc = new wxMemoryDC(tmp);
			_lcdDigitCtrl[i]->draw(mdc);
			delete mdc;

			if (i - _startId == _marker) {
				dc.SetPen(wxPen(wxColour(255, 0, 0)));
				dc.DrawRectangle(10 + (i - _startId) * (_lcdDigitCtrl[0]->GetSize().x + 10) - 2,
					GetClientSize().y - 40 - 2,
					_lcdDigitCtrl[0]->GetSize().x + 4,
					_lcdDigitCtrl[0]->GetSize().y + 4);
			}

			dc.DrawBitmap(tmp, 10 + (i - _startId) * (_lcdDigitCtrl[0]->GetSize().x + 10),
				GetClientSize().y - 40);
		}

	}

	evt.Skip();
}

void LcdDigitViewer::addDigitCtrl(LcdDigitCtrl *ctrl) {
	_lcdDigitCtrl.push_back(ctrl);
	ctrl->Show(false);
	if (_lcdDigitCtrl.size() == 1) {
		_view->update(12, 16, _lcdDigitCtrl[_startId + _marker]->getData());
		delete _name;
		std::vector<LabelInfo> li = _lcdDigitCtrl[_startId + _marker]->getLabel();
		unsigned int len = li.size();
		wxString *tmp = new wxString[len];
		for (unsigned int i = 0; i < len; i++) {
			tmp[i] = (wxString)li[i];
		}
		wxSize size = GetClientSize();
		_name = new wxComboBox(this, wxID_ANY, wxT(""), wxPoint(size.x / 2 - 48, 2), wxSize(96, 30), len, tmp);
		_name->SetSelection(0);
		delete[]tmp;
	}
}

void LcdDigitViewer::onButtonClicked(wxCommandEvent &evt) {

	switch (evt.GetId()) {
	case right:	if (_marker < min(5, _lcdDigitCtrl.size() - 1)) _marker++;
				else {
					if (_lcdDigitCtrl.size() > 6) {
						_startId++;
						if (_startId > _lcdDigitCtrl.size() - 6) {
							_startId = max(0, _lcdDigitCtrl.size() - 6);
						}
					}
				}
				break;
	case left:    if (_marker > 0) _marker--;
				  else
					  if (_startId > 0)
						  _startId--;
	}

	if (_startId + _marker < _lcdDigitCtrl.size()) {
		_view->update(12, 16, _lcdDigitCtrl[_startId + _marker]->getData());
		delete _name;
		//		_name = (wxString)(_lcdDigitCtrl[_startId + _marker]->getLabel()[0]);
		std::vector<LabelInfo> li = _lcdDigitCtrl[_startId + _marker]->getLabel();
		unsigned int len = li.size();
		wxString *tmp = new wxString[len];
		for (unsigned int i = 0; i < len; i++)
			tmp[i] = (wxString)li[i];
		wxSize size = GetClientSize();
		_name = new wxComboBox(this, wxID_ANY, wxT(""), wxPoint(size.x / 2 - 48, 2), wxSize(96, 30), len, tmp);
		_name->SetSelection(0);
		delete[]tmp;
	}

	Refresh(false);
	evt.Skip();
}

LcdDigitViewer::~LcdDigitViewer() {
	for (unsigned int i = 0; i < _lcdDigitCtrl.size(); i++)
		delete _lcdDigitCtrl[i];
	_lcdDigitCtrl.clear();
}
