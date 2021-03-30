#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;

void UpdateBranches(int speed);

const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];

enum class side
{
    LEFT,
    RIGHT,
    NONE
};
side branchPositions[NUM_BRANCHES];

int main()
{

    VideoMode vm(1920, 1080);
    RenderWindow window(vm, "Timber!!!", Style::Fullscreen);

    Texture textureBackground;
    textureBackground.loadFromFile("graphics/background.png");
    Sprite spriteBackground;
    spriteBackground.setTexture(textureBackground);
    spriteBackground.setPosition(0, 0);

    Texture textureTree;
    textureTree.loadFromFile("graphics/tree.png");
    Sprite spriteTree;
    spriteTree.setTexture(textureTree);
    spriteTree.setPosition(810, 0);

    std::vector<Sprite> allBackgroundTrees;
    for (int i = 0; i < 8; i++)
    {
        Sprite backgroundTree;
        allBackgroundTrees.push_back(backgroundTree);
        backgroundTree.setTexture(textureTree);
        srand((int)time(0));
        float posX = (rand() % 200);
        float posY = (rand() % 200);
        backgroundTree.setPosition(posX, posY);
    }

    Texture textureBee;
    textureBee.loadFromFile("graphics/bee.png");
    Sprite spriteBee;
    spriteBee.setTexture(textureBee);
    spriteBee.setPosition(0, 800);

    bool beeActive = false;
    float beeSpeed = 0.0f;

    Texture textureCloud;
    textureCloud.loadFromFile("graphics/cloud.png");
    Sprite spriteCloud1;
    Sprite spriteCloud2;
    Sprite spriteCloud3;
    spriteCloud1.setTexture(textureCloud);
    spriteCloud2.setTexture(textureCloud);
    spriteCloud3.setTexture(textureCloud);

    spriteCloud1.setPosition(0, 0);
    spriteCloud2.setPosition(0, 250);
    spriteCloud3.setPosition(0, 500);

    bool cloud1Active = false;
    bool cloud2Active = false;
    bool cloud3Active = false;

    float cloud1Speed = 0.0f;
    float cloud2Speed = 0.0f;
    float cloud3Speed = 0.0f;

    Clock clock;

    //TransparencyBehindTimeBar
    RectangleShape transBar;
    float transBarWidth = 420;
    float transBarHeight = 400;
    transBar.setSize(Vector2f(transBarWidth, transBarHeight));
    transBar.setFillColor(Color(0, 0, 0, (255 / 2)));
    transBar.setPosition((1920 / 2) - transBarWidth / 2, 980);

    //Time Bar
    RectangleShape timeBar;
    float timeBarStartWidth = 400;
    float timeBarHeight = 80;
    timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
    timeBar.setFillColor(Color::Red);
    timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

    Time gameTimeTotal;
    float timeRemaining = 6.0f;
    float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

    //Wheater the game is running or not
    bool paused = true;

    int score = 0;

    Text messageText;
    Text scoreText;

    Font font;
    font.loadFromFile("fonts/KOMIKAP_.ttf");

    messageText.setFont(font);
    scoreText.setFont(font);

    messageText.setString("Press Enter To Start!");
    scoreText.setString("Score = 0");

    messageText.setCharacterSize(75);
    scoreText.setCharacterSize(100);

    messageText.setFillColor(Color::White);
    scoreText.setFillColor(Color::White);

    FloatRect textRect = messageText.getLocalBounds();

    messageText.setOrigin(textRect.left +
        textRect.width / 2.0f,
        textRect.top +
        textRect.height / 2.0f);

    messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

    scoreText.setPosition(20, 20);

    //Branches
    Texture textureBranches;
    textureBranches.loadFromFile("graphics/branch.png");

    for (int i = 0; i < NUM_BRANCHES; i++)
    {
        branches[i].setTexture(textureBranches);
        branches[i].setPosition(-2000, -2000);
        branches[i].setOrigin(220, 20);
    }

    //Player
    Texture texturePlayer;
    texturePlayer.loadFromFile("graphics/player.png");
    Sprite spritePlayer;
    spritePlayer.setTexture(texturePlayer);
    spritePlayer.setPosition(580, 720);

    side playerSide = side::LEFT;

    //Gravestone
    Texture textureRIP;
    textureRIP.loadFromFile("graphics/rip.png");
    Sprite spriteRIP;
    spriteRIP.setTexture(textureRIP);
    spriteRIP.setPosition(600, 860);

    //Axe
    Texture textureAxe;
    textureAxe.loadFromFile("graphics/axe.png");
    Sprite spriteAxe;
    spriteAxe.setTexture(textureAxe);
    spriteAxe.setPosition(700, 830);

    //Line up the axe with the tree
    const float AXE_POSITION_LEFT = 700;
    const float AXE_POSITION_RIGHT = 1075;

    //Flying Log
    Texture textureLog;
    textureLog.loadFromFile("graphics/log.png");
    Sprite spriteLog;
    spriteLog.setTexture(textureLog);
    spriteLog.setPosition(810, 720);

    //INDICATOR
    RectangleShape indicatorrect;
    FloatRect indicatorSize = spriteLog.getGlobalBounds();
    std::cout << "IndicatorSizeX = " << indicatorSize.width << "IndicatorSizeY = " << indicatorSize.height << std::endl;
    indicatorrect.setSize(Vector2f(indicatorSize.width, indicatorSize.height));
    indicatorrect.setFillColor(Color(255, 255, 0, (255 / 4)));
    indicatorrect.setPosition(810, 720);

    //Usefull
    bool logActive = false;
    float logSpeedX = 1000;
    float logSpeedY = -1500;

    //ControlPlayerInput
    bool acceptInput = false;

    //Sounds
    //TreeChopping
    SoundBuffer chopBuffer;
    chopBuffer.loadFromFile("sounds/chop.wav");
    Sound chop;
    chop.setVolume(50);
    chop.setBuffer(chopBuffer);

    //PlayerDeath
    SoundBuffer deathBuffer;
    deathBuffer.loadFromFile("sounds/death.wav");
    Sound death;
    death.setVolume(10);
    death.setBuffer(deathBuffer);

    //Out of Time
    SoundBuffer ootBuffer;
    ootBuffer.loadFromFile("sounds/out_of_time.wav");
    Sound outOfTime;
    outOfTime.setVolume(10);
    outOfTime.setBuffer(ootBuffer);

    while (window.isOpen())
    {
        Event event;

        while (window.pollEvent(event))
        {
            if (event.type == Event::KeyReleased && !paused)
            {
                acceptInput = true;

                spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
            }
        }

        //Player Input
        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }

        if (Keyboard::isKeyPressed(Keyboard::Return))
        {
            paused = false;

            score = 0;
            timeRemaining = 6.0f;

            //Branches Dissapear
            for (int i = 0; i < NUM_BRANCHES; i++)
            {
                branchPositions[i] = side::NONE;
            }

            //Hide Gravestone
            spriteRIP.setPosition(675, 2000);

            spritePlayer.setPosition(580, 720);

            acceptInput = true;
        }

        if (acceptInput)
        {
            if (Keyboard::isKeyPressed(Keyboard::Right))
            {
                playerSide = side::RIGHT;
                score++;
                timeRemaining += (2 / score) + .15;

                spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);
                spritePlayer.setPosition(1200, 720);

                UpdateBranches(score);

                spriteLog.setPosition(810, 720);
                logSpeedX = -5000;
                logActive = true;

                acceptInput = false;

                chop.play();
            }

            if (Keyboard::isKeyPressed(Keyboard::Left))
            {
                playerSide = side::LEFT;
                score++;
                timeRemaining += (2 / score) + .15;

                spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);
                spritePlayer.setPosition(580, 720);

                UpdateBranches(score);

                spriteLog.setPosition(810, 720);
                logSpeedX = 5000;
                logActive = true;

                acceptInput = false;

                chop.play();
            }
        }

        //Update the scene


        if (!paused)
        {
            //Measure the time
            Time deltaTime = clock.restart();

            timeRemaining -= deltaTime.asSeconds();

            timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));

            if (timeRemaining <= 0.0f)
            {
                paused = true;

                messageText.setString("Out Of Time!!");

                FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin(textRect.left +
                    textRect.width / 2.0f,
                    textRect.top +
                    textRect.height / 2.0f);

                messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

                outOfTime.play();
            }

            if (!beeActive)
            {
                srand((int)time(0));
                beeSpeed = (rand() % 200) + 200;

                srand((int)time(0) * 10);
                float height = (rand() % 500) + 500;
                spriteBee.setPosition(2000, height);
                beeActive = true;
            }
            else
            {
                spriteBee.setPosition(spriteBee.getPosition().x - (beeSpeed * deltaTime.asSeconds()),
                    spriteBee.getPosition().y);

                if (spriteBee.getPosition().x < -100)
                {
                    beeActive = false;
                }
            }

            if (!cloud1Active)
            {
                srand((int)time(0) * 10);
                cloud1Speed = (rand() % 200);

                srand((int)time(0) * 10);
                float height = (rand() % 150);
                spriteCloud1.setPosition(-200, height);
                cloud1Active = true;
            }
            else
            {
                spriteCloud1.setPosition(spriteCloud1.getPosition().x + (cloud1Speed * deltaTime.asSeconds()),
                    spriteCloud1.getPosition().y);

                if (spriteCloud1.getPosition().x > 1920)
                {
                    cloud1Active = false;
                }
            }

            if (!cloud2Active)
            {
                srand((int)time(0) * 20);
                cloud2Speed = (rand() % 200);

                srand((int)time(0) * 20);
                float height = (rand() % 300) - 150;
                spriteCloud2.setPosition(-200, height);
                cloud2Active = true;
            }
            else
            {
                spriteCloud2.setPosition(spriteCloud2.getPosition().x + (cloud2Speed * deltaTime.asSeconds()),
                    spriteCloud2.getPosition().y);

                if (spriteCloud2.getPosition().x > 1920)
                {
                    cloud2Active = false;
                }
            }

            if (!cloud3Active)
            {
                srand((int)time(0) * 30);
                cloud3Speed = (rand() % 200);

                srand((int)time(0) * 30);
                float height = (rand() % 450) - 150;
                spriteCloud3.setPosition(-200, height);
                cloud3Active = true;
            }
            else
            {
                spriteCloud3.setPosition(spriteCloud3.getPosition().x + (cloud3Speed * deltaTime.asSeconds()),
                    spriteCloud3.getPosition().y);

                if (spriteCloud3.getPosition().x > 1920)
                {
                    cloud3Active = false;
                }
            }

            std::stringstream ss;
            ss << "Score = " << score;
            scoreText.setString(ss.str());

            for (int i = 0; i < NUM_BRANCHES; i++)
            {
                float height = i * 150;
                if (branchPositions[i] == side::LEFT)
                {
                    branches[i].setPosition(610, height);

                    //FlipSprite
                    branches[i].setRotation(180);
                }
                else if (branchPositions[i] == side::RIGHT)
                {
                    branches[i].setPosition(1330, height);

                    //Normal
                    branches[i].setRotation(0);
                }
                else
                {
                    //HideTheBranch
                    branches[i].setPosition(3000, height);
                }
            }

            //Handle flying log
            if (logActive)
            {
                spriteLog.setPosition(spriteLog.getPosition().x + (logSpeedX * deltaTime.asSeconds()),
                spriteLog.getPosition().y + (logSpeedY * deltaTime.asSeconds()));

                if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 2000)
                {
                    logActive = false;
                    spriteLog.setPosition(810, 720);
                }
            }

            //DeathByBranch
            if (branchPositions[5] == playerSide)
            {
                paused = true;
                acceptInput = false;

                spriteRIP.setPosition(spritePlayer.getPosition());

                spritePlayer.setPosition(2000, 660);

                messageText.setString("SQUISHED!!");

                FloatRect textRect = messageText.getLocalBounds();

                messageText.setOrigin(textRect.left +
                    textRect.width / 2.0f,
                    textRect.top +
                    textRect.height / 2.0f);

                messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

                death.play();
            }
        } //End if(!paused)

        //Draw the scene

        //Clear from last frame
        window.clear();

        //Draw game scene
        window.draw(spriteBackground);

        window.draw(spriteCloud1);
        window.draw(spriteCloud2);
        window.draw(spriteCloud3);

        for (int i = 0; i < NUM_BRANCHES; i++)
        {
            window.draw(branches[i]);
        }

        for (int i = 0; i < allBackgroundTrees.size(); i++)
        {
            window.draw(allBackgroundTrees[i]);
        }

        window.draw(spriteLog);

        window.draw(spriteTree);

        window.draw(spritePlayer);

        window.draw(spriteAxe);

        window.draw(indicatorrect);

        window.draw(spriteRIP);

        window.draw(spriteBee);

        window.draw(scoreText);

        window.draw(transBar);

        window.draw(timeBar);

        if (paused)
        {
            window.draw(messageText);
        }

        //Show everything we drew
        window.display();

    }

    return 0;
}

void UpdateBranches(int speed)
{
    for (int i = NUM_BRANCHES - 1; i > 0; i--)
    {
        branchPositions[i] = branchPositions[i - 1];
    }

    srand((int)time(0) + speed);
    int r = (rand() % 5);

    switch (r)
    {
    case 0:
        branchPositions[0] = side::LEFT;
        break;
    case 1:
        branchPositions[0] = side::RIGHT;
        break;
    default:
        branchPositions[0] = side::NONE;
        break;
    }
}