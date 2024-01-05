#include "stdio.h"
#include "raylib.h"

int main() {
  const int SCREEN_WIDTH = 800;
  const int SCREEN_HEIGHT = 450;

  bool isPaused = false;

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib [core] example - basic window");

  InitAudioDevice();              // Initialize audio device

  Music music = LoadMusicStream("./tori-no-uta.mp3");

  PlayMusicStream(music);
  
  SetTargetFPS(60);

  // Main game loop
  while (!WindowShouldClose()) {

	UpdateMusicStream(music); 

	if (IsKeyPressed(KEY_ESCAPE)) {
	  CloseWindow();
	}

	if (IsKeyPressed(KEY_SPACE)) {
	  if (!isPaused) {
		PauseMusicStream(music);
	  }

	  else { 
		ResumeMusicStream(music);
	  }
	  
	  isPaused = !isPaused;
	}
	
	BeginDrawing();

	ClearBackground(RAYWHITE);

	DrawText("Window", 190, 200, 20, LIGHTGRAY);

	EndDrawing();
  }

  UnloadMusicStream(music);

  CloseAudioDevice();

  CloseWindow();
  
  
  return 0;
}
