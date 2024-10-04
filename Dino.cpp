#include <chrono>
#include <cmath>
#include <raylib.h>
#include <cstdlib>
#include <ctime>
#include <chrono>


enum CactusType {
	TYPE1, 
	TYPE2, 
	TYPE3
}; 
CactusType getRandomCactus() {
	return static_cast<CactusType>(rand() % 3); 
}

class Dino{
	public: 
		Vector2 position; 
		Vector2 DinoSize; 
		float JumpHeight; 
		bool is_ground; 
	Dino(Vector2 DinoPosition, Vector2 DinoSize, float JumpHeight, int is_ground) {
		this->position = DinoPosition; 
		this->DinoSize = DinoSize; 
		this->JumpHeight = JumpHeight;  
		this->is_ground = is_ground; 
	}
	
	void DinoJump() {
		if (IsKeyDown(KEY_SPACE)) {
			position.y -= JumpHeight; 	
			is_ground = false; 
		}
		if (!is_ground) {
		    position.y += 4.0f; 

		    if (position.y >= 250.0f) { 
			position.y = 250.0f; 
			is_ground = true; 
		    }
		} 
	}
	void Draw() {
		DrawRectangleV(position, DinoSize, BLACK); 
	}
	Rectangle GetRectangle() {
		return {position.x, position.y, DinoSize.x, DinoSize.y}; 
	} 

}; 

class Cactus{
	public: 
		Vector2 position; 
		Vector2 CactusSize; 
		float MoveSpeed;

	Cactus(Vector2 CactusPosition, Vector2 CacSize, float Speed) {
		this->position = CactusPosition; 
		this->CactusSize = CacSize; 
		this->MoveSpeed = Speed;  
	}
	void update() {
		position.x -= MoveSpeed * GetFrameTime() ; 
		if (position.x <= 0) {
			position.x = GetScreenWidth(); 
			CactusType cactusType = getRandomCactus(); 
			if (cactusType == TYPE1) {
				CactusSize.x = 20.0; 
				CactusSize.y = 50.0;
			}
			if (cactusType == TYPE2) {
				CactusSize.x = 50.0; 
				CactusSize.y = 50.0;
			}
			if (cactusType == TYPE3) {
				CactusSize.x = 80.0; 
				CactusSize.y = 50.0;
			}
		}
	}
	bool CheckCollision(const Rectangle& rectangle) {
		if (position.x < rectangle.x + rectangle.width && position.x + CactusSize.x > rectangle.x &&
		    position.y < rectangle.y + rectangle.height && position.y + CactusSize.y > rectangle.y) {
			return true; 
		}
		return false; 
	}
	void Draw() {
		DrawRectangleV(position, CactusSize, DARKGREEN); 
	}
};  
int main() {
	
	const int window_width = 800; 
	const int window_height = 450; 
	
	InitWindow(window_width, window_height,"Google-Dino"); 
	SetTargetFPS(60);

	srand(static_cast<unsigned int>(time(0)));
	auto StartTimer = std::chrono::high_resolution_clock::now(); 
	int playerScore = 0; 	
	bool GameOver = false; 

	Vector2 DinoPosition = {100.0, 250.0}; 
	Vector2 DinoSize = {50.0, 50.0}; 
	Dino dino(DinoPosition, DinoSize, 40.0, true);  

	Vector2 CactusPosition = {800.0, 250.0}; 
	Vector2 CactusSize = {10.0, 50.0}; 
	Cactus cactus(CactusPosition, CactusSize, 500.0f);

	while (!WindowShouldClose()) {
		if (!GameOver) {
			auto currentTime = std::chrono::high_resolution_clock::now(); 	
			std::chrono::duration<float> lapTime = currentTime - StartTimer; 
			playerScore = static_cast<int>(lapTime.count() * 10); 

			dino.DinoJump(); 
			cactus.update(); 
		}

		if (cactus.CheckCollision(dino.GetRectangle())) {
			GameOver = true; 
		}	

		BeginDrawing(); 
			
			ClearBackground(RAYWHITE); 
			if (!GameOver) {	
				DrawRectangle(0, 300, GetScreenWidth(), 50, PINK); 
								
				DrawText(TextFormat("SCORE: %i", playerScore), 500, 0, 30, BLACK);	
				cactus.Draw(); 
				dino.Draw(); 
			}
			else {
				DrawText(TextFormat("HI-SCORE: %i", playerScore), 210, 200, 50, BLACK);	
			}

		EndDrawing(); 
	}
	CloseWindow(); 	
	return 0; 
}
