#include "titlescreen.h"

#include <tari/input.h>
#include <tari/mugenanimationhandler.h>
#include <tari/screeneffect.h>

#include "explanationscreen.h"
#include "gamelogic.h"

static struct {
	MugenSpriteFile mSprites;
	int mBGID;

} gData;

static void loadTitleScreen() {
	instantiateActor(getMugenAnimationHandlerActorBlueprint());
	gData.mSprites = loadMugenSpriteFileWithoutPalette("assets/title/TITLE.sff");
	gData.mBGID = addMugenAnimation(createOneFrameMugenAnimationForSprite(1, 0), &gData.mSprites, makePosition(0, 0, 1));
	addFadeIn(30, NULL, NULL);
}


static void gotoExplanationScreen(void* tCaller) {
	(void)tCaller;
	startObjectificationGame();
}

static void updateTitleScreen() {
	if (hasPressedAbortFlank()) {
		abortScreenHandling();
	}

	if (hasPressedStartFlank()) {
		addFadeOut(30, gotoExplanationScreen, NULL);
	}
}

Screen TitleScreen = {
	.mLoad = loadTitleScreen,
	.mUpdate = updateTitleScreen,
};