#include "clockhandler.h"

#include <tari/mugenanimationhandler.h>
#include <tari/texthandler.h>
#include <tari/math.h>

static struct {
	MugenSpriteFile mSprites;
	MugenAnimations mAnimations;

	int mHour;
	int mDay;

	TextureData mWhiteTexture;
	int mBlackBGID;
} gData;

static void loadClockHandler(void* tCaller) {
	gData.mSprites = loadMugenSpriteFileWithoutPalette("assets/clock/CLOCK.sff");
	gData.mAnimations = loadMugenAnimationFile("assets/clock/CLOCK.air");

	Position pos = makePosition(260, 20, 20);

	gData.mWhiteTexture = loadTexture("$/rd/effects/white.pkg");
	gData.mBlackBGID = playOneFrameAnimationLoop(vecAdd(pos, makePosition(-10, -10, -1)), &gData.mWhiteTexture);
	setAnimationSize(gData.mBlackBGID, makePosition(640, 50, 1), makePosition(0, 0, 0));
	setAnimationColorType(gData.mBlackBGID, COLOR_BLACK);
	setAnimationTransparency(gData.mBlackBGID, 0.6);

	char text[100];
	sprintf(text, "DAY %d -", gData.mDay);
	addHandledText(pos, text, 0, COLOR_WHITE, makePosition(30, 30, 1), makePosition(-5, -5, 0), makePosition(INF, INF, INF), INF);
	int length = strlen(text);

	pos = vecAdd(pos, makePosition((length - 1) * 30, 0, 0));
	if(gData.mHour >= 10) sprintf(text, "%d", gData.mHour);
	else sprintf(text, "0%d", gData.mHour);
	addHandledText(pos, text, 0, COLOR_WHITE, makePosition(30, 30, 1), makePosition(-5, -5, 0), makePosition(INF, INF, INF), INF);

	length = strlen(text);
	pos = vecAdd(pos, makePosition(length * 30, 0, 0));

	addMugenAnimation(getMugenAnimation(&gData.mAnimations, 1), &gData.mSprites, pos);

	pos = vecAdd(pos, makePosition(30, 0, 0));
	addHandledText(pos, "00", 0, COLOR_WHITE, makePosition(30, 30, 1), makePosition(-5, -5, 0), makePosition(INF, INF, INF), INF);
}

ActorBlueprint ClockHandler = {
	.mLoad = loadClockHandler,
};

void resetClock()
{
	gData.mDay = 1;
	gData.mHour = 14;
}

int increaseClockAndReturnIfNewDay()
{
	gData.mHour++;
	if (gData.mHour == 19) {
		gData.mDay++;
		gData.mHour = 14;
		return 1;
	}

	return 0;
}

