#ifndef _TILES_VIEW_HPP_
#define _TILES_VIEW_HPP_

class TilesView : public View {
    /**

    Tiles menu where use can select tiles

    */
public:
    RectWrapper *tileBackground;
    int16_t tileRow;

    TilesView();
    ~TilesView();
    void handleEvent(sf::Event event, Map *editMap);
    MODE run(sf::RenderWindow *window, RectWrapper *tileRect, Map *editMap, int &selectedTile, int &scrollX, int &scrollY);
};

#endif // _TILES_VIEW_HPP_
