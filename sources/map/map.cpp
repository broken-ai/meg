#include "../../headers/map/map.hpp"
#include "map_utils.cpp"


Tileset::Tileset() {}

Tileset::~Tileset() {
    delete &this->texture;
    delete &this->mask;
    delete &this->tileSprites;
}

bool Tileset::initialize(std::string filepath) {
    bool success = true;

    this->mask.clear();
    this->tileSprites.clear();

    PHYSFS_addToSearchPath(filepath.c_str(), 1);
    PhysFsStream tctStream;

    // Loading image
    tctStream.open("image.png");
    if (!this->texture.loadFromStream(tctStream)) success = false;
    tctStream.close();
    if (!success) return false;
    sf::Vector2u imgSize = this->texture.getSize();

    // Loading mask
    unsigned int bufferSize = (imgSize.x + 2) * imgSize.y;
    char buffer[bufferSize];
    tctStream.open("mask.msk");
    tctStream.read(buffer, bufferSize);
    tctStream.close();

    // Convert buffer to boolean mask
    std::vector<bool> maskRow;

    for (unsigned int i = 0; i < bufferSize; i++) {
        if (buffer[i] == '\n' && maskRow.size() != 0) {
            this->mask.push_back(maskRow);
            maskRow.clear();
        }

        if (buffer[i] == '0' || buffer[i] == '1') {
            maskRow.push_back(buffer[i] == '1');
        }
    }

    for (unsigned int j = 0; j < (unsigned int)(imgSize.y / 32); j++) {
        for (unsigned int i = 0; i < 10; i++) {
            //this->tileSprites[j * 10 + i].setTexture(this->texture);
            //this->tileSprites[j * 10 + i].setTextureRect(sf::IntRect(i * 32, j * 32, 32, 32));
            this->tileSprites.push_back(sf::Sprite(this->texture, sf::IntRect(i * 32, j * 32, 32, 32)));
        }
    }
    return true;
}

Map::Map() {}

Map::~Map() {
    delete[] &this->layers[0];
    delete[] &this->layers[1];
    delete[] &this->layers[2];
    delete[] &this->layers[3];
    delete[] &this->layers[4];
    delete[] &this->layers[5];
    delete[] &this->layers[6];
    delete[] &this->layers[7];
    delete &this->tileset;
    delete &this->name;
}

void Map::drawLayer(sf::RenderWindow* inWindow, uint8_t inLayer, int inCamX, int inCamY, RectWrapper* borderRect, RectWrapper* eventRect, sf::Text* inText) {
    int startX;
    int startY;
    int endX;
    int endY;

    sf::Sprite currentSprite;

    startX = (int)(inCamX - 1);
    if (startX < 0) startX = 0;
    startY = (int)(inCamY - 1);
    if (startY < 0) startY = 0;

    endX = (int)(inCamX + 800 / 32 + 1);
    if (endX > this->layers[inLayer].width - 1) endX = this->layers[inLayer].width - 1;
    endY = (int)(inCamY + 600 / 32 + 1);
    if (endY > this->layers[inLayer].height - 1) endY = this->layers[inLayer].height - 1;

    for (int i = startY; i <= (int)(inCamY + 600 / 32 + 1); i++) {
        for (int j = startX; j <= (int)(inCamX + 800 / 32 + 1); j++) {
            if (i <= endY && j <= endX) {
                if (this->layers[inLayer].tiles[i][j] != 0) {
                    currentSprite = this->tileset.tileSprites[this->layers[inLayer].tiles[i][j]];
                    currentSprite.setPosition((j * 32) - inCamX * 32, (i * 32) - inCamY * 32);
                    inWindow->draw(currentSprite);
                }

                if (inLayer == 4 && this->layers[4].events[i][j].type != EVENT::NONE) {
                    eventRect->setPosition((j * 32) - inCamX * 32, (i * 32) - inCamY * 32);
                    eventRect->draw(inWindow);

                    inText->setPosition((j * 32) - inCamX * 32, (i * 32) - inCamY * 32);
                    inText->setString(eventStrings2[(int)this->layers[4].events[i][j].type]);
                    inWindow->draw(*inText);
                }
            } else {
                borderRect->setPosition((j * 32) - inCamX * 32, (i * 32) - inCamY * 32);
                borderRect->draw(inWindow);
            }
        }
    }
}

std::vector<uint16_t> Map::parseTileRow(std::string line) {
    std::vector<uint16_t> row;
    std::string currentNum = "";

    for (int i = 0; i < line.length(); i++) {
        if (line[i] == ' ' || line[i] == '\t') {
            continue;
        }

        if (line[i] == ',') {
            try {
                row.push_back((uint16_t) std::stoi(currentNum));
            }
            catch (std::exception) {}
            currentNum = "";
            continue;
        }

        currentNum += line[i];
    }

    // Save the last one too
    try {
        row.push_back(std::stoi(currentNum));
    }
    catch (std::exception) {}
    return row;
}

