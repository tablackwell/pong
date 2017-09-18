#include <GameView.hpp>

GameView::GameView(sf::RenderWindow* view, GameLogic* logic){
  this->logic = logic;
  window = view;
}
