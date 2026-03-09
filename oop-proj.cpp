//#include <SFML/Graphics.hpp>
//#include <SFML/Audio.hpp>
//#include <vector>
//#include <iostream>
//#include <cstdlib>
//#include <ctime>
//
//using namespace sf;
//using namespace std;
//
//class Entity // base abstract class
//{
//protected:
//    RectangleShape shape; // SFML Functionality
//public:
//    virtual void draw(RenderWindow& window) = 0;
//
//    virtual FloatRect getBounds() const
//    {
//        return shape.getGlobalBounds();
//    }
//    Vector2f getSize() const
//    {
//        return shape.getSize();
//    }
//    Vector2f getPosition() const {
//        return shape.getPosition();
//    }
//};
//
//class Wall : public Entity
//{
//public:
//    Wall(Vector2f pos, Vector2f size = { 30.f, 30.f })
//    {
//        shape.setSize(size);
//        shape.setFillColor(Color::Black);
//        shape.setOutlineColor(Color::Cyan);
//        shape.setOutlineThickness(2.f);
//        shape.setPosition(pos);
//    }
//
//    void update(float) {}
//
//    void setColor(Color color)
//    {         ///////////
//        shape.setOutlineColor(color);
//    }
//    void draw(RenderWindow& window) override
//    {
//        window.draw(shape);
//    }
//};
//
//class Player : public Entity
//{
//private:
//    float speed;
//    Vector2f currentDirection;
//    Vector2f desiredDirection;
//public:
//    Player(Vector2f pos, float speed = 100.f) : speed(speed), currentDirection(0.f, 0.f), desiredDirection(0.f, 0.f)
//    {
//        shape.setSize({ 20.f, 20.f });
//        shape.setFillColor(Color::Yellow);
//        shape.setPosition(pos);
//    }
//    void handleInput()
//    {
//        if (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up)) // real-time access of keyboard being pressed 
//        {
//            desiredDirection = { 0.f, -1.f };
//        }
//        else if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left))
//        {
//            desiredDirection = { -1.f, 0.f };
//        }
//        else if (Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down))
//        {
//            desiredDirection = { 0.f, 1.f };
//        }
//        else if (Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right))
//        {
//            desiredDirection = { 1.f, 0.f };
//        }
//    }
//
//    void setClickedDirection(Vector2f dir) //mouse and touch screen controlled
//    { 
//        desiredDirection = dir;
//    }
//
//    void update(float deltaTime, const vector<Wall>& walls)
//    {
//        Vector2f oldPos = shape.getPosition();
//        if (canMove(desiredDirection, walls, deltaTime))
//        {
//            currentDirection = desiredDirection;
//        }
//
//        Vector2f movement = currentDirection * speed * deltaTime;
//
//        shape.move(movement);
//
//        for (const auto& wall : walls)
//        {
//            if (shape.getGlobalBounds().intersects(wall.getBounds()))
//            {
//                shape.setPosition(oldPos);
//                currentDirection = { 0.f, 0.f };
//                break;
//            }
//        }
//
//        Vector2f pos = shape.getPosition();
//        float width = shape.getSize().x;
//        float screenWidth = 800.f;
//
//        if (pos.x + width < 0.f)
//        {
//            shape.setPosition(screenWidth, pos.y);
//        }
//        else if (pos.x > screenWidth)
//        {
//            shape.setPosition(-width, pos.y);
//        }
//    }
//    bool canMove(Vector2f dir, const vector<Wall>& walls, float deltaTime)
//    {
//        if (dir == Vector2f(0.f, 0.f)) return false;
//
//        Vector2f testMove = dir * speed * deltaTime;
//        shape.move(testMove);
//        FloatRect bounds = shape.getGlobalBounds();
//        shape.move(-testMove);
//
//        for (const auto& wall : walls)
//        {
//            if (bounds.intersects(wall.getBounds()))
//                return false;
//        }
//        return true;
//    }
//    void draw(RenderWindow& window) override
//    {
//        window.draw(shape);
//    }
//    Vector2f getPosition() const
//    {
//        return shape.getPosition();
//    }
//};
//
//class Food : public Entity
//{
//public:
//    Food(Vector2f pos)
//    {
//        shape.setSize({ 8.f, 8.f });
//        shape.setFillColor(Color::White);
//        shape.setPosition(pos); // center the food in tile
//    }
//
//    void draw(RenderWindow& window) override
//    {
//        window.draw(shape);
//    }
//};
//
//class Ghost : public Entity // ghost wooooooooooooooooooooooo
//{
//private:
//    float speed;
//    Vector2f currentDirection;
//    Vector2f spawnPosition;
//    float decisionCooldown;
//    static constexpr float DECISION_INTERVAL = 0.25f;
//    bool frightened;
//    float frightenedTimer;
//    Color originalColor;
//    static constexpr float FRIGHTENED_DURATION = 5.f;
//public:
//    Ghost(Vector2f pos, float speed, Color col) : currentDirection(0.f, -1.f), speed(speed), spawnPosition(pos), decisionCooldown(DECISION_INTERVAL), frightened(false), frightenedTimer(0.f), originalColor(col)
//    {
//        shape.setSize({ 22.f, 22.f });
//        shape.setFillColor(col);
//        shape.setPosition(pos);
//    }
//    void enterFrightenedMode() {
//        frightened = true;
//        frightenedTimer = FRIGHTENED_DURATION;
//        shape.setFillColor(Color::Blue);
//        // Optional: Reverse direction immediately
//        // if (currentDirection != Vector2f(0.f, 0.f) && canMove(-currentDirection, walls_ref, some_delta_time_ref)) {
//        //    currentDirection = -currentDirection;
//        // }
//        decisionCooldown = 0;
//    }
//    bool isFrightened() const 
//    {
//        return frightened;
//    }
//    void getEaten() {
//        reset();
//        // score += 50;
//    }
//    Vector2f chooseRandomAllowedDirection(const vector<Wall>walls, float deltaTime) 
//    {
//        vector<Vector2f> validMoves;
//        const Vector2f directions[] = { {0.f,-1.f}, {0.f, 1.f}, {-1.f,0.f}, {1.f,0.f} };
//        for (const auto& dir : directions) 
//        {
//            if (canMove(dir, walls, deltaTime)) 
//            {
//                validMoves.push_back(dir);
//            }
//        }
//        if (validMoves.empty()) 
//        {
//            return { 0.f, 0.f };
//        }
//        if (validMoves.size() > 1 && currentDirection != Vector2f(0.f, 0.f))
//        {
//            vector<Vector2f> nonReversingMoves;
//            for (const auto& move : validMoves)
//            {
//                if (move != -currentDirection)
//                {
//                    nonReversingMoves.push_back(move);
//                }
//            }
//            if (!nonReversingMoves.empty())
//            {
//                return nonReversingMoves[rand() % nonReversingMoves.size()];
//            }
//        }
//        return validMoves[rand() % validMoves.size()];
//    }
//    void draw(RenderWindow& window) override
//    {
//        window.draw(shape);
//    }
//    bool canMove(Vector2f dir, const vector<Wall>& walls, float deltaTime) const
//    {
//        if (dir == Vector2f(0.f, 0.f)) return true;
//        RectangleShape testShape = shape;
//        Vector2f testMove = dir * speed * deltaTime;
//        testShape.move(testMove);
//        FloatRect bounds = testShape.getGlobalBounds();
//        for (const auto& wall : walls)
//        {
//            if (bounds.intersects(wall.getBounds()))
//            {
//                return false;
//            }
//        }
//        return true;
//    }
//    Vector2f chooseBestDirection(const vector<Wall>& walls, const Player& player, float deltaTime)
//    {
//        Vector2f bestDir = { 0.f, 0.f };
//        float minDistanceSq = -1.f;
//        Vector2f ghostCenter = getPosition() + getSize() / 2.f;
//        Vector2f playerCenter = player.getPosition() + player.getSize() / 2.f;
//        const Vector2f directionsToTry[] = {
//            {0.f, -1.f},
//            {-1.f, 0.f},
//            {0.f, 1.f},
//            {1.f, 0.f}
//        };
//        vector<Vector2f> allowedActualMoves;
//        for (const auto& dir : directionsToTry)
//        {
//            if (currentDirection != Vector2f(0.f, 0.f) &&
//                dir.x == -currentDirection.x && dir.y == -currentDirection.y)
//            {
//                continue;
//            }
//            if (this->canMove(dir, walls, deltaTime))
//            {
//                allowedActualMoves.push_back(dir);
//            }
//        }
//        if (allowedActualMoves.empty() && currentDirection != Vector2f(0.f, 0.f))
//        {
//            Vector2f reverseDir = -currentDirection;
//            if (this->canMove(reverseDir, walls, deltaTime))
//            {
//                allowedActualMoves.push_back(reverseDir);
//            }
//        }
//
//        if (allowedActualMoves.empty()) return { 0.f, 0.f };
//        for (const auto& prioritizedDir : directionsToTry)
//        {
//            bool isThisDirectionAllowed = false;
//            for (const auto& allowedMove : allowedActualMoves)
//            {
//                if (allowedMove == prioritizedDir)
//                {
//                    isThisDirectionAllowed = true;
//                    break;
//                }
//            }
//
//            if (!isThisDirectionAllowed) continue;
//            Vector2f projectedPos = ghostCenter + prioritizedDir * speed * deltaTime;
//            float dx = playerCenter.x - projectedPos.x;
//            float dy = playerCenter.y - projectedPos.y;
//            float currentDistanceSq = dx * dx + dy * dy;
//            if (bestDir == Vector2f(0.f, 0.f) || currentDistanceSq < minDistanceSq)
//            {
//                minDistanceSq = currentDistanceSq;
//                bestDir = prioritizedDir;
//            }
//        }
//        return bestDir;
//    }
//    void update(float deltaTime, const vector<Wall>& walls, const Player& player) {
//        if (frightened) {
//            frightenedTimer -= deltaTime;
//            if (frightenedTimer <= 0.f) {
//                frightened = false;
//                shape.setFillColor(originalColor);
//            }
//        }
//
//        decisionCooldown -= deltaTime;
//        Vector2f oldPos = getPosition();
//        bool shouldChangeDirection = false;
//
//        if (currentDirection == Vector2f(0.f, 0.f)) {
//            shouldChangeDirection = true;
//        }
//        else if (!this->canMove(currentDirection, walls, deltaTime)) {
//            shouldChangeDirection = true;
//        }
//        else if (decisionCooldown <= 0.f) {
//            // Check for junctions
//            int openPaths = 0;
//            const Vector2f directionsToTest[] = { {0.f,-1.f},{-1.f,0.f},{0.f,1.f},{1.f,0.f} };
//            for (const auto& testDir : directionsToTest) {
//                // Don't count reversing as an "open path" for decision making unless it's the only way
//                if (currentDirection != Vector2f(0.f, 0.f) && testDir.x == -currentDirection.x && testDir.y == -currentDirection.y) continue;
//                if (this->canMove(testDir, walls, deltaTime)) openPaths++;
//            }
//
//            if (frightened) {
//                // Frightened ghosts make decisions more often / randomly at any junction
//                if (openPaths > 0) { // If there's any alternative path (not just continuing straight)
//                    shouldChangeDirection = true;
//                }
//            }
//            else {
//                // Normal ghosts decide at significant junctions (more than one alternative)
//                if (openPaths > 0) { // Original logic: if (openPaths > 1)
//                    // Changed to > 0 to make them choose target even if only one other path
//                    shouldChangeDirection = true;
//                }
//            }
//        }
//
//        if (shouldChangeDirection) {
//            Vector2f newDirection;
//            if (frightened) {
//                newDirection = chooseRandomAllowedDirection(walls, deltaTime);
//            }
//            else {
//                newDirection = chooseBestDirection(walls, player, deltaTime);
//            }
//
//            if (newDirection != Vector2f(0.f, 0.f)) {
//                currentDirection = newDirection;
//            }
//            else {
//                // If no new direction found (e.g. stuck), and current is blocked, stop
//                if (!this->canMove(currentDirection, walls, deltaTime)) {
//                    currentDirection = { 0.f, 0.f };
//                }
//            }
//            decisionCooldown = DECISION_INTERVAL;
//        }
//
//        if (currentDirection != Vector2f(0.f, 0.f)) 
//        {
//            Vector2f movement = currentDirection * speed * deltaTime;
//            shape.move(movement);
//            for (const auto& wall : walls) {
//                if (getBounds().intersects(wall.getBounds()))
//                {
//                    shape.setPosition(oldPos);
//                    currentDirection = { 0.f, 0.f }; // Stop if bonked
//                    decisionCooldown = 0.f; // Force re-evaluation
//                    break;
//                }
//            }
//        }
//        // Screen wrapping for ghosts (optional, Pac-Man ghosts don't typically wrap)
//        /*
//        Vector2f pos = shape.getPosition();
//        float width = shape.getSize().x;
//        float screenWidth = 800.f; // Assuming same screen width as player
//
//        if (pos.x + width < 0.f) {
//            shape.setPosition(screenWidth, pos.y);
//        } else if (pos.x > screenWidth) {
//            shape.setPosition(-width, pos.y);
//        }
//        */
//    }
//    void reset() // Make sure to add virtual in Entity if not already, or use this as specific reset
//    {
//        shape.setPosition(spawnPosition);
//        currentDirection = { 0.f, -1.f }; // Or a default spawn direction
//        decisionCooldown = DECISION_INTERVAL;
//        frightened = false;
//        frightenedTimer = 0.f;
//        if (shape.getFillColor() != originalColor) { // Reset color if it was changed (e.g. to blue)
//            shape.setFillColor(originalColor);
//        }
//    }
//
//};
//
//
//class Energizer : public Entity // power-up wala khana, wohoooooooooooooo
//{
//public:
//    Energizer(Vector2f pos)
//    {
//        shape.setSize({ 12.f,12.f });
//        shape.setFillColor(Color::Magenta);
//        shape.setPosition(pos);
//    }
//    void draw(RenderWindow& window) override
//    {
//        window.draw(shape);
//    }
//};
//
//class GameMap
//{
//private:
//    vector<Wall> walls;
//    vector<Food> foodPellets;
//    const float tileSize = 30.f;
//    int currentMapIndex = 0;
//    vector<Energizer> energizers;
//    vector<vector<string>> map_layout = 
//    {
//    {
//        "OOOOOOOOOOOOOOOOOOOO",
//        "O        OO         O",
//        "O OO OOO OO OOO OO O",
//        "OE                EO",
//        "O OO   OOOOO    OO O",
//        "O                  O",
//        "OOO              OOO",
//        "    O  OOOOO   O    ",
//        "O           O  OOOOO",
//        "O      O     E     O",
//        "O OO   OOO OOO  OO O",
//        "O  O            O  O",
//        "OO O O  OOOOO O O OO",
//        "O    O    O   O    O",
//        "O OEOOOO  O OOOOOO O",
//        "O         E        O",
//        "OOOOOOOOOOOOOOOOOOOO"
//    }, {
//        "OOOOOOOOOOOOOOOOOOOO",
//        "O                  O",
//        "O OO OOOO OO OOOO  O",
//        "O OO E OO O OOOO  OO",
//        "O                  O",
//        "OOO   OO     OO   OO",
//        "    O   O   O   OE  ",
//        "O   O OOO  OOO    OO",
//        "O      O     O     O",
//        "O O OO O     OOOO  O",
//        "O                  O",
//        "OOO OOOO OOOO OOO OO",
//        "O         O        O",
//        "O OOOOOO  O OOOOOO O",
//        "O   E              O",
//        "OOOOOOOOOOOOOOOOOOOO"
//    }
//    };
//public:
//    GameMap(int mapIndex = 0)
//    {
//        loadMap(mapIndex);
//    }
//    void loadMap(int index)
//    {
//        walls.clear();
//        foodPellets.clear();
//        energizers.clear(); // Clear energizers too
//
//        if (index < 0 || index >= map_layout.size())
//            index = 0;
//        currentMapIndex = index;
//        const vector<string>& map = map_layout[index];
//
//        for (int y = 0; y < map.size(); ++y)
//        {
//            for (int x = 0; x < map[y].size(); ++x)
//            {
//                char tile = map[y][x];
//                Vector2f pos(x * tileSize, y * tileSize);
//                if (tile == 'O')
//                {
//                    Wall wall(pos);
//                    if (currentMapIndex == 1) 
//                    {
//                        wall.setColor(Color(128, 0, 128));
//                    }
//                    walls.push_back(wall);
//                }
//                else if (tile == ',' || tile == ' ') // Keep ' ' as food for now, or remove if ' ' means empty
//                {
//                    foodPellets.push_back(Food(pos + Vector2f((tileSize - 8.f) / 2.f, (tileSize - 8.f) / 2.f)));
//                }
//                else if (tile == 'E') // Load Energizers
//                {
//                    energizers.push_back(Energizer(pos + Vector2f((tileSize - 12.f) / 2.f, (tileSize - 12.f) / 2.f)));
//                }
//            }
//        }
//    }
//
//    void draw(RenderWindow& window)
//    {
//        for (auto& wall : walls)
//        {
//            wall.draw(window);
//        }
//        for (auto& pellet : foodPellets)
//        {
//            pellet.draw(window);
//        }
//        for (auto& energizer : energizers) // Draw Energizers
//        {
//            energizer.draw(window);
//        }
//    }
//
//    const vector<Wall>& getWalls() const
//    {
//        return walls;
//    }
//    vector<Food>& getFoodPellets()
//    {
//        return foodPellets;
//    }
//    Vector2f getTilePixelPosition(int tileX, int tileY) const {
//        if (currentMapIndex < 0 || static_cast<size_t>(currentMapIndex) >= map_layout.size()) {
//            cerr << "Critical Error: currentMapIndex in getTilePixelPosition is invalid: " << currentMapIndex << endl;
//            return Vector2f(1 * tileSize, 1 * tileSize);
//        }
//        const vector<string>& current_map_data = map_layout[currentMapIndex];
//        if (tileY >= 0 && static_cast<size_t>(tileY) < current_map_data.size() &&
//            tileX >= 0 && static_cast<size_t>(tileX) < current_map_data[tileY].size() &&
//            current_map_data[tileY][tileX] != 'O') {
//            return Vector2f(static_cast<float>(tileX) * tileSize, static_cast<float>(tileY) * tileSize);
//        }
//        cerr << "Warning: Requested spawn tile (" << tileX << "," << tileY
//            << ") on map " << currentMapIndex;
//        if (tileY < 0 || static_cast<size_t>(tileY) >= current_map_data.size() ||
//            tileX < 0 || static_cast<size_t>(tileX) >= current_map_data[tileY].size()) {
//            cerr << " is out of bounds.";
//        }
//        else {
//            cerr << " is a wall ('" << current_map_data[tileY][tileX] << "').";
//        }
//        cerr << " Defaulting." << endl;
//        return Vector2f(1 * tileSize, 1 * tileSize);
//    }
//    vector<Energizer>& getEnergizers() // Getter for Energizers
//    {
//        return energizers;
//    }
//};
//
//int main()
//{
//    srand(time(nullptr));
//    RenderWindow window(VideoMode(880, 600), "Pac-Man OOP Version");
//    window.setFramerateLimit(60);
//
//    // Loading of SFML ka samaan starts here.
//    Texture menuBackgroundTexture;            //////////////
//    Sprite menuBackground;
//    menuBackgroundTexture.loadFromFile("1147475.jpg");
//    menuBackground.setTexture(menuBackgroundTexture);
//    menuBackground.setScale(
//        window.getSize().x / (float)menuBackgroundTexture.getSize().x,
//        window.getSize().y / (float)menuBackgroundTexture.getSize().y);
//
//    Texture selectt;            ////////////// T is used to deifferentiate, T wala is texture and simple wala is sprite.
//    Sprite select1;
//    selectt.loadFromFile("1147475.jpg");
//    select1.setTexture(selectt);
//    select1.setScale(100, 400);
//
//    Texture map1t;            
//    Sprite map1;
//    map1t.loadFromFile("map1.png");
//    map1.setTexture(map1t);
//    map1.setScale(0.5f, 0.5f);
//    map1.setPosition(50.f, 150.f);        
//
//    Texture map2t;            
//    Sprite map2;
//    map2t.loadFromFile("map2.png");
//    map2.setTexture(map2t);
//    map2.setScale(0.5f, 0.5f);
//    map2.setPosition(550.f, 150.f);
//
//    Texture endt;            //////////////
//    Sprite end;
//    endt.loadFromFile("IMG_3084.jpg");
//    end.setTexture(endt);
//    end.setScale
//    (window.getSize().x / (float)endt.getSize().x,
//     window.getSize().y / (float)endt.getSize().y);
//
//     
//    Font font;
//    font.loadFromFile("Crackman.otf");
//
//    Text title("PAC-MAN", font, 64);
//    title.setFillColor(Color::Yellow);
//    title.setPosition(475.f, 150.f);
//
//    Music bgmusic;
//    bgmusic.openFromFile("mi-bg.wav");
//    bgmusic.setVolume(35.f);
//    bgmusic.play();
//
//    Music eatmusic;
//    eatmusic.openFromFile("pacman_chomp.wav");
//    eatmusic.setVolume(70.f);
//
//    Music deathmusic;
//    deathmusic.openFromFile("pacman_death.wav");
//    deathmusic.setVolume(70.f);
//
//    Text playText("Play", font, 50);
//    playText.setFillColor(Color::White);
//    playText.setPosition(550.f,400.f);      
//
//    Text quitText("Quit", font, 50);
//    quitText.setFillColor(Color::White);
//    quitText.setPosition(550.f, 475.f);     
//
//    Text selectText("Select", font, 50);      
//    selectText.setFillColor(Color::White);
//    selectText.setPosition(350.f, 500.f);
//
//    Text map1Text("First Map", font, 40);     
//    map1Text.setFillColor(Color::White);
//    map1Text.setPosition(100.f, 400.f);
//
//    Text map2Text("Second Map", font, 40);     
//    map2Text.setFillColor(Color::White);
//    map2Text.setPosition(550.f, 400.f);
//
//    Text gameOverText("GAME OVER", font, 48);
//    gameOverText.setFillColor(Color::Red);
//    gameOverText.setPosition(250.f, 90.f);
//
//    //controls on screen 
//
//    RectangleShape upButton(Vector2f(100.f, 50.f));
//    upButton.setFillColor(Color::White); // CornflowerBlue
//    upButton.setPosition(700.f, 390.f);
//
//    RectangleShape downButton(Vector2f(100.f, 50.f));
//    downButton.setFillColor(Color::White);
//    downButton.setPosition(700.f, 510.f);
//
//    RectangleShape leftButton(Vector2f(100.f, 50.f));
//    leftButton.setFillColor(Color::White);
//    leftButton.setPosition(630.f, 450.f);
//
//    RectangleShape rightButton(Vector2f(100.f, 50.f));
//    rightButton.setFillColor(Color::White);
//    rightButton.setPosition(770.f, 450.f);
// 
//    Text upText("Up", font, 30);
//    upText.setStyle(Text::Bold);
//    upText.setFillColor(Color::Black);
//    upText.setPosition(700.f, 390.f);
//
//    Text downText("Down", font, 30);
//    upText.setStyle(Text::Bold);
//    downText.setFillColor(Color::Black);
//    downText.setPosition(700.f, 510.f);
//
//    Text leftText("Left", font, 30);
//    upText.setStyle(Text::Bold);
//    leftText.setFillColor(Color::Black);
//    leftText.setPosition(630.f, 450.f);
//
//    Text rightText("Right", font, 30);
//    upText.setStyle(Text::Bold);
//    rightText.setFillColor(Color::Black);
//    rightText.setPosition(770.f, 450.f);
//
//    RectangleShape end1({ 350.f,80.f });
//    end1.setOutlineColor(Color::Yellow);
//    end1.setOutlineThickness(10);
//    end1.setPosition(225.f, 90.f);                    
//    end1.setFillColor(Color::White);
//
//    float ghostSpeed = 70.f;
//    Player player({ 32.f, 50.f }); // Ehan my goat
//    GameMap gameMap;
//    vector<Ghost> ghosts;
//    ghosts.emplace_back(Ghost(gameMap.getTilePixelPosition(11, 9), ghostSpeed, Color::Red)); // waqar bake
//    ghosts.emplace_back(Ghost(gameMap.getTilePixelPosition(10, 9), ghostSpeed + 5.f, Color(255, 182, 193)));// saifullah
//    ghosts.emplace_back(Ghost(gameMap.getTilePixelPosition(9, 9), ghostSpeed + 5.f, Color::Magenta)); // aqib shezan
//
//    Clock clock;
//    int selectedMapIndex = 0;
//    int score = 0;
//    Text scoreText("Score: 0", font, 35);
//    scoreText.setFillColor(Color::White);
//    scoreText.setPosition(645.f, 100.f);
//
//    RectangleShape boundary({ 230.f,100.f });
//    boundary.setOutlineColor(Color::Yellow);
//    boundary.setOutlineThickness(10);
//    boundary.setPosition(625, 90);                      ///////////
//    boundary.setFillColor(Color::Transparent);
//
//
//    bool inMenu = true;         /////Game State Control
//    bool select = false;
//    bool over = false;
//
//    while (window.isOpen()) // Main game loop, wohooooo
//    {
//        float deltaTime = clock.restart().asSeconds();
//        Event event;
//
//        while (window.pollEvent(event))
//        {
//            if (bgmusic.getStatus() != Music::Playing && window.isOpen())
//            {
//                bgmusic.play();  // Restarts the music if it stops mid-play
//            }
//            if (Keyboard::isKeyPressed(Keyboard::Escape) || event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
//                Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
//                if (quitText.getGlobalBounds().contains(mousePos) || Keyboard::isKeyPressed(Keyboard::Escape))
//                {
//                    window.close();
//                    deathmusic.play();
//                }
//            }
//
//            // Menu input
//            if (inMenu && (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left || Keyboard::isKeyPressed(Keyboard::P)))
//            {
//                Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
//                if (playText.getGlobalBounds().contains(mousePos) || Keyboard::isKeyPressed(Keyboard::P))
//                {
//                    inMenu = false;
//                    select = true;
//                }
//            }
//            if (select && (event.type == Event::KeyPressed || (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)))
//            {
//                Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
//                if (map1.getGlobalBounds().contains(mousePos) || Keyboard::isKeyPressed(Keyboard::Num1))
//                {
//                    selectedMapIndex = 0;
//                    gameMap.loadMap(selectedMapIndex);
//                    select = false;
//                }
//                else if (map2.getGlobalBounds().contains(mousePos) || Keyboard::isKeyPressed(Keyboard::Num2))
//                {
//                    selectedMapIndex = 1;
//                    gameMap.loadMap(selectedMapIndex);
//                    select = false;
//                }
//            }
//            if (event.type == Event::KeyPressed && event.key.code == Keyboard::K)
//            {
//                over = true;
//            }
//            if (!inMenu && !select && !over && event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
//            { // logic for direction control using screen
//                Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
//                if (upButton.getGlobalBounds().contains(mousePos))
//                {
//                    player.setClickedDirection({ 0.f, -1.f });
//                }
//                else if (downButton.getGlobalBounds().contains(mousePos))
//                {
//                    player.setClickedDirection({ 0.f, 1.f });
//                }
//                else if (leftButton.getGlobalBounds().contains(mousePos))
//                {
//                    player.setClickedDirection({ -1.f, 0.f });
//                }
//                else if (rightButton.getGlobalBounds().contains(mousePos))
//                {
//                    player.setClickedDirection({ 1.f, 0.f });
//                }
//            }
//
//        }
//
//        window.clear();
//
//        if (inMenu)
//        {
//            window.draw(menuBackground);
//            window.draw(title);
//            window.draw(playText);
//            window.draw(quitText);
//        }
//        else if (select) {
//            window.draw(select1);
//            window.draw(selectText);
//            window.draw(map1);       
//            window.draw(map2);
//            window.draw(map1Text);
//            window.draw(map2Text);
//        }
//        else if (over)
//        {
//            window.draw(end);
//            window.draw(end1);
//            window.draw(gameOverText);
//        }
//        else
//        {
//
//            player.handleInput();
//            player.update(deltaTime, gameMap.getWalls());
//            for (auto& ghost : ghosts)
//            {
//                ghost.update(deltaTime, gameMap.getWalls(), player);
//            }
//            vector<Food>& pellets = gameMap.getFoodPellets();
//            FloatRect playerBounds = player.getBounds();
//            vector<Energizer>& energizers = gameMap.getEnergizers();
//            for (size_t i = 0; i < energizers.size(); ++i)
//            {
//                if (playerBounds.intersects(energizers[i].getBounds()))
//                {
//                    energizers.erase(energizers.begin() + i);
//                    // score += 10; // Optional: score for eating energizer itself
//                    // scoreText.setString("Score: " + to_string(score));
//                    for (auto& ghost : ghosts)
//                    {
//                        ghost.enterFrightenedMode();
//                    }
//                    // Potentially play a power-up sound
//                    break;
//                }
//            }
//            for (auto& ghost : ghosts) // Use reference to modify ghost (e.g., call getEaten)
//            {
//                if (player.getBounds().intersects(ghost.getBounds()))
//                {
//                    if (ghost.isFrightened())
//                    {
//                        ghost.getEaten(); // This will reset the ghost
//                        score += 50;      // Add score for eating ghost
//                        scoreText.setString("Score: " + to_string(score));
//                        // Play a ghost eaten sound effect if you have one
//                    }
//                    else
//                    {
//                        over = true;
//                        bgmusic.stop();
//                        deathmusic.play(); // Play death sound
//                        gameOverText.setString("GAME OVER");
//                    }
//                    break; // Process one collision per frame to avoid multiple game overs/eats
//                }
//            }
//            for (int i = 0; i < pellets.size(); ++i)
//            {
//                if (playerBounds.intersects(pellets[i].getBounds()))
//                {
//                    eatmusic.play();
//                    pellets.erase(pellets.begin() + i);
//                    score += 10;
//                    scoreText.setString("Score: " + to_string(score));
//                    break;
//                }
//            }
//            if (!over && pellets.empty() && energizers.empty()) {
//                over = true;
//                bgmusic.stop();
//                // Play win sound
//                gameOverText.setString("YOU WIN!"); // Or next level
//            }
//            gameMap.draw(window);
//            player.draw(window);
//            window.draw(upButton);
//            window.draw(downButton);
//            window.draw(leftButton);
//            window.draw(rightButton);
//            window.draw(upText);
//            window.draw(downText);
//            window.draw(leftText);
//            window.draw(rightText);
//
//            for (auto& ghost : ghosts)
//            {
//                ghost.draw(window);
//            }
//            window.draw(scoreText);
//            window.draw(boundary);
//        }
//        window.display();
//    }
//
//    return 0;
//}