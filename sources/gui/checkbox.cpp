#include "../../headers/gui/checkbox.hpp"

Checkbox::Checkbox(uint16_t initX, uint16_t initY, std::string initLabel, bool lLeft, sf::Font *inFont) {
    uint8_t textW, textH;

    this->buttonHeld = false;
    this->state = STATE::UNCHECKED;
    this->xPos = initX;
    this->yPos = initY;
    this->label.setFont(*inFont);
    this->label.setCharacterSize(10);
    this->label.setFillColor(sf::Color::Black);
    this->label.setStyle(sf::Text::Regular);
    this->label.setString(initLabel);
    textW = label.getLocalBounds().width;
    textH = label.getLocalBounds().height;

	if (lLeft) {
        this->label.setOrigin(textW, textH / 2);
        this->label.setPosition(xPos - 12, yPos + 6);
    } else {
        this->label.setOrigin(0, textH / 2);
        this->label.setPosition(xPos + 24, yPos + 6);
    }

    this->body.setSize(sf::Vector2f(16, 16));
    this->body.setFillColor(sf::Color(210, 218, 226, 255));
    this->body.setOutlineColor(sf::Color(37, 37, 37, 255));
    this->body.setOutlineThickness(1);
    this->body.setPosition(this->xPos, this->yPos);

    this->mark.setSize(sf::Vector2f(10, 10));
    this->mark.setFillColor(sf::Color(200, 32, 32, 255));
    this->mark.setPosition(this->xPos + 3, this->yPos + 3);
}

void Checkbox::updateAndDraw(sf::RenderWindow *inWindow) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(*inWindow);
    if (mousePos.x > xPos && mousePos.y > yPos && mousePos.x < xPos + 16 && mousePos.y < yPos + 16) {
        if (!sf::Mouse::isButtonPressed(sf::Mouse::Left) && buttonHeld) {
            if (state == STATE::UNCHECKED) state = STATE::CHECKED;
            else state = STATE::UNCHECKED;
        }
        buttonHeld = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    }

    inWindow->draw(this->body);
    if (this->state == STATE::CHECKED) inWindow->draw(this->mark);
    inWindow->draw(label);
}
