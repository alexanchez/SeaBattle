//---------------------------------------------------------------------------

#ifndef FieldH
#define FieldH
//---------------------------------------------------------------------------
#include "Ship.h"
#include "Cell.h"
#include <vector.h>
//---------------------------------------------------------------------------

struct ShipMatrix
{
	int initialX,finalX;
	int initialY,finalY;
	void setMatrix(int _initialYPos,int _initialXPos,int _finalYPos,int _finalXPos)
	{
		initialY = _initialYPos;
		initialX = _initialXPos;
		finalX = _finalXPos;
		finalY = _finalYPos;
	}
};
struct ShipCoordinates
{
	int x;
	int y;
};
//---------------------------------------------------------------------------

class Field
{
	private:
		Ship **ship;
		Cell ***cell;
		int shipNumber;
		bool possibilityToPlace; //Чтобы в placeShip снова не вызывать метод canShipBePlaced
		vector <ShipCoordinates> possibleShipCoordinates;
		ShipMatrix shipMatrix;
	public:
		Field();
		~Field();
		int placingMode; //0 - Horizontal; 1 - Vertical
		int getCellState(int yPos, int xPos);
		bool canShipBePlaced(int yPos, int xPos);
		void placeShip(int yPos, int xPos);
		void createShipMatrix(int yPos, int xPos);
		ShipCoordinates getObject(int _y,int _x);
		vector <ShipCoordinates> getPossibleShipCoordinates();
		int getShipNumber();
		//Shooting---
		bool shoot(int yPos, int xPos);
		//Not used
		bool isCellOccupied(int yPos, int xPos);
};
//---------------------------------------------------------------------------
#endif
