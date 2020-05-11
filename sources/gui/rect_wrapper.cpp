#include "../../headers/gui/rect_wrapper.hpp"


RectWrapper::RectWrapper() {}

RectWrapper::RectWrapper(uint16_t initX, uint16_t initY, uint16_t initW, uint16_t initH, sf::Color color) {
    this->xPos = initX;
    this->yPos = initY;
    this->width = initW;
    this->height = initH;

    this->body.setSize(sf::Vector2f(this->width, this->height));
    this->body.setFillColor(color);
    this->body.setOutlineColor(sf::Color(7, 7, 7, 255));
    this->body.setOutlineThickness(0.75);
    this->body.setPosition(this->xPos, this->yPos);
}

void RectWrapper::setPosition(uint16_t inX, uint16_t inY) {
    this->xPos = inX;
    this->yPos = inY;
    this->body.setPosition(this->xPos, this->yPos);
}

void RectWrapper::draw(sf::RenderWindow *inWindow) {
    inWindow->draw(this->body);
}
