#include "girlbarhandler.h"

#include <tari/mugenanimationhandler.h>
#include <tari/animation.h>
#include <tari/texthandler.h>
#include <tari/math.h>

#include "girlfriendhandler.h"

typedef struct {
	int mPortraitID;
	int mTextID;

	int mBarID;

} SingleBar;

static struct {
	List mBars;
	TextureData mWhiteTexture;

	Position mBasePosition;

	int mIsSingleGirl;
	int mGirlfriendID;

} gData;


static void updateBarSize(SingleBar* e, int i) {
	int health = getGirlfriendHealth(i);
	int maxHealth = getGirlfriendMaxHealth(i);

	char text[100];
	sprintf(text, "%d/%d", health, maxHealth);
	setHandledText(e->mTextID, text);

	double heightPercentage = health / (double)maxHealth;
	setAnimationSize(e->mBarID, makePosition(20, heightPercentage * 100, 1), makePosition(0, 8, 0));
	setAnimationColorType(e->mBarID, COLOR_YELLOW);
}

void updateBarSizeCall(int i) {
	SingleBar* e = list_get(&gData.mBars, 0);
	updateBarSize(e, i);
}


static void loadSingleBar(int i, Position tPosition) {
	SingleBar* e = allocMemory(sizeof(SingleBar));

	char text[100];
	sprintf(text, "");
	e->mTextID = addHandledText(vecAdd(tPosition, makePosition(20, 0, 0)), text, 0, COLOR_WHITE, makePosition(20, 20, 1), makePosition(-5, -5, 0), makePosition(INF, INF, INF), INF);
	
	e->mBarID = playOneFrameAnimationLoop(vecAdd(tPosition, makePosition(35, 120, 0)), &gData.mWhiteTexture);
	updateBarSize(e, i);

	e->mPortraitID = addMugenAnimation(getGirlfriendPortraitAnimation(i), getGirlfriendPortraitSprites(), vecAdd(tPosition, makePosition(45, 150, 0)));
	setMugenAnimationDrawSize(e->mPortraitID, makePosition(30, 30, 1));

	list_push_back_owned(&gData.mBars, e);
}

static void loadBars() {
	int amount = getGirlfriendAmount();
	int i;

	Position pos = gData.mBasePosition;
	for (i = 0; i < amount; i++) {
		if (gData.mIsSingleGirl && i != gData.mGirlfriendID) continue;
		loadSingleBar(i, pos);
		pos.x -= 100;
	}
}

static void loadBarHandler(void* tCaller) {
	gData.mBars = new_list();
	gData.mWhiteTexture = loadTexture("$/rd/effects/white.pkg");

	loadBars();
}

ActorBlueprint GirlBarHandler = {
	.mLoad = loadBarHandler,
};

void setGirlBarHandlerSingleGirl(int tGirlfriendID)
{
	gData.mIsSingleGirl = 1;
	gData.mGirlfriendID = tGirlfriendID;
}

void setGirlBarHandlerAllGirls()
{
	gData.mIsSingleGirl = 0;
}

void setGirlBarHandlerBasePosition(Position tPosition)
{
	gData.mBasePosition = tPosition;
}
