#include "../../headers/views/tiles_view.hpp"


TilesView::TilesView() {
    this->tileBackground = new RectWrapper(240, 12, 320, 576, sf::Color(30, 39, 46, 255));
    this->tileRow = 0;
}

void TilesView::handleEvent(sf::Event event, Map *editMap) {
    // Handle mouse scrolling
    if (event.type == sf::Event::MouseWheelScrolled && event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
        if(event.mouseWheelScroll.delta == 1 && this->tileRow > 0) {
            this->tileRow--;
        }
        else if (event.mouseWheelScroll.delta == -1 && this->tileRow + 17 < editMap->tileset.tileSprites.size() / 10 - 1) {
            this->tileRow++;
        }
    }
}

MODE TilesView::run(sf::RenderWindow *window, RectWrapper *tileRect, Map *editMap, int &selectedTile, int &scrollX, int &scrollY) {
    /*

    Fires when you open the tile menu

    */
    sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
    sf::Sprite currentSprite;
    this->tileBackground->draw(window);

    // Selection of a tile
    if (editMap->tileset.tileSprites.size() > 0) {
        uint16_t tilesEnd = 18 * 10;
        //if ((tileRow + 10) > editMap->tileset.tileSprites.size() / 10) tilesEnd = (tileRow + 10) * 10;
        for (uint16_t i = 0; i < tilesEnd; i++) {
            if (mousePos.x > (i % 10) * 32 + 240 && mousePos.x < (i % 10) * 32 + 240 + 32 &&
                    mousePos.y > (unsigned int)(i / 10) * 32 + 12 && mousePos.y < (unsigned int)(i / 10) * 32 + 32 + 12) {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    selectedTile = tileRow * 10 + i;
                }
                tileRect->setPosition(((mousePos.x - this->tileBackground->xPos) &~31) + this->tileBackground->xPos,
                                     ((mousePos.y - this->tileBackground->yPos) &~31) + this->tileBackground->yPos);
            }
            currentSprite = editMap->tileset.tileSprites[tileRow * 10 + i];
            currentSprite.setPosition((i % 10) * 32 + 240, (unsigned int)(i / 10) * 32 + 12);
            window->draw(currentSprite);
            tileRect->body.setFillColor(sf::Color(255, 255, 255, 1));
            tileRect->draw(window);
        }
    }

    return MODE::TILES;
}

TilesView::~TilesView() {}
