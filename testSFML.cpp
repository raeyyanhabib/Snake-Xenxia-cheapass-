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

const int windowWidth = 900;
const int windowHeight = 600;
const int tileSize = 20;
const int snakeSpeedMs = 200;
const int maxSegments = 100;
const int maxObstacles = 50;

SoundBuffer startBuffer;
Sound start;

enum GameMode { modeEndless, modeLevel, modeCustom };
GameMode activeMode;

int currentScore = 0;
int maxScore = 0;

Vector2i snakeSegments[maxSegments];
int      snakeLength = 0;
Vector2i snakeMovement;

Color snakeColorBody = Color::Cyan;
Color snakeColorHead = Color::White;

int foodX, foodY;

RectangleShape obstacleWalls[maxObstacles];
int numObstacles = 0;
int currentLevel = 1;
int customSnakeSpeed = snakeSpeedMs;

// ── Forward declarations ──────────────────────────────────────────────────────
void SpawnFood();
void InitializeSnake();
void ProcessInput();
void UpdateSnake();
bool Collision();
bool hasWallCollision(Vector2i head);
void CreateObstacles();
void RenderGame(RenderWindow& window, Text& scoreDisplay, Text& maxScoreDisplay, Sprite& background);
int  loadHighestScore();
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

// ── Implementations ───────────────────────────────────────────────────────────

