
/**

Opens a simple window with the given message.

*/
bool openMessageBox(std::string message, sf::Font *guiFont,
                    sf::Color bgColor=sf::Color(15, 188, 249, 255), sf::Color fontColor=sf::Color::Black) {
    const int charSize = 14;
    const int windowWidth = charSize * message.length() / 2 + charSize * 2;
    const int windowHeight = charSize * 5;

    sf::RenderWindow msgWindow(sf::VideoMode(windowWidth, windowHeight), "Map Editor Gadget", sf::Style::Close);
    msgWindow.setFramerateLimit(24);
    msgWindow.resetGLStates();

    sf::Event event;
    sf::Text messageText(message, *guiFont, charSize);
    Button okButton(10, (int) (windowHeight - charSize * 1.5), 27, charSize, "Ok", guiFont);

    okButton.label.setCharacterSize(charSize);
    okButton.label.setColor(fontColor);
    messageText.setColor(fontColor);
    messageText.setPosition((int) (windowWidth / 2 - message.length() * charSize / 4), (int) (charSize / 2));

    while (msgWindow.isOpen()) {
        while (msgWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                msgWindow.close();
            }
        }
        msgWindow.clear(bgColor);

        // Handle ok button press
        if (okButton.state == STATE::RELEASED) {
            okButton.state = STATE::NORMAL;
            return true;
        }

        msgWindow.draw(messageText);
        okButton.updateAndDraw(&msgWindow);
        msgWindow.display();
    }

    return false;
}

/**

Opens a dialog box for asking a string.

*/
std::string askForString(std::string message, sf::Font *guiFont,
                         sf::Color bgColor=sf::Color(15, 188, 249, 255), sf::Color fontColor=sf::Color::Black) {
    const int charSize = 14;
    const int windowWidth = charSize * message.length() / 2 + charSize * 2;
    const int windowHeight = charSize * 5;

    sf::RenderWindow askWindow(sf::VideoMode(windowWidth, windowHeight), "Map Editor Gadget", sf::Style::Close);
    askWindow.setFramerateLimit(24);
    askWindow.resetGLStates();

    sf::Event event;
    sf::Text messageText(message, *guiFont, charSize);
    Button okButton(10, (int) (windowHeight - charSize * 1.5), 27, charSize, "Ok", guiFont);
    Textbox textBox(false, (int) (windowWidth / 2 - (charSize * 10) / 2),
                    (int) (windowHeight - charSize * 2.5) - 3,
                    charSize * 10, charSize + 2, guiFont);
    std::string newFileName = "";

    okButton.label.setCharacterSize(charSize);
    okButton.label.setColor(fontColor);
    messageText.setColor(fontColor);
    messageText.setPosition((int) (windowWidth / 2 - message.length() * charSize / 4), (int) (charSize / 2));

    while (askWindow.isOpen()) {
        while (askWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                askWindow.close();
            }

            if (event.type == sf::Event::TextEntered) {
                textBox.updateContent(event);
                newFileName = textBox.inputString;
            }
        }
        askWindow.clear(bgColor);

        // Handle ok button press
        if (okButton.state == STATE::RELEASED) {
            okButton.state = STATE::NORMAL;
            return newFileName;
        }

        askWindow.draw(messageText);
        okButton.updateAndDraw(&askWindow);
        textBox.updateAndDraw(&askWindow);

        askWindow.display();
    }

    return newFileName;
}
