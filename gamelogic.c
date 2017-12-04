#include "gamelogic.h"

#include "clockhandler.h"
#include "girlfriendhandler.h"
#include "congratsgirlfriendscreen.h"


void startObjectificationGame()
{
	initGirlfriends();
	resetClock();

	setCongratsGirlfriendID(getNewGirlID());
	setNewScreen(&CongratsGirlfriendScreen);
}
