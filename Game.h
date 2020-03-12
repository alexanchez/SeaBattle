//---------------------------------------------------------------------------

#ifndef GameH
#define GameH

#include "Field.h"
//#include <vcl.h>
//---------------------------------------------------------------------------
class Game
{
	private:
		bool started;
		char *state;//Placing/Shooting/WinnerDetermination
		char *currentPlayer; //Player1/Player2
	public:
		Field *field1;
		Field *field2;
		Game();
		~Game();
		void setState(char *_state);
		char* getState();
};
//---------------------------------------------------------------------------
#endif

