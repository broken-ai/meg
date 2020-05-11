#ifndef _BUTTON_HPP_
#define _BUTTON_HPP_

class Button {
public:
    uint16_t xPos;
    uint16_t yPos;
    uint16_t width;
    uint16_t height;
    STATE state;
    sf::Text label;
    sf::RectangleShape body;

    Button(uint16_t initX, uint16_t initY, uint16_t initW, uint16_t initH, std::string initLabel, sf::Font *inFont);
    void updateAndDraw(sf::RenderWindow *inWindow);
};

#endif // _BUTTON_HPP_
