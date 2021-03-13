// Timber.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// Timber game
// M.Ramon Sorell
// Last updated: 13 March 2019

// Status: In progress need to resolve several problems and improve user experience

#include <iostream>
#include <SFML/Graphics.hpp>
#include <sstream>

//make code easier to type with using namespace
using namespace sf;
using namespace std;

// Function declaration
void updateBranches(int seed);
const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];
// Where is the player/branch?
// Left or Right
enum class side {LEFT, RIGHT, NONE};
side branchPositions[NUM_BRANCHES];


int main()
{
    //create a video mode object ::: resolution of player screen
    VideoMode vm(1920, 1080);

    //low res code
    // VideoMode vm(1280, 720);

    //create and open a window for the game
    //RenderWindow window(vm, "Timber!!", Style::Fullscreen);

    //low res code
    RenderWindow window(vm, "Timber!!!");
    View view(sf::FloatRect(0,0,1920,1080));

    //create a texture to hold a graphic on the GPU
    Texture textureBackground;

    //Load a gaphic in the texture
    textureBackground.loadFromFile("graphics/background.png");

    // Create a sprite
    Sprite spriteBackground;

    // Attach the texture to the sprite
    spriteBackground.setTexture(textureBackground);

    //set the spriteBackground to the cover screen
    spriteBackground.setPosition(0, 0);

    // Make a tree sprite
    Texture textureTree;
    textureTree.loadFromFile("graphics/tree.png");
    Sprite spriteTree;
    spriteTree.setTexture(textureTree);
    spriteTree.setPosition(810, 0);

    //Prepare the bee
    Texture textureBee;
    textureBee.loadFromFile("graphics/bee.png");
    Sprite spriteBee;
    spriteBee.setTexture(textureBee);
    spriteBee.setPosition(0, 800);
    // is the bee moving
    bool beeActive = false;
    // how fast can the bee fly
    float beeSpeed = 0.0f;

    // add the clouds
    Texture textureCloud;
    // load 1 new texture
    textureCloud.loadFromFile("graphics/cloud.png");
    // create cloud sprite
    Sprite spriteCloud1;
    Sprite spriteCloud2;
    Sprite spriteCloud3;
    spriteCloud1.setTexture(textureCloud);
    spriteCloud2.setTexture(textureCloud);
    spriteCloud3.setTexture(textureCloud);
    // position the clouds off scree
    spriteCloud1.setPosition(0, 0);
    spriteCloud2.setPosition(0, 250);
    spriteCloud3.setPosition(0, 500);
    // are the clouds currently on the screen
    bool cloud1Active = false;
    bool cloud2Active = false;
    bool cloud3Active = false;
    //How fast if the cloud going?
    float cloud1Speed = 0.0f;
    float cloud2Speed = 0.0f;
    float cloud3Speed = 0.0f;

    // Variables to control time itself
    Clock clock;

    // time bar
    RectangleShape timeBar;
    float timeBarStartWidth = 400;
    float timeBarHeight = 80;
    timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
    timeBar.setFillColor(Color::Red);
    timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

    Time gameTimeTotal;
    float timeRemaining = 6.0f;
    float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

    //track wheth the game is running
    bool paused = true;

    // add text
    //Draw some text
    int score = 0;
    sf::Text messageText;
    sf::Text scoreText;

    // We need to choose a font
    Font font;
    font.loadFromFile("fonts/KOMIKAP_.ttf");

    //Set the font to our message
    messageText.setString("Press Enter to Start!");
    scoreText.setString("Score = 0");

    //set the font to our message
    messageText.setFont(font);
    scoreText.setFont(font);

    //Make it really big
    messageText.setCharacterSize(75);
    scoreText.setCharacterSize(100);

    //Choose a color
    messageText.setFillColor(Color::White);
    scoreText.setFillColor(Color::White);

    //Position the text
    FloatRect textRect= messageText.getLocalBounds();
    messageText.setOrigin(textRect.left +
        textRect.width / 2.0f,
        textRect.top +
        textRect.height / 2.0f);
    messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
    scoreText.setPosition(20, 20);

    // Prepare 6 branches
    Texture textureBranch;
    textureBranch.loadFromFile("graphics/branch.png");
    // set the texture for each branch sprite
    for (int i = 0; i < NUM_BRANCHES; i++)
    {
        branches[i].setTexture(textureBranch);
        branches[i].setPosition(-2000, -2000);
        // Set the sprite's origin to dead center
        // We can then spin it round withour changing its position
        branches[i].setOrigin(220, 20);
    }

    // Prepare the player
    Texture texturePlayer;
    texturePlayer.loadFromFile("graphics/player.png");
    Sprite spritePlayer;
    spritePlayer.setTexture(texturePlayer);
    spritePlayer.setPosition(580, 720);

    // The player stats on the left
    side playerSide = side::LEFT;

    // Pepare the gravestone
    Texture textureRIP;
    textureRIP.loadFromFile("graphics/rip.png");
    Sprite spriteRIP;
    spriteRIP.setTexture(textureRIP);
    spriteRIP.setPosition(600, 860);

    // Prepare the axe
    Texture textureAxe;
    textureAxe.loadFromFile("graphics/axe.png");
    Sprite spriteAxe;
    spriteAxe.setTexture(textureAxe);
    spriteAxe.setPosition(700, 300);

    // Line the axe up with the tree
    const float AXE_POSITION_LEFT = 700;
    const float AXE_POSITION_RIGHT = 1075;

    // Prepare the flying log
    Texture textureLog;
    textureLog.loadFromFile("graphics/log.png");
    Sprite spriteLog;
    spriteLog.setTexture(textureLog);
    spriteLog.setPosition(810, 720);

    // Some other useful log related variables
    bool logActive = false;
    float logSpeedX = 1000;
    float logSpeedY = -1500;



    

    /*
    updateBranches(1);
    updateBranches(2);
    updateBranches(3);
    updateBranches(4);
    updateBranches(5);
    */

    // Game while loop
    while (window.isOpen())
    {
        /*
        Handle the player's input
        */
        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();

        }

        // start the game
        if (Keyboard::isKeyPressed(Keyboard::Return))
        {
            paused = false;

            // Reset the time and score
            score = 0;
            timeRemaining = 5;

        }

        /*
        Update the scene
        */
        if (!paused)
        {

            //Measure time
            //Meausure frame speed
            // dt equals delta time
            Time dt = clock.restart();

            // Subtract from the amount of time remainig
            timeRemaining -= dt.asSeconds();

            // size up the time bar
            timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));

            if (timeRemaining <= 0.0f)
            {
                // pause the game
                paused = true;

                // change the message shown to the player
                messageText.setString("Out of time !!");

                // Reposition the text based on its new size
                FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin(textRect.left +
                    textRect.width / 2.0f,
                    textRect.top +
                    textRect.height / 2.0f);
                messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
            }

            // Setup beeActive == beeActive default is false
            if (!beeActive)
            {

                // How fast is the bee
                srand((int)time(0) * 100);
                beeSpeed = (rand() % 200) + 200;

                // How high is the bee
                srand((int)time(0) * 10);
                float height = (rand() % 500) + 500;
                spriteBee.setPosition(2000, height);
                beeActive = true;

            }

            else
            {
                // Move the bee
                spriteBee.setPosition(
                    spriteBee.getPosition().x -
                    (beeSpeed * dt.asSeconds()),
                    spriteBee.getPosition().y);

                // Has the bee reached the right hand edge of the screen ?
                if (spriteBee.getPosition().x < -100)
                {
                    // set it up ready to be a whole new bee next frame
                    beeActive = false;

                }
            }

            // Manage the clouds
            // Setup cloud1Active == cloud1Active default is false
            if (!cloud1Active)
            {

                // How fast is the cloud
                srand((int)time(0) * 10);
                cloud1Speed = (rand() % 200) + 50;

                // How high is the cloud
                srand((int)time(0) * 10);
                float height = (rand() % 150);
                spriteCloud1.setPosition(-200, height);
                cloud1Active = true;

            }

            else
            {
                // Move the cloud
                spriteCloud1.setPosition(
                    spriteCloud1.getPosition().x +
                    (cloud1Speed * dt.asSeconds()),
                    spriteCloud1.getPosition().y);

                // Has the cloud reached the right hand edge of the screen ?
                if (spriteCloud1.getPosition().x > 1920)
                {
                    // set it up ready to be a whole new bee next frame
                    cloud1Active = false;

                }
            }

            // update the score text
            std::stringstream ss;
            ss << "Score = " << score;
            scoreText.setString(ss.str());

            // update the branch sprites
            for (int i = 0; i < NUM_BRANCHES; i++)
            {
                float height  = i * 150;
                if (branchPositions[i] == side::LEFT)
                {
                    // Move the sprite to the left side
                    branches[i].setPosition(610, height);
                    // Flip the sprit round the other way
                    branches[i].setRotation(180);
                }
                else if (branchPositions[i] == side::RIGHT)
                {
                    // Move the sprite to the right side
                    branches[i].setPosition(1330, height);
                    // Flip the sprit rotation to normal
                    branches[i].setRotation(0);
                }
                else
                {
                    //hide the branch
                    branches[i].setPosition(3000, height);
                }

            }

        } // end if(!paused)

        

        /*
        draw the scene
        */

        // Clear everything from the last frame
        window.clear();

        // Draw the game scene here
        window.draw(spriteBackground);

        // draw the clouds
        window.draw(spriteCloud1);
        //window.draw(spriteCloud2);
        //window.draw(spriteCloud3);

        //draw the branches
        for (int i = 0; i < NUM_BRANCHES; i++)
        {
            window.draw(branches[i]);
        }

        // draw the tree in front of the clouds
        window.draw(spriteTree);

        // darw the player
        window.draw(spritePlayer);

        // draw the axe
        window.draw(spriteAxe);

        // draew the flying log
        window.draw(spriteLog);

        //draw the gravestone
        window.draw(spriteRIP);

        // draw the bee
        window.draw(spriteBee);

        //Draw the score
        window.draw(scoreText);

        //draw the timebar
        window.draw(timeBar);

        if (paused)
        {
            //Draw our message
            window.draw(messageText);
        }



        // show everything we just drew
        window.display();

    }

    return 0;
}

// Function definition
void updateBranches(int seed)
{
    //Move all the branches down one place
    for (int j = NUM_BRANCHES - 1; j > 0; j--)
    {
        branchPositions[j] = branchPositions[j - 1];
    }

    // Spawn a new branch at position 0
    // LEFT, RIGHT or NONE
    srand((int)time(0) + seed);
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