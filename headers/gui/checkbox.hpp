#ifndef _CHECKBOX_HPP_
#define _CHECKBOX_HPP_

class Checkbox {
    public:
    uint16_t xPos;
    uint16_t yPos;
    STATE state;
    sf::Text label;
    sf::RectangleShape body;
    sf::RectangleShape mark;

    Checkbox(uint16_t initX, uint16_t initY, std::string initLabel, bool lLeft, sf::Font *inFont);
    void updateAndDraw(sf::RenderWindow *inWindow);

    private:
    bool buttonHeld;
};

#endif // _CHECKBOX_HPP_
