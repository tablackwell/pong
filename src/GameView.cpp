/*
Please read:

Unfortunately this program is not set up quite right, so
GameView does nothing on its own quite yet. In the next release, this will
be altered so that GameView and GameLogic do the appropriate things.

*/

#include <GameView.hpp>

GameView::GameView(sf::RenderWindow* view, GameLogic* logic){
  this->logic = logic;
  window = view;
}
