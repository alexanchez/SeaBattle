//---------------------------------------------------------------------------

#include <vcl.h>
#include <time.h>
#pragma hdrstop

#include "Unit1.h"
#include "Game.h"
#include "Cell.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
TImage **Player1Images;
TImage **Player2Images;
int VOLUME = 20, SHIFT_LEFT, SHIFT_TOP;
int placingMode = 0;
Game *game;
vector <int> opponentMoves;
//---------------------------------------------------------------------------
void RepaintFields();
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	SHIFT_TOP = 80;
	SHIFT_LEFT = 25;
	srand(time(NULL));
}
//---------------------------------------------------------------------------
void __fastcall TForm1::NewGameButtonClick(TObject *Sender)
{
	if(game){Beep();delete game;}  //ShowMessage("Game was deleted");
	game = new Game();
	Label1->Caption="Place ships. LMB place. RMB rotate";
	RepaintFields();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormShow(TObject *Sender)
{
	Player1Images = new TImage*[100];
	Player2Images = new TImage*[100];
	for(int i=0;i<100;i++)
	{
		//Player1----------------------------------------------
		Player1Images[i] = new TImage(Form1);
		Player1Images[i]->Parent = Form1;
		Player1Images[i]->Name = "Player1Image" + AnsiString(i+1);
		Player1Images[i]->Top = int(i/10)*VOLUME + SHIFT_TOP;
		Player1Images[i]->Left = int(i%10)*VOLUME + SHIFT_LEFT;
		Player1Images[i]->Height = VOLUME;
		Player1Images[i]->Width = VOLUME;
		Player1Images[i]->Tag = i; //(i<10)?
		Player1Images[i]->Canvas->Rectangle(0,0,VOLUME,VOLUME);
		Player1Images[i]->Canvas->Brush->Color = clGreen;
		Player1Images[i]->OnMouseEnter = ImageMouseEnter;
		Player1Images[i]->OnMouseLeave = ImageMouseLeave;
		Player1Images[i]->OnMouseDown = ImageMouseDown;
		Player1Images[i]->OnClick = ImageClick;
		//Player2----------------------------------------------
		Player2Images[i] = new TImage(Form1);
		Player2Images[i]->Parent = Form1;
		Player2Images[i]->Name = "Player2Image" + AnsiString(i+1);
		Player2Images[i]->Top = int(i/10)*VOLUME + SHIFT_TOP;
		Player2Images[i]->Left = int(i%10)*VOLUME + SHIFT_LEFT + VOLUME*10+50;  //+250 - смещение поля2 от поля1
		Player2Images[i]->Height = VOLUME;
		Player2Images[i]->Width = VOLUME;
		Player2Images[i]->Tag = i;
		Player2Images[i]->Canvas->Rectangle(0,0,VOLUME,VOLUME);
		Player2Images[i]->Canvas->Brush->Color = clGreen;
		Player2Images[i]->OnMouseEnter = ImageMouseEnter;
		Player2Images[i]->OnMouseLeave = ImageMouseLeave;
		//Player2Images[i]->OnMouseDown = ImageMouseDown;
		Player2Images[i]->OnClick = ImageClick;
	}
	//Labels positions
	Player1Label->Left = SHIFT_LEFT;Player1Label->Width = VOLUME*10;
	Player2Label->Left = Player2Images[0]->Left;Player2Label->Width = VOLUME*10;
	Label1->Left = SHIFT_LEFT;Label1->Width = VOLUME*20 + 50;
}
//---------------------------------------------------------------------------

void RepaintField(TImage **PlayerImages,Field *field,int y,int x)
{
	TColor cellColor = PlayerImages[y*10+x]->Canvas->Pixels[5][5];
	int state = field->getCellState(y,x);
	switch(state)
	{
		case(empty):
		{
			PlayerImages[y*10+x]->Canvas->Brush->Color=clWhite;
			PlayerImages[y*10+x]->Canvas->FloodFill(5,5,cellColor,fsSurface);
			PlayerImages[y*10+x]->Canvas->Rectangle(0,0,VOLUME,VOLUME);
			break;
		}
		case(bordered):
		{
			PlayerImages[y*10+x]->Canvas->Brush->Color=clWhite;
			PlayerImages[y*10+x]->Canvas->FloodFill(5,5,cellColor,fsSurface);
			break;
		}
		case(occupied):
		{
			if(field==game->field1)//Спрятать отрисовку врага
			{
				PlayerImages[y*10+x]->Canvas->Brush->Color=clGreen;
				PlayerImages[y*10+x]->Canvas->FloodFill(5,5,cellColor,fsSurface);
			}
			break;
		}
		case(shotPast):
		{
			PlayerImages[y*10+x]->Canvas->Brush->Color=clWhite;
			PlayerImages[y*10+x]->Canvas->FloodFill(5,5,cellColor,fsSurface);
			PlayerImages[y*10+x]->Canvas->Brush->Color=clBlack;
			PlayerImages[y*10+x]->Canvas->Ellipse(VOLUME/2-0.1*VOLUME,VOLUME/2-0.1*VOLUME,VOLUME/2+0.1*VOLUME,VOLUME/2+0.1*VOLUME);
			break;
		}
		case(shotWell):
		{
			PlayerImages[y*10+x]->Canvas->Brush->Color=clRed;
			PlayerImages[y*10+x]->Canvas->FloodFill(5,5,cellColor,fsSurface);
			PlayerImages[y*10+x]->Canvas->Brush->Color=clBlack;
			PlayerImages[y*10+x]->Canvas->Ellipse(VOLUME/2-0.1*VOLUME,VOLUME/2-0.1*VOLUME,VOLUME/2+0.1*VOLUME,VOLUME/2+0.1*VOLUME);
			break;
		}
		default:{break;}
	}
}
//---------------------------------------------------------------------------

void RepaintFields()
{
	for(int y=0;y<10;y++)
		for(int x=0;x<10;x++)
		{
			RepaintField(Player1Images,game->field1,y,x);
			RepaintField(Player2Images,game->field2,y,x);
		}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ImageMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y)
{
   if(Button == mbRight && game && AnsiString(game->getState())=="Placing")
   {
		placingMode = placingMode==0?1:0;
		RepaintFields();
		ImageMouseEnter(Sender);
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ImageMouseEnter(TObject *Sender)
{
	TImage *current=static_cast<TImage *>(Sender);
	int y = current->Tag/10, x = current->Tag%10;           //YX
	if(game)
	{
		if(current->Name.Pos("Player1Image"))
		{
			if(AnsiString(game->getState())=="Placing")
			{
				game->field1->placingMode = placingMode==0?0:1;
				TColor paintColor = game->field1->canShipBePlaced(y,x)?clGreen:clRed;
				vector <ShipCoordinates> possibleShipCoordinates = game->field1->getPossibleShipCoordinates();
				for (unsigned int i=0;i<possibleShipCoordinates.size();i++)
				{
					Player1Images[possibleShipCoordinates[i].y*10+possibleShipCoordinates[i].x]->Canvas->Brush->Color=paintColor;
					Player1Images[possibleShipCoordinates[i].y*10+possibleShipCoordinates[i].x]->Canvas->FloodFill(5,5,clWhite,fsSurface);
				}
			}
			if(AnsiString(game->getState())=="Shooting");
		}
		else
		{
        	if(AnsiString(game->getState())=="Shooting")
			{
				if(game->field2->getCellState(y,x)==empty||game->field2->getCellState(y,x)==bordered||
				   game->field2->getCellState(y,x)==occupied)
				{
					current->Canvas->Brush->Color=clSilver;
					current->Canvas->FloodFill(5,5,clWhite,fsSurface);
				}
			}
        }
	}
	current=NULL;delete current;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ImageMouseLeave(TObject *Sender)
{
	TImage *current=static_cast<TImage *>(Sender);
	if(game)
	{
		RepaintFields();
		if(AnsiString(game->getState())=="Placing");
		if(AnsiString(game->getState())=="Shooting" && current->Name.Pos("Player2Image"))
		{
			current->Canvas->Brush->Color=clWhite;
			current->Canvas->FloodFill(5,5,clSilver,fsSurface);
		}
	}
	current=NULL;delete current;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ImageClick(TObject *Sender)
{
	TImage *current=static_cast<TImage *>(Sender);
	int y = current->Tag/10, x = current->Tag%10;
	if(game)
	{
		if(AnsiString(game->getState())=="Placing")
		{
			game->field1->placeShip(y,x);
			if(game->field1->getShipNumber()==10)//Если расставлены все свои корабли - расставить вражеские
			{
				game->setState("Shooting");
				//Расстановка кораблей врага
				for(int i=0;i<100;i++)
				   opponentMoves.push_back(i);
				for(int i=0;i<10;i++)
				{
				   while(true)
				   {
					   int number = rand()%opponentMoves.size();
					   game->field2->placingMode = rand()%2;
					   if(game->field2->canShipBePlaced(opponentMoves[number]/10,opponentMoves[number]%10))
					   {
							game->field2->placeShip(opponentMoves[number]/10,opponentMoves[number]%10);
							opponentMoves.erase(opponentMoves.begin()+number);
							break;
					   }
				   }
				}
				RepaintFields();
				Label1->Caption="Your turn";//"Player1's turn"
				//Инициализация ходов врага
				opponentMoves.clear();
				for(int i=0;i<100;i++)
				   opponentMoves.push_back(i);
			}
		}
		if(AnsiString(game->getState())=="Shooting" && current->Name.Pos("Player2Image") && !Timer1->Enabled)//Враг не ходит
			if(game->field2->getCellState(y,x)!=shotPast && game->field2->getCellState(y,x)!=shotWell)//Не выполнять, если shotPast/shotWell
			{
				bool hit = game->field2->shoot(y,x);
				if(game->field2->getShipNumber()==0){Label1->Caption="You win";game->setState("WinnerDetermination");}
				if(!hit)
				{
					//Ходит враг
					Label1->Caption="Opponent turn";
					int number;
					while(true && opponentMoves.size()!=0)//Чтобы не стрелял по shotPast и shotWell
					{
					   number = rand()%opponentMoves.size();
					   if(game->field1->getCellState(opponentMoves[number]/10,opponentMoves[number]%10)!=shotPast &&
						  game->field1->getCellState(opponentMoves[number]/10,opponentMoves[number]%10)!=shotWell)
						  break;
					   opponentMoves.erase(opponentMoves.begin()+number);
					}
					//int number = rand()%opponentMoves.size();
					hit = game->field1->shoot(opponentMoves[number]/10,opponentMoves[number]%10);
					opponentMoves.erase(opponentMoves.begin()+number);
					if(hit && game->field1->getShipNumber()!=0)Timer1->Enabled=true;//Проверка: Если попал, продолжать
					if(!hit)Label1->Caption="Your turn";
					if(game->field1->getShipNumber()==0){Label1->Caption="Opponent win";game->setState("WinnerDetermination");}
					//Caption=opponentMoves.size();
				}
				RepaintFields();
			}
	}
	current=NULL;delete current;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
	//Продолжать ходы врага после попадания
	int number;
	while(true && opponentMoves.size()!=0)//Чтобы не стрелял по shotPast и shotWell
	{
	   number = rand()%opponentMoves.size();
	   if(game->field1->getCellState(opponentMoves[number]/10,opponentMoves[number]%10)!=shotPast &&
		  game->field1->getCellState(opponentMoves[number]/10,opponentMoves[number]%10)!=shotWell)
		  break;
	   opponentMoves.erase(opponentMoves.begin()+number);
	}
	//int number = rand()%opponentMoves.size();
	bool hit = game->field1->shoot(opponentMoves[number]/10,opponentMoves[number]%10);
	opponentMoves.erase(opponentMoves.begin()+number);
	if(!hit||game->field1->getShipNumber()==0){Timer1->Enabled=false;Label1->Caption="Your turn";}
	if(game->field1->getShipNumber()==0){Label1->Caption="Opponent win";game->setState("WinnerDetermination");}
	RepaintFields();
	//Caption=opponentMoves.size();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
   /*
   vector <int> uniqueVector;
   for(int i=0;i<100;i++)
	   uniqueVector.push_back(i);
   for(int i=0;i<10;i++)
   {
	   while(true)
	   {
		   int number = rand()%uniqueVector.size();
		   game->field2->placingMode = rand()%2;
		   if(game->field2->canShipBePlaced(uniqueVector[number]/10,uniqueVector[number]%10))
		   {
				game->field2->placeShip(uniqueVector[number]/10,uniqueVector[number]%10);
				uniqueVector.erase(uniqueVector.begin()+number);
				break;
		   }
	   }
   }
   RepaintFields();
   Caption=game->field2->getShipNumber();*/

}
//---------------------------------------------------------------------------

	//return (strcmp(state,"Empty") != 0)?true:false;
void __fastcall TForm1::Button2Click(TObject *Sender)
{
	/*int empty1 = 0,border1 = 0,occupy1 = 0;
	for(int y=0;y<10;y++)
		for(int x=0;x<10;x++)
		{
			int state = game->field2->getCellState(y,x);
			switch(state)
			{
				case(empty):
				{
					empty1++;
					break;
				}
				case(bordered):
				{
					border1++;
					break;
				}
				case(occupied):
				{
					occupy1++;
					break;
				}
				case(shotPast):{break;}
				case(shotWell):{break;}
				default:{break;}
			}
		}
	Form1->Caption="Empty:"+AnsiString(empty1)+"Border:"+AnsiString(border1)+";Occupy:"+AnsiString(occupy1);*/
	//Player1Images[0]->Canvas->Brush->Color=clWhite;
	//Player1Images[0]->Canvas->Rectangle(0,0,VOLUME,VOLUME);
	/*Player1Images[0]->Canvas->Brush->Color=clRed;
	Player1Images[0]->Canvas->FloodFill(5,5,clWhite,fsSurface);*/
	//Timer1->Enabled=true;
}
//---------------------------------------------------------------------------


