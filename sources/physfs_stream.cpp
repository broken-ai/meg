#include "../headers/physfs_stream.hpp"


PhysFsStream::PhysFsStream(const char * filename) : m_File(0x0) {
    if(filename) open(filename);
}

PhysFsStream::~PhysFsStream() {
    close();
}

bool PhysFsStream::isOpen() const {
    return (m_File != 0x0);
}

bool PhysFsStream::open(const char * filename) {
    close();
    m_File = PHYSFS_openRead(filename);
    return isOpen();
}

void PhysFsStream::close() {
    if(isOpen()) PHYSFS_close(m_File);
    m_File = 0x0;
}

sf::Int64 PhysFsStream::read(void * data, sf::Int64 size) {
    if(!isOpen()) return -1;

    // PHYSFS_read returns the number of 'objects' read or -1 on error.
    // We assume our objects are single bytes and request to read size
    // amount of them.
    return PHYSFS_read(m_File, data, 1, static_cast<PHYSFS_uint32>(size));
}

sf::Int64 PhysFsStream::seek(sf::Int64 position) {
    if(!isOpen()) return -1;

    // PHYSFS_seek return 0 on error and non zero on success
    if(PHYSFS_seek(m_File, position)) return tell();
    else return -1;
}

sf::Int64 PhysFsStream::tell() {
    if(!isOpen()) return -1;

    // PHYSFS_tell returns the offset in bytes or -1 on error just like SFML wants.
    return PHYSFS_tell(m_File);
}

sf::Int64 PhysFsStream::getSize() {
    if(!isOpen()) return -1;

    // PHYSFS_fileLength also the returns length of file or -1 on error just like SFML wants.
    return PHYSFS_fileLength(m_File);
}
