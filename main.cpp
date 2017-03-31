#include <iostream>
#include "CompressImage.h"

using namespace std;
int main() {
    CompressImage ci;
    ci.readFile("1.bmp");
    ci.compress("temp.xds");

    CompressImage ci2;
    ci2.unCompress("temp.xds");
    ci2.writeFile("output.bmp");
    return 0;
}