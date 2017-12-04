#include "backgroundhandler.h"

#include <tari/mugenanimationhandler.h>
#include <tari/math.h>

static struct {
	MugenSpriteFile mSprites;

} gData;

static void loadBackgroundHandler(void* tCaller) {
	int amount = 3;
	int index = randfromInteger(0, amount-1);

	char path[1000];
	sprintf(path, "assets/bg/%d.sff", index);
	gData.mSprites = loadMugenSpriteFileWithoutPalette(path);
	addMugenAnimation(createOneFrameMugenAnimationForSprite(1, 1), &gData.mSprites, makePosition(0, 0, 1));
}

ActorBlueprint BackgroundHandler = {
	.mLoad = loadBackgroundHandler,
};