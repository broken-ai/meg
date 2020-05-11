#ifndef _MAP_UTILS_HPP_
#define _MAP_UTILS_HPP_
namespace MapUtils {


/**
    @returns the tileID of the tile at xTile and yTile
*/
uint16_t getTile(Map* inMap, uint8_t layer, uint16_t xTile, uint16_t yTile) {
    return inMap->layers[layer].tiles[yTile][xTile];
}

/**
    @returns the event type at xTile and yTile
*/
EVENT getEventType(Map* inMap, uint16_t xTile, uint16_t yTile) {
    return inMap->layers[4].events[yTile][xTile].type;
}

/**
    @returns the event parameter at xTile and yTile
*/
uint16_t getEventParameter(Map* inMap, uint16_t xTile, uint16_t yTile) {
    return inMap->layers[4].events[yTile][xTile].parameter;
}

/**
    @returns true if a pixel in the map is masked at the given coordinates
*/
bool maskedPixel(Map* inMap, uint8_t layer, uint16_t xPos, uint16_t yPos) { //WATCH OUT!!! IT CAN INDEX OUT OF THE LAYER! FIX IT ASAP!
    uint16_t tileAtCoords = getTile(inMap, layer, xPos / 32, yPos / 32);
    uint8_t xTile = tileAtCoords % 10;
    uint16_t yTile = (unsigned int)(tileAtCoords / 10);
    return inMap->tileset.mask[yTile * 32 + yPos % 32][xTile * 32 + xPos % 32];
    //return inMap->tileset.mask[(tileAtCoords - (unsigned int)(tileAtCoords / 10) * 32) + (xPos - xPos&~31)][((unsigned int)(tileAtCoords / 10) * 32) + (yPos - yPos&~31)]; //maybe this will work, dunno, it's so fucking complicated
}

/**
    @returns true if any pixel in a horizontal line line is masked
*/
bool maskedHLine(Map* inMap, uint8_t layer, uint16_t xPos, uint16_t yPos, uint16_t length) {
    for (uint16_t i = xPos; i < xPos + length; ++i) {
        if (maskedPixel(inMap, layer, i, yPos)) return true;
    }
    return false;
}

/**
    @returns true if any pixel in a vertical line line is masked
*/
bool maskedVLine(Map* inMap, uint8_t layer, uint16_t xPos, uint16_t yPos, uint16_t length) {
    for (uint16_t i = yPos; i < yPos + length; ++i) {
        if (maskedPixel(inMap, layer, xPos, i)) return true;
    }
    return false;
}

}
#endif // _MAP_UTILS_HPP_
