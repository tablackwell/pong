#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>

class GameView{
public:
  GameView(sf::RenderWindow* window);
  void update();
  void updateLogic(float);
  void updateGraphics();
  void updateScore();
  void endScreen();
  void menuScreen();
  void init();
  bool getStatus();
  bool getMenuActive();
  bool isPlayerReady();
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
  sf::Font font;
  sf::Clock globalClock;
  std::string menuString;
  std::string scoreString;
  std::string resultsString;

  bool twoPlayerMode; //If two player mode is enabled... not yet implemented
  bool ballWaiting;
  bool playerReady;
  bool menuActive;
  bool done;
  int p1y;
  int p2y;
  int cdx; // Computer movement direction
  int bdx;
  int bdy;

  int p1Score;
  int p2Score;
  int cpuScore;
  float ballTimer;
};
