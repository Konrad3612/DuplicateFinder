#include "MainGUI.h"
using namespace System;
using namespace System::Windows::Forms;

[STAThread]
//#include "Analyzer.h"
//#include <iostream>
int main(array<System::String^>^ args)
{
	DuplicateFinderv2::MainGUI form;
	Application::Run(%form);
	return 0;
}
