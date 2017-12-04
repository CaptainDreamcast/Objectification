#include "congratsgirlfriendscreen.h"

#include <tari/mugenanimationhandler.h>
#include <tari/timer.h>
#include <tari/screeneffect.h>
#include <tari/input.h>

#include "girlfriendhandler.h"
#include "congratsscreen.h"
#include "titlescreen.h"
#include "selectgirlscreen.h"

static struct {
	MugenSpriteFile mSprites;
	MugenAnimations mAnimations;
	int mGirlID;

	int mBGID;
	int mGirlAnimationID;
	int mCongratsID;
	int mYouGotItID;
} gData;

static void goToNextScreen(void* tCaller) {
	if (hasEnoughGirlfriends()) {
		setNewScreen(&CongratsScreen);
	}
	else {
		setNewScreen(&SelectGirlScreen);
	}
}

static void goToNextScreenCB(void* tCaller) {
	addFadeOut(30, goToNextScreen, NULL);
}

static void loadCongratsScreen() {
	instantiateActor(getMugenAnimationHandlerActorBlueprint());
	instantiateActor(GirlfriendHandler);
	addNewGirlfriend(gData.mGirlID);

	gData.mSprites = loadMugenSpriteFileWithoutPalette("assets/congratsgirl/SPRITES.sff");
	gData.mAnimations = loadMugenAnimationFile("assets/congratsgirl/ANIM.air");

	gData.mBGID = addMugenAnimation(createOneFrameMugenAnimationForSprite(0, 1), &gData.mSprites, makePosition(0, 0, 1));
	gData.mGirlAnimationID = addMugenAnimation(getMugenAnimation(getGirlfriendPortraitAnimations(), gData.mGirlID), getGirlfriendPortraitSprites(), makePosition(320, 240, 20));
	setMugenAnimationDrawSize(gData.mGirlAnimationID, makePosition(200, 200, 1));

	gData.mCongratsID = addMugenAnimation(getMugenAnimation(&gData.mAnimations, 1), &gData.mSprites, makePosition(320, 240, 10));
	setMugenAnimationDrawScale(gData.mCongratsID, makePosition(5, 5, 1));

	gData.mYouGotItID = addMugenAnimation(createOneFrameMugenAnimationForSprite(2, 1), &gData.mSprites, makePosition(320, 400, 10));

	addTimerCB(400, goToNextScreenCB, NULL);
}

static void updateCongratsScreen() {
	if (hasPressedAbortFlank()) {
		setNewScreen(&TitleScreen);
	}

	if (hasPressedAFlank() || hasPressedStartFlank()) {
		goToNextScreenCB(NULL);
	}
}

Screen CongratsGirlfriendScreen = {
	.mLoad = loadCongratsScreen,
	.mUpdate = updateCongratsScreen,
};

void setCongratsGirlfriendID(int tID)
{
	gData.mGirlID = tID;
}
