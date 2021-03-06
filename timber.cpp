#include "stdafx.h"
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;

void updateBranches(int seed);
const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];
enum class side {LEFT, RIGHT, NONE};
side branchPositions[NUM_BRANCHES];

int main()
{
	//Establish video mode and window
	VideoMode vm(1920, 1080);
	RenderWindow window(vm, "Timber!!!", Style::Fullscreen);

	//Set background texture from image src file
	Texture textureBackground;
	textureBackground.loadFromFile("graphics/background.png");

	//Set background sprite to background texture and draw
	Sprite spriteBackground;
	spriteBackground.setTexture(textureBackground);
	spriteBackground.setPosition(0, 0);

	//Set tree texture and apply to sprite
	Texture textureTree;
	textureTree.loadFromFile("graphics/tree.png");
	Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(810, 0);

	//Set secondary tree texture and apply to several sprites
	Texture textureTree2;
	textureTree2.loadFromFile("graphics/tree2.png");

	Sprite spriteTree2;
	spriteTree2.setTexture(textureTree2);
	spriteTree2.setPosition(20, 0);

	Sprite spriteTree3;
	spriteTree3.setTexture(textureTree2);
	spriteTree3.setPosition(300, -400);

	Sprite spriteTree4;
	spriteTree4.setTexture(textureTree2);
	spriteTree4.setPosition(1300, -400);

	Sprite spriteTree5;
	spriteTree5.setTexture(textureTree2);
	spriteTree5.setPosition(1500, -500);

	Sprite spriteTree6;
	spriteTree6.setTexture(textureTree2);
	spriteTree6.setPosition(1900, 0);

	//Set bee texture and apply to sprite
	Texture textureBee;
	textureBee.loadFromFile("graphics/bee.png");
	Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(0, 800);
	//Is the bee currently moving?
	bool beeActive = false;
	//How fast can the bee move?
	float beeSpeed = 0.0f;

	//Make identical cloud sprites from one texture
	Texture textureCloud;
	textureCloud.loadFromFile("graphics/cloud.png");
	
	const int NUM_CLOUDS = 6;
	Sprite clouds[NUM_CLOUDS];
	int cloudSpeeds[NUM_CLOUDS];
	bool cloudsActive[NUM_CLOUDS];

	for (int i = 0; i < NUM_CLOUDS; i++) {
		clouds[i].setTexture(textureCloud);
		clouds[i].setPosition(-300, i * 150);
		cloudsActive[i] = false;
		cloudSpeeds[i] = 0;
	}
	
	/* Obsolete cloud initialization */
	/*
	Sprite cloud1;
	Sprite cloud2;
	Sprite cloud3;
	cloud1.setTexture(textureCloud);
	cloud2.setTexture(textureCloud);
	cloud3.setTexture(textureCloud);
	//Position clouds offscreen
	cloud1.setPosition(0, 0);
	cloud2.setPosition(0, 250);
	cloud3.setPosition(0, 500);
	//Are the clouds currently on screen?
	bool cloud1Active = false;
	bool cloud2Active = false;
	bool cloud3Active = false;
	//How fast does each cloud move?
	float cloud1Speed = 0.0f;
	float cloud2Speed = 0.0f;
	float cloud3Speed = 0.0f;

	*/

	//Time variables
	Clock clock;

	//Time bar
	RectangleShape timeBar;
	float timeBarStartWidth = 400;
	float timeBarHeight = 80;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

	bool paused = true;

	//Draw some text
	int score = 0;
	Text messageText;
	Text scoreText;
	Text fpsText;

	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");

	//Set the font to message
	messageText.setFont(font);
	scoreText.setFont(font);
	fpsText.setFont(font);

	//Assign the messages to strings
	messageText.setString("Press Enter to start!");
	scoreText.setString("Score = 0");
	
	//Set up the fps text
	fpsText.setFillColor(Color::White);
	fpsText.setCharacterSize(100);
	fpsText.setPosition(1200, 20);

	//Set text size
	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(100);

	//Set text color
	messageText.setFillColor(Color::White);
	scoreText.setFillColor(Color::White);

	//Position the message text
	FloatRect textRect = messageText.getLocalBounds();
	messageText.setOrigin(
		textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);
	messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
	
	//Position the score text
	scoreText.setPosition(20, 20);

	// Backgrounds for the text
	RectangleShape rect1;
	rect1.setFillColor(sf::Color(0, 0, 0, 150));
	rect1.setSize(Vector2f(600, 105));
	rect1.setPosition(0, 30);

	RectangleShape rect2;
	rect2.setFillColor(sf::Color(0, 0, 0, 150));
	rect2.setSize(Vector2f(1000, 105));
	rect2.setPosition(1150, 30);

	//Prepare the six branches
	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");
	//Set texture for each branch
	for (int i = 0; i < NUM_BRANCHES; i++) {
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, -2000);
		branches[i].setOrigin(220, 20);
	}

	//Prepare the player
	Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(580, 720);

	side playerSide = side::LEFT;

	//Prepare the gravestone
	Texture textureRIP;
	textureRIP.loadFromFile("graphics/rip.png");
	Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(600, 860);

	//Prepare the axe
	Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(700, 830);

	//Line the axe up with the tree
	const float AXE_POSITION_LEFT = 700;
	const float AXE_POSITION_RIGHT = 1075;

	//Prepare the flying log
	Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(810, 720);

	bool logActive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1500;

	//Control the player input
	bool acceptInput = false;

	SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("sound/chop.wav");
	Sound chopSound;
	chopSound.setBuffer(chopBuffer);

	SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("sound/death.wav");
	Sound deathSound;
	deathSound.setBuffer(deathBuffer);

	SoundBuffer ootBuffer;
	ootBuffer.loadFromFile("sound/out_of_time.wav");
	Sound ootSound;
	ootSound.setBuffer(ootBuffer);

	//Control the drawing of the score
	int lastDrawn = 0;

	while (window.isOpen()) {

		/*
		*******************Handle Player's  Input****************
		*/

		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::KeyReleased && !paused) {
				//Listen for key presses
				acceptInput = true;

				//Hide the axe
				spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::Return)) {
			paused = false;

			//Reset the time bar
			score = 0;
			timeRemaining = 5;

			for (int i = 0; i < NUM_BRANCHES; i++) {
				branchPositions[i] = side::NONE;
			}

			//Position the gravestone offscreen
			spriteRIP.setPosition(675, 2000);

			//Position the player near tree
			spritePlayer.setPosition(580, 720);

			//Accept future inputs
			acceptInput = true;
		}

		//Wrap the player controls to ensure we are accepting player input
		if (acceptInput) {
			//First, handle input when right arrow key is pressed
			if (Keyboard::isKeyPressed(Keyboard::Right)) {
				//Ensure player is on the right side
				playerSide = side::RIGHT;
				score++;

				//Play chop sound
				chopSound.play();

				//Add to the amount of time remaining
				timeRemaining += 0.15;

				spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);

				spritePlayer.setPosition(1200, 720);

				updateBranches(score);

				//Set the log flying to the left
				spriteLog.setPosition(810, 720);
				logSpeedX = -5000;
				logActive = true;

				acceptInput = false;
			}

			if (Keyboard::isKeyPressed(Keyboard::Left)) {
				//Ensure player is on the left side
				playerSide = side::LEFT;
				score++;

				//Play chop sound
				chopSound.play();

				//Add to the amount of time remaining
				timeRemaining += 0.15;

				spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);

				spritePlayer.setPosition(580, 720);

				updateBranches(score);

				//Set the log flying to the left
				spriteLog.setPosition(810, 720);
				logSpeedX = 5000;
				logActive = true;

				acceptInput = false;
			}
		}
		
		/*
		********************Update the scene************************
		*/

		if (!paused) {

			Time dt = clock.restart();

			//Subtract the amount of time remaining
			timeRemaining -= dt.asSeconds();
			//Size up the time bar
			timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));

			if (timeRemaining <= 0.0f) {

				//Pause the game
				paused = true;

				//Change the message to Out of Time
				messageText.setString("Out of time!");

				//Play chop sound
				ootSound.play();

				//Reposition the text based on its new size
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(
					textRect.left + textRect.width / 2.0f,
					textRect.top + textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

			}

			//Set up the bee
			if (!beeActive) {

				//How fast is the bee?
				srand((int)time(0) * 10);
				beeSpeed = (rand() % 200) + 200;

				//How high is the bee?
				srand((int)time(0) * 10);
				float height = (rand() % 500) + 500;
				spriteBee.setPosition(2000, height);
				beeActive = true;

			}
			//Move the bee to the left edge, then prep for reset
			else {
				spriteBee.setPosition(
					spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()),
					spriteBee.getPosition().y);

				//Has the bee reached the left edge of the screen?
				if (spriteBee.getPosition().x < -100) {
					beeActive = false;
				}

			}

			//////////////////Manage the clouds////////////////

			for (int i = 0; i < NUM_CLOUDS; i++) {
				if (!cloudsActive[i]) {
					//How fast is the cloud
					srand((int)time(0) * i);
					cloudSpeeds[i] = (rand() % 200);

					//How high is the cloud
					srand((int)time(0) * i);
					float height = (rand() % 150);
					clouds[i].setPosition(-200, height);
					cloudsActive[i] = true;
				}
				else {
					clouds[i].setPosition(clouds[i].getPosition().x + (cloudSpeeds[i] * dt.asSeconds()), clouds[i].getPosition().y);
					//Has the cloud reached the right screen edge?
					if (clouds[i].getPosition().x > 1920) {
						cloudsActive[i] = false;
					}
				}
			}

			/* Obsolete cloud handling */
			/*
			if (!cloud1Active) {

				//How fast is the cloud?
				srand((int)time(0) * 10);
				cloud1Speed = (rand() % 200);

				//How high is the cloud?
				srand((int)time(0) * 10);
				float height = (rand() % 150);
				cloud1.setPosition(-200, height);
				cloud1Active = true;

			}
			else {
				cloud1.setPosition(
					cloud1.getPosition().x + (cloud1Speed * dt.asSeconds()),
					cloud1.getPosition().y);

				//Has the cloud reached the right edge of the screen?
				if (cloud1.getPosition().x > 1920) {
					cloud1Active = false;
				}
			}

			if (!cloud2Active) {

				//How fast is the cloud?
				srand((int)time(0) * 20);
				cloud2Speed = (rand() % 200);

				//How high is the cloud?
				srand((int)time(0) * 20);
				float height = (rand() % 150);
				cloud2.setPosition(-200, height);
				cloud2Active = true;

			}
			else {
				cloud2.setPosition(
					cloud2.getPosition().x + (cloud2Speed * dt.asSeconds()),
					cloud2.getPosition().y);

				//Has the cloud reached the right edge of the screen?
				if (cloud2.getPosition().x > 1920) {
					cloud2Active = false;
				}
			}

			if (!cloud3Active) {

				//How fast is the cloud?
				srand((int)time(0) * 30);
				cloud3Speed = (rand() % 200);

				//How high is the cloud?
				srand((int)time(0) * 30);
				float height = (rand() % 150);
				cloud3.setPosition(-200, height);
				cloud3Active = true;

			}
			else {
				cloud3.setPosition(
					cloud3.getPosition().x + (cloud3Speed * dt.asSeconds()),
					cloud3.getPosition().y);

				//Has the cloud reached the right edge of the screen?
				if (cloud3.getPosition().x > 1920) {
					cloud3Active = false;
				}
			}
			*/

			//Draw the score and fps text every 100 frames
			lastDrawn++;
			if (lastDrawn == 100) {

				//Update the score text
				std::stringstream ss;
				ss << "Score = " << score;
				scoreText.setString(ss.str());

				//Update the fps text
				std::stringstream ss2;
				ss2 << "FPS = " << 1/dt.asSeconds();
				fpsText.setString(ss2.str());

				//Reset the last drawn counter
				lastDrawn = 0;
			}

			//Update the branch sprites
			for (int i = 0; i < NUM_BRANCHES; i++) {

				float height = i * 150;

				if (branchPositions[i] == side::LEFT) {
					//Move the sprite to the left side and flip around
					branches[i].setPosition(610, height);
					branches[i].setOrigin(220, 40);
					branches[i].setRotation(180);
				}
				else if (branchPositions[i] == side::RIGHT) {
					//Move the sprite to the right side and leave sprite in place
					branches[i].setPosition(1330, height);
					branches[i].setOrigin(220, 40);
					branches[i].setRotation(0);
				}
				else {
					//Hide the branch
					branches[i].setPosition(3000, height);
				}
			}

			if (logActive) {
				spriteLog.setPosition(spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()),
					spriteLog.getPosition().y + (logSpeedY * dt.asSeconds()));

				//Has log reached the screen edge?
				if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 2000) {
					//Set it up to be a whole new log next frame
					logActive = false;
					spriteLog.setPosition(810, 720);
				}
				
			}

			//Has the player been squished?
			if (branchPositions[5] == playerSide) {
				//Player death
				paused = true;
				acceptInput = false;

				//Replace player sprite with gravestone
				spriteRIP.setPosition(525, 760);
				spritePlayer.setPosition(2000, 660);

				//Change the message text
				messageText.setString("Squished!");

				//Play death sound
				deathSound.play();

				//Center message on screen
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f, 1020 / 2.0f);
			}

		} //End if(!paused)

		/*
		*************************Draw the scene************************
		*/

		// Clear last frame
		window.clear();
		
		window.draw(spriteBackground);

		//window.draw(cloud1);
		//window.draw(cloud2);
		//window.draw(cloud3);

		for (int i = 0; i < NUM_CLOUDS; i++)
		{
			window.draw(clouds[i]);
		}

		window.draw(spriteTree2);
		window.draw(spriteTree3);
		window.draw(spriteTree4);
		window.draw(spriteTree5);
		window.draw(spriteTree6);

		for (int i = 0; i < NUM_BRANCHES; i++) {
			window.draw(branches[i]);
		}

		window.draw(spriteTree);

		window.draw(spritePlayer);

		window.draw(spriteAxe);

		window.draw(spriteLog);

		window.draw(spriteRIP);

		window.draw(spriteBee);

		window.draw(rect1);
		window.draw(rect2);

		window.draw(scoreText);
		window.draw(fpsText);

		window.draw(timeBar);

		if (paused) {
			window.draw(messageText);
		}

		window.display();
	}

    return 0;
}

//Function definition
void updateBranches(int seed) {

	//Move all the branches down one place
	for (int j = NUM_BRANCHES - 1; j > 0; j--) {
		branchPositions[j] = branchPositions[j - 1];
	}

	//Spawn a new branch at position 0: LEFT, RIGHT, or NONE
	srand((int)time(0) + seed);
	int r = (rand() % 5);
	switch (r) {
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