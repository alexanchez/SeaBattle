//---------------------------------------------------------------------------

#ifndef ShipH
#define ShipH

#include "Cell.h"
#include <vector.h>
//---------------------------------------------------------------------------
//enum state {alive, injured, killed};

class Ship
{
	private:
		char type;    //4,3,2,1
		int health;
		vector <Cell*> coordinates;
	public:
		Ship();
		~Ship();
		void setShipType(char _type);
		char Ship::getShipType();
		void decreaseHealth();
		int getHealth();
		void addShipCoordinates(Cell* cell);
		vector <Cell*> getShipCoordinates();
};
//---------------------------------------------------------------------------
#endif
