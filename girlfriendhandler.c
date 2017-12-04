#include "girlfriendhandler.h"

#include <tari/math.h>

#include "girlbarhandler.h"

#define MAX_CONVERSATIONS 6
#define MAX_GIRLS 5

typedef struct {
	int mIsActive;

	int mGirlID;
	int mConversationUsed[20];
	int mConversationAmount;

	int mHealth;
	int mMaxHealth;
} Girlfriend;

typedef struct {
	int mIsUsed;
} Girl;

static struct {
	int mGirlfriendAmount;
	Girlfriend mGirlfriends[10];
	Girl mGirls[20];

	MugenSpriteFile mPortraitSprites;
	MugenAnimations mPortraitAnimations;

	MugenSpriteFile mGirlfriendSprites;
	MugenAnimations mGirlfriendAnimations;
} gData;


static char gGirlNames[20][1024] = {
"Rihanna",
"Candice",
"Guiselle",
"Panela",
"Shoeanna",
};

void initGirlfriends()
{
	gData.mGirlfriendAmount = 0;
	int i;
	for (i = 0; i < 10; i++) {
		gData.mGirlfriends[i].mIsActive = 0;
	}

	for (i = 0; i < 20; i++) {
		gData.mGirls[i].mIsUsed = 0;
	}
}


static void loadGirlfriendHandler(void* tCaller) {
	gData.mPortraitSprites = loadMugenSpriteFileWithoutPalette("assets/portraits/PORTRAITS.sff");
	gData.mPortraitAnimations = loadMugenAnimationFile("assets/portraits/PORTRAITS.air");
}

ActorBlueprint GirlfriendHandler = {
	.mLoad = loadGirlfriendHandler,
};

int getNewGirlID()
{
	int id;
	
	int i;
	for (i = 0; i < 300; i++) {
		id = randfromInteger(0, MAX_GIRLS - 1);
		if (!gData.mGirls[id].mIsUsed) break;
	}

	return id;
}

void addNewGirlfriend(int tID)
{
	int index = gData.mGirlfriendAmount;
	gData.mGirlfriends[index].mIsActive = 1;
	gData.mGirlfriends[index].mGirlID = tID;
	gData.mGirlfriends[index].mConversationAmount = 0;
	int i;
	for (i = 0; i < MAX_CONVERSATIONS; i++) {
		gData.mGirlfriends[index].mConversationUsed[i] = 0;
	}

	gData.mGirlfriends[index].mMaxHealth = 5;
	gData.mGirlfriends[index].mHealth = gData.mGirlfriends[index].mMaxHealth;

	gData.mGirls[tID].mIsUsed = 1;

	gData.mGirlfriendAmount++;
}

void removeGirlfriend(int tIndex)
{
	gData.mGirls[gData.mGirlfriends[tIndex].mGirlID].mIsUsed = 0;

	int i;
	for (i = tIndex; i < gData.mGirlfriendAmount - 1; i++) {
		gData.mGirlfriends[i] = gData.mGirlfriends[i + 1];
	}
	gData.mGirlfriendAmount--;

	
	gData.mGirlfriends[gData.mGirlfriendAmount - 1].mIsActive = 0;
}

int hasEnoughGirlfriends()
{
	return gData.mGirlfriendAmount == 5;
}

MugenSpriteFile * getGirlfriendPortraitSprites()
{
	return &gData.mPortraitSprites;
}

MugenAnimations * getGirlfriendPortraitAnimations()
{
	return &gData.mPortraitAnimations;
}

MugenAnimation * getGirlfriendPortraitAnimation(int tIndex)
{
	return getMugenAnimation(&gData.mPortraitAnimations, gData.mGirlfriends[tIndex].mGirlID);
}

void loadSpecificGirlfriend(int tGirlID)
{
	char path[1000];
	sprintf(path, "assets/girls/%s/%s.sff", gGirlNames[tGirlID], gGirlNames[tGirlID]);
	gData.mGirlfriendSprites = loadMugenSpriteFileWithoutPalette(path);

	sprintf(path, "assets/girls/%s/%s.air", gGirlNames[tGirlID], gGirlNames[tGirlID]);
	gData.mGirlfriendAnimations = loadMugenAnimationFile(path);
}

