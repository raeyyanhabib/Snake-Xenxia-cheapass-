#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <fstream>

using namespace sf;
using namespace std;

const int window_width = 900;
const int window_height = 600;
const int tile_size = 20;
const int snake_speed_ms = 200;
const int max_segments = 100;
const int max_obstacles = 50;

SoundBuffer startBuffer;
Sound start;


enum GameMode { mode_endless, mode_level, mode_custom };
GameMode activeMode;

int currentScore = 0;
int maxScore = 0;
Vector2i snakeSegments[max_segments];
int snakeLength = 0;
Vector2i snakeMovement;
Color snakeColorBody = Color::Cyan;
Color snakeColorHead = Color::White;
int foodX, foodY;
RectangleShape obstacleWalls[max_obstacles];
int numObstacles = 0;
int currentLevel = 1;
int customSnakeSpeed = snake_speed_ms;

void SpawnFood();
void InitializeSnake();
void ProcessInput();
void UpdateSnake();
bool Collision();
bool hasWallCollision(Vector2i head);
void CreateObstacles();
void RenderGame(RenderWindow& window, Text& scoreDisplay, Text& maxScoreDisplay, Sprite& background);
int loadHighestScore();
void saveHighestScore(int score);
void ColorSelection(RenderWindow& window);
void GameModeSelection(RenderWindow& window);
void EndlessMode(RenderWindow& window);
void LevelsMode(RenderWindow& window);

void CreateObstacles(RenderWindow& window);
void SelectSnakeSpeed(RenderWindow& window);
void StartGame(RenderWindow& window);
bool LoadAssets(RenderWindow& window);
void CustomMode(RenderWindow& window);


void SpawnFood() {
    while (true)
    {
        foodX = rand() % (window_width / tile_size);
        foodY = rand() % (window_height / tile_size);

        bool validPosition = true;

        for (int i = 0; i < snakeLength; ++i) 
        {
            if (foodX == snakeSegments[i].x && foodY == snakeSegments[i].y) 
            {
                validPosition = false;
                break;
            }
        }
        for (int i = 0; i < numObstacles; i++) 
        {
            int wallX = static_cast<int>(obstacleWalls[i].getPosition().x / tile_size);
            int wallY = static_cast<int>(obstacleWalls[i].getPosition().y / tile_size);
            if (foodX == wallX && foodY == wallY) 
            {
                validPosition = false;
                break;
            }
        }

        if (validPosition)
         break; 
    }
}

void InitializeSnake() {
    snakeLength = 2;
    snakeSegments[0] = { 10, 10 };
    snakeSegments[1] = { 9, 10 };
    snakeMovement = { 1, 0 };
}

void ProcessInput() {
    if (Keyboard::isKeyPressed(Keyboard::W) && snakeMovement.y == 0)
        snakeMovement = { 0, -1 };
    else if (Keyboard::isKeyPressed(Keyboard::S) && snakeMovement.y == 0)
        snakeMovement = { 0, 1 };
    else if (Keyboard::isKeyPressed(Keyboard::A) && snakeMovement.x == 0)
        snakeMovement = { -1, 0 };
    else if (Keyboard::isKeyPressed(Keyboard::D) && snakeMovement.x == 0)
        snakeMovement = { 1, 0 };

    if (Keyboard::isKeyPressed(Keyboard::Up) && snakeMovement.y == 0)
        snakeMovement = { 0, -1 };
    else if (Keyboard::isKeyPressed(Keyboard::Down) && snakeMovement.y == 0)
        snakeMovement = { 0, 1 };
    else if (Keyboard::isKeyPressed(Keyboard::Left) && snakeMovement.x == 0)
        snakeMovement = { -1, 0 };
    else if (Keyboard::isKeyPressed(Keyboard::Right) && snakeMovement.x == 0)
        snakeMovement = { 1, 0 };
}

void UpdateSnake() {
    Vector2i newHead = snakeSegments[0] + snakeMovement;
    newHead.x = (newHead.x + (window_width / tile_size)) % (window_width / tile_size);
    newHead.y = (newHead.y + (window_height / tile_size)) % (window_height / tile_size);

    for (int i = snakeLength; i > 0; --i)
        snakeSegments[i] = snakeSegments[i - 1];
    
    snakeSegments[0] = newHead;

    if (newHead.x == foodX && newHead.y == foodY)
    {
        SpawnFood();

        currentScore += 10;
        maxScore = max(maxScore, currentScore);

        if (snakeLength < max_segments) 
            snakeLength++;
        
    }
}

