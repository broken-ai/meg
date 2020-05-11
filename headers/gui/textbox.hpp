#ifndef _TEXTBOX_HPP_
#define _TEXTBOX_HPP_

class Textbox {
public:
    bool numeric;
    uint16_t xPos;
    uint16_t yPos;
    uint16_t width;
    uint8_t height;
    bool active;
    bool doStuff;
    int upperLimit;
    int lowerLimit;
    unsigned int charLimit;
    std::string inputString;
    sf::Text text;
    sf::RectangleShape body;

    Textbox(bool isNumeric, uint16_t initX, uint16_t initY, uint16_t initW, uint8_t initH, sf::Font *inFont);
    void updateContent(sf::Event inEvent);
    void updateAndDraw(sf::RenderWindow *inWindow);
    void setString(std::string str);
};

#endif // _TEXTBOX_HPP_
