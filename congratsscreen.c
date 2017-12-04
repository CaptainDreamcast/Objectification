#include "congratsscreen.h"

#include "storyscreen.h"

static void updateGameOverScreen() {

	setCurrentStoryDefinitionFile("assets/story/CONGRATS.def");
	setNewScreen(&StoryScreen);
}

Screen CongratsScreen = {
	.mUpdate = updateGameOverScreen,
};