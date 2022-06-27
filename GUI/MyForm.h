#pragma once

#include "Function.h"

#include <algorithm>
#include <fstream>
#include <msclr/marshal_cppstd.h>
#include <string>
#include <vector>

namespace CppWinForm1 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Diagnostics;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
		Graphics^ gr;
		Pen^ penLine, ^ penFunction, ^ penConditions, ^ penPoint;
	private: System::Windows::Forms::Button^ button2;
	private: System::Windows::Forms::Button^ button1;


	public:
		MyForm(void)
		{
			InitializeComponent();
			this->gr = CreateGraphics();
			this->penLine = gcnew Pen(Color::Black);
			this->penFunction = gcnew Pen(Color::LightBlue);
			this->penConditions = gcnew Pen(Color::Yellow);
			this->penPoint = gcnew Pen(Color::Red);
			this->penFunction->Width = 2.0;
			this->penConditions->Width = 2.0;
			this->penPoint->Width = 2.0;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
			}
		}


	private: System::ComponentModel::IContainer^ components;

#pragma region Windows Form Designer generated code
		   /// <summary>
		   /// Required method for Designer support - do not modify
		   /// the contents of this method with the code editor.
		   /// </summary>
		   void InitializeComponent(void)
		   {
			   this->button1 = (gcnew System::Windows::Forms::Button());
			   this->button2 = (gcnew System::Windows::Forms::Button());
			   this->SuspendLayout();
			   // 
			   // button1
			   // 
			   this->button1->BackColor = System::Drawing::SystemColors::Highlight;
			   this->button1->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			   this->button1->Font = (gcnew System::Drawing::Font(L"Calibri", 12, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				   static_cast<System::Byte>(204)));
			   this->button1->Location = System::Drawing::Point(373, 457);
			   this->button1->Name = L"button1";
			   this->button1->Size = System::Drawing::Size(350, 60);
			   this->button1->TabIndex = 0;
			   this->button1->Text = L"Start one-dimensional!";
			   this->button1->UseVisualStyleBackColor = false;
			   this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click_1);
			   // 
			   // button2
			   // 
			   this->button2->BackColor = System::Drawing::SystemColors::Highlight;
			   this->button2->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			   this->button2->Font = (gcnew System::Drawing::Font(L"Calibri", 12, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
				   static_cast<System::Byte>(204)));
			   this->button2->Location = System::Drawing::Point(373, 567);
			   this->button2->Name = L"button2";
			   this->button2->Size = System::Drawing::Size(350, 60);
			   this->button2->TabIndex = 1;
			   this->button2->Text = L"Start multi-dimensional!";
			   this->button2->UseVisualStyleBackColor = false;
			   this->button2->Click += gcnew System::EventHandler(this, &MyForm::button2_Click);
			   // 
			   // MyForm
			   // 
			   this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			   this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			   this->BackColor = System::Drawing::SystemColors::Window;
			   this->ClientSize = System::Drawing::Size(1082, 1053);
			   this->Controls->Add(this->button2);
			   this->Controls->Add(this->button1);
			   this->Margin = System::Windows::Forms::Padding(4);
			   this->Name = L"MyForm";
			   this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			   this->Text = L"IndexAlgotirhm";
			   this->ResumeLayout(false);

		   }
#pragma endregion