bool Collision() {

    Vector2i head = snakeSegments[0];
    for (int i = 1; i < snakeLength; ++i)
    {
        if (head.x == snakeSegments[i].x && head.y == snakeSegments[i].y) 
            return true;
    }
    return false;
}

bool hasWallCollision(Vector2i head) {
    for (int i = 0; i < numObstacles; ++i) 
    {
        int wallX = static_cast<int>(obstacleWalls[i].getPosition().x / tile_size);
        int wallY = static_cast<int>(obstacleWalls[i].getPosition().y / tile_size);
        if (head.x == wallX && head.y == wallY)
            return true;
        
    }
    return false;
}

void CreateObstacles() {
    numObstacles = currentLevel * 5 + 3;

    for (int i = 0; i < numObstacles; ++i) 
    {
        Vector2i obstaclePosition;

        do
        {
            obstaclePosition = { rand() % (window_width / tile_size), rand() % (window_height / tile_size) };
        } 
        while (find(snakeSegments, snakeSegments + snakeLength, obstaclePosition) != snakeSegments + snakeLength ||
            (obstaclePosition.x == 0 || obstaclePosition.x == (window_width / tile_size - 1) ||
                obstaclePosition.y == 0 || obstaclePosition.y == (window_height / tile_size - 1)));

        RectangleShape obstacle(Vector2f(tile_size, tile_size));
        obstacle.setFillColor(Color::Red);
        obstacle.setPosition(obstaclePosition.x * tile_size, obstaclePosition.y * tile_size);
        obstacleWalls[i] = obstacle;
    }
}

void RenderGame(RenderWindow& window, Text& scoreDisplay, Text& maxScoreDisplay, Sprite& background) {

    window.draw(background);
    for (int i = 0; i < snakeLength; ++i) //Snake display
    {
        RectangleShape segment(Vector2f(tile_size, tile_size));
        segment.setFillColor(i == 0 ? snakeColorHead : snakeColorBody);
        segment.setPosition(snakeSegments[i].x * tile_size, snakeSegments[i].y * tile_size);
        window.draw(segment);
    }

    CircleShape foodCircle(tile_size / 2);//Food
    foodCircle.setFillColor(Color::Magenta);
    foodCircle.setPosition(foodX * tile_size, foodY * tile_size);
    window.draw(foodCircle);

    scoreDisplay.setString("Score: " + to_string(currentScore));//Score
    window.draw(scoreDisplay);
    maxScoreDisplay.setString("Highest Score: " + to_string(maxScore));
    window.draw(maxScoreDisplay);

    for (int i = 0; i < numObstacles; ++i) //Obstacles
        window.draw(obstacleWalls[i]);
    

    window.display();
}

int loadHighestScore() {
    ifstream scoreFile("highscore.txt");
    int score = 0;

    if (scoreFile.is_open()) {
        scoreFile >> score;
        scoreFile.close();
    }
    return score;
}

void saveHighestScore(int score) {
    ofstream scoreFile("highscore.txt");
    if (scoreFile.is_open()) {
        scoreFile << score;
        scoreFile.close();
    }
}

void ColorSelection(RenderWindow& window) {

    Texture menuBackground;
    menuBackground.loadFromFile("menu_background.jpg");
    Sprite background(menuBackground);

    while (true) 
    {
        window.clear();
        window.draw(background);

        Font font;
        font.loadFromFile("arial.ttf");

        Text title("Choose Snake Color", font, 50);
        title.setFillColor(Color::White);
        title.setPosition(window_width / 2 - title.getGlobalBounds().width / 2, window_height / 4);
        window.draw(title);

        Text redOption("Press R for Red", font, 30);
        redOption.setPosition(window_width / 2 - redOption.getGlobalBounds().width / 2, window_height / 2);
        window.draw(redOption);

        Text greenOption("Press G for Green", font, 30);
        greenOption.setPosition(window_width / 2 - greenOption.getGlobalBounds().width / 2, window_height / 2 + 50);
        window.draw(greenOption);

        Text blueOption("Press B for Blue", font, 30);
        blueOption.setPosition(window_width / 2 - blueOption.getGlobalBounds().width / 2, window_height / 2 + 100);
        window.draw(blueOption);

        for (Event event; window.pollEvent(event);) {
            if (event.type == Event::Closed) {
                window.close();
            }
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::R) {
                    snakeColorBody = Color::Red;
                    return;
                }
                if (event.key.code == Keyboard::G) {
                    snakeColorBody = Color::Green;
                    return;
                }
                if (event.key.code == Keyboard::B) {
                    snakeColorBody = Color::Blue;
                    return;
                }
            }
        }
        window.display();
    }
}

