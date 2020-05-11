#ifndef _MAP_HPP_
#define _MAP_HPP_

enum class EVENT {
    NONE, //What it says on the tin.
    SPAWN_POS, //Starting position for the player. If more than one are placed on the level, the player will spawn at one of them randomly.
    TELEPORT, //If the player touches this event, they will be teleported to the location of a teleport destination with the corresponding ID.
    TELEPORT_DEST, //The destination of a teleport event. Use the same ID as a teleport event to link them together.
    ONE_WAY, //Placing this on a solid tile will make it possible to jump through it from underneath.
    SPRING, //Making contact with a spring will give the player a boost upwards.
    ELEVATOR, //Walking onto an elevator will activate it and take the player up to a given height.
    PITFALL, //Touching this event will instantly kill the player.
    INTERACT, //Special event with the intention of supporting custom scripted interactions (eg.: text sign)
    MAIN_AMMO, //Ammunition pickup for the player's selected main weapon.
    SUB_AMMO, //Ammunition pickup for the player's selected sub weapon.
    HEALTH_5, //Small health pickup. Gives 5 health points.
    HEALTH_25, //Medium health pickup. Gives 25 health points.
    HEALTH_50, //Large health pickup. Gives 50 health points.
    SHIELD_5, //Small shield pickup. Gives 5 shield points.
    SHIELD_25, //Medium shield pickup. Gives 25 shield points.
    SHIELD_50 //Large shield pickup. Gives 50 shield points.
};

std::vector<std::string> eventStrings1 = { //Event names as they appear on the event screen
    "None",
    "Spawn Position",
    "Teleport",
    "Teleport Destination",
    "One Way",
    "Spring",
    "Elevator",
    "Pitfall",
    "Interaction",
    "Main Ammo",
    "Sub Ammo",
    "Small Health",
    "Medium Health",
    "Large Health",
    "Small Shield",
    "Medium Shield",
    "Large Shield"
};

std::vector<std::string> eventStrings2 = { //Event names as they appear on layer 4
    "None",
    "Spawn\nPosi-\ntion",
    "Tele-\nport",
    "Tele-\nport\nDest.",
    "One\nWay",
    "Spring",
    "Ele-\nvator",
    "Pit-\nfall",
    "Inter-\naction",
    "Main\nAmmo",
    "Sub\nAmmo",
    "Small\nHealth",
    "Medium\nHealth",
    "Large\nHealth",
    "Small\nShield",
    "Medium\nShield",
    "Large\nShield"
};

class Event {
public:
    EVENT type;
    uint16_t parameter;

    Event(EVENT initType, uint16_t initParam) {
        this->type = initType;
        this->parameter = initParam;
    }
};

class Tileset {
public:
    sf::Texture texture;
    std::vector<std::vector<bool> > mask; // Contains the collision data of the tileset
    std::vector<sf::Sprite> tileSprites; // The drawable sprite for each tile

    Tileset();
    ~Tileset();
    bool initialize(std::string filepath);
};

typedef struct Layer {
    std::vector<std::vector<uint16_t> > tiles;
    std::vector<std::vector<Event> > events; //only the fifth layer makes use of this matrix
    uint16_t width; // Layer width in tiles
    uint16_t height; // Layer height in tiles
    float xSpeed; // How much the layer moves horizontally compared to camera (1 is normal scrolling state, fixed for layer 4)
    float ySpeed; // How much the layer moves vertically compared to camera (1 is normal scrolling state, fixed for layer 4)
    float xSpeedAuto; // How much the layer moves horizontally independent of camera movement
    float ySpeedAuto; // How much the layer moves vertically independent of camera movement
    int16_t xOffset; // How much the layer is initially offset horizontally
    int16_t yOffset; // How much the layer is initially offset vertically
    bool loopX; //Whether the layer wraps around on the X axis (useful together with xSpeedAuto)
    bool loopY; //Whether the layer wraps around on the Y axis (useful together with ySpeedAuto)

    float xPos; // Horizontal scrolling state of the layer
    float yPos; // Vertical scrolling state of the layer
} Layer;

class Map {
    /**
    Example use:

    ```
    Map *_map_ = new Map();
    if (!_map_->load(BASE_FOLDER + "maps/test.map")) {
        debug("Can't load test map.");
        return EXIT_FAILURE;
    }
    delete _map_;
    ```
    */
public:
    std::string name; // Name of the map to be displayed in menus and in-game
    Tileset tileset;
    Layer layers[8]; // The layers the map consists of, the last layer being the event layer

    Map(); // Should be initialized instead of using a load function
    ~Map();
    bool load(std::string filepath, Textbox* inTextbox);

    /**
        Updates and draws a piece of layers 0-4 based on camera position and layer position
    */
    void drawLayer(sf::RenderWindow* inWindow, uint8_t inLayer, int inCamX, int inCamY, RectWrapper* borderRect, RectWrapper* eventRect, sf::Text* inText);

    /**
        @returns false if save was not successful
    */
    bool save(std::string fileName, std::string tileSetFileName);

private:
    /**
        @returns One tile row from the given line
    */
    std::vector<uint16_t> parseTileRow(std::string line);

    /**
        Changes the input layer and sets it's given key to the given value
    */
    bool setLayerProperty(Layer *layer, std::string key, std::string value);
};

#endif  // _MAP_HPP_
