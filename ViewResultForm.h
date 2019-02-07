#pragma once
#include "SimilarityList.h"
namespace DuplicateFinderv2 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;
	using namespace System::IO;

	/// <summary>
	/// Summary for ViewResultForm
	/// </summary>
	public ref class ViewResultForm : public System::Windows::Forms::Form
	{
	public:
		ViewResultForm(SimilarityList *listin)
		{
			list = listin;
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~ViewResultForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::ListView^  listView;
	protected:

	protected:
	private: System::Windows::Forms::PictureBox^  pictureBox1;
	private: System::Windows::Forms::PictureBox^  pictureBox2;
	private: System::Windows::Forms::ImageList^  imageList;
	private: System::ComponentModel::BackgroundWorker^  thumbnailLoader;
	private: System::Windows::Forms::Label^  differenceLabel;
	private: System::Windows::Forms::Label^  fileLabel1;
	private: System::Windows::Forms::Label^  fileLabel2;

	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->listView = (gcnew System::Windows::Forms::ListView());
			this->imageList = (gcnew System::Windows::Forms::ImageList(this->components));
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox2 = (gcnew System::Windows::Forms::PictureBox());
			this->thumbnailLoader = (gcnew System::ComponentModel::BackgroundWorker());
			this->differenceLabel = (gcnew System::Windows::Forms::Label());
			this->fileLabel1 = (gcnew System::Windows::Forms::Label());
			this->fileLabel2 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->BeginInit();
			this->SuspendLayout();
			// 
			// listView
			// 
			this->listView->LargeImageList = this->imageList;
			this->listView->Location = System::Drawing::Point(12, 12);
			this->listView->Name = L"listView";
			this->listView->Size = System::Drawing::Size(319, 554);
			this->listView->TabIndex = 0;
			this->listView->UseCompatibleStateImageBehavior = false;
			this->listView->ItemSelectionChanged += gcnew System::Windows::Forms::ListViewItemSelectionChangedEventHandler(this, &ViewResultForm::listView_ItemSelectionChanged);
			this->listView->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &ViewResultForm::listView_KeyUp);
			// 
			// imageList
			// 
			this->imageList->ColorDepth = System::Windows::Forms::ColorDepth::Depth32Bit;
			this->imageList->ImageSize = System::Drawing::Size(64, 64);
			this->imageList->TransparentColor = System::Drawing::Color::Transparent;
			// 
			// pictureBox1
			// 
			this->pictureBox1->Location = System::Drawing::Point(337, 12);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(459, 554);
			this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pictureBox1->TabIndex = 1;
			this->pictureBox1->TabStop = false;
			this->pictureBox1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &ViewResultForm::pictureBox1_Paint);
			// 
			// pictureBox2
			// 
			this->pictureBox2->Location = System::Drawing::Point(802, 12);
			this->pictureBox2->Name = L"pictureBox2";
			this->pictureBox2->Size = System::Drawing::Size(450, 554);
			this->pictureBox2->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->pictureBox2->TabIndex = 2;
			this->pictureBox2->TabStop = false;
			this->pictureBox2->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &ViewResultForm::pictureBox2_Paint);
			// 
			// thumbnailLoader
			// 
			this->thumbnailLoader->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &ViewResultForm::thumbnailLoader_DoWork);
			// 
			// differenceLabel
			// 
			this->differenceLabel->AutoSize = true;
			this->differenceLabel->Location = System::Drawing::Point(9, 569);
			this->differenceLabel->Name = L"differenceLabel";
			this->differenceLabel->Size = System::Drawing::Size(59, 13);
			this->differenceLabel->TabIndex = 3;
			this->differenceLabel->Text = L"Difference:";
			// 
			// fileLabel1
			// 
			this->fileLabel1->AutoSize = true;
			this->fileLabel1->Location = System::Drawing::Point(334, 569);
			this->fileLabel1->Name = L"fileLabel1";
			this->fileLabel1->Size = System::Drawing::Size(0, 13);
			this->fileLabel1->TabIndex = 4;
			// 
			// fileLabel2
			// 
			this->fileLabel2->AutoSize = true;
			this->fileLabel2->Location = System::Drawing::Point(802, 569);
			this->fileLabel2->Name = L"fileLabel2";
			this->fileLabel2->Size = System::Drawing::Size(0, 13);
			this->fileLabel2->TabIndex = 5;
			// 
			// ViewResultForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1264, 591);
			this->Controls->Add(this->fileLabel2);
			this->Controls->Add(this->fileLabel1);
			this->Controls->Add(this->differenceLabel);
			this->Controls->Add(this->pictureBox2);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->listView);
			this->Name = L"ViewResultForm";
			this->Text = L"ViewResultForm";
			this->Load += gcnew System::EventHandler(this, &ViewResultForm::ViewResultForm_Load);
			this->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &ViewResultForm::ViewResultFom_Paint);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: SimilarityList *list = nullptr;
	private: int selectionIndex=-1;
	private: bool leftRect = false;
	private: bool rightRect = false;
	private: Brush^ selectionBrush = gcnew SolidBrush(Color::FromArgb(255, 72, 145, 225));
	private: Pen^ selectionPen = gcnew Pen(selectionBrush,8.0);
	private: Image ^ getThumbnailImage(int width, String^ path)
	{
		Image^ img = Image::FromFile(path);
		Image^ thumb = gcnew Bitmap(width, width);
		Image^ tmp = nullptr;
		if (img->Width < width && img->Height < width)
		{
			Graphics^ g = Graphics::FromImage(thumb);
			int xoffset = (int)((width - img->Width) / 2);
			int yoffset = (int)((width - img->Height) / 2);
			g->DrawImage(img, xoffset, yoffset, img->Width, img->Height);

		}
		else
		{
			System::Drawing::Image::GetThumbnailImageAbort^ myCallback = gcnew System::Drawing::Image::GetThumbnailImageAbort(this, &ViewResultForm::ThumbnailCallback);
			if (img->Width == img->Height)
			{
				thumb = img->GetThumbnailImage(width, width, myCallback, IntPtr::Zero);
			}
			else
			{
				int k = 0;
				int xoffset = 0;
				int yoffset = 0;
				if (img->Width < img->Height)
				{
					k = (int)(width * img->Width / img->Height);
					tmp = img->GetThumbnailImage(k, width, myCallback, IntPtr::Zero);
					xoffset = (int)((width - k) / 2);
				}
				if (img->Width > img->Height)
				{
					k = (int)(width * img->Height / img->Width);
					tmp = img->GetThumbnailImage(width, k, myCallback, IntPtr::Zero);
					yoffset = (int)((width - k) / 2);
				}
				Graphics^ g = Graphics::FromImage(thumb);
				g->DrawImage(tmp, xoffset, yoffset, tmp->Width, tmp->Height);
			}
		}
		Graphics^ g = Graphics::FromImage(thumb);
		g->DrawRectangle(Pens::Green, 0, 0, thumb->Width - 1, thumb->Height - 1);
		return thumb;
	}
	public: bool ThumbnailCallback()
	{
		return true;
	}
	private: System::Void ViewResultForm_Load(System::Object^  sender, System::EventArgs^  e) 
	{
		String^ FileIcon("file-icon.png");
		imageList->Images->Add(getThumbnailImage(this->imageList->ImageSize.Width, FileIcon));
		thumbnailLoader->RunWorkerAsync(imageList->ImageSize.Width);
	}
	private: delegate System::Void SetThumbnailDelegate(Image^ thumbnail, int itemIndex);
	private: delegate System::Void AddItemDelegate(String^ name, int imageIndex);
	private: System::Void thumbnailLoader_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) 
	{
		if (list == nullptr)
			return;
		int size = safe_cast<Int32>(e->Argument);
		for (int i = 0; i < list->list.size(); i++)
		{
			String^ name = gcnew String(list->list.at(i)->image1);
			name = name->Substring(name->LastIndexOf(gcnew String("\\")) + 1);
			Invoke(gcnew AddItemDelegate(this, &ViewResultForm::AddItem), gcnew array<Object^>{name, 0});
			name = gcnew String(list->list.at(i)->image2);
			name = name->Substring(name->LastIndexOf(gcnew String("\\")) + 1);
			Invoke(gcnew AddItemDelegate(this, &ViewResultForm::AddItem), gcnew array<Object^>{name, 0});
		}
		for (int i = 0; i < list->list.size(); i++)
		{
			Image^ thumb = getThumbnailImage(size, gcnew String(list->list.at(i)->image1));
			Invoke(gcnew SetThumbnailDelegate(this, &ViewResultForm::SetThumbnail), gcnew array<Object^>{thumb, 2 * i});
			thumb = getThumbnailImage(size, gcnew String(list->list.at(i)->image2));
			Invoke(gcnew SetThumbnailDelegate(this, &ViewResultForm::SetThumbnail), gcnew array<Object^>{thumb, 2 * i + 1});
		}
	}
	private: System::Void SetThumbnail(Image^ thumbnail, int itemIndex)
	{
		imageList->Images->Add(thumbnail);
		listView->Items[itemIndex]->ImageIndex = imageList->Images->Count - 1;
	}
	private: System::Void AddItem(String^ name, int imageIndex)
	{
		listView->Items->Add(name, imageIndex);
	}
	private: System::Void listView_ItemSelectionChanged(System::Object^  sender, System::Windows::Forms::ListViewItemSelectionChangedEventArgs^  e) 
	{
		for (int i = 0; i < listView->Items->Count; i++)
		{
			if (listView->Items[i]->Selected == true)
				listView->Items[i]->Selected = false;
		}
		/*this->Update();
		if (listView->FocusedItem->Index / 2 == selectionIndex)
			return;
		listView->Items[selectionIndex * 2]->Selected = false;
		listView->Items[selectionIndex * 2 + 1]->Selected = false;*/
		int index = listView->FocusedItem->Index;
		if (selectionIndex != index)
		{
			differenceLabel->Text = String::Format("Difference: {0}", list->list.at(index / 2)->similarity);
			pictureBox1->LoadAsync(gcnew String(list->list.at(index / 2)->image1));
			pictureBox2->LoadAsync(gcnew String(list->list.at(index / 2)->image2));
			/*MyAsyncLoad(pictureBox1, gcnew String(list->list.at(index / 2)->image1));
			MyAsyncLoad(pictureBox2, gcnew String(list->list.at(index / 2)->image2));*/
		}
		if (index % 2 == 0)
		{
			leftRect = true;
			rightRect = false;
			pictureBox1->Invalidate();
			pictureBox2->Invalidate();
		}
		else
		{
			rightRect = true;
			leftRect = false;
			pictureBox1->Invalidate();
			pictureBox2->Invalidate();
		}
		/*Control^ control = pictureBox1;
		Control::InvokePaint(control, gcnew PaintEventArgs(control->CreateGraphics(), *(gcnew Drawing::Rectangle(control->Location, control->Size))));
		control = pictureBox2;
		Control::InvokePaint(control, gcnew PaintEventArgs(control->CreateGraphics(), *(gcnew Drawing::Rectangle(control->Location, control->Size))));*/
		fileLabel1->Text = gcnew String(list->list.at(index / 2)->image1);
		fileLabel2->Text = gcnew String(list->list.at(index / 2)->image2);
		selectionIndex = index / 2;
	}
	private: void pictureBox1_Paint(Object^ sender, System::Windows::Forms::PaintEventArgs^ e)
	{
		// Draw the rectangle...
		if (leftRect&&pictureBox1->Image != nullptr)
		{
			Control^ control = pictureBox1;
			Image^ image = pictureBox1->Image;
			double ratio = Math::Min((double)control->Width / image->Width, (double)control->Height / image->Height);
			int width = ratio * image->Width;
			int height = ratio * image->Height;
			Drawing::Rectangle Rect;
			Rect.X = (control->Width - width) / 2;
			Rect.Y = (control->Height - height) / 2;
			Rect.Width = width;
			Rect.Height = height;
			//e->Graphics->FillRectangle(selectionBrush, Rect);
			e->Graphics->DrawRectangle(selectionPen, Rect);
		}
	}
	private: void pictureBox2_Paint(Object^ sender, System::Windows::Forms::PaintEventArgs^ e)
	{
		// Draw the rectangle...
		if (rightRect&&pictureBox2->Image!=nullptr)
		{
			Control^ control = pictureBox2;
			Image^ image = pictureBox2->Image;
			double ratio = Math::Min((double)control->Width / image->Width, (double)control->Height / image->Height);
			int width = ratio * image->Width;
			int height = ratio * image->Height;
			Drawing::Rectangle Rect;
			Rect.X = (control->Width - width) / 2;
			Rect.Y = (control->Height - height) / 2;
			Rect.Width = width;
			Rect.Height = height;
			//e->Graphics->FillRectangle(selectionBrush, Rect);
			e->Graphics->DrawRectangle(selectionPen, Rect);
		}
	}
	private: System::Void ViewResultFom_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) 
	{
		//if (leftRect&&pictureBox1->Image != nullptr)
		//{
		//	Control^ control = pictureBox1;
		//	Image^ image = pictureBox1->Image;
		//	double ratio = Math::Min((double)control->Width / image->Width, (double)control->Height / image->Height);
		//	int width = ratio * image->Width;
		//	int height = ratio * image->Height;
		//	Drawing::Rectangle Rect;
		//	Rect.X = control->Location.X + (control->Width - width) / 2;
		//	Rect.Y = control->Location.Y + (control->Height - height) / 2;
		//	Rect.Width = width;
		//	Rect.Height = height;
		//	//e->Graphics->FillRectangle(selectionBrush, Rect);
		//	e->Graphics->DrawRectangle(selectionPen, Rect);
		//}
		//if (rightRect&&pictureBox2->Image != nullptr)
		//{
		//	Control^ control = pictureBox2;
		//	Image^ image = pictureBox2->Image;
		//	double ratio = Math::Min((double)control->Width / image->Width, (double)control->Height / image->Height);
		//	int width = ratio * image->Width;
		//	int height = ratio * image->Height;
		//	Drawing::Rectangle Rect;
		//	Rect.X = control->Location.X + (control->Width - width) / 2;
		//	Rect.Y = control->Location.Y + (control->Height - height) / 2;
		//	Rect.Width = width;
		//	Rect.Height = height;
		//	//e->Graphics->FillRectangle(selectionBrush, Rect);
		//	e->Graphics->DrawRectangle(selectionPen, Rect);
		//}
	}
	private: delegate void SetImageDelegate(PictureBox^ pictureBox, Bitmap^ image);
	private: ref class ThreadX
	{
		ViewResultForm^ form;
		PictureBox^ pictureBox;
		String^ filename;
	public:
		ThreadX(ViewResultForm^ formin,PictureBox^ pictureBoxin, String^ filenamein)
		{
			form = formin;
			pictureBox = pictureBoxin;
			filename = filenamein;
		}
		void Work()
		{
			Drawing::Size^ size = pictureBox->Size;
			Bitmap^ image = gcnew Bitmap(filename);
			double ratio = Math::Min((double)size->Width / image->Width, (double)size->Height / image->Height);
			Bitmap^ resized = gcnew Bitmap(safe_cast<Image^>(image), (int)(ratio*image->Width), (int)(ratio*image->Height));
			form->Invoke(gcnew SetImageDelegate(form, &ViewResultForm::SetImage), gcnew array<Object^>{pictureBox, resized});
		}
	};
	private: void MyAsyncLoad(PictureBox^ pictureBox, String^ filename)
	{
		ThreadX^ args = gcnew ThreadX(this,pictureBox, filename);
		Thread^ loader = gcnew Thread(gcnew ThreadStart(args, &ThreadX::Work));
		loader->Start();
	}
	/*private: void MyAsyncLoadThread(PictureBox^ pictureBox , String^ filename)
	{
		Drawing::Size^ size = pictureBox->Size;
		Bitmap^ image = gcnew Bitmap(filename);
		double ratio = Math::Min((double)size->Width / image->Width, (double)size->Height / image->Height);
		Bitmap^ resized = gcnew Bitmap(image, gcnew Drawing::Size((int)(ratio*image->Width), (int)(ratio*image->Height)))
		Invoke(gcnew SetImageDelegate(this, &ViewResultForm::SetImage), gcnew array<Object^>{pictureBox, resized});
	}*/
	private: void SetImage(PictureBox^ pictureBox, Bitmap^ image)
	{
		pictureBox->Image = image;
	}

	private: System::Void listView_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) 
	{
		if (e->KeyCode == Keys::Enter)
		{
			wchar_t *filepath;
			int index = listView->FocusedItem->Index;
			if (index % 2 == 0)
				filepath = list->list.at(index / 2)->image2;
			else
				filepath = list->list.at(index / 2)->image1;
			FileInfo^ fileInfo = gcnew FileInfo(gcnew String(filepath));
			int waitingTime = 10;
			for (int i = 0; i < waitingTime; i++)
				if (IsFileLocked(fileInfo))
					if (i != waitingTime - 1)
						Sleep(500);
					else
						return;
				else
					break;
			MoveFile(gcnew String(filepath),gcnew String("RemovedFiles"));
			std::vector<int> indexList = list->Remove(filepath);
			for (int i = indexList.size() - 1; i >= 0; i--)
			{
				listView->Items->RemoveAt(indexList.at(i) * 2 + 1);
				listView->Items->RemoveAt(indexList.at(i) * 2);
			}
		}
		if (e->KeyCode == Keys::Space)
		{
			int index = listView->FocusedItem->Index;
			wchar_t *filepath1 = list->list.at(index / 2)->image1;
			wchar_t *filepath2 = list->list.at(index / 2)->image2;
			FileInfo^ fileInfo1 = gcnew FileInfo(gcnew String(filepath1));
			FileInfo^ fileInfo2 = gcnew FileInfo(gcnew String(filepath2));
			int waitingTime = 10;
			for (int i = 0; i < waitingTime; i++)
				if (IsFileLocked(fileInfo1) || IsFileLocked(fileInfo2))
					if (i != waitingTime - 1)
						Sleep(500);
					else
						return;
				else
					break;
			MoveFile(gcnew String(filepath1), gcnew String("SpecialFolder"));
			MoveFile(gcnew String(filepath2), gcnew String("SpecialFolder"));
			std::vector<int> indexList1 = list->Remove(filepath1);
			for (int i = indexList1.size() - 1; i >= 0; i--)
			{
				listView->Items->RemoveAt(indexList1.at(i) * 2 + 1);
				listView->Items->RemoveAt(indexList1.at(i) * 2);
			}
			std::vector<int> indexList2 = list->Remove(filepath2);
			for (int i = indexList2.size() - 1; i >= 0; i--)
			{
				listView->Items->RemoveAt(indexList2.at(i) * 2 + 1);
				listView->Items->RemoveAt(indexList2.at(i) * 2);
			}
		}
	}
	private: System::Void MoveFile(String^ filepath,String^ folder)
	{
		String^ filename = filepath->Substring(filepath->LastIndexOf(gcnew String("\\"))+1);
		String^ directory = filepath->Substring(0, filepath->LastIndexOf(gcnew String("\\")));
		String^ newDirectory = String::Concat(directory, gcnew String("\\"),folder);
		Directory::CreateDirectory(String::Concat(newDirectory));
		FileInfo^ file = gcnew FileInfo(filepath);
		File::Move(filepath, String::Concat(newDirectory, gcnew String("\\"), filename));
	}
	private: bool IsFileLocked(FileInfo^ file)
	{
		FileStream^ stream = nullptr;

		try
		{
			stream = file->Open(FileMode::Open, FileAccess::Read, FileShare::None);
		}
		catch (...)
		{
			//the file is unavailable because it is:
			//still being written to
			//or being processed by another thread
			//or does not exist (has already been processed)
			return true;
		}
		finally
		{
			if (stream != nullptr)
				stream->Close();
		}

			//file is not locked
		return false;
	}
};
}