void GameModeSelection(RenderWindow& window) {

    Texture menuBackground;
    menuBackground.loadFromFile("menu_background.jpg");
    Sprite background(menuBackground);

    while (true) 
    {
        window.clear();
        window.draw(background);

        Font font;
        font.loadFromFile("arial.ttf");

        Text title("Select Game Mode", font, 50);
        title.setFillColor(Color::White);
        title.setPosition(window_width / 2 - title.getGlobalBounds().width / 2, window_height / 4);
        window.draw(title);

        Text endlessMode("Press 1 for Endless Mode", font, 30);
        endlessMode.setPosition(window_width / 2 - endlessMode.getGlobalBounds().width / 2, window_height / 2);
        window.draw(endlessMode);

        Text levelsMode("Press 2 for Levels Mode", font, 30);
        levelsMode.setPosition(window_width / 2 - levelsMode.getGlobalBounds().width / 2, window_height / 2 + 50);
        window.draw(levelsMode);

        Text customMode("Press 3 for Custom Mode", font, 30);
        customMode.setPosition(window_width / 2 - customMode.getGlobalBounds().width / 2, window_height / 2 + 100);
        window.draw(customMode);

        for (Event event; window.pollEvent(event);)
        {
            if (event.type == Event::Closed)
                window.close();
            
            if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Num1) {
                    activeMode = mode_endless;
                    return;
                }
                if (event.key.code == Keyboard::Num2) {
                    activeMode = mode_level;
                    return;
                }
                if (event.key.code == Keyboard::Num3) {
                    activeMode = mode_custom;
                    return;
                }
            }
        }
        window.display();
    }
}

void EndlessMode(RenderWindow& window)
{
    InitializeSnake();
    SpawnFood();

    Texture gameBackground;
    gameBackground.loadFromFile("background.jpg");
    Sprite background(gameBackground);

    Font font;
    font.loadFromFile("arial.ttf");

    Text scoreDisplay;
    scoreDisplay.setFont(font);
    scoreDisplay.setCharacterSize(24);
    scoreDisplay.setFillColor(Color::White);
    scoreDisplay.setPosition(10, 10);

    Text maxScoreDisplay;
    maxScoreDisplay.setFont(font);
    maxScoreDisplay.setCharacterSize(24);
    maxScoreDisplay.setFillColor(Color::White);
    maxScoreDisplay.setPosition(10, 40);


    while (window.isOpen()) 
    {
        Event event;

        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
                return;
            }
        }

        ProcessInput();
        UpdateSnake();

        if (Collision())
        {
            saveHighestScore(maxScore);
            window.close();
            return;
        }

        RenderGame(window, scoreDisplay, maxScoreDisplay, background);
        sleep(milliseconds(snake_speed_ms));
    }
}

void LevelsMode(RenderWindow& window) {
    currentLevel = 1;

    Texture gameBackground;
    gameBackground.loadFromFile("background.jpg");
    Sprite background(gameBackground);

    while (true)
    {
        CreateObstacles();
        InitializeSnake();
        SpawnFood();

        Font font;
        font.loadFromFile("arial.ttf");

        Text scoreDisplay;
        scoreDisplay.setFont(font);
        scoreDisplay.setCharacterSize(24);
        scoreDisplay.setFillColor(Color::White);
        scoreDisplay.setPosition(10, 10);

        Text maxScoreDisplay;
        maxScoreDisplay.setFont(font);
        maxScoreDisplay.setCharacterSize(24);
        maxScoreDisplay.setFillColor(Color::White);
        maxScoreDisplay.setPosition(10, 40);

        while (true)
        {
            Event event;
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed) {
                    window.close();
                    return;
                }
            }

            ProcessInput();
            UpdateSnake();

            if (Collision() || hasWallCollision(snakeSegments[0]))
            {
                window.close();
                return;
            }

            if (snakeSegments[0].x == foodX && snakeSegments[0].y == foodY)
            {
                currentScore += 10;
                SpawnFood();
            }

            RenderGame(window, scoreDisplay, maxScoreDisplay, background);

            if (currentScore >= 50 * currentLevel) 
            {
                currentLevel++;
                break;
            }
            sleep(milliseconds(snake_speed_ms));
        }
    }
}

