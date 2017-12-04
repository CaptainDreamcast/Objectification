#include "explanationscreen.h"

#include <tari/mugendefreader.h>
#include <tari/mugenanimationhandler.h>
#include <tari/input.h>
#include <tari/screeneffect.h>
#include <tari/texthandler.h>
#include <tari/math.h>
#include <tari/optionhandler.h>

#include "titlescreen.h"
#include "gamelogic.h"

static struct {
	MugenDefScript mScript;
	MugenDefScriptGroup* mCurrentGroup;

	MugenSpriteFile mSprites;
	MugenAnimations mAnimations;

	int mHasAnimation;
	int mIsShowingOptions;

	int mBuildUpTextID;
	int mAnimationID;
	int mOptionHandlerID;
} gData;

static void loadNextScriptGroup();

static void loadExplanationScreen() {
	instantiateActor(getMugenAnimationHandlerActorBlueprint());
	gData.mScript = loadMugenDefScript("assets/explanation/EXPLANATION.def");
	gData.mSprites = loadMugenSpriteFileWithoutPalette("assets/explanation/EXPLANATION.sff");
	gData.mAnimations = loadMugenAnimationFile("assets/explanation/EXPLANATION.air");
	gData.mCurrentGroup = gData.mScript.mFirstGroup;

	gData.mHasAnimation = 0;
	gData.mBuildUpTextID = -1;
	gData.mIsShowingOptions = 0;

	loadNextScriptGroup();

	addFadeIn(30, NULL, NULL);
}

static void loadNextScriptGroup() {
	char* allocatedText = getAllocatedMugenDefStringVariableAsGroup(gData.mCurrentGroup, "maintext");
	
	if (gData.mBuildUpTextID != -1) removeHandledText(gData.mBuildUpTextID);

	int length = strlen(allocatedText);
	gData.mBuildUpTextID = addHandledTextWithBuildup(makePosition(20, 20, 40), allocatedText, 0, COLOR_WHITE, makePosition(20, 20, 1), makePosition(-5, -5, 0), makePosition(620, 480, 1), INF, length);


	if (gData.mHasAnimation) removeMugenAnimation(gData.mAnimationID);

	gData.mHasAnimation = isMugenDefNumberVariableAsGroup(gData.mCurrentGroup, "animation");
	if (gData.mHasAnimation) {
		int animationNumber = getMugenDefNumberVariableAsGroup(gData.mCurrentGroup, "animation");
		gData.mAnimationID = addMugenAnimation(getMugenAnimation(&gData.mAnimations, animationNumber), &gData.mSprites, getMugenDefVectorOrDefaultAsGroup(gData.mCurrentGroup, "animationposition", makePosition(0, 0, 30)));
	}

	gData.mIsShowingOptions = 0;

	freeMemory(allocatedText);
}

static void selectOptionCB(void* tCaller) {
	(void)tCaller;

	removeActor(gData.mOptionHandlerID);
	
	gData.mCurrentGroup = gData.mCurrentGroup->mNext;
	if (gData.mCurrentGroup != NULL) {
		loadNextScriptGroup();
	}
	else {
		startObjectificationGame();
	}
}

static void showGroupOptions() {
	gData.mOptionHandlerID = instantiateActor(getOptionHandlerBlueprint());
	setOptionButtonA();
	setOptionTextSize(20);
	setOptionTextBreakSize(-5);
	char* option1 = getAllocatedMugenDefStringVariableAsGroup(gData.mCurrentGroup, "option1");
	char* option2 = getAllocatedMugenDefStringVariableAsGroup(gData.mCurrentGroup, "option2");

	addOption(makePosition(20, 300, 30), option1, selectOptionCB, NULL);
	addOption(makePosition(20, 340, 30), option2, selectOptionCB, NULL);

	freeMemory(option1);
	freeMemory(option2);

	gData.mIsShowingOptions = 1;
}

static void checkScreenAInput() {
	if (!isHandledTextBuiltUp(gData.mBuildUpTextID)) {
		setHandledTextBuiltUp(gData.mBuildUpTextID);
		showGroupOptions();
	}
}

static void updateScreenInput() {
	if (hasPressedAbortFlank()) {
		setNewScreen(&TitleScreen);
	}
	if (hasPressedAFlank()) {
		checkScreenAInput();
	}
}

static void updateScreenOptions() {
	if (!gData.mIsShowingOptions && isHandledTextBuiltUp(gData.mBuildUpTextID)) {
		showGroupOptions();
	}
}

static void updateExplanationScreen() {
	updateScreenInput();
	updateScreenOptions();
}

Screen ExplanationScreen = {
	.mLoad = loadExplanationScreen,
	.mUpdate = updateExplanationScreen,
};