void SpawnFood()
{
    while (true)
    {
        foodX = rand() % (windowWidth / tileSize);
        foodY = rand() % (windowHeight / tileSize);

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
            int wallX = static_cast<int>(obstacleWalls[i].getPosition().x / tileSize);
            int wallY = static_cast<int>(obstacleWalls[i].getPosition().y / tileSize);
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

void InitializeSnake()
{
    snakeLength = 2;
    snakeSegments[0] = { 10, 10 };
    snakeSegments[1] = { 9,  10 };
    snakeMovement = { 1,  0 };
}

void ProcessInput()
{
    if (Keyboard::isKeyPressed(Keyboard::W) && snakeMovement.y == 0) snakeMovement = { 0, -1 };
    else if (Keyboard::isKeyPressed(Keyboard::S) && snakeMovement.y == 0) snakeMovement = { 0,  1 };
    else if (Keyboard::isKeyPressed(Keyboard::A) && snakeMovement.x == 0) snakeMovement = { -1,  0 };
    else if (Keyboard::isKeyPressed(Keyboard::D) && snakeMovement.x == 0) snakeMovement = { 1,  0 };

    if (Keyboard::isKeyPressed(Keyboard::Up) && snakeMovement.y == 0) snakeMovement = { 0, -1 };
    else if (Keyboard::isKeyPressed(Keyboard::Down) && snakeMovement.y == 0) snakeMovement = { 0,  1 };
    else if (Keyboard::isKeyPressed(Keyboard::Left) && snakeMovement.x == 0) snakeMovement = { -1,  0 };
    else if (Keyboard::isKeyPressed(Keyboard::Right) && snakeMovement.x == 0) snakeMovement = { 1,  0 };
}

void UpdateSnake()
{
    Vector2i newHead = snakeSegments[0] + snakeMovement;
    newHead.x = (newHead.x + (windowWidth / tileSize)) % (windowWidth / tileSize);
    newHead.y = (newHead.y + (windowHeight / tileSize)) % (windowHeight / tileSize);

    for (int i = snakeLength; i > 0; --i)
        snakeSegments[i] = snakeSegments[i - 1];

    snakeSegments[0] = newHead;

    if (newHead.x == foodX && newHead.y == foodY)
    {
        currentScore += 10;
        maxScore = max(maxScore, currentScore);

        if (snakeLength < maxSegments)
            snakeLength++;

        SpawnFood();
    }
}

bool Collision()
{
    Vector2i head = snakeSegments[0];
    for (int i = 1; i < snakeLength; ++i)
    {
        if (head.x == snakeSegments[i].x && head.y == snakeSegments[i].y)
            return true;
    }
    return false;
}

bool hasWallCollision(Vector2i head)
{
    for (int i = 0; i < numObstacles; ++i)
    {
        int wallX = static_cast<int>(obstacleWalls[i].getPosition().x / tileSize);
        int wallY = static_cast<int>(obstacleWalls[i].getPosition().y / tileSize);
        if (head.x == wallX && head.y == wallY)
            return true;
    }
    return false;
}

void CreateObstacles()
{
    numObstacles = currentLevel * 5 + 3;

    for (int i = 0; i < numObstacles; ++i)
    {
        Vector2i obstaclePosition;

        do
        {
            obstaclePosition =
            {
                rand() % (windowWidth / tileSize),
                rand() % (windowHeight / tileSize)
            };
        } while (
            find(snakeSegments, snakeSegments + snakeLength, obstaclePosition) != snakeSegments + snakeLength ||
            obstaclePosition.x == 0 ||
            obstaclePosition.x == (windowWidth / tileSize - 1) ||
            obstaclePosition.y == 0 ||
            obstaclePosition.y == (windowHeight / tileSize - 1)
            );

        RectangleShape obstacle(Vector2f(tileSize, tileSize));
        obstacle.setFillColor(Color::Red);
        obstacle.setPosition(obstaclePosition.x * tileSize, obstaclePosition.y * tileSize);
        obstacleWalls[i] = obstacle;
    }
}

void RenderGame(RenderWindow& window, Text& scoreDisplay, Text& maxScoreDisplay, Sprite& background)
{
    window.draw(background);

    // Draw snake
    for (int i = 0; i < snakeLength; ++i)
    {
        RectangleShape segment(Vector2f(tileSize, tileSize));
        segment.setFillColor(i == 0 ? snakeColorHead : snakeColorBody);
        segment.setPosition(snakeSegments[i].x * tileSize, snakeSegments[i].y * tileSize);
        window.draw(segment);
    }

    // Draw food
    CircleShape foodCircle(tileSize / 2);
    foodCircle.setFillColor(Color::Magenta);
    foodCircle.setPosition(foodX * tileSize, foodY * tileSize);
    window.draw(foodCircle);

    // Draw scores
    scoreDisplay.setString("Score: " + to_string(currentScore));
    window.draw(scoreDisplay);
    maxScoreDisplay.setString("Highest Score: " + to_string(maxScore));
    window.draw(maxScoreDisplay);

    // Draw obstacles
    for (int i = 0; i < numObstacles; ++i)
        window.draw(obstacleWalls[i]);

    window.display();
}

int loadHighestScore()
{
    ifstream scoreFile("highscore.txt");
    int score = 0;

    if (scoreFile.is_open())
    {
        scoreFile >> score;
        scoreFile.close();
    }
    return score;
}

void saveHighestScore(int score)
{
    ofstream scoreFile("highscore.txt");
    if (scoreFile.is_open())
    {
        scoreFile << score;
        scoreFile.close();
    }
}

void ColorSelection(RenderWindow& window)
{
    Texture menuBackground;
    menuBackground.loadFromFile("menu_background.jpg");
    Sprite background(menuBackground);

    Font font;
    font.loadFromFile("arial.ttf");

    while (true)
    {
        window.clear();
        window.draw(background);

        Text title("Choose Snake Color", font, 50);
        title.setFillColor(Color::White);
        title.setPosition(windowWidth / 2 - title.getGlobalBounds().width / 2, windowHeight / 4);
        window.draw(title);

        Text redOption("Press R for Red", font, 30);
        redOption.setPosition(windowWidth / 2 - redOption.getGlobalBounds().width / 2, windowHeight / 2);
        window.draw(redOption);

        Text greenOption("Press G for Green", font, 30);
        greenOption.setPosition(windowWidth / 2 - greenOption.getGlobalBounds().width / 2, windowHeight / 2 + 50);
        window.draw(greenOption);

        Text blueOption("Press B for Blue", font, 30);
        blueOption.setPosition(windowWidth / 2 - blueOption.getGlobalBounds().width / 2, windowHeight / 2 + 100);
        window.draw(blueOption);

        for (Event event; window.pollEvent(event);)
        {
            if (event.type == Event::Closed)
            {
                window.close();
                return;
            }
            if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::R) { snakeColorBody = Color::Red;   return; }
                if (event.key.code == Keyboard::G) { snakeColorBody = Color::Green; return; }
                if (event.key.code == Keyboard::B) { snakeColorBody = Color::Blue;  return; }
            }
        }
        window.display();
    }
}

void GameModeSelection(RenderWindow& window)
{
    Texture menuBackground;
    menuBackground.loadFromFile("menu_background.jpg");
    Sprite background(menuBackground);

    Font font;
    font.loadFromFile("arial.ttf");

    while (true)
    {
        window.clear();
        window.draw(background);

        Text title("Select Game Mode", font, 50);
        title.setFillColor(Color::White);
        title.setPosition(windowWidth / 2 - title.getGlobalBounds().width / 2, windowHeight / 4);
        window.draw(title);

        Text endlessModeText("Press 1 for Endless Mode", font, 30);
        endlessModeText.setPosition(windowWidth / 2 - endlessModeText.getGlobalBounds().width / 2, windowHeight / 2);
        window.draw(endlessModeText);

        Text levelsModeText("Press 2 for Levels Mode", font, 30);
        levelsModeText.setPosition(windowWidth / 2 - levelsModeText.getGlobalBounds().width / 2, windowHeight / 2 + 50);
        window.draw(levelsModeText);

        Text customModeText("Press 3 for Custom Mode", font, 30);
        customModeText.setPosition(windowWidth / 2 - customModeText.getGlobalBounds().width / 2, windowHeight / 2 + 100);
        window.draw(customModeText);

        for (Event event; window.pollEvent(event);)
        {
            if (event.type == Event::Closed)
            {
                window.close();
                return;
            }
            if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Num1) { activeMode = modeEndless; return; }
                if (event.key.code == Keyboard::Num2) { activeMode = modeLevel;   return; }
                if (event.key.code == Keyboard::Num3) { activeMode = modeCustom;  return; }
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
        for (Event event; window.pollEvent(event);)
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
        sleep(milliseconds(snakeSpeedMs));
    }
}