MugenDefScript getGirlfriendConversation(int tGirlfriendID)
{
	int index;
	int i;

	if (gData.mGirlfriends[tGirlfriendID].mConversationAmount >= MAX_CONVERSATIONS) {
		index = randfromInteger(0, MAX_CONVERSATIONS - 1);
	}
	else {
		for (i = 0; i < 100; i++) {
			index = randfromInteger(0, MAX_CONVERSATIONS - 1);
			if (!gData.mGirlfriends[tGirlfriendID].mConversationUsed[index]) break;
		}
	}

	gData.mGirlfriends[tGirlfriendID].mConversationUsed[index] = 1;
	gData.mGirlfriends[tGirlfriendID].mConversationAmount++;

	char path[1000];
	sprintf(path, "assets/girls/%s/conversations/%d.def", gGirlNames[gData.mGirlfriends[tGirlfriendID].mGirlID], index);
	return loadMugenDefScript(path);
}

MugenDefScript getGirlfriendQuestion(int tGirlfriendID)
{
	int index;
	int i;

	for (i = 0; i < 100; i++) {
		index = randfromInteger(0, MAX_CONVERSATIONS - 1);
		if (gData.mGirlfriends[tGirlfriendID].mConversationUsed[index]) break;
	}	

	char path[1000];
	sprintf(path, "assets/girls/%s/questions/%d.def", gGirlNames[gData.mGirlfriends[tGirlfriendID].mGirlID], index);
	return loadMugenDefScript(path);
}

MugenSpriteFile * getGirlfriendSpecificSprites(int tGirlID)
{
	(void)tGirlID;
	return &gData.mGirlfriendSprites;
}

MugenAnimation * getGirlfriendSpecificAnimation(int tGirlID, int tAnimationID)
{
	(void)tGirlID;
	return getMugenAnimation(&gData.mGirlfriendAnimations, tAnimationID);
}

int getGirlfriendAmount()
{
	return gData.mGirlfriendAmount;
}

int getGirlfriendHealth(int tIndex)
{
	return gData.mGirlfriends[tIndex].mHealth;
}

void increaseGirlfriendHealth(int tIndex)
{
	gData.mGirlfriends[tIndex].mHealth = gData.mGirlfriends[tIndex].mMaxHealth;
	updateBarSizeCall(tIndex);
}

void decreaseGirlfriendHealth(int tIndex)
{
	gData.mGirlfriends[tIndex].mHealth = max(0, gData.mGirlfriends[tIndex].mHealth - 2);
	updateBarSizeCall(tIndex);
}

typedef struct {
	int mHealthMaxDelta;
	int mHealthDelta;
	int mExceptID;
} HealthChangeCaller;


void decreaseGirlfriendHealthExcept(int tIndex)
{
	int i;
	for (i = 0; i < gData.mGirlfriendAmount; i++) {
		if (i == tIndex) continue;
		gData.mGirlfriends[i].mHealth--;
	}
}

void increaseGirlfriendMaxHealthExcept(int tIndex)
{
	int i;
	for (i = 0; i < gData.mGirlfriendAmount; i++) {
		if (i == tIndex) continue;
		gData.mGirlfriends[i].mMaxHealth++;
	}
}

void removeDeadGirlfriends() {
	int i;
	for (i = gData.mGirlfriendAmount - 1; i >= 0; i--) {
		if (gData.mGirlfriends[i].mHealth <= 0) {
			removeGirlfriend(i);
		}
	}
}

int getGirlfriendMaxHealth(int tIndex)
{
	return gData.mGirlfriends[tIndex].mMaxHealth;
}

int getGirlIDFromGirlfriendID(int tIndex)
{
	return gData.mGirlfriends[tIndex].mGirlID;
}

void getGirlfriendName(char * tDst, int tIndex)
{
	strcpy(tDst, gGirlNames[gData.mGirlfriends[tIndex].mGirlID]);
}
