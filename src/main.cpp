#include "lissajous_app.hpp"
#include "ofMain.h"

int main() {
    ofSetupOpenGL(1024, 768, OF_WINDOW);
    ofRunApp(new LissajousApp());
}
