#include "graphics/font/FontDrawer.h"

using namespace Graphics::Font;


FontDrawer::FontDrawer(Drawing::PixelDrawer* pixelDrawer) {

    if (pixelDrawer == nullptr) {
        logWrite("FontDrawer::FontDrawer(PixelDrawer*) was called on a nullptr!", true);
        return;
    }

    this->pixelDrawer = pixelDrawer;

    this->initFont();

    return;

}

FontDrawer::~FontDrawer() {

    delete[] this->chars;

    return;

}

void FontDrawer::drawChar(uint32 color, char ch, int x, int y) {

    const bool* pixels = this->getCharRef(ch);

    this->drawCharPixels(color, pixels, x, y);
    
    return;

}

void FontDrawer::drawString(uint32 color, const char* string, int x, int y) {

    int dx = 0;

    for (int i = 0; string[i] != '\0'; i++) {

        this->drawChar(color, string[i], x + dx, y);

        // Offset by +6 pixels because chars are 7 pixels wide
        dx += 6;

    }

    return;

}

void FontDrawer::drawInt(uint32 color, int num, int x, int y) {

    char buffer[16];
    intToString(num, buffer, 16);

    this->drawString(color, buffer, x, y);

    return;

}

void FontDrawer::initFont() {

    this->chars = new bool[this->rawCharsSize];

    for (int i = 0; i < this->rawCharsSize; i++) {

        switch (this->rawChars[i]) {
            
            // false pixel
            case ' ':
                this->chars[i] = false;
                break;

            // true pixel
            case '0':
                this->chars[i] = true;
                break;

            // End of string
            case '\0':
                return;

        }

    }

    return;

}

void FontDrawer::drawCharPixels(uint32 color, const bool* pixels, int x, int y) {

    int dx = 0;
    int dy = 0;

    for (int i = 0; i < 49; i++) {

        if (pixels[i]) {
            this->pixelDrawer->drawPixel(color, x + dx, y + dy);
        }

        dx++;
        if (dx > 6) {
            dx = 0;
            dy++;
        }

    }

    return;

}

bool* FontDrawer::getCharRef(char ch) const {

    /*
        Because the chars 'a' - 'z' are in order for their cooresponding integer value, I can use this to simplify the indexing
        Same goes for 'A' - 'Z' and '0' - '9'
        Since I dont have lower/upper variants for the letters, they both map to the same pixels
    */

    // This is the index of the character with respect to the character count (not array length)
    int charIndex = -1;

    // Chars 'a' - 'z' turn to 0 - 25
    if ( ch >= 'a' && ch <= 'z' ) {
        charIndex = ch - 'a';
    }

    // Chars 'A' - 'Z' turn to 0 - 25
    else if ( ch >= 'A' && ch <= 'Z' ) {
        charIndex = ch - 'A';
    }

    // Chars '0' - '9' turn to 26 - 35
    else if ( ch >= '0' && ch <= '9' ) {
        charIndex = (ch - '0') + 26;
    }

    // Other chars
    else {
        switch (ch) {

            // Space
            case ' ':
                charIndex = 36;
                break;
            
            // Minus / Negative Sign
            case '-':
                charIndex = 37;
                break;

            // Period / Decimal
            case '.':
                charIndex = 38;
                break;

        }
    }

    // Use null char if nothing else applied
    if (charIndex == -1) charIndex = 39;

    // Return a pointer in the chars array that is offset by charIndex
    return &(this->chars[charIndex * 49]);

}

bool* FontDrawer::getCharRef(int num) const {

    int charIndex;

    // Use null char if the int is negative or not single digit
    if ( num < 0 || num > 9 ) {
        charIndex = 39;
    }

    // Otherwise point to the proper int char
    // Since the numbers are ordered properly in my list, I use this to simplify the indexing
    else {
        charIndex = 26 + num;
    }

    // Return a pointer in the chars array that is offset by charIndex
    return &(this->chars[charIndex * 49]);

}
