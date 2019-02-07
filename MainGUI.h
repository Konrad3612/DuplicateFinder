#pragma once
#include "HashDatabase.h"
#include "ViewResultForm.h"
namespace DuplicateFinderv2 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::IO;
	using namespace System::Drawing;
	using namespace System::Runtime::InteropServices;

	/// <summary>
	/// Summary for MainGUI
	/// </summary>
	public ref class MainGUI : public System::Windows::Forms::Form
	{
	public:
		MainGUI(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainGUI()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TextBox^  FolderTextBox1;
	protected:
	private: System::Windows::Forms::TextBox^  FolderTextBox2;
	private: System::Windows::Forms::Button^  SearchButton;
	private: System::Windows::Forms::ProgressBar^  progressBar1;
	private: System::Windows::Forms::Label^  TaskLabel;
	private: System::ComponentModel::BackgroundWorker^  Searcher;
	private: System::Windows::Forms::Label^  FileLabel;
	private: System::Windows::Forms::Button^  showListButton;


	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->FolderTextBox1 = (gcnew System::Windows::Forms::TextBox());
			this->FolderTextBox2 = (gcnew System::Windows::Forms::TextBox());
			this->SearchButton = (gcnew System::Windows::Forms::Button());
			this->progressBar1 = (gcnew System::Windows::Forms::ProgressBar());
			this->TaskLabel = (gcnew System::Windows::Forms::Label());
			this->Searcher = (gcnew System::ComponentModel::BackgroundWorker());
			this->FileLabel = (gcnew System::Windows::Forms::Label());
			this->showListButton = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// FolderTextBox1
			// 
			this->FolderTextBox1->Location = System::Drawing::Point(12, 12);
			this->FolderTextBox1->Name = L"FolderTextBox1";
			this->FolderTextBox1->Size = System::Drawing::Size(368, 20);
			this->FolderTextBox1->TabIndex = 0;
			// 
			// FolderTextBox2
			// 
			this->FolderTextBox2->Location = System::Drawing::Point(12, 38);
			this->FolderTextBox2->Name = L"FolderTextBox2";
			this->FolderTextBox2->Size = System::Drawing::Size(368, 20);
			this->FolderTextBox2->TabIndex = 1;
			// 
			// SearchButton
			// 
			this->SearchButton->Location = System::Drawing::Point(406, 10);
			this->SearchButton->Name = L"SearchButton";
			this->SearchButton->Size = System::Drawing::Size(75, 23);
			this->SearchButton->TabIndex = 2;
			this->SearchButton->Text = L"Search";
			this->SearchButton->UseVisualStyleBackColor = true;
			this->SearchButton->Click += gcnew System::EventHandler(this, &MainGUI::SearchButton_Click);
			// 
			// progressBar1
			// 
			this->progressBar1->Location = System::Drawing::Point(12, 94);
			this->progressBar1->Name = L"progressBar1";
			this->progressBar1->Size = System::Drawing::Size(479, 23);
			this->progressBar1->Step = 5;
			this->progressBar1->TabIndex = 3;
			// 
			// TaskLabel
			// 
			this->TaskLabel->AutoSize = true;
			this->TaskLabel->Location = System::Drawing::Point(12, 69);
			this->TaskLabel->Name = L"TaskLabel";
			this->TaskLabel->Size = System::Drawing::Size(0, 13);
			this->TaskLabel->TabIndex = 4;
			// 
			// Searcher
			// 
			this->Searcher->WorkerReportsProgress = true;
			this->Searcher->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &MainGUI::Searcher_DoWork);
			this->Searcher->ProgressChanged += gcnew System::ComponentModel::ProgressChangedEventHandler(this, &MainGUI::Searcher_ProgressChanged);
			this->Searcher->RunWorkerCompleted += gcnew System::ComponentModel::RunWorkerCompletedEventHandler(this, &MainGUI::Searcher_RunWorkerCompleted);
			// 
			// FileLabel
			// 
			this->FileLabel->AutoSize = true;
			this->FileLabel->Location = System::Drawing::Point(231, 69);
			this->FileLabel->Name = L"FileLabel";
			this->FileLabel->Size = System::Drawing::Size(0, 13);
			this->FileLabel->TabIndex = 5;
			// 
			// showListButton
			// 
			this->showListButton->Location = System::Drawing::Point(406, 35);
			this->showListButton->Name = L"showListButton";
			this->showListButton->Size = System::Drawing::Size(75, 23);
			this->showListButton->TabIndex = 6;
			this->showListButton->Text = L"Show List";
			this->showListButton->UseVisualStyleBackColor = true;
			this->showListButton->Click += gcnew System::EventHandler(this, &MainGUI::showListButton_Click);
			// 
			// MainGUI
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(503, 141);
			this->Controls->Add(this->showListButton);
			this->Controls->Add(this->FileLabel);
			this->Controls->Add(this->TaskLabel);
			this->Controls->Add(this->progressBar1);
			this->Controls->Add(this->SearchButton);
			this->Controls->Add(this->FolderTextBox2);
			this->Controls->Add(this->FolderTextBox1);
			this->Name = L"MainGUI";
			this->Text = L"MainGUI";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: HashCollection *hashes1 = nullptr;
	private: HashCollection *hashes2 = nullptr;
	private: HashDatabase *collection = nullptr;
	private: SimilarityList *similarities = nullptr;
	private: OpenCLDevice *device = nullptr;
	private: System::Void SearchButton_Click(System::Object^  sender, System::EventArgs^  e) 
	{
		if (device == nullptr)
		{
			device = new OpenCLDevice;
			device->InitializeDevice();
		}
		Collections::Generic::List<String^>^ arguments = gcnew Collections::Generic::List<String^>;
		arguments->Add(FolderTextBox1->Text);
		arguments->Add(FolderTextBox2->Text);
		Searcher->RunWorkerAsync(arguments);
	}
	private: delegate void SetTextDelegate(Control^ control,String^ text);
	private: System::Void Searcher_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) 
	{
		double threshold = 15;
		if (similarities != nullptr)
			delete similarities;
		if (collection == nullptr)
		{
			collection = new HashDatabase(L"DatabaseNames.txt",L"DatabaseHashes.txt");
		}
		Collections::Generic::List<String^>^ arguments = safe_cast<Collections::Generic::List<String^>^>(e->Argument);
		String^ FolderPath1 = arguments[0];
		String^ FolderPath2 = arguments[1];
		Invoke(gcnew SetTextDelegate(this, &MainGUI::SetText), gcnew array<Object^>{ TaskLabel,gcnew String("Creating image hashes") });
		if (String::IsNullOrEmpty(FolderPath2))
		{
			if (hashes1 != nullptr)
				delete hashes1;
			hashes1 = new HashCollection(device);
			hashes1->AddDatabase(collection);
			array<System::String^>^ FileNames = Directory::GetFiles(FolderPath1);
			int size = FileNames->Length;
			IEnumerator^ names = FileNames->GetEnumerator();
			int i = 0;
			while (names->MoveNext())
			{
				String^ FileName = safe_cast<String^>(names->Current);
				String^ Name = FileName->Substring(FileName->LastIndexOf(gcnew String("\\")) + 1);
				Invoke(gcnew SetTextDelegate(this, &MainGUI::SetText), gcnew array<Object^>{ FileLabel, Name->Substring(0, Math::Min(50, Name->Length - 1))});
				hashes1->Add(ConvertToWCHAR(FileName));
				Searcher->ReportProgress(i * 100 / size);
				i++;
			}
			Invoke(gcnew SetTextDelegate(this, &MainGUI::SetText), gcnew array<Object^>{ TaskLabel, gcnew String("Comparing hashes")});
			similarities = hashes1->DetectDuplicatesOneFolder(threshold);
		}
		else
		{
			if (hashes1 != nullptr)
				delete hashes1;
			if (hashes2 != nullptr)
				delete hashes2;
			hashes1 = new HashCollection(device);
			hashes2 = new HashCollection(device);
			hashes1->AddDatabase(collection);
			hashes2->AddDatabase(collection);
			array<System::String^>^ FileNames1 = Directory::GetFiles(FolderPath1);
			array<System::String^>^ FileNames2 = Directory::GetFiles(FolderPath2);
			int size = FileNames1->Length + FileNames2->Length;
			IEnumerator^ names1 = FileNames1->GetEnumerator();
			IEnumerator^ names2 = FileNames2->GetEnumerator();
			int i;
			while (names1->MoveNext())
			{
				String^ FileName = safe_cast<String^>(names1->Current);
				String^ Name = FileName->Substring(FileName->LastIndexOf(gcnew String("\\")) + 1);
				Invoke(gcnew SetTextDelegate(this, &MainGUI::SetText), gcnew array<Object^>{ FileLabel, Name->Substring(0, Math::Min(50, Name->Length - 1))});
				hashes1->Add(ConvertToWCHAR(FileName));
				Searcher->ReportProgress(i * 100 / size);
				i++;
			}
			while (names2->MoveNext())
			{
				String^ FileName = safe_cast<String^>(names2->Current);
				String^ Name = FileName->Substring(FileName->LastIndexOf(gcnew String("\\")) + 1);
				Invoke(gcnew SetTextDelegate(this, &MainGUI::SetText), gcnew array<Object^>{ FileLabel, Name->Substring(0, Math::Min(50, Name->Length - 1))});
				hashes2->Add(ConvertToWCHAR(FileName));
				Searcher->ReportProgress(i * 100 / size);
				i++;
			}
			Invoke(gcnew SetTextDelegate(this, &MainGUI::SetText), gcnew array<Object^>{ TaskLabel, gcnew String("Comparing hashes")});
			similarities = hashes1->DetectDuplicatesTwoFolders(threshold,hashes2);
			
		}
		e->Result = 0;
		Invoke(gcnew SetTextDelegate(this, &MainGUI::SetText), gcnew array<Object^>{ TaskLabel, gcnew String("Done") });
		collection->AddFromCollection(hashes1);
		collection->AddFromCollection(hashes2);
		collection->Save();
	}
	private: const wchar_t *ConvertToWCHAR(System::String^ string)
	{
		const wchar_t* chars = (const wchar_t*)(Marshal::StringToHGlobalUni(string)).ToPointer();
		return chars;
	}
	private: void SetText(Control^ control,String^ text)
	{
		control->Text = text;
	}
	private: System::Void Searcher_ProgressChanged(System::Object^  sender, System::ComponentModel::ProgressChangedEventArgs^  e) 
	{
		progressBar1->Value = e->ProgressPercentage;
	}
	private: System::Void Searcher_RunWorkerCompleted(System::Object^  sender, System::ComponentModel::RunWorkerCompletedEventArgs^  e)
	{
		similarities->Sort();
		ViewResultForm^ form = gcnew ViewResultForm(similarities);
		form->Show();
	}
	private: System::Void showListButton_Click(System::Object^  sender, System::EventArgs^  e) 
	{
		if (similarities != nullptr)
			similarities->Sort();
		ViewResultForm^ form = gcnew ViewResultForm(similarities);
		form->Show();
	}
};
}
