#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#ifndef blah
#define blah
class GameLogic{
public:
  GameLogic(sf::RenderWindow* window);

  void updateLogic(float);
  void updateGraphics();

  void endScreen();
  void menuScreen();
  void setWindow(sf::RenderWindow* app);

  void init();

  //Getters and setters
  bool getStatus();
  bool getMenuActive();
  bool isPlayerReady();
  bool getTwoPlayerMode();
  bool getSpeedupToggled();


private:
  void reset();
  void updateScore();
  void perturb();
  void resetBall();

private:
  sf::RenderWindow* window;
  sf::SoundBuffer buffer;
  sf::SoundBuffer buffer2;
  sf::SoundBuffer scoreBuffer;
  sf::SoundBuffer cpuScoreBuffer;
  sf::Sound blip;
  sf::Sound blip2;
  sf::Sound scoreSound;
  sf::Sound cpuScoreSound;

  sf::RectangleShape playerOne;
  sf::RectangleShape playerTwo;
  sf::CircleShape ball;

  sf::Text scoreboard;
  sf::Text onePlayerText;
  sf::Text twoPlayerText;
  sf::Text bulletin;
  sf::Text speedupText;
  sf::Font font;

  sf::Clock globalClock;

  std::string menuString;
  std::string scoreString;
  std::string resultsString;

  bool twoPlayerMode;
  bool ballWaiting;
  bool playerReady;
  bool speedupToggled;
  bool menuActive;
  bool done;

  int p1y;
  int p2y;
  int cdx; // Computer movement direction
  float bdx;
  float bdy;
  float ballSpeed;

  int p1Score;
  int p2Score;
  int cpuScore;

  float ballTimer;
};

#endif