private: System::Void button1_Click_1(System::Object^ sender, System::EventArgs^ e) {
	//draw a coordinate system
	this->button1->Visible = false;
	this->button2->Visible = false;

	gr->DrawLine(penLine, 147, 350, 750, 350);
	gr->DrawLine(penLine, 450, 100, 450, 603);
	gr->DrawLine(penLine, 450, 100, 455, 105);
	gr->DrawLine(penLine, 450, 100, 445, 105);
	gr->DrawLine(penLine, 750, 350, 745, 345);
	gr->DrawLine(penLine, 750, 350, 745, 355);

	int step = 70;

	for (int i = 450; i < 750; i += step)
		gr->DrawLine(penLine, i, 345, i, 355);

	for (int i = 450; i > 150; i -= step)
		gr->DrawLine(penLine, i, 345, i, 355);

	for (int i = 350; i > 100; i -= step)
		gr->DrawLine(penLine, 448, i, 452, i);

	for (int i = 350; i < 600; i += step)
		gr->DrawLine(penLine, 448, i, 452, i);

	//initialize func
	const int bufferSize = 250;
	char buffer[bufferSize];
	std::ifstream funcFile("../Algoritms/inputFile.txt");

	funcFile.getline(buffer, bufferSize, '\n');
	Function func(buffer);

	funcFile.getline(buffer, bufferSize, '\n');
	double leftBorder = atof(buffer);

	funcFile.getline(buffer, bufferSize, '\n');
	double rightBorder = atof(buffer);

	funcFile.getline(buffer, bufferSize, '\n');
	int countConditions = atoi(buffer);

	std::vector<Function> conditionsContainer;
	for (int i = 0; i < countConditions; i++)
	{
		funcFile.getline(buffer, bufferSize, '\n');
		conditionsContainer.push_back(Function(buffer));
	}

	//drawing func
	float stepFunction = step;
	float lastValue = static_cast<float>(func.getValue(leftBorder));
	for (float x = leftBorder + 0.01; x <= rightBorder; x += 0.01)
	{
		float startX = static_cast<float>(450. + (x - 0.01) * stepFunction);
		float startY = static_cast<float>(350. - lastValue * stepFunction);
		float finishX = static_cast<float>(450. + x * stepFunction);
		float finishY = static_cast<float>(350. - func.getValue(x) * stepFunction);

		gr->DrawLine(penFunction, startX, startY, finishX, finishY);

		lastValue = func.getValue(x);
	}

	//drawing condition
	for (int i = 0; i < countConditions; i++)
	{
		lastValue = static_cast<float>(conditionsContainer[i].getValue(leftBorder));
		for (float x = leftBorder + 0.01; x <= rightBorder; x += 0.01)
		{
			float startX = static_cast<float>(450. + (x - 0.01) * stepFunction);
			float startY = static_cast<float>(350. - lastValue * stepFunction);
			float finishX = static_cast<float>(450. + x * stepFunction);
			float finishY = static_cast<float>(350. - conditionsContainer[i].getValue(x) * stepFunction);

			gr->DrawLine(penConditions, startX, startY, finishX, finishY);

			lastValue = conditionsContainer[i].getValue(x);
		}
	}

	//initialize point 
	std::ifstream pointFile("../Algoritms/outputFile.txt");
	std::vector<long double>pointContainer;

	while (!pointFile.eof())
	{
		pointFile.getline(buffer, bufferSize, '\n');
		pointContainer.push_back(atof(buffer));
	}

	//drawing point
	for (std::vector<long double>::size_type i = 0; i < pointContainer.size() - 2; i++)
	{
		float x = static_cast<float>(450. + pointContainer[i] * stepFunction);
		float y = static_cast<float>(350. - func.getValue(pointContainer[i]) * stepFunction);

		gr->DrawEllipse(penPoint, x, y, 2., 2.);
	}

	this->penPoint->Width = 5.0;
	this->penPoint->Color = Color::Red;
	float x = static_cast<float>(450. + pointContainer[pointContainer.size() - 2] * stepFunction);
	float y = static_cast<float>(350. - func.getValue(pointContainer[pointContainer.size() - 2]) * stepFunction);
	gr->DrawEllipse(penPoint, x, y, 5., 5.);

}
private: System::Void button2_Click(System::Object^ sender, System::EventArgs^ e) {
	//draw a coordinate system
	this->button1->Visible = false;
	this->button2->Visible = false;

	gr->DrawLine(penLine, 100, 600, 600, 600);
	gr->DrawLine(penLine, 100, 100, 600, 100);
	gr->DrawLine(penLine, 100, 100, 100, 600);
	gr->DrawLine(penLine, 600, 100, 600, 600);

	//initialize point 
	std::ifstream pointFile("../Algoritms/outputFile.txt");
	std::vector<long double>pointsX;
	std::vector<long double>pointsY;

	std::vector<long double> borders;

	const int bufferSize = 250;
	char buffer[bufferSize];

	for (int i = 0; i < 2; i++) {
		pointFile.getline(buffer, bufferSize, ' ');
		borders.push_back(atof(buffer));
		pointsX.push_back(atof(buffer));

		pointFile.getline(buffer, bufferSize, '\n');
		borders.push_back(atof(buffer));
		pointsY.push_back(atof(buffer));
	}

	long double deltaX = borders[2] - borders[0];
	long double deltaY = borders[3] - borders[1];

	while (!pointFile.eof())
	{
		pointFile.getline(buffer, bufferSize, ' ');
		pointsX.push_back(atof(buffer));

		pointFile.getline(buffer, bufferSize, '\n');
		pointsY.push_back(atof(buffer));
	}

	//drawing point
	for (std::vector<long double>::size_type i = 0; i < pointsX.size() - 2; i++)
	{
		float x = static_cast<float>(100. + 500. * ((pointsX[i] - borders[0]) / deltaX));
		float y = static_cast<float>(100. + 500. * ((pointsY[i] - borders[1]) / deltaY));

		gr->DrawEllipse(penLine, x, y, 2., 2.);
	}

	this->penPoint->Width = 5.0;
	this->penPoint->Color = Color::Red;
	float x = static_cast<float>(100. + 500 * ((pointsX[pointsX.size() - 2] - borders[0]) / deltaX));
	float y = static_cast<float>(100. + 500 * ((pointsY[pointsY.size() - 2] - borders[1]) / deltaY));
	gr->DrawEllipse(penPoint, x, y, 5., 5.);
}
};
}
