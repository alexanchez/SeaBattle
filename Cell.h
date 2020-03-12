//---------------------------------------------------------------------------

#ifndef CellH
#define CellH
//---------------------------------------------------------------------------
//#include "Ship.h"
//---------------------------------------------------------------------------
enum occupation {empty, bordered, occupied, shotPast, shotWell};

class Cell
{
	private:
		//Ship *ship;//Not used
		int shipNumber;
		occupation state;
	public:
		Cell();
		~Cell();
		void setState(occupation _state);
		void setShipNumber(int _shipNumber);
		int getShipNumber();
		int getState();
		bool isShipped();
		bool isOccupied();
		//Not used
		/*void setShip(Ship *_ship);
		Ship* getShip();*/
};
//---------------------------------------------------------------------------
#endif

/*
state:
	Empty 		- empty
	Bordered	- bordered
	Occupied	- occupied (by ship)
	ShotPast 	- shot past (without ship)
	ShotWell	- shot well (with ship)

*/
