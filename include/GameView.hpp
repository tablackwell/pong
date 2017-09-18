#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <GameLogic.hpp>

class GameView{
public:
  GameView(sf::RenderWindow* view, GameLogic* logic);
  void setWindow(sf::RenderWindow* app);
  void menuScreen();
  void updateGraphics();
private:

private:
  sf::RenderWindow* window;
  GameLogic* logic;

  sf::RectangleShape* playerOne;
  sf::RectangleShape* playerTwo;
  sf::CircleShape* ball;

  sf::Text* scoreboard;
  sf::Text* onePlayerText;
  sf::Text* twoPlayerText;
  sf::Text* bulletin;
  sf::Text* speedupText;
  sf::Font* font;

  sf::Clock* globalClock;
  std::string* menuString;
  std::string* scoreString;
  std::string* resultsString;

};
