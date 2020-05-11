#ifndef _PHYSFS_STREAM_HPP_
#define _PHYSFS_STREAM_HPP_

#include <SFML/System/InputStream.hpp>
#include <SFML/System/NonCopyable.hpp>

struct PHYSFS_File;

class PhysFsStream : public sf::InputStream, sf::NonCopyable {
    /**
    Example use:

    ```
    PHYSFS_init(argv[0]);
    std::string tctFileName = "maps/testset/testset.tct";
    PHYSFS_addToSearchPath((BASE_FOLDER + tctFileName).c_str(), 1);

    PhysFsStream testStream;
    testStream.open("image.png");

    // Make a sprite from our file.
    sf::Texture tex;
    tex.loadFromStream(testStream);
    sf::Sprite sprite(tex);
    ```
    */

public:
    PhysFsStream(const char * filename = 0x0);
    virtual ~PhysFsStream();
    bool isOpen() const;
    bool open(const char * filename);
    void close();
    virtual sf::Int64 read(void * data, sf::Int64 size);
    virtual sf::Int64 seek(sf::Int64 position);
    virtual sf::Int64 tell();
    virtual sf::Int64 getSize();

private:
    PHYSFS_File * m_File;

};

#endif // _PHYSFS_STREAM_HPP_