bool Map::setLayerProperty(Layer *layer, std::string key, std::string value) {
    if (key.empty() || value.empty()) {  // Check them first
        return false;
    }

    if (key == "width") {
        layer->width = std::strtol(value.c_str(), NULL, 0);
    }
    else if (key == "height") {
        layer->height = std::strtol(value.c_str(), NULL, 0);
    }
    else if (key == "xSpeed") {
        layer->xSpeed = std::stof(value.c_str(), NULL);
    }
    else if (key == "ySpeed") {
        layer->ySpeed = std::stof(value.c_str(), NULL);
    }
    else if (key == "xSpeedAuto") {
        layer->xSpeedAuto = std::stof(value.c_str(), NULL);
    }
    else if (key == "ySpeedAuto") {
        layer->ySpeedAuto = std::stof(value.c_str(), NULL);
    }
    else if (key == "xOffset") {
        layer->xOffset = std::strtol(value.c_str(), NULL, 0);
    }
    else if (key == "yOffset") {
        layer->yOffset = std::strtol(value.c_str(), NULL, 0);
    }
    else if (key == "loopX") {
        layer->loopX = (value == "true");
    }
    else if (key == "loopY") {
        layer->loopY = (value == "true");
    }
    return true;
}

bool Map::save(std::string fileName, std::string tileSetFileName) {
    std::ofstream outFile;
    std::string data = "tileset_file = \"" + tileSetFileName + "\"\n";

    // Gather data
    for (int layerIndex = 0; layerIndex < 8; layerIndex++) {
        data += "{\n";

        // Save the tiles
        data += "\t[\n";
        for (int i = 0; i < this->layers[layerIndex].height; i++) {
            data += "\t\t";

            for (int j = 0; j < this->layers[layerIndex].width; j++) {
                data += to_string(this->layers[layerIndex].tiles[i][j]) + ", ";
            }

            data += "\n";
        }
        data += "\t]\n";

        // Write the properties of the layer
        data += "\twidth = " + to_string(this->layers[layerIndex].width) + "\n";
        data += "\theight = " + to_string(this->layers[layerIndex].height) + "\n";
        data += "\txOffset = " + to_string(this->layers[layerIndex].xOffset) + "\n";
        data += "\tyOffset = " + to_string(this->layers[layerIndex].yOffset) + "\n";
        data += "\txSpeed = " + to_string(this->layers[layerIndex].xSpeed) + "\n";
        data += "\tySpeed = " + to_string(this->layers[layerIndex].ySpeed) + "\n";
        data += "\txSpeedAuto = " + to_string(this->layers[layerIndex].xSpeedAuto) + "\n";
        data += "\tySpeedAuto = " + to_string(this->layers[layerIndex].ySpeedAuto) + "\n";
        data += "\tloopX = " + to_string(this->layers[layerIndex].loopX == true ? "true" : "false") + "\n";
        data += "\tloopY = " + to_string(this->layers[layerIndex].loopY == true ? "true" : "false") + "\n";

        data += "}\n";
    }

    data += "{\n";

    for (unsigned int i = 0; i < this->layers[4].events.size(); i++) {
        for (unsigned int j = 0; j < this->layers[4].events[i].size(); j++) {
            data += "[" + to_string((int) this->layers[4].events[i][j].type) + ", " + to_string((int) this->layers[4].events[i][j].parameter) + "], ";
        }
        data += "\n";
    }

    data += "}\n";

    outFile.open(BASE_FOLDER + fileName);
    outFile.write(data.c_str(), data.length());
    outFile.close();
    return true;
}