void CreateObstacles(RenderWindow& window) {
    while (numObstacles < max_obstacles)
    {
        window.clear();
        Texture gameBackground;
        gameBackground.loadFromFile("background.jpg");
        Sprite background(gameBackground);
        window.draw(background);

        Font font;
        font.loadFromFile("arial.ttf");
        Text instruction("Click to place a wall, press Enter to finish", font, 24);
        instruction.setFillColor(Color::White);
        instruction.setPosition(10, 10);
        window.draw(instruction);

        for (int i = 0; i < numObstacles; ++i)
            window.draw(obstacleWalls[i]);
        

        window.display();

        Event event;

        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
                return;
            }

            if (event.type == Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == Mouse::Left) {

                    Vector2i mousePos = Mouse::getPosition(window);

                    if (numObstacles < max_obstacles)
                    {
                        RectangleShape obstacle(Vector2f(tile_size, tile_size));
                        obstacle.setFillColor(Color::Red);
                        obstacle.setPosition(mousePos.x - (mousePos.x % tile_size), mousePos.y - (mousePos.y % tile_size));
                        obstacleWalls[numObstacles] = obstacle;
                        numObstacles++;
                    }
                }
            }
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter)
                return;
        }
       
        
    }
}

void SelectSnakeSpeed(RenderWindow& window) {

    while (true) 
    {
        window.clear();
        Texture gameBackground;
        gameBackground.loadFromFile("background.jpg");
        Sprite background(gameBackground);
        window.draw(background);

        Font font;
        font.loadFromFile("arial.ttf");
        Text instruction("Press 1 for Fast (100ms), 2 for Normal (200ms), 3 for Slow (300ms)", font, 24);
        instruction.setFillColor(Color::White);
        instruction.setPosition(10, 10);
        window.draw(instruction);
        window.display();

        Event event;

        while (window.pollEvent(event)) 
        {
            if (event.type == Event::Closed) 
            {
                window.close();
                return;
            }
            if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Num1)
                {
                    customSnakeSpeed = 100; 
                    return;
                }
                if (event.key.code == Keyboard::Num2)
                {
                    customSnakeSpeed = 200; 
                    return;
                }
                if (event.key.code == Keyboard::Num3)
                {
                    customSnakeSpeed = 300; 
                    return;
                }
            }
        }
    }
}

void StartGame(RenderWindow& window) {

    InitializeSnake();
    SpawnFood();

    Texture gameBackground;
    gameBackground.loadFromFile("background.jpg");
    Sprite background(gameBackground);
    Font font;
    font.loadFromFile("arial.ttf");

    Text scoreDisplay;
    scoreDisplay.setFont(font);
    scoreDisplay.setCharacterSize(24);
    scoreDisplay.setFillColor(Color::White);
    scoreDisplay.setPosition(10, 10);

    Text maxScoreDisplay;
    maxScoreDisplay.setFont(font);
    maxScoreDisplay.setCharacterSize(24);
    maxScoreDisplay.setFillColor(Color::White);
    maxScoreDisplay.setPosition(10, 40);

    while (window.isOpen()) 
    {
        Event event;

        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
                return;
            }
        }

        ProcessInput();
        UpdateSnake();

        if (Collision() || hasWallCollision(snakeSegments[0])) 
        {
            saveHighestScore(maxScore);
            window.close();
            return;
        }

        RenderGame(window, scoreDisplay, maxScoreDisplay, background);
        sleep(milliseconds(customSnakeSpeed));
    }
}

bool LoadAssets(RenderWindow& window) {
    
    Texture gameBackground;
    gameBackground.loadFromFile("background.jpg");
    return true; 
}

void CustomMode(RenderWindow& window) {
    numObstacles = 0;

    LoadAssets(window);
    CreateObstacles(window); 
    SelectSnakeSpeed(window); 
    StartGame(window);
}


int main() {
    RenderWindow window(VideoMode(window_width, window_height), "Snake Game");
    window.setFramerateLimit(60);

    startBuffer.loadFromFile("merx.wav");
    start.setBuffer(startBuffer);
    start.play();

    srand(static_cast<unsigned>(time(0)));

    maxScore = loadHighestScore();
    ColorSelection(window);
    GameModeSelection(window);

    if (activeMode == mode_endless)
    {
        EndlessMode(window);
        startBuffer.loadFromFile("merx.wav");
        start.setBuffer(startBuffer);
        start.play();
    }
    else if (activeMode == mode_level)
    {
        LevelsMode(window);
        startBuffer.loadFromFile("merx.wav");
        start.setBuffer(startBuffer);
        start.play();
    }
    else if (activeMode == mode_custom) 
    {
        CustomMode(window);
        startBuffer.loadFromFile("merx.wav");
        start.setBuffer(startBuffer);
        start.play(); 
    }

    return 0;
}