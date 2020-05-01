#include "cMain.h"

wxBEGIN_EVENT_TABLE(cMain, wxFrame)

wxEND_EVENT_TABLE()

cMain::cMain() : wxFrame(nullptr, wxID_ANY, "Prato Fiorito",wxPoint(30,30), wxSize(800, 600))
{
	btn = new wxButton * [nFieldWidth * nFieldHeight];
	wxGridSizer* grid = new wxGridSizer(nFieldWidth, nFieldHeight, 0, 0);

	nField = new int[nFieldWidth * nFieldHeight];

	wxFont font(24, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);

	for (int i = 0; i < nFieldWidth; i++) {
		for (int j = 0; j < nFieldHeight; j++) {
			btn[j * nFieldWidth + i] = new wxButton(this, 10000 + (j * nFieldWidth + i));
			btn[j * nFieldWidth + i]->SetFont(font);
			grid->Add(btn[j * nFieldWidth + i], 1, wxEXPAND | wxALL);

			btn[j * nFieldWidth + i]->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cMain::OnButtonClicked, this);
			nField[j * nFieldWidth + i] = 0;
		}
	}

	this->SetSizer(grid);
	grid -> Layout();
}

cMain::~cMain()
{
	delete[]btn;
}

void cMain::OnButtonClicked(wxCommandEvent& evt)
{
	//prende le coordinate dei bottoni nellarray
	int x = (evt.GetId() - 10000) % nFieldWidth;
	int y = (evt.GetId() - 10000) / nFieldWidth;

	if (bFirstClick) {
		int mines = 30;

		while (mines) {
			int rx = rand() % nFieldWidth;
			int ry = rand() % nFieldHeight;

			if (nField[ry * nFieldWidth + rx] == 0 && rx != y) {
				nField[ry * nFieldWidth + rx] = -1;
				mines--;
			}

		}
		bFirstClick = false;
	}

	//disabilitare il bottone prima che vanga premuto nuovamente
	btn[y * nFieldWidth + x]->Enable(false);

	//controllo mina
	if (nField[y * nFieldWidth + x] == -1) {
		wxMessageBox("BOOOOOOM!!! - Game Over:(");

		//reset game
		bFirstClick = true;
		for (int x=0;x< nFieldWidth;x++)
			for (int y = 0; y < nFieldHeight; y++) {
				nField[y * nFieldWidth + x] = 0;
				btn[y * nFieldWidth + x]->SetLabel("");
				btn[y * nFieldWidth + x]->Enable(true);
			}
	}
	else {
		//conta le mine coperte
		int mine_count = 0;
		for (int i = -1; i < 2; i++) {
			for (int j = -1; j < 2; j++) {
				if (x + i >= 0 && x + 1 < nFieldWidth && y + j >= 0 && y + j < nFieldHeight) {
					if (nField[(y + j) * nFieldWidth + (x + i)] == -1)
						mine_count++;
				}
			}
		}
		//scrivere sul bottone se il conto is >0
		if (mine_count > 0) {
			btn[y * nFieldWidth + x]->SetLabel(std::to_string(mine_count));
		}
	}


	evt.Skip();
}
