//---------------------------------------------------------------------------


#pragma hdrstop

#include "Field.h"
//#include <string.h>
//#include <vcl.h>
//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

Field::Field()
{
	//Создание ячеек поля-----------
	cell = new Cell**[10];
	for(int i=0;i<10;i++)
	{
		cell[i] = new Cell*[10];
		for(int j=0;j<10;j++)
			cell[i][j] = new Cell();
	}
	//Создание кораблей-------------
	ship = new Ship*[10];
	for(int i=0;i<10;i++)
	{
		ship[i] = new Ship();
		if(i<1){ship[i]->setShipType(4);continue;}
		if(i>=1 && i<3){ship[i]->setShipType(3);continue;}
		if(i>=3 && i<6){ship[i]->setShipType(2);continue;}
		if(i>=6){ship[i]->setShipType(1);continue;}
	}
	shipNumber = 0;
	placingMode = 0;
	//------
	//vector <Cell*> c;
}
//---------------------------------------------------------------------------

Field::~Field()
{
	for (int i=0;i<10;i++)
	{
		for (int j=0;j<10;j++)
			delete cell[i][j];
		delete []cell[i];
	}
	delete []cell;

	for (int i=0;i<10;i++)
		delete ship[i];
	delete []ship;
}
//---------------------------------------------------------------------------

bool Field::isCellOccupied(int yPos, int xPos)
{
	return (cell[yPos][xPos]->isShipped() || cell[yPos][xPos]->isOccupied())? true : false;
}
//---------------------------------------------------------------------------

int Field::getCellState(int yPos, int xPos)
{
	int state = cell[yPos][xPos]->getState();
	return state;
}
//---------------------------------------------------------------------------

void Field::placeShip(int yPos, int xPos)                                       //Сделано под ячейки
{
	if(possibilityToPlace)
	{
		char shipType = ship[shipNumber]->getShipType();
		for(int y=shipMatrix.initialY;y<=shipMatrix.finalY;y++)
			for(int x=shipMatrix.initialX;x<=shipMatrix.finalX;x++)
			{
				if(placingMode == 0)
				{
					if(y==yPos && x>=xPos && x<=xPos-1+shipType)
						{cell[y][x]->setState(occupied);cell[y][x]->setShipNumber(shipNumber);}//cell[y][x]->setShip(ship[shipNumber]);
					else cell[y][x]->setState(bordered);
				}
				else
				{
					if(x==xPos && y>=yPos && y<=yPos-1+shipType)
						{cell[y][x]->setState(occupied);cell[y][x]->setShipNumber(shipNumber);}
					else cell[y][x]->setState(bordered);
				}
				ship[shipNumber]->addShipCoordinates(cell[y][x]);
			}
		/*
		for (unsigned int i=0;i<possibleShipCoordinates.size();i++)
		{
			cell[possibleShipCoordinates[i].y][possibleShipCoordinates[i].x]->setState(occupied);
			cell[possibleShipCoordinates[i].y][possibleShipCoordinates[i].x]->setShip(ship[shipToBePlaced]);
		}*/
		if(shipNumber<=9)shipNumber++;
		possibilityToPlace = false;
	}
}
//---------------------------------------------------------------------------

void Field::createShipMatrix(int yPos, int xPos)
{
	char shipType = ship[shipNumber]->getShipType();
	int initialYPos = (yPos-1)>=0 ? yPos-1:yPos;
	int initialXPos = (xPos-1)>=0 ? xPos-1:xPos;
	int finalYPos, finalXPos;
	if(placingMode == 0)//Horizontal
	{
		finalYPos = (yPos+1)<10 ? yPos+1:yPos;
		possibilityToPlace = (xPos-1+shipType)<10 ? true:false; //Проверка на длину (не вылазит ли за предел карты)
		finalXPos = (xPos-1+shipType+1)<10 ? xPos-1+shipType+1:9;
	}
	else 				//Vertical
	{
		finalXPos = (xPos+1)<10 ? xPos+1:xPos;
		possibilityToPlace = (yPos-1+shipType)<10 ? true:false; //Проверка на высоту (не вылазит ли за предел карты)
		finalYPos = (yPos-1+shipType+1)<10 ? yPos-1+shipType+1:9;
	}
	shipMatrix.setMatrix(initialYPos,initialXPos,finalYPos,finalXPos);
}
//---------------------------------------------------------------------------

bool Field::canShipBePlaced(int yPos, int xPos)
{
	char shipType = ship[shipNumber]->getShipType();
	possibilityToPlace = true;
	createShipMatrix(yPos,xPos); //possibilityToPlace can be changed
	if(!possibleShipCoordinates.empty()) possibleShipCoordinates.clear();
	for(int y=shipMatrix.initialY;y<=shipMatrix.finalY;y++)
		for(int x=shipMatrix.initialX;x<=shipMatrix.finalX;x++)
		{
			if(possibilityToPlace)
				if(cell[y][x]->isOccupied())
					possibilityToPlace = false;
			if(placingMode == 0)
			{
				if(y==yPos && x>=xPos && x<=xPos-1+shipType && !cell[y][x]->isOccupied())
					possibleShipCoordinates.push_back(getObject(y,x));
			}
			else
			{
				if(x==xPos && y>=yPos && y<=yPos-1+shipType && !cell[y][x]->isOccupied())
					possibleShipCoordinates.push_back(getObject(y,x));
			}
		}
	return possibilityToPlace;
}
//---------------------------------------------------------------------------

ShipCoordinates Field::getObject(int _y,int _x)
{
	ShipCoordinates shipCoordinates;
	shipCoordinates.y = _y;
	shipCoordinates.x = _x;
	return shipCoordinates;
}
//---------------------------------------------------------------------------

vector <ShipCoordinates> Field::getPossibleShipCoordinates()
{
	return possibleShipCoordinates;
}
//---------------------------------------------------------------------------

int Field::getShipNumber()
{
	return shipNumber;
}
//---------------------------------------------------------------------------

bool Field::shoot(int yPos, int xPos)
{
	int state = getCellState(yPos,xPos);
	switch(state)
	{
		case(empty):{cell[yPos][xPos]->setState(shotPast);break;}
		case(bordered):{cell[yPos][xPos]->setState(shotPast);break;}
		case(occupied):
		{
			cell[yPos][xPos]->setState(shotWell);
			int number = cell[yPos][xPos]->getShipNumber();
			ship[number]->decreaseHealth();
			if(ship[number]->getHealth()==0)
			{
				vector <Cell*> shipCoordinates = ship[number]->getShipCoordinates();
				for (unsigned int i=0;i<shipCoordinates.size();i++)
					if(shipCoordinates[i]->getState()==bordered)
						shipCoordinates[i]->setState(shotPast);
				shipNumber--;
			}
			return true;
		}
		default:{break;}
	}
	return false;
}
//---------------------------------------------------------------------------