void LevelsMode(RenderWindow& window)
{
    currentLevel = 1;

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

    while (true)
    {
        CreateObstacles();
        InitializeSnake();
        SpawnFood();

        while (true)
        {
            for (Event event; window.pollEvent(event);)
            {
                if (event.type == Event::Closed)
                {
                    window.close();
                    return;
                }
            }

            ProcessInput();
            UpdateSnake();  // handles food eating, scoring, and growth

            if (Collision() || hasWallCollision(snakeSegments[0]))
            {
                saveHighestScore(maxScore);
                window.close();
                return;
            }

            RenderGame(window, scoreDisplay, maxScoreDisplay, background);

            if (currentScore >= 50 * currentLevel)
            {
                currentLevel++;
                break;
            }

            sleep(milliseconds(snakeSpeedMs));
        }
    }
}

void CreateObstacles(RenderWindow& window)
{
    Font font;
    font.loadFromFile("arial.ttf");

    Texture gameBackground;
    gameBackground.loadFromFile("background.jpg");
    Sprite background(gameBackground);

    while (numObstacles < maxObstacles)
    {
        window.clear();
        window.draw(background);

        Text instruction("Click to place a wall, press Enter to finish", font, 24);
        instruction.setFillColor(Color::White);
        instruction.setPosition(10, 10);
        window.draw(instruction);

        for (int i = 0; i < numObstacles; ++i)
            window.draw(obstacleWalls[i]);

        window.display();

        for (Event event; window.pollEvent(event);)
        {
            if (event.type == Event::Closed)
            {
                window.close();
                return;
            }
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
            {
                Vector2i mousePos = Mouse::getPosition(window);
                RectangleShape obstacle(Vector2f(tileSize, tileSize));
                obstacle.setFillColor(Color::Red);
                obstacle.setPosition(
                    mousePos.x - (mousePos.x % tileSize),
                    mousePos.y - (mousePos.y % tileSize)
                );
                obstacleWalls[numObstacles] = obstacle;
                numObstacles++;
            }
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter)
                return;
        }
    }
}

void SelectSnakeSpeed(RenderWindow& window)
{
    Texture gameBackground;
    gameBackground.loadFromFile("background.jpg");
    Sprite background(gameBackground);

    Font font;
    font.loadFromFile("arial.ttf");

    while (true)
    {
        window.clear();
        window.draw(background);

        Text instruction("Press 1 for Fast (100ms), 2 for Normal (200ms), 3 for Slow (300ms)", font, 24);
        instruction.setFillColor(Color::White);
        instruction.setPosition(10, 10);
        window.draw(instruction);
        window.display();

        for (Event event; window.pollEvent(event);)
        {
            if (event.type == Event::Closed)
            {
                window.close();
                return;
            }
            if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Num1) { customSnakeSpeed = 100; return; }
                if (event.key.code == Keyboard::Num2) { customSnakeSpeed = 200; return; }
                if (event.key.code == Keyboard::Num3) { customSnakeSpeed = 300; return; }
            }
        }
    }
}

void StartGame(RenderWindow& window)
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
        for (Event event; window.pollEvent(event);)
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

bool LoadAssets(RenderWindow& window)
{
    Texture gameBackground;
    gameBackground.loadFromFile("background.jpg");
    return true;
}

void CustomMode(RenderWindow& window)
{
    numObstacles = 0;

    LoadAssets(window);
    CreateObstacles(window);
    SelectSnakeSpeed(window);
    StartGame(window);
}

// ── Entry point ───────────────────────────────────────────────────────────────

int main()
{
    RenderWindow window(VideoMode(windowWidth, windowHeight), "Snake Game");
    window.setFramerateLimit(60);

    startBuffer.loadFromFile("merx.wav");
    start.setBuffer(startBuffer);
    start.play();

    srand(static_cast<unsigned>(time(0)));

    maxScore = loadHighestScore();
    ColorSelection(window);
    GameModeSelection(window);

    if (activeMode == modeEndless)
    {
        EndlessMode(window);
    }
    else if (activeMode == modeLevel)
    {
        LevelsMode(window);
    }
    else if (activeMode == modeCustom)
    {
        CustomMode(window);
    }

    startBuffer.loadFromFile("merx.wav");
    start.setBuffer(startBuffer);
    start.play();

    return 0;
}