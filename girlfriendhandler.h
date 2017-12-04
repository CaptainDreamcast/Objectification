#pragma once

#include <tari/mugenanimationhandler.h>
#include <tari/actorhandler.h>
#include <tari/mugendefreader.h>

void initGirlfriends();
int getNewGirlID();
void addNewGirlfriend(int tID);
void removeGirlfriend(int tIndex);
int hasEnoughGirlfriends();

MugenSpriteFile* getGirlfriendPortraitSprites();
MugenAnimations* getGirlfriendPortraitAnimations();
MugenAnimation* getGirlfriendPortraitAnimation(int tIndex);

MugenDefScript getGirlfriendConversation(int tGirlfriendID);
MugenDefScript getGirlfriendQuestion(int tGirlfriendID);

void loadSpecificGirlfriend(int tGirlID);
MugenDefScript getGirlfriendConversation(int tGirlID);
MugenDefScript getGirlfriendQuestion(int tGirlID);

MugenSpriteFile* getGirlfriendSpecificSprites(int tGirlID);
MugenAnimation* getGirlfriendSpecificAnimation(int tGirlID, int tAnimationID);

int getGirlfriendAmount();
int getGirlfriendHealth(int tIndex);
void increaseGirlfriendHealth(int tIndex);
void decreaseGirlfriendHealth(int tIndex);
void decreaseGirlfriendHealthExcept(int tIndex);
void increaseGirlfriendMaxHealthExcept(int tIndex);
void removeDeadGirlfriends();
int getGirlfriendMaxHealth(int tIndex);
int getGirlIDFromGirlfriendID(int tIndex);
void getGirlfriendName(char* tDst, int tIndex);


extern ActorBlueprint GirlfriendHandler;