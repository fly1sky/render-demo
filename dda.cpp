#include "tgaimage.h"
#include<stdio.h>
struct Point{
    int x, y;
};



void draw_line(int x1, int y1, int x2, int y2, TGAImage& image, const TGAColor& color) {
    const int dx = x2-x1;
    const int dy = y2-y1;
    size_t num_points = std::max(std::abs(dx), std::abs(dy));
    const double dlx = (double)dx / num_points;
    const double dly = (double)dy / num_points;
    double x = x1, y = y1;
    while(num_points--) {
        // printf("(%f,%f)\n", x, y);
        image.set(x, y, color);
        x += dlx;
        y += dly;
    }
}

int main(int argc, char** argv) {
    const int width = 1280, height = 720;
    const TGAColor red = TGAColor(255, 0, 0);
    const TGAColor green = TGAColor(0, 255, 0);
    const TGAColor blue = TGAColor(0, 0, 255);
    TGAImage image(width, height, TGAImage::RGB);  

    for(int x = 0; x <= width; x += 10) {
        draw_line(0,0,x-1,height-1, image, red);
    }
    for(int y = 0; y <= height; y += 10) {
        draw_line(0,0,width-1,y-1, image, red);
    }
    for(int x = 0; x <= width; x += 10) {
        draw_line(x-1,height-1,width/2,height/2, image, green);
    }
   
    for(int x = 0; x <= width; x += 10) {
        draw_line(x-1,0,width/2,height/2, image, blue);
    }
    image.flip_vertically();
    image.write_tga_file("DDA.tga");
    system("pause");
    return 0;
}