#include "../../headers/gui/button.hpp"


Button::Button(uint16_t initX, uint16_t initY, uint16_t initW, uint16_t initH, std::string initLabel, sf::Font *inFont) {
    this->xPos = initX;
    this->yPos = initY;
    this->width = initW;
    this->height = initH;
    this->state = STATE::NORMAL;

    this->label.setString(initLabel);
    this->label.setFont(*inFont);
    this->label.setCharacterSize(CHARACTER_SIZE);
    this->label.setFillColor(sf::Color::Black);
    this->label.setStyle(sf::Text::Regular);
    this->label.setPosition(this->xPos + 4, this->yPos);

    this->body.setSize(sf::Vector2f(this->width, this->height));
    this->body.setFillColor(sf::Color(75, 207, 250, 255));
    this->body.setOutlineColor(sf::Color(44, 62, 80, 255));
    this->body.setOutlineThickness(0);
    this->body.setPosition(this->xPos, this->yPos);
}

void Button::updateAndDraw(sf::RenderWindow *inWindow) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(*inWindow);

    if (mousePos.x > this->xPos && mousePos.y > this->yPos && mousePos.x < this->xPos + this->width && mousePos.y < this->yPos + this->height) {
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            this->state = STATE::CLICKED;
            this->body.setFillColor(sf::Color(7, 7, 7, 100));
        }
        else if (state == STATE::CLICKED) {
            this->state = STATE::RELEASED;
        }
        else {
            this->state = STATE::HOVERED;
            this->body.setFillColor(sf::Color(50, 50, 50, 100));
            this->body.setOutlineThickness(0.7);
        }
    }
    else {
        this->state = STATE::NORMAL;
        this->body.setFillColor(sf::Color(75, 207, 250, 255));
        this->body.setOutlineThickness(0);
    }

    inWindow->draw(this->body);
    inWindow->draw(this->label);
}
