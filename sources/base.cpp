
// Global variables
std::string BASE_FOLDER = "";

#define TILESET_FOLDER "tilesets/"
#define MAP_FOLDER "maps/"
#define CHARACTER_SIZE 12
#define WINDOW_WIDTH 864
#define WINDOW_HEIGHT 632

const std::string HELP_TEXT =
    "\n\n\t\t\tNo tileset loaded!"
    "\n\n\n\n\n"
    "\t\t\tA guide to using M.E.G.:"
    "\n\n\n"
    "\t\t\t - Tilesets must be located in the \"tilesets\" folder, next to the executable file of M.E.G."
    "\n\n"
    "\t\t\t - Maps to be loaded must be located in the \"maps\" folder. Your maps will also be saved there."
    "\n\n"
    "\t\t\t - To load a tileset, just type the name of the file into the tileset textbox (without extension)."
    "\n\n"
    "\t\t\t - Ctrl + Left Click picks the tile under the mouse cursor."
    "\n\n"
    "\t\t\t - To erase a tile, move the cursor over it and press Backspace."
    "\n\n"
    "\t\t\t - You can use Ctrl + Z to undo the last action you made."
;


enum class MODE {
    EDIT,
    TILES,
    LAYER,
    EVENT
};


class UndoAction {
public:
    int xPos;
    int yPos;
    int layerIndex;
    int newTileIndex;
    int prevTileIndex;
    bool initialized;

    UndoAction() {
        this->xPos = 0;
        this->yPos = 0;
        this->layerIndex = 0;
        this->newTileIndex = 0;
        this->prevTileIndex = 0;
        this->initialized = false;
    }

    UndoAction(int xPos, int yPos, int layerIndex, int newTileIndex, int prevTileIndex) {
        this->xPos = xPos;
        this->yPos = yPos;
        this->layerIndex = layerIndex;
        this->newTileIndex = newTileIndex;
        this->prevTileIndex = prevTileIndex;
        this->initialized = true;
    }

    bool operator==(UndoAction other) {
        // other.prevTileIndex == this->prevTileIndex
        return other.xPos == this->xPos &&
                other.yPos == this->yPos &&
                other.layerIndex == this->layerIndex &&
                other.newTileIndex == this->newTileIndex;
    }

    bool operator!=(UndoAction other) {
        return !(*this == other);
    }
};


template<typename T> std::string to_string(const T& n) {
    std::ostringstream stm;
    stm << n;
    return stm.str();
}

/**
@returns true if the given file exists
*/
bool isFileExists(const std::string fileName) {
    std::ifstream infile(fileName);
    return infile.good();
}

/**

Parses the current working directory from the exeFilePath parameter.

*/
void setBaseDir(std::string exeFilePath) {
    int lastSlashIndex = 0;

    for (unsigned int i = 0; i < exeFilePath.length(); i++) {
        if (exeFilePath[i] == '\\' || exeFilePath[i] == '/') {
            lastSlashIndex = i;
        }
    }

    BASE_FOLDER = exeFilePath.substr(0, lastSlashIndex + 1);
}
