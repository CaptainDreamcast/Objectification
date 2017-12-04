#include "selectgirlscreen.h"

#include <tari/mugenanimationhandler.h>
#include <tari/input.h>
#include <tari/optionhandler.h>
#include <tari/texthandler.h>
#include <tari/math.h>
#include <tari/screeneffect.h>
#include <tari/animation.h>

#include "girlfriendhandler.h"
#include "girlbarhandler.h"
#include "titlescreen.h"
#include "clockhandler.h"
#include "talktogirlscreen.h"

static struct {
	MugenSpriteFile mSprites;

	int mBGID;

	TextureData mWhiteTexture;
	int mBlackBGID;
} gData;

static void gotoTalkScreen(void* tCaller) {
	(void)tCaller;
	setNewScreen(&TalkToGirlScreen);
}

static void selectCB(void* tCaller) {
	int i = (int)tCaller;
	setTalkToGirlID(i);
	addFadeOut(30, gotoTalkScreen, NULL);
}

static void loadSelection() {
	Position pos = makePosition(60, 100, 20);

	gData.mWhiteTexture = loadTexture("$/rd/effects/white.pkg");
	gData.mBlackBGID = playOneFrameAnimationLoop(vecAdd(pos, makePosition(-30, -10, -1)), &gData.mWhiteTexture);
	setAnimationSize(gData.mBlackBGID, makePosition(320, 180, 1), makePosition(0, 0, 0));
	setAnimationColorType(gData.mBlackBGID, COLOR_BLACK);
	setAnimationTransparency(gData.mBlackBGID, 0.6);

	addHandledText(pos, "Spend time with...", 0, COLOR_WHITE, makePosition(20, 20, 1), makePosition(-5, -5, 0), makePosition(INF, INF, INF), INF);


	instantiateActor(getOptionHandlerBlueprint());
	setOptionButtonA();
	setOptionTextSize(20);
	setOptionTextBreakSize(-5);

	pos = vecAdd(pos, makePosition(0, 10, 0));

	int i;
	for (i = 0; i < getGirlfriendAmount(); i++) {

		char girlfriendName[100];
		getGirlfriendName(girlfriendName, i);

		addOption(vecAdd(pos, makePosition(0, (i+1)*30, 0)), girlfriendName, selectCB, (void*)i);
	}
}


static void loadSelectScreen() {
	instantiateActor(getMugenAnimationHandlerActorBlueprint());

	instantiateActor(GirlfriendHandler);
	setGirlBarHandlerAllGirls();
	setGirlBarHandlerBasePosition(makePosition(500, 300, 20));
	instantiateActor(GirlBarHandler);
	instantiateActor(ClockHandler);

	loadSelection();



	gData.mSprites = loadMugenSpriteFileWithoutPalette("assets/select/SELECT.sff");
	gData.mBGID = addMugenAnimation(createOneFrameMugenAnimationForSprite(0, 1), &gData.mSprites, makePosition(0, 0, 1));
}

static void updateSelectScreen() {
	if (hasPressedAbortFlank()) {
		setNewScreen(&TitleScreen);
	}
}

Screen SelectGirlScreen = {
	.mLoad = loadSelectScreen,
	.mUpdate = updateSelectScreen,
};