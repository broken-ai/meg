#ifndef _RECT_WRAPPER_HPP_
#define _RECT_WRAPPER_HPP_

class RectWrapper {
public:
    uint16_t xPos;
    uint16_t yPos;
    uint16_t width;
    uint16_t height;
    sf::RectangleShape body;

    RectWrapper();
    RectWrapper(uint16_t initX, uint16_t initY, uint16_t initW, uint16_t initH, sf::Color color);
    void setPosition(uint16_t inX, uint16_t inY);
    void draw(sf::RenderWindow *inWindow);
};

#endif // _RECT_WRAPPER_HPP_
