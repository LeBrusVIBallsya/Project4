#include "MyForm.h"


using namespace System;
using namespace System::Windows::Forms;


[STAThreadAttribute]


void main(array <String^>^ args) {

	Application::EnableVisualStyles();

	Application::SetCompatibleTextRenderingDefault(false);


	Project4::MyForm form;

	Application::Run(% form);

}


struct Vector2
{

	int X, Y;

};


Vector2 Direction;

Vector2 PositionFruit;

Vector2 GameArea;


Project4::MyForm::MyForm() {

	InitializeComponent();

	GameArea.X = 500;

	GameArea.Y = 500;

	FirstLaunch = true;

	NewGame();

}


System::Void Project4::MyForm::информациоОИгреToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
	MessageBox::Show("Правила игры: \n"
		"1. Для управления использовать стрелки \n"
		"2. Ешьте фрукты для набора счёта и увеличения змейки \n"
		"3. Нельзя Есть себя и врезаться в стенки \n"
		"4. Наслаждайтесь ;)", "Правила игры!");

	return System::Void();

}


System::Void Project4::MyForm::новаяИграToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
	NewGame();

	return System::Void();

}


System::Void Project4::MyForm::паузаПродолжитьToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (Play) {

		Play = false;

	}
	else {

		Play = true;

		timer1->Start();

	}

	return System::Void();

}


System::Void Project4::MyForm::button1_Click(System::Object^ sender, System::EventArgs^ e)
{
	UpdateInterval = Convert::ToSingle(numericUpDown1->Value);

	timer1->Interval = UpdateInterval;


	button1->Enabled = false;

	numericUpDown1->Enabled = false;

	groupBox2->Enabled = false;


	Play = true;

	timer1->Start();


	return System::Void();

}

System::Void Project4::MyForm::GameTablet(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e)
{
	if (e->KeyCode.ToString() == "Right") {

		Direction.X = 1;

		Direction.Y = 0;

	}
	else if (e->KeyCode.ToString() == "Left") {

		Direction.X = -1;

		Direction.Y = 0;

	}
	else if (e->KeyCode.ToString() == "Up") {

		Direction.X = 0;

		Direction.Y = -1;

	}
	else if (e->KeyCode.ToString() == "Down") {

		Direction.X = 0;

		Direction.Y = 1;

	}
	else if (e->KeyCode.ToString() == "Space") {

		Project4::MyForm::паузаПродолжитьToolStripMenuItem_Click(nullptr, nullptr);

	}

	return System::Void();

}


System::Void Project4::MyForm::настройкиToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (groupBox2->Visible == false) {

		Play = false;

		button1->Enabled = true;

		numericUpDown1->Enabled = true;

		groupBox2->Enabled = true;

	}
	else {

		Play = true;

		timer1->Start();

		button1->Enabled = false;

		numericUpDown1->Enabled = false;

		groupBox2->Enabled = false;

	}

}


void Project4::MyForm::GeneratePositionFruit() {

	Random^ rand = gcnew Random();

	PositionFruit.X = rand->Next(40, GameArea.X);

	PositionFruit.Y = rand->Next(140, GameArea.Y);

	for (int i = 0; i < Score; i++) {

		if (PositionFruit.X == Serpens[i]->Location.X && PositionFruit.Y == Serpens[i]->Location.Y) {

			GeneratePositionFruit();

		}

	}

	int TempX = PositionFruit.X % Step;

	PositionFruit.X -= TempX;

	int TempY = PositionFruit.Y % Step;

	PositionFruit.Y -= TempY;

	Fruit->Location = Point(PositionFruit.X, PositionFruit.Y);

	this->Controls->Add(Fruit);

}


void Project4::MyForm::Eating() {

	if (Serpens[0]->Location.X == PositionFruit.X && Serpens[0]->Location.Y == PositionFruit.Y) {

		label1->Text = "Счёт: " + ++Score;

		Serpens[Score] = gcnew PictureBox();

		Serpens[Score]->Location = Point(Serpens[Score - 1]->Location.X + Step * Direction.X, Serpens[Score - 1]->Location.Y - Step * Direction.Y);

		Serpens[Score]->BackColor = Color::LightGreen;

		Serpens[Score]->Width = Step;

		Serpens[Score]->Height = Step;

		this->Controls->Add(Serpens[Score]);

		GeneratePositionFruit();

	}

}


void Project4::MyForm::Movement() {

	for (int i = Score; i >= 1; i--) {

		Serpens[i]->Location = Serpens[i - 1]->Location;

	}

	Serpens[0]->Location = Point(Serpens[0]->Location.X + Direction.X * Step, Serpens[0]->Location.Y + Direction.Y * Step);

}


void Project4::MyForm::EatYourself() {

	for (int i = 1; i < Score; i++) {

		if (Serpens[0]->Location == Serpens[i]->Location) {

			GameOver();

		}

	}

}


void Project4::MyForm::GameOver() {

	Play = true;

	Die = true;

	label4->Visible = true;

}


void Project4::MyForm::NewGame() {

	if (!FirstLaunch) {

		this->Controls->Remove(Fruit);

		for (int i = 0; i <= Score; i++) {

			this->Controls->Remove(Serpens[i]);

		}

		Score = 0;

	}
	else {

		FirstLaunch = false;

	}

	Serpens = gcnew array <PictureBox^, 1> (400);

	Serpens[0] = gcnew PictureBox();

	Serpens[0]->Location = Point(200, 200);

	Serpens[0]->BackColor = Color::Green;

	Serpens[0]->Width = Step;

	Serpens[0]->Height = Step;

	Score = 0;

	this->Controls->Add(Serpens[0]);


	Fruit = gcnew PictureBox();

	Fruit->BackColor = Color::Maroon;

	Fruit->Width = Step;

	Fruit->Height = Step;

	GeneratePositionFruit();


	timer1->Interval = UpdateInterval;

	timer1->Start();


	Direction.X = 1;

	Direction.Y = 0;


	Play = true;

	Die = false;


	label1->Text = "Счёт: 0";


	label4->Visible = false;

	groupBox2->Visible = false;

}


void Project4::MyForm::ChackBorders()
{
	if (Serpens[0]->Location.X >= pictureBox2->Location.X || Serpens[0]->Location.X <= pictureBox1->Location.X) {
		
		GameOver();

	}

	if (Serpens[0]->Location.Y <= pictureBox4->Location.Y || Serpens[0]->Location.Y >= pictureBox3->Location.Y) {
		
		GameOver();

	}

	label2->Text = "Кооринаты змейки: (" + Convert::ToString(Serpens[0]->Location.X) +", " + Convert::ToString(Serpens[0]->Location.Y) + ")";
}


System::Void Project4::MyForm::выходToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {

	Application::Exit();

}


void Project4::MyForm::Form_Update(Object^ obgect, EventArgs^ e) {

	if (!Die && Play) {

		Movement();

		Eating();

		EatYourself();

		ChackBorders();

	}
	else if (Die && Play) {

		timer1->Stop();

		MessageBox::Show("Игра окончена", "Внимание");

	}
	else if (!Play && !Die) {

		timer1->Stop();

		MessageBox::Show("Игра приостановлена", "Внимание");

	}
}
