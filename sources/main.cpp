/**

TODO:

- Sliders for camera scrolling
- Zooming (12.5%, 25%, 50%, 100%)
- Fix undo

*/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <physfs.h>
#include "physfs_stream.cpp"

using std::cout;
using std::endl;

#include "base.cpp"
#include "gui/gui.cpp"
#include "map/map.cpp"
#include "utils.cpp"


int main(const int argc, const char** argv) {
    setBaseDir(argv[0]);
    PHYSFS_init(argv[0]);

    sf::Event event;
    sf::View view;
    UndoAction currentAction;
    UndoAction prevAction;
    int undoBufferIndex = 1;
    int scrollX = 0;
    int scrollY = 0;
    int16_t tileRow = 0;
    int16_t eventRow = 0;
    uint16_t selectedTile = 0;
    uint16_t eventX = 0;
    uint16_t eventY = 0;
    uint16_t selectedEvent = 0;

    unsigned int selectStartX, selectStartY;
    unsigned int selectEndX, selectEndY;
    bool drawSelectionHighlight = false;
    std::vector<std::vector<uint16_t>> tileSelection;

    uint16_t currentLayer = 4;
    bool undoKeyHeld = false;
    bool redoKeyHeld = false;
    bool isCtrlYPushed = false;
    bool isCtrlZPushed = false;
    bool mouseHeld = false;
    bool TPEHeld = false;
    MODE mode = MODE::EDIT;
    sf::Vector2i mousePos;
    std::vector<UndoAction> undoBuffer;
    undoBuffer.push_back(UndoAction());
    Map* editMap = new Map();


    // Load font
    sf::Font *guiFont = new sf::Font();
    if (!guiFont->loadFromFile(BASE_FOLDER + "font/FreeSans.ttf")) {
        std::cout << "Cannot open font file." << std::endl;
        return EXIT_FAILURE;
    }

    // GUI elements
    sf::Text tileSetText = sf::Text("Tileset:", *guiFont);
    sf::Text layerText = sf::Text("Layer:", *guiFont);
    sf::Text camScrollText = sf::Text("Nothing to scroll on", *guiFont);
    sf::Text helpText = sf::Text(HELP_TEXT, *guiFont);
    sf::Text eventText1 = sf::Text("", *guiFont);
    sf::Text eventText2 = sf::Text("", *guiFont);

    RectWrapper bottomGUIRect(0, 600, WINDOW_WIDTH, 32, sf::Color(15, 188, 249, 255));
    RectWrapper shadowedTile(0, 0, 32, 32, sf::Color(30, 39, 46, 255));
    RectWrapper tileRect(0, 0, 32, 32, sf::Color(255, 255, 255, 64));
    RectWrapper eventRect(0, 0, 32, 32, sf::Color(64, 64, 127, 64));
    eventRect.body.setOutlineThickness(0);
    RectWrapper eventHighlight(256, 28, 288, 16, sf::Color(96, 96, 96, 64));
    eventHighlight.body.setOutlineThickness(0);
    RectWrapper tileBackground(240, 12, 320, 576, sf::Color(30, 39, 46, 255));
    RectWrapper propertiesBackground(240, 12, 320, 576, sf::Color(30, 39, 46, 255));
    RectWrapper eventBackground(256, 28, 288, 464, sf::Color(255, 255, 255, 255));
    RectWrapper saveBackground(240, 12, 320, 576, sf::Color(30, 39, 46, 255));

    Textbox tilesetTextBox(false, 52, 608, 150, 16, guiFont);
    Textbox layerTextBox(true, 370, 608, 17, 16, guiFont);

    Button tileMenuButton = Button(211, 608, 65, 16, "Tile menu", guiFont);
    Button loadMapButton = Button(511, 608, 66, 16, "Load map", guiFont);
    Button saveButton = Button(587, 608, 66, 16, "Save map", guiFont);
    Button minusLayerButton = Button(352, 608, 16, 16, "-", guiFont);
    Button plusLayerButton = Button(389, 608, 16, 16, "+", guiFont);
    Button layerPropertiesButton = Button(414, 608, 64, 16, "Properties", guiFont);

    // Properties' buttons
    Button propertiesApplyButton = Button(propertiesBackground.xPos + 30, propertiesBackground.yPos + propertiesBackground.height - 2 * 17,
                                            85, 18, "Apply & Close", guiFont);
    // Button propertiesCloseButton = Button(propertiesBackground.xPos + 30 + 85 + 15, propertiesBackground.yPos + propertiesBackground.height - 2 * 17, 40, 18, "Close", guiFont);

    // Properties' texts
    sf::Text propertiesTitleText = sf::Text("Properties of layer " + to_string(currentLayer), *guiFont, CHARACTER_SIZE + 5);
    sf::Text propertiesWidthText = sf::Text("Width: ", *guiFont, CHARACTER_SIZE + 3);
    sf::Text propertiesHeightText = sf::Text("Height: ", *guiFont, CHARACTER_SIZE + 3);
    sf::Text propertiesXPosText = sf::Text("Position on X axis: ", *guiFont, CHARACTER_SIZE + 3);
    sf::Text propertiesYPosText = sf::Text("Position on Y axis: ", *guiFont, CHARACTER_SIZE + 3);
    sf::Text propertiesXSpeedText = sf::Text("Speed on X axis: ", *guiFont, CHARACTER_SIZE + 3);
    sf::Text propertiesYSpeedText = sf::Text("Speed on Y axis: ", *guiFont, CHARACTER_SIZE + 3);
    sf::Text propertiesXSpeedAutoText = sf::Text("Auto speed on X axis: ", *guiFont, CHARACTER_SIZE + 3);
    sf::Text propertiesYSpeedAutoText = sf::Text("Auto speed on Y axis: ", *guiFont, CHARACTER_SIZE + 3);
    sf::Text propertiesXOffsetText = sf::Text("Offset on X axis: ", *guiFont, CHARACTER_SIZE + 3);
    sf::Text propertiesYOffsetText = sf::Text("Offset on Y axis: ", *guiFont, CHARACTER_SIZE + 3);
    sf::Text propertiesLoopXText = sf::Text("Loop X", *guiFont, CHARACTER_SIZE + 3);
    sf::Text propertiesLoopYText = sf::Text("Loop Y", *guiFont, CHARACTER_SIZE + 3);
    sf::Text eventParamText = sf::Text("Parameter:", *guiFont, CHARACTER_SIZE + 3);

    // Properties's text boxes
    Textbox propertiesWidthTextBox(true, propertiesBackground.xPos + propertiesBackground.width - 64,
                                        propertiesBackground.yPos + 4 * 16, 37, 16, guiFont);
    Textbox propertiesHeightTextBox(true, propertiesBackground.xPos + propertiesBackground.width - 64,
                                        propertiesBackground.yPos + 6 * 16, 37, 16, guiFont);
    Textbox propertiesXSpeedTextBox(true, propertiesBackground.xPos + propertiesBackground.width - 64,
                                        propertiesBackground.yPos + 8 * 16, 37, 16, guiFont);
    Textbox propertiesYSpeedTextBox(true, propertiesBackground.xPos + propertiesBackground.width - 64,
                                        propertiesBackground.yPos + 10 * 16, 37, 16, guiFont);
    Textbox propertiesXSpeedAutoTextBox(true, propertiesBackground.xPos + propertiesBackground.width - 64,
                                        propertiesBackground.yPos + 12 * 16, 37, 16, guiFont);
    Textbox propertiesYSpeedAutoTextBox(true, propertiesBackground.xPos + propertiesBackground.width - 64,
                                        propertiesBackground.yPos + 14 * 16, 37, 16, guiFont);
    Textbox propertiesXOffsetTextBox(true, propertiesBackground.xPos + propertiesBackground.width - 64,
                                        propertiesBackground.yPos + 16 * 16, 37, 16, guiFont);
    Textbox propertiesYOffsetTextBox(true, propertiesBackground.xPos + propertiesBackground.width - 64,
                                        propertiesBackground.yPos + 18 * 16, 37, 16, guiFont);
    Textbox eventParamTextBox(true, propertiesBackground.xPos + propertiesBackground.width - 64,
                                        propertiesBackground.yPos + 62 * 8, 37, 16, guiFont);

    Checkbox propertiesLoopLayerX(propertiesBackground.xPos + propertiesBackground.width - 64, propertiesBackground.yPos + 20 * 16, "", true, guiFont);
    Checkbox propertiesLoopLayerY(propertiesBackground.xPos + propertiesBackground.width - 64, propertiesBackground.yPos + 22 * 16, "", true, guiFont);

    // Setting limits for property inputs
    propertiesWidthTextBox.upperLimit = 8000;
    propertiesHeightTextBox.upperLimit = 8000;
    propertiesXSpeedTextBox.upperLimit = 30;
    propertiesYSpeedTextBox.upperLimit = 30;
    propertiesXSpeedAutoTextBox.upperLimit = 30;
    propertiesYSpeedAutoTextBox.upperLimit = 30;
    propertiesXOffsetTextBox.upperLimit = 64;
    propertiesYOffsetTextBox.upperLimit = 64;

    propertiesWidthTextBox.lowerLimit = 1;
    propertiesHeightTextBox.lowerLimit = 1;
    propertiesXSpeedTextBox.lowerLimit = -30;
    propertiesYSpeedTextBox.lowerLimit = -30;
    propertiesXSpeedAutoTextBox.lowerLimit = -30;
    propertiesYSpeedAutoTextBox.lowerLimit = -30;
    propertiesXOffsetTextBox.lowerLimit = -64;
    propertiesYOffsetTextBox.lowerLimit = -64;

    const int PROP_BOX_COUNT = 8;
    Textbox* propertiesTextBoxes[PROP_BOX_COUNT] = {
        &propertiesWidthTextBox,
        &propertiesHeightTextBox,
        &propertiesXSpeedTextBox,
        &propertiesYSpeedTextBox,
        &propertiesXSpeedAutoTextBox,
        &propertiesYSpeedAutoTextBox,
        &propertiesXOffsetTextBox,
        &propertiesYOffsetTextBox
    };


    // Initialize
    view.reset(sf::FloatRect(scrollX, scrollY, WINDOW_WIDTH, WINDOW_HEIGHT));
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "M.E.G. - Map Editor Gadget", sf::Style::Close);
    window.setFramerateLimit(60);
    window.resetGLStates();

    sf::Image* icon = new sf::Image();
    icon->loadFromFile(BASE_FOLDER + "media/icon.png");
    window.setIcon(64, 32, icon->getPixelsPtr());

    helpText.setPosition(0, 10);
    helpText.setColor(sf::Color(220, 220, 220, 255));
    helpText.setCharacterSize(CHARACTER_SIZE + 5);

    tileSetText.setCharacterSize(CHARACTER_SIZE);
    tileSetText.setFillColor(sf::Color::Black);
    tileSetText.setStyle(sf::Text::Regular);
    tileSetText.setPosition(8, 608);

    layerText.setCharacterSize(CHARACTER_SIZE);
    layerText.setFillColor(sf::Color::Black);
    layerText.setStyle(sf::Text::Regular);
    layerText.setPosition(312, 608);
    layerTextBox.setString(to_string(currentLayer));
    shadowedTile.body.setOutlineThickness(0);

    camScrollText.setCharacterSize(CHARACTER_SIZE);
    camScrollText.setFillColor(sf::Color::Black);
    camScrollText.setStyle(sf::Text::Regular);
    camScrollText.setPosition(700, 608);

    eventText1.setCharacterSize(12);
    eventText1.setFillColor(sf::Color::Black);
    eventText1.setStyle(sf::Text::Regular);
    eventText1.setPosition(0, 0);

    eventText2.setCharacterSize(9);
    eventText2.setFillColor(sf::Color::White);
    eventText2.setStyle(sf::Text::Regular);
    eventText2.setPosition(0, 0);

    propertiesTitleText.setStyle(sf::Text::Bold);
    propertiesTitleText.setPosition(propertiesBackground.xPos + 16, propertiesBackground.yPos + 16);

    propertiesWidthText.setPosition(propertiesBackground.xPos + 10, propertiesBackground.yPos + 4 * 16);
    propertiesHeightText.setPosition(propertiesBackground.xPos + 10, propertiesBackground.yPos + 6 * 16);
    propertiesXSpeedText.setPosition(propertiesBackground.xPos + 10, propertiesBackground.yPos + 8 * 16);
    propertiesYSpeedText.setPosition(propertiesBackground.xPos + 10, propertiesBackground.yPos + 10 * 16);
    propertiesXSpeedAutoText.setPosition(propertiesBackground.xPos + 10, propertiesBackground.yPos + 12 * 16);
    propertiesYSpeedAutoText.setPosition(propertiesBackground.xPos + 10, propertiesBackground.yPos + 14 * 16);
    propertiesXOffsetText.setPosition(propertiesBackground.xPos + 10, propertiesBackground.yPos + 16 * 16);
    propertiesYOffsetText.setPosition(propertiesBackground.xPos + 10, propertiesBackground.yPos + 18 * 16);
    propertiesLoopXText.setPosition(propertiesBackground.xPos + 10, propertiesBackground.yPos + 20 * 16);
    propertiesLoopYText.setPosition(propertiesBackground.xPos + 10, propertiesBackground.yPos + 22 * 16);
    eventParamText.setPosition(propertiesBackground.xPos + 128, propertiesBackground.yPos + 62 * 8);


    // Initialize editMap as an empty map
    for (int i = 0; i < 8; i++) {
        editMap->layers[i].width = (i + 1) * 20;
        editMap->layers[i].height = (i + 1) * 15;
        editMap->layers[i].xSpeed = i * 0.25;
        editMap->layers[i].ySpeed = i * 0.25;
        editMap->layers[i].xSpeedAuto = 0;
        editMap->layers[i].ySpeedAuto = 0;
        editMap->layers[i].xOffset = 0;
        editMap->layers[i].yOffset = 0;
        editMap->layers[i].loopX = false;
        editMap->layers[i].loopY = false;

        for (int j = 0; j < editMap->layers[i].height; j++) {
            editMap->layers[i].tiles.push_back((std::vector<uint16_t>){});
            if (i == 4) {
                editMap->layers[i].events.push_back((std::vector<Event>){});
            }
            for (int k = 0; k < editMap->layers[i].width; k++) {
                editMap->layers[i].tiles[j].push_back(0);
                if (i == 4) {
                    editMap->layers[i].events[j].push_back(Event(EVENT::NONE, 0));
                }
            }
        }
    }

    // main loop
    while (window.isOpen()) {

        // Handle events
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Mouse scrolling in tile menu
            if (event.type == sf::Event::MouseWheelScrolled && event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                if (mode == MODE::TILES) {
                    if(event.mouseWheelScroll.delta >= 1 && tileRow > 0) {
                        tileRow--;
                    }
                    else if (event.mouseWheelScroll.delta <= -1 && tileRow + 18 < editMap->tileset.tileSprites.size() / 10) {
                        tileRow++;
                    }
                } else if (mode == MODE::EVENT) {
                    if(event.mouseWheelScroll.delta >= 1 && eventRow > 0) {
                        eventRow--;
                    }
                    else if (event.mouseWheelScroll.delta <= -1 && eventRow + 29 < eventStrings1.size()) {
                        eventRow++;
                    }
                }
            }

            // Handle text entering event
            if (event.type == sf::Event::TextEntered) {
                // cout << std::hex << event.text.unicode << endl;

                tilesetTextBox.updateContent(event);
                layerTextBox.updateContent(event);

                if (mode == MODE::LAYER) {
                    for (int i = 0; i < PROP_BOX_COUNT; i++) {
                        propertiesTextBoxes[i]->updateContent(event);

                        // Cut the string to the char limit of the given class
                        if (propertiesTextBoxes[i]->inputString.length() > propertiesTextBoxes[i]->charLimit) {
                            std::string temp = "";
                            for (int j = 0; j < propertiesTextBoxes[i]->charLimit; j++) {
                                temp += propertiesTextBoxes[i]->inputString[j];
                            }
                            propertiesTextBoxes[i]->setString(to_string(std::atoi(temp.c_str())));
                        }
                    }
                }
                else if (mode == MODE::EVENT) {
                    eventParamTextBox.updateContent(event);
                }

                if (tilesetTextBox.active) {
                    if (tilesetTextBox.doStuff) {
                        if (!editMap->tileset.initialize(BASE_FOLDER + TILESET_FOLDER + tilesetTextBox.inputString + ".tct")) {
                            std::cout << "Error: cannot load tileset file.\n";
                        }
                        tilesetTextBox.body.setFillColor(sf::Color(210, 218, 226, 255));
                        tilesetTextBox.active = false;
                        tilesetTextBox.doStuff = false;
                    }
                }

                // Live checking of layer textbox
                if (layerTextBox.active) {
                    if (layerTextBox.inputString.length() > 1) {
                        layerTextBox.setString(to_string(layerTextBox.inputString[1]));
                    }

                    if (layerTextBox.inputString.empty()) {
                        layerTextBox.setString(to_string(currentLayer));
                    }
                    else if (std::atoi(layerTextBox.inputString.c_str()) > 7) {
                        layerTextBox.setString("7");
                        currentLayer = 7;
                    }
                    else if (std::atoi(layerTextBox.inputString.c_str()) < 0) {
                        layerTextBox.setString("0");
                        currentLayer = 0;
                    }
                    else {
                        layerTextBox.setString(layerTextBox.inputString);
                        currentLayer = std::atoi(layerTextBox.inputString.c_str());
                    }

                    // Live update
                    layerTextBox.updateAndDraw(&window);
                }

            }
        }

        mousePos = sf::Mouse::getPosition(window);
        int mouseTileX;
        int mouseTileY;
        if (mode == MODE::EDIT) {
            mouseTileX = scrollX + (int)(mousePos.x / 32);
            mouseTileY = scrollY + (int)(mousePos.y / 32);
        } else if (mode == MODE::TILES) {
            mouseTileX = (int)((mousePos.x - 240) / 32);
            mouseTileY = (int)((mousePos.y - 12) / 32);
        }

        if (mouseTileX < 0) mouseTileX = 0;
        if (mouseTileY < 0) mouseTileY = 0;

        // Set scrolling view
        window.clear(sf::Color(72, 84, 96, 255));

        // Draw the current layer
        editMap->drawLayer(&window, currentLayer, scrollX, scrollY, &shadowedTile, &eventRect, &eventText2);

        // Loading a map
        if (loadMapButton.state == STATE::RELEASED) {
            std::string mapFileName = askForString("What is the name of your map file?", guiFont);
            if (!mapFileName.empty()) {
                if (!editMap->load(BASE_FOLDER + MAP_FOLDER + mapFileName + ".tcm", &tilesetTextBox)) {
                    openMessageBox("Sorry we can't seem to find your map file ("
                                   + BASE_FOLDER + MAP_FOLDER + mapFileName + ".tcm" + ")!", guiFont);
                }
                else {
                    openMessageBox("Your map has been successfully loaded!", guiFont);
                }

            }
        }

        // Displaying help text
        if (mode == MODE::EDIT && editMap->tileset.tileSprites.size() == 0) {
            window.draw(helpText);
        }


        if (mode == MODE::EDIT && editMap->tileset.tileSprites.size() > 0) {
            /*

            Edit mode, where the layers can be edited.

            */
            // Layer minus and plus buttons
            if (minusLayerButton.state == STATE::RELEASED && currentLayer > 0) {
                currentLayer -= 1;
                layerTextBox.setString(to_string(currentLayer));
            }
            if (plusLayerButton.state == STATE::RELEASED && currentLayer < 7) {
                currentLayer += 1;
                layerTextBox.setString(to_string(currentLayer));
            }

            if (!drawSelectionHighlight && !tilesetTextBox.active && (tileMenuButton.state == STATE::RELEASED || (sf::Keyboard::isKeyPressed(sf::Keyboard::T) && !TPEHeld))) {
                mode = MODE::TILES;
            }

            if (!drawSelectionHighlight && !tilesetTextBox.active && (layerPropertiesButton.state == STATE::RELEASED || (sf::Keyboard::isKeyPressed(sf::Keyboard::P) && !TPEHeld))) {
                propertiesTitleText.setString("Properties of layer " + to_string(currentLayer));
                propertiesWidthTextBox.inputString = to_string(editMap->layers[currentLayer].width);
                propertiesHeightTextBox.inputString = to_string(editMap->layers[currentLayer].height);
                propertiesXSpeedTextBox.inputString = to_string(editMap->layers[currentLayer].xSpeed);
                propertiesYSpeedTextBox.inputString = to_string(editMap->layers[currentLayer].ySpeed);
                propertiesXSpeedAutoTextBox.inputString = to_string(editMap->layers[currentLayer].xSpeedAuto);
                propertiesYSpeedAutoTextBox.inputString = to_string(editMap->layers[currentLayer].ySpeedAuto);
                propertiesXOffsetTextBox.inputString = to_string(editMap->layers[currentLayer].xOffset);
                propertiesYOffsetTextBox.inputString = to_string(editMap->layers[currentLayer].yOffset);

                propertiesLoopLayerX.state = editMap->layers[currentLayer].loopX == true ? STATE::CHECKED : STATE::UNCHECKED;
                propertiesLoopLayerY.state = editMap->layers[currentLayer].loopY == true ? STATE::CHECKED : STATE::UNCHECKED;

                mode = MODE::LAYER;
            }

            if (!drawSelectionHighlight && !tilesetTextBox.active && (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && !TPEHeld)) {
                eventX = mouseTileX;
                eventY = mouseTileY;
                selectedEvent = (uint16_t)(editMap->layers[4].events[mouseTileY][mouseTileX].type);
                eventParamTextBox.setString(to_string(editMap->layers[4].events[mouseTileY][mouseTileX].parameter));
                mode = MODE::EVENT;
            }

            sf::Sprite currentSprite;

            // Placing tiles
            if (mouseTileX >= 0 && mouseTileX < editMap->layers[currentLayer].width && mouseTileY >= 0 && mouseTileY < editMap->layers[currentLayer].height) {

                // Various actions with the mouse
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) {
                        selectedTile = editMap->layers[currentLayer].tiles[mouseTileY][mouseTileX];
                    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)) {
                        if (!mouseHeld) {
                            tileSelection.clear();
                            selectStartX = mouseTileX;
                            selectStartY = mouseTileY;
                        }
                        drawSelectionHighlight = true;
                    } else if (mousePos.y < 600) {
                        currentAction = UndoAction(mouseTileX, mouseTileY, currentLayer, selectedTile, editMap->layers[currentLayer].tiles[mouseTileY][mouseTileX]);
                        for (int i = 0; i < tileSelection.size(); i++) {
                            for (int j = 0; j < tileSelection[i].size(); j++) {
                                if (mouseTileX + j < editMap->layers[currentLayer].width && mouseTileY + i < editMap->layers[currentLayer].height) editMap->layers[currentLayer].tiles[mouseTileY + i][mouseTileX + j] = tileSelection[i][j];
                            }
                        }
                    }
                } else {
                    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && mouseHeld) || (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift) && mouseHeld)) {
                        selectEndX = mouseTileX;
                        selectEndY = mouseTileY;

                        if (selectEndX < selectStartX) {
                            int tempX = selectStartX;
                            selectStartX = selectEndX;
                            selectEndX = tempX;
                        }

                        if (selectEndY < selectStartY) {
                            int tempY = selectStartY;
                            selectStartY = selectEndY;
                            selectEndY = tempY;
                        }

                        drawSelectionHighlight = false;

                        for (int i = selectStartY; i <= selectEndY; i++) {
                            tileSelection.push_back((std::vector<uint16_t>){});
                            for (int j = selectStartX; j <= selectEndX; j++) {
                                tileSelection.back().push_back(editMap->layers[currentLayer].tiles[i][j]);
                            }
                        }
                    }
                }

                // Remove the current tile
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace)) {
                    currentAction = UndoAction(mouseTileX, mouseTileY, currentLayer, 0, editMap->layers[currentLayer].tiles[mouseTileY][mouseTileX]);
                    editMap->layers[currentLayer].tiles[mouseTileY][mouseTileX] = 0;
                }

                /*if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && !TPEHeld && currentLayer == 4) {
                    unsigned int temp = (unsigned int)editMap->layers[4].events[mouseTileY][mouseTileX].type;
                    temp++;
                    editMap->layers[4].events[mouseTileY][mouseTileX].type = (EVENT)temp;
                }*/
            }


            // Scrolling on X axis
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                scrollX++;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && scrollX > 0) {
                scrollX--;
            }

            // Scrolling on Y axis
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && scrollY > 0) {
                scrollY--;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                scrollY++;
            }

            camScrollText.setString("scrollX: " + to_string(scrollX) + ", scrollY: " + to_string(scrollY));


            /** TODO */
            // Undo last action
            isCtrlZPushed = sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && (sf::Keyboard::isKeyPressed(sf::Keyboard::RControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::LControl));

            if (!undoKeyHeld && isCtrlZPushed) {
                prevAction = undoBuffer[undoBufferIndex - 1];
                if (prevAction.initialized) {
                    editMap->layers[prevAction.layerIndex].tiles[prevAction.yPos][prevAction.xPos] = prevAction.prevTileIndex;
                    undoBufferIndex--;
                }
                undoKeyHeld = true;
            }
            else if (!isCtrlZPushed) {
                undoKeyHeld = false;

                // Check if correct action is a duplicate of the last action
                if (currentAction.initialized && currentAction != undoBuffer[undoBufferIndex - 1]) {
                    undoBuffer.push_back(currentAction);
                    currentAction = UndoAction();
                    undoBufferIndex = undoBuffer.size();
                }
            }

            /*
            isCtrlYPushed = sf::Keyboard::isKeyPressed(sf::Keyboard::Y) && (sf::Keyboard::isKeyPressed(sf::Keyboard::RControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::LControl));

            if (!redoKeyHeld && isCtrlYPushed) {
                if (undoBufferIndex < undoBuffer.size()) {

                    undoBufferIndex--;
                }
                redoKeyHeld = true;
            }
            else if (!isCtrlYPushed) {
                redoKeyHeld = false;
            }
            */
            /** /TODO */

            // Save the map
            if (saveButton.state == STATE::RELEASED) {
                std::string givenName = askForString("Please give a name to your map.", guiFont);
                if (!givenName.empty()) {
                    if (!editMap->save(MAP_FOLDER + givenName + ".tcm", tilesetTextBox.inputString + ".tct")) {
                        openMessageBox("ERROR: Map cannot be saved.", guiFont);
                    }
                    else {
                        openMessageBox("Your map has been successfully saved!", guiFont);
                    }
                }
            }

            currentSprite = editMap->tileset.tileSprites[selectedTile];
            currentSprite.setPosition(mousePos.x &~31, mousePos.y &~31);
            tileRect.setPosition(mousePos.x &~31, mousePos.y &~31);
            if (!drawSelectionHighlight) {
                //window.draw(currentSprite);
                for (int i = 0; i < tileSelection.size(); i++) {
                    for (int j = 0; j < tileSelection[i].size(); j++) {
                        if (tileSelection[i][j] != 0) {
                            currentSprite = editMap->tileset.tileSprites[tileSelection[i][j]];
                            currentSprite.setPosition((mouseTileX - scrollX + j) * 32, (mouseTileY - scrollY + i) * 32);
                            window.draw(currentSprite);
                        }
                    }
                }
            } else {
                for (int i = (selectStartX > mouseTileX ? mouseTileX : selectStartX) - scrollX; i <= (selectStartX > mouseTileX ? selectStartX : mouseTileX) - scrollX; i++) {
                    for (int j = (selectStartY > mouseTileY ? mouseTileY : selectStartY) - scrollY; j <= (selectStartY > mouseTileY ? selectStartY : mouseTileY) - scrollY; j++) {
                        tileRect.setPosition(i * 32, j * 32);
                        tileRect.draw(&window);
                    }
                }
            }
            tileRect.body.setFillColor(sf::Color(255, 255, 255, 64));
            tileRect.draw(&window);
        }
        else if (mode == MODE::TILES) {
            /*

            Fires when you open the tile menu

            */
            if (tileMenuButton.state == STATE::RELEASED || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) || (sf::Keyboard::isKeyPressed(sf::Keyboard::T) && !TPEHeld && !tilesetTextBox.active)) {
                mode = MODE::EDIT;
            }

            sf::Sprite currentSprite;
            tileBackground.draw(&window);

            // Selection of a tile
            if (editMap->tileset.tileSprites.size() > 0) {
                uint16_t tilesEnd = 18;

                if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && mousePos.x > tileBackground.xPos && mousePos.x < tileBackground.xPos + tileBackground.width && mousePos.y > tileBackground.yPos && mousePos.y < tileBackground.yPos + tileBackground.height) {
                    if (!mouseHeld) {
                        tileSelection.clear();

                        if (mouseTileX < 0) selectStartX = 0;
                        else if (mouseTileX > 9) selectStartX = 9;
                        else selectStartX = mouseTileX;

                        if (mouseTileY < 0) selectStartY = 0;
                        else if (mouseTileY > tilesEnd - 1) selectStartY = tilesEnd - 1;
                        else selectStartY = mouseTileY;

                        drawSelectionHighlight = true;
                    }
                } else if (mousePos.x > tileBackground.xPos && mousePos.x < tileBackground.xPos + tileBackground.width && mousePos.y > tileBackground.yPos && mousePos.y < tileBackground.yPos + tileBackground.height) {
                    if (mouseHeld) {
                        if (mouseTileX < 0) selectEndX = 0;
                        else if (mouseTileX > 9) selectEndX = 9;
                        else selectEndX = mouseTileX;

                        if (mouseTileY < 0) selectEndY = 0;
                        else if (mouseTileY > tilesEnd - 1) selectEndY = tilesEnd - 1;
                        else selectEndY = mouseTileY;

                        if (selectStartX > selectEndX) {
                            int tempX = selectStartX;
                            selectStartX = selectEndX;
                            selectEndX = tempX;
                        }
                        if (selectStartY > selectEndY) {
                            int tempY = selectStartY;
                            selectStartY = selectEndY;
                            selectEndY = tempY;
                        }

                        drawSelectionHighlight = false;

                        for (int k = selectStartY; k <= selectEndY; k++) {
                            tileSelection.push_back((std::vector<uint16_t>){});
                            for (int l = selectStartX; l <= selectEndX; l++) {
                                tileSelection.back().push_back((tileRow + k) * 10 + l);
                            }
                        }
                    }
                }

                for (uint16_t i = 0; i < tilesEnd; i++) {
                    for (uint16_t j = 0; j < 10; j++) {
                        currentSprite = editMap->tileset.tileSprites[(tileRow + i) * 10 + j];
                        currentSprite.setPosition(j * 32 + 240, i * 32 + 12);
                        window.draw(currentSprite);
                    }
                }

                if (drawSelectionHighlight) {
                    for (int i = (selectStartX > mouseTileX ? mouseTileX : selectStartX); i <= (selectStartX > mouseTileX ? selectStartX : (mouseTileX > 9 ? 9 : mouseTileX)); i++) {
                        for (int j = (selectStartY > mouseTileY ? mouseTileY : selectStartY); j <= (selectStartY > mouseTileY ? selectStartY : (mouseTileY > tilesEnd - 1 ? tilesEnd - 1 : mouseTileY)); j++) {
                            tileRect.setPosition(i * 32 + 240, j * 32 + 12);
                            tileRect.draw(&window);
                        }
                    }
                }
            }
        }
        else if (mode == MODE::LAYER) {
            /*

            Properties menu is opened

            */
            if (layerPropertiesButton.state == STATE::RELEASED || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) || (sf::Keyboard::isKeyPressed(sf::Keyboard::P) && !TPEHeld && !tilesetTextBox.active)) {
                mode = MODE::EDIT;
            }

            if (propertiesApplyButton.state == STATE::RELEASED) {
                mode = MODE::EDIT;

                // Safety check for each textbox
                for (int i = 0; i < PROP_BOX_COUNT; i++) {
                    if (std::atoi(propertiesTextBoxes[i]->inputString.c_str()) > propertiesTextBoxes[i]->upperLimit) {
                        propertiesTextBoxes[i]->inputString = to_string(propertiesTextBoxes[i]->upperLimit);
                    }
                    else if (std::atoi(propertiesTextBoxes[i]->inputString.c_str()) < propertiesTextBoxes[i]->lowerLimit) {
                        propertiesTextBoxes[i]->inputString = to_string(propertiesTextBoxes[i]->lowerLimit);
                    }
                    propertiesTextBoxes[i]->setString(to_string(std::atof(propertiesTextBoxes[i]->inputString.c_str())));
                }

                // Update layer properties
                editMap->layers[currentLayer].width = std::atoi(propertiesWidthTextBox.inputString.c_str());
                editMap->layers[currentLayer].height = std::atoi(propertiesHeightTextBox.inputString.c_str());
                editMap->layers[currentLayer].xSpeed = std::stof(propertiesXSpeedTextBox.inputString.c_str());
                editMap->layers[currentLayer].ySpeed = std::stof(propertiesYSpeedTextBox.inputString.c_str());
                editMap->layers[currentLayer].xSpeedAuto = std::stof(propertiesXSpeedAutoTextBox.inputString.c_str());
                editMap->layers[currentLayer].ySpeedAuto = std::stof(propertiesYSpeedAutoTextBox.inputString.c_str());
                editMap->layers[currentLayer].xOffset = std::atoi(propertiesXOffsetTextBox.inputString.c_str());
                editMap->layers[currentLayer].yOffset = std::atoi(propertiesYOffsetTextBox.inputString.c_str());
                editMap->layers[currentLayer].loopX = propertiesLoopLayerX.state == STATE::CHECKED ? true : false;
                editMap->layers[currentLayer].loopY = propertiesLoopLayerY.state == STATE::CHECKED ? true : false;

                unsigned int prevWidth = editMap->layers[currentLayer].tiles[0].size();
                unsigned int prevHeight = editMap->layers[currentLayer].tiles.size();

                // Update the height of the layer
                if (prevHeight > editMap->layers[currentLayer].height) {
                    for (int i = prevHeight; i > editMap->layers[currentLayer].height; i--) {
                        editMap->layers[currentLayer].tiles.pop_back();
                        if (currentLayer == 4) {
                            editMap->layers[4].events.pop_back();
                        }
                    }
                }
                else if (prevHeight < editMap->layers[currentLayer].height) {
                    for (int i = prevHeight; i < editMap->layers[currentLayer].height; i++) {
                        editMap->layers[currentLayer].tiles.push_back((std::vector<uint16_t>){});
                        if (currentLayer == 4) {
                            editMap->layers[4].events.push_back((std::vector<Event>){});
                        }
                        for (int j = 0; j < editMap->layers[currentLayer].width; j++) {
                            editMap->layers[currentLayer].tiles[i].push_back(0);
                            if (currentLayer == 4) {
                                editMap->layers[4].events[i].push_back(Event(EVENT::NONE, 0));
                            }
                        }
                    }
                }

                // Update the width of the layer
                if (prevWidth > editMap->layers[currentLayer].width) {
                    for (int i = 0; i < editMap->layers[currentLayer].height; i++) {
                        for (int j = prevWidth; j > editMap->layers[currentLayer].width; j--) {
                            editMap->layers[currentLayer].tiles[i].pop_back();
                            if (currentLayer == 4) {
                                editMap->layers[4].events[i].pop_back();
                            }
                        }
                    }
                } else if (prevWidth < editMap->layers[currentLayer].width) {
                    for (int i = 0; i < editMap->layers[currentLayer].height; i++) {
                        for (int j = prevWidth; j < editMap->layers[currentLayer].width; j++) {
                            editMap->layers[currentLayer].tiles[i].push_back(0);
                            if (currentLayer == 4) {
                                editMap->layers[4].events[i].push_back(Event(EVENT::NONE, 0));
                            }
                        }
                    }
                }
            }

            // Draw GUI elements
            propertiesBackground.draw(&window);
            propertiesApplyButton.updateAndDraw(&window);
            window.draw(propertiesTitleText);
            window.draw(propertiesWidthText);
            window.draw(propertiesHeightText);
            window.draw(propertiesXSpeedText);
            window.draw(propertiesYSpeedText);
            window.draw(propertiesXSpeedAutoText);
            window.draw(propertiesYSpeedAutoText);
            window.draw(propertiesXOffsetText);
            window.draw(propertiesYOffsetText);
            window.draw(propertiesLoopXText);
            window.draw(propertiesLoopYText);
            for (int i = 0; i < PROP_BOX_COUNT; i++) {
                propertiesTextBoxes[i]->updateAndDraw(&window);
                propertiesTextBoxes[i]->setString(propertiesTextBoxes[i]->inputString);
            }
            propertiesLoopLayerX.updateAndDraw(&window);
            propertiesLoopLayerY.updateAndDraw(&window);
        }
        else if (mode == MODE::EVENT) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) || (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && !TPEHeld)) {
                mode = MODE::EDIT;
            }

            if (propertiesApplyButton.state == STATE::RELEASED) {
                editMap->layers[4].events[eventY][eventX].type = (EVENT)selectedEvent;
                editMap->layers[4].events[eventY][eventX].parameter = std::atoi(eventParamTextBox.inputString.c_str());

                mode = MODE::EDIT;
            }

            propertiesBackground.draw(&window);
            eventBackground.draw(&window);
            eventHighlight.setPosition(eventBackground.xPos, eventBackground.yPos + (selectedEvent - eventRow) * 16);
            for (uint16_t i = 0; i < (eventStrings1.size() < 29 ? eventStrings1.size() : 29); i++) {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && mousePos.x > eventBackground.xPos && mousePos.x < eventBackground.xPos + eventBackground.width && mousePos.y > eventBackground.yPos + i * 16 && mousePos.y < eventBackground.yPos + i * 16 + 16) {
                    selectedEvent = eventRow + i;
                }

                if (eventRow + i == selectedEvent) {
                    eventHighlight.draw(&window);
                }

                eventText1.setString(eventStrings1[eventRow + i]);
                eventText1.setPosition(propertiesBackground.xPos + 32, propertiesBackground.yPos + 16 +  i * 16);
                window.draw(eventText1);
            }
            window.draw(eventParamText);
            eventParamTextBox.updateAndDraw(&window);
            propertiesApplyButton.updateAndDraw(&window);
        }

        mouseHeld = sf::Mouse::isButtonPressed(sf::Mouse::Left);
        TPEHeld = sf::Keyboard::isKeyPressed(sf::Keyboard::T) || sf::Keyboard::isKeyPressed(sf::Keyboard::P) || sf::Keyboard::isKeyPressed(sf::Keyboard::E);

        // Draw GUI elements
        bottomGUIRect.draw(&window);
        tilesetTextBox.updateAndDraw(&window);
        layerTextBox.updateAndDraw(&window);
        tileMenuButton.updateAndDraw(&window);
        loadMapButton.updateAndDraw(&window);
        minusLayerButton.updateAndDraw(&window);
        plusLayerButton.updateAndDraw(&window);
        layerPropertiesButton.updateAndDraw(&window);
        saveButton.updateAndDraw(&window);
        window.draw(layerText);
        window.draw(camScrollText);
        window.draw(tileSetText);
        window.display();
    }
    PHYSFS_deinit();
    return EXIT_SUCCESS;
}
