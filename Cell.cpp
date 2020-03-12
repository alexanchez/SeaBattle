//---------------------------------------------------------------------------


#pragma hdrstop

#include "Cell.h"
#include <vcl.h>
//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------

Cell::Cell()
{
	state = empty;
	//ship = NULL;
}
//---------------------------------------------------------------------------

Cell::~Cell()
{
	//if(ship)delete ship;
}
//---------------------------------------------------------------------------

void Cell::setState(occupation _state)
{
	state = _state;
}
//---------------------------------------------------------------------------

int Cell::getState()
{
	return state;
}
//---------------------------------------------------------------------------

/*void Cell::setShip(Ship *_ship)
{
	//ship = new Ship();
	ship = _ship;
}
//---------------------------------------------------------------------------

Ship* Cell::getShip()
{
	return ship;
}
//--------------------------------------------------------------------------- */

void Cell::setShipNumber(int _shipNumber)
{
	shipNumber = _shipNumber;
}
//---------------------------------------------------------------------------

int Cell::getShipNumber()
{
	return shipNumber;
}
//---------------------------------------------------------------------------

bool Cell::isShipped()
{
	return true;//ship?true:false;
}
//---------------------------------------------------------------------------

bool Cell::isOccupied()
{
	return (state==occupied)?true:false;
}
//---------------------------------------------------------------------------