bool Map::load(std::string fileName, Textbox* inTextbox) {
    if (!isFileExists(fileName)) {
        return false;
    }

    std::string currentLine;
    bool inALayer = false;
    bool inATile = false;
    bool inKey = false;
    bool isTileSetLoaded = false;
    unsigned int layerIndex = 0;
    unsigned int keyValueLinePos = 0;
    unsigned int pos = 0;
    unsigned int prevLineIndex = 0;
    unsigned int lineIndex = 0;
    std::string keyStr = "";
    std::string valueStr = "";
    std::vector<std::vector<uint16_t>> currentTiles;
    std::ifstream mapFile;
    bool loadInProgress = true;

    // Wait for opening the file
    mapFile.open(fileName);
    while (!mapFile.is_open()) {}

    // Go line by line
    while (std::getline(mapFile, currentLine) && loadInProgress) {

        // Load in global properties too
        if (!isTileSetLoaded && currentLine.substr(0, 12) == "tileset_file") {
            std::string value;
            bool inValue = false;
            bool inString = false;

            for (unsigned int i = 0; i < currentLine.length(); i++) {
                if (currentLine[i] == ' ' && !inString) continue;

                if (currentLine[i] == '=') {
                    inValue = true;
                    continue;
                }

                if (inValue) {
                    if (currentLine[i] == '"') {
                        inString = true;
                        continue;
                    }
                    else if (inString && currentLine[i] == '"') {
                        inString = false;
                        break;
                    }

                    if (inString) {
                        value += currentLine[i];
                    }
                }
            }

            this->tileset.initialize(BASE_FOLDER + TILESET_FOLDER + value);
            inTextbox->setString(value.substr(0, value.length() - 4));
            isTileSetLoaded = true;
        }


        // Go char by char
        for (pos = 0; pos < currentLine.length(); pos++) {

            // Ignore comment
            if (currentLine[pos] == '#') {
                break;
            }
            else if(currentLine[pos] == ' ' || currentLine[pos] == '\t') {
                continue;
            }

            if (currentLine[pos] == '{') {
                inALayer = true;
                continue;
            }
            else if (currentLine[pos] == '}') {
                inALayer = false;

                // Save this layer
                this->layers[layerIndex].tiles = currentTiles;
                currentTiles.clear();

                layerIndex++;
                if (layerIndex > 7) {
                    loadInProgress = false;
                    break;
                }
                continue;
            }

            if (inALayer) {

                // Handle tiles
                if (currentLine[pos] == '[') {
                    inATile = true;
                    continue;
                }
                else if (currentLine[pos] == ']') {
                    inATile = false;
                    continue;
                }

                if (inATile && lineIndex != prevLineIndex) {
                    prevLineIndex = lineIndex;
                    currentTiles.push_back(this->parseTileRow(currentLine));
                }

                // Handle properties
                if (currentLine[pos] == '=') {
                    inKey = true;
                    keyStr = "";
                    valueStr = "";

                    // Parse value and key
                    // Go char by char (again)
                    for (keyValueLinePos = 0; keyValueLinePos < currentLine.length(); keyValueLinePos++) {
                        if (currentLine[keyValueLinePos] == ' ' || currentLine[keyValueLinePos] == '\t') continue;

                        if (currentLine[keyValueLinePos] == '=') {
                            inKey = false;
                            continue;
                        }

                        if (inKey) {
                            keyStr.append(std::string(1, currentLine[keyValueLinePos]));
                        }
                        else {
                            valueStr.append(std::string(1, currentLine[keyValueLinePos]));
                        }
                    }

                    this->setLayerProperty(&this->layers[layerIndex], keyStr, valueStr);
                    //pos = keyValueLinePos;  ///0xC0000005
                    continue;
                }

            }

        }

        lineIndex++;
    }
    mapFile.close();

    // Read in the event layer too
    mapFile.open(fileName);
    while (!mapFile.is_open()) {}

    std::vector<Event> eventRow;
    Event currentEvent(EVENT::NONE, 0);
    std::string eventString = "";
    std::string paramString = "";
    uint8_t pCounter = 0;
    bool inEvent = false;
    bool inEventValue = false;
    loadInProgress = true;
    this->layers[4].events = {};

    while (std::getline(mapFile, currentLine) && loadInProgress) {
        for (pos = 0; pos < currentLine.length(); pos++) {

            // Ignore comment and whitespace
            if (currentLine[pos] == '#') {
                break;
            }
            else if(currentLine[pos] == ' ' || currentLine[pos] == '\t') {
                continue;
            }

            if (currentLine[pos] == '{') {
                pCounter++;
                continue;
            }

            if (pCounter == 9) {
                if (currentLine[pos] == '}') {
                    loadInProgress = false;
                    break;
                }

                if (currentLine[pos] == '[') {
                    inEvent = true;
                    inEventValue = true;
                    continue;
                }
                else if (currentLine[pos] == ']') {
                    inEvent = false;
                    inEventValue = true;

                    currentEvent.type = (EVENT) std::atoi(eventString.c_str());
                    currentEvent.parameter = std::atoi(paramString.c_str());

                    eventRow.push_back(currentEvent);

                    currentEvent.type = EVENT::NONE;
                    currentEvent.parameter = 0;

                    eventString = "";
                    paramString = "";
                    continue;
                }

                if (inEvent) {
                    if (currentLine[pos] == ',') {
                        inEventValue = !inEventValue;
                        continue;
                    }

                    if (inEventValue) {
                        eventString += currentLine[pos];
                    }
                    else {
                        paramString += currentLine[pos];
                    }

                }

            }
        }

        if (pCounter == 9 && eventRow.size() > 0) {
            this->layers[4].events.push_back(eventRow);
            eventRow = {};
        }
    }

    mapFile.close();

    /*
    for (std::vector<Event> eventRow : this->layers[4].events) {
        for (Event e : eventRow) {
            std::cout << (int) e.type << ", " << (int) e.parameter << std::endl;
        }
    }
    */

    return true;
}
