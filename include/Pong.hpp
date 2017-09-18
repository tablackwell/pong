
class Pong{

public:
  Pong();
  void run();
private: // functions
  void handleEvents();
private: //vars and objs
  sf::RenderWindow App;
  sf::Clock clock;
};
