#include "tgaimage.h"
#include<stdio.h>

void draw_line(int x1, int y1, int x2, int y2, TGAImage& image, const TGAColor& color) {
    bool steep = false;
    if(std::abs(x2-x1) < std::abs(y2-y1)) {
        std::swap(x1, y1);
        std::swap(x2, y2);
        steep = true;
    }
    if(x2 < x1) {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }
    
    int dx = x2 - x1, dy = std::abs(y2 - y1), error = 0;
    for(int x = x1, y = y1; x <= x2; ++x) {
        if(!steep)  image.set(x, y, color);
        else image.set(y, x, color);
        error += dy;
        if(error*2 > dx) {
            y += y2-y1>0 ? 1 : -1;
            error -= dx;
        }
    }
}

int main(int argc, char** argv) {
    const int width = 1920, height = 1080;
    const TGAColor red = TGAColor(255, 0, 0);
    const TGAColor green = TGAColor(0, 255, 0);
    const TGAColor blue = TGAColor(0, 0, 255);
    const TGAColor unknown = TGAColor(255, 0, 255);
    TGAImage image(width, height, TGAImage::RGB);  
    for(int y = 0; y <= height; y += 10) {
        draw_line(width/2, height/2 , width-1, y-1, image, unknown);
    }
    for(int y = 0; y <= height; y += 10) {
        draw_line(width/2, height/2 , 0, y-1, image, red);
    }
    for(int x = 0; x <= width; x += 10) {
        draw_line(x-1,height-1,width/2,height/2, image, green);
    }
    for(int x = 0; x <= width; x += 10) {
        draw_line(x-1,0,width/2,height/2, image, blue);
    }
    image.flip_vertically();
    image.write_tga_file("bresenham.tga");
    system("timeout 1");
    return 0;
}
//g++ bresenham.cpp tgaimage.cpp -o bresenham.exe; .\bresenham.exe; .\bresenham.tga
