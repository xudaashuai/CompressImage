#include <iostream>
#include <cmath>
#include "CompressImage.h"

using namespace std;

int main() {
    CompressImage ci;
    ci.readFile("1.bmp");
    ci.compress("1.xds",0xff);
    ci.unCompress("1.xds");
    ci.writeFile("1output.bmp");
    CompressImage ci2;
    ci.readFile("2.bmp");
    ci.compress("2.xds",0x34);
    ci.unCompress("2.xds");
    ci.writeFile("2output.bmp");
    return 0;
}