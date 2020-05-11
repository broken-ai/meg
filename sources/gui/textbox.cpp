#include "../../headers/gui/textbox.hpp"


Textbox::Textbox(bool isNumeric, uint16_t initX, uint16_t initY, uint16_t initW, uint8_t initH,sf::Font *inFont) {
    this->numeric = isNumeric;
    this->xPos = initX;
    this->yPos = initY;
    this->width = initW;
    this->height = initH;
    this->active = false;
    this->doStuff = false;
    this->inputString = "";
    this->charLimit = 4;
    this->upperLimit = 9999;
    this->lowerLimit = 0;

    this->text.setFont(*inFont);
    this->text.setCharacterSize(CHARACTER_SIZE);
    this->text.setFillColor(sf::Color::Black);
    this->text.setStyle(sf::Text::Regular);
    this->text.setPosition(this->xPos + 4, this->yPos);

    this->body.setSize(sf::Vector2f(this->width, this->height));
    this->body.setFillColor(sf::Color(210, 218, 226, 255));
    this->body.setOutlineColor(sf::Color(37, 37, 37, 255));
    this->body.setOutlineThickness(1);
    this->body.setPosition(this->xPos, this->yPos);
}

void Textbox::updateContent(sf::Event inEvent) {
    if (this->active) {
        if (!this->numeric) {
            if (inEvent.text.unicode == 0x08 && this->inputString.length() != 0) {
                this->inputString.pop_back();
                this->text.setString(this->inputString);
            } else if (inEvent.text.unicode == 0x0D) {
                this->doStuff = true;
            } else if (inEvent.text.unicode != 0x08) {
                this->inputString += inEvent.text.unicode;
                this->text.setString(this->inputString);
            }
        } else {
            if (inEvent.text.unicode == 0x08 && this->inputString.length() != 0) {
                this->inputString.pop_back();
                this->text.setString(this->inputString);
            } else if (inEvent.text.unicode == 0x0D) {
                this->doStuff = true;
            } else if ((inEvent.text.unicode >= 0x30 && inEvent.text.unicode <= 0x39)
                        || inEvent.text.unicode == 0x2e || inEvent.text.unicode == 0x2d) {
                this->inputString += inEvent.text.unicode;
                this->text.setString(this->inputString);
            }
        }
    }
}

void Textbox::updateAndDraw(sf::RenderWindow *inWindow) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(*inWindow);

    if (mousePos.x > this->xPos && mousePos.y > this->yPos && mousePos.x < this->xPos + width && mousePos.y < this->yPos + this->height) {
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            this->body.setFillColor(sf::Color(255, 255, 255, 255));
            this->active = true;
        }
    } else {
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            this->body.setFillColor(sf::Color(210, 218, 226, 255));
            this->active = false;
        }
    }

    inWindow->draw(this->body);
    inWindow->draw(this->text);
}

void Textbox::setString(std::string str) {
    this->inputString = str;
    this->text.setString(str);
}

