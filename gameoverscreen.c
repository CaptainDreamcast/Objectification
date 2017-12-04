#include "gameoverscreen.h"

#include "storyscreen.h"

static void updateGameOverScreen() {
	
	setCurrentStoryDefinitionFile("assets/story/GAMEOVER.def");
	setNewScreen(&StoryScreen);
}

Screen GameOverScreen = {
	.mUpdate = updateGameOverScreen,
};