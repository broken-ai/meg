#ifndef _VIEWS_HPP_
#define _VIEWS_HPP_

class View {
    /**

    Abstract view class

    */
public:
    virtual void handleEvent(sf::Event event, Map *editMap) = 0;
    virtual MODE run(sf::RenderWindow *window, RectWrapper *tileRect, Map *editMap, int &selectedTile, int &scrollX, int &scrollY) = 0;
};


#define PROP_BOX_COUNT 8


class PropertiesView : public View {
    /**

    Properties screen, for each layer

    */
public:
    RectWrapper propertiesBackground;
    Button *propertiesApplyButton;
    sf::Text *propertiesTitleText;
    sf::Text *propertiesWidthText;
    sf::Text *propertiesHeightText;
    sf::Text *propertiesXPosText;
    sf::Text *propertiesYPosText;
    sf::Text *propertiesXSpeedText;
    sf::Text *propertiesYSpeedText;
    sf::Text *propertiesXSpeedAutoText;
    sf::Text *propertiesYSpeedAutoText;
    sf::Text *propertiesXOffsetText;
    sf::Text *propertiesYOffsetText;
    Textbox *propertiesWidthTextBox;
    Textbox *propertiesHeightTextBox;
    Textbox *propertiesXSpeedTextBox;
    Textbox *propertiesYSpeedTextBox;
    Textbox *propertiesXSpeedAutoTextBox;
    Textbox *propertiesYSpeedAutoTextBox;
    Textbox *propertiesXOffsetTextBox;
    Textbox *propertiesYOffsetTextBox;
    Textbox *propertiesTextBoxes[PROP_BOX_COUNT];

    PropertiesView(sf::Font *guiFont);
    ~PropertiesView();
    void handleEvent(sf::Event event, Map *editMap);
    MODE run(sf::RenderWindow *window, RectWrapper *tileRect, Map *editMap, int &selectedTile, int &scrollX, int &scrollY);
};

//////////////////////////////
void PropertiesView::handleEvent(sf::Event event, Map *editMap) {}

MODE PropertiesView::run(sf::RenderWindow *window, RectWrapper *tileRect, Map *editMap, int &selectedTile, int &scrollX, int &scrollY) {
    this->propertiesBackground.draw(window);
    this->propertiesApplyButton->updateAndDraw(window);
    window->draw(*this->propertiesTitleText);
    window->draw(*this->propertiesWidthText);
    window->draw(*this->propertiesHeightText);
    window->draw(*this->propertiesXSpeedText);
    window->draw(*this->propertiesYSpeedText);
    window->draw(*this->propertiesXSpeedAutoText);
    window->draw(*this->propertiesYSpeedAutoText);
    window->draw(*this->propertiesXOffsetText);
    window->draw(*this->propertiesYOffsetText);
    return MODE::LAYER;
}

PropertiesView::PropertiesView(sf::Font *guiFont) {
    this->propertiesBackground = RectWrapper(240, 12, 320, 576, sf::Color(30, 39, 46, 255));

    // Properties's buttons
    this->propertiesApplyButton = new Button(this->propertiesBackground.xPos + 30, this->propertiesBackground.yPos + this->propertiesBackground.height - 2 * 17, 85, 18, "Apply & Close", guiFont);

    // Properties's texts
    this->propertiesTitleText = new sf::Text("Properties of layer 4", *guiFont, CHARACTER_SIZE + 5);
    this->propertiesWidthText = new sf::Text("Width: ", *guiFont, CHARACTER_SIZE + 3);
    this->propertiesHeightText = new sf::Text("Height: ", *guiFont, CHARACTER_SIZE + 3);
    this->propertiesXPosText = new sf::Text("Position on X axis: ", *guiFont, CHARACTER_SIZE + 3);
    this->propertiesYPosText = new sf::Text("Position on Y axis: ", *guiFont, CHARACTER_SIZE + 3);
    this->propertiesXSpeedText = new sf::Text("Speed on X axis: ", *guiFont, CHARACTER_SIZE + 3);
    this->propertiesYSpeedText = new sf::Text("Speed on Y axis: ", *guiFont, CHARACTER_SIZE + 3);
    this->propertiesXSpeedAutoText = new sf::Text("Auto speed on X axis: : ", *guiFont, CHARACTER_SIZE + 3);
    this->propertiesYSpeedAutoText = new sf::Text("Auto speed on Y axis: ", *guiFont, CHARACTER_SIZE + 3);
    this->propertiesXOffsetText = new sf::Text("Offset on X axis: ", *guiFont, CHARACTER_SIZE + 3);
    this->propertiesYOffsetText = new sf::Text("Offset on Y axis: ", *guiFont, CHARACTER_SIZE + 3);

    // Properties's text boxes
    this->propertiesWidthTextBox = new Textbox(true, this->propertiesBackground.xPos + this->propertiesBackground.width - 64, this->propertiesBackground.yPos + 4 * 16, 37, 16, guiFont);
    this->propertiesHeightTextBox = new Textbox(true, this->propertiesBackground.xPos + this->propertiesBackground.width - 64, this->propertiesBackground.yPos + 6 * 16, 37, 16, guiFont);
    this->propertiesXSpeedTextBox = new Textbox(true, this->propertiesBackground.xPos + this->propertiesBackground.width - 64, this->propertiesBackground.yPos + 8 * 16, 37, 16, guiFont);
    this->propertiesYSpeedTextBox = new Textbox(true, this->propertiesBackground.xPos + this->propertiesBackground.width - 64, this->propertiesBackground.yPos + 10 * 16, 37, 16, guiFont);
    this->propertiesXSpeedAutoTextBox = new Textbox(true, this->propertiesBackground.xPos + this->propertiesBackground.width - 64, this->propertiesBackground.yPos + 12 * 16, 37, 16, guiFont);
    this->propertiesYSpeedAutoTextBox = new Textbox(true, this->propertiesBackground.xPos + this->propertiesBackground.width - 64, this->propertiesBackground.yPos + 14 * 16, 37, 16, guiFont);
    this->propertiesXOffsetTextBox = new Textbox(true, this->propertiesBackground.xPos + this->propertiesBackground.width - 64, this->propertiesBackground.yPos + 16 * 16, 37, 16, guiFont);
    this->propertiesYOffsetTextBox = new Textbox(true, this->propertiesBackground.xPos + this->propertiesBackground.width - 64, this->propertiesBackground.yPos + 18 * 16, 37, 16, guiFont);

    /*
    this->propertiesTextBoxes = {
        this->propertiesWidthTextBox,
        this->propertiesHeightTextBox,
        this->propertiesXSpeedTextBox,
        this->propertiesYSpeedTextBox,
        this->propertiesXSpeedAutoTextBox,
        this->propertiesYSpeedAutoTextBox,
        this->propertiesXOffsetTextBox,
        this->propertiesYOffsetTextBox
    };
    */

    // Initialization
    // Setting limits for property inputs
    this->propertiesWidthTextBox->upperLimit = 8000;
    this->propertiesHeightTextBox->upperLimit = 8000;
    this->propertiesXSpeedTextBox->upperLimit = 30;
    this->propertiesYSpeedTextBox->upperLimit = 30;
    this->propertiesXSpeedAutoTextBox->upperLimit = 30;
    this->propertiesYSpeedAutoTextBox->upperLimit = 30;
    this->propertiesXOffsetTextBox->upperLimit = 64;
    this->propertiesYOffsetTextBox->upperLimit = 64;

    this->propertiesWidthTextBox->lowerLimit = 1;
    this->propertiesHeightTextBox->lowerLimit = 1;
    this->propertiesXSpeedTextBox->lowerLimit = -30;
    this->propertiesYSpeedTextBox->lowerLimit = -30;
    this->propertiesXSpeedAutoTextBox->lowerLimit = -30;
    this->propertiesYSpeedAutoTextBox->lowerLimit = -30;
    this->propertiesXOffsetTextBox->lowerLimit = -64;
    this->propertiesYOffsetTextBox->lowerLimit = -64;
}

PropertiesView::~PropertiesView() {}

#endif // _VIEWS_HPP_
