#include<stdio.h>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"
void line(int x1, int y1, int x2, int y2, TGAImage& image, const TGAColor& color) {
    bool steep = false;
    if(std::abs(x2-x1) < std::abs(y2-y1)) {
        std::swap(x1, y1);
        std::swap(x2, y2);
        bool steep = true;
    }
    if(x2 < x1) {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }
    int dx = x2 - x1, dy = std::abs(y2 - y1), error = 0;
    int inc = y2-y1>0 ? 1 : -1;
    for(int x = x1, y = y1; x <= x2; ++x) {
        if(!steep) image.set(x, y, color);
        else image.set(y, x, color);
        error += dy;
        if(error*2 > dx) {
            y += inc;
            error -= dx;
        }
    }
}

int main(int argc, char** argv) {
    const int width = 1920, height = 1080;
    const TGAColor white = TGAColor(255, 255, 255);
    TGAImage image(width, height, TGAImage::RGB);  
    Model model = Model("african_head.obj");
    // 循环模型里的所有三角形
    for(int i = 0; i < model.nfaces(); i++) {
        const std::vector<int>& face = model.face(i);
        // 循环三角形三个顶点，每两个顶点连一条线
        for(int j = 0; j < 3; j++) {
            Vec3f v0 = model.vert(face[j]);
            Vec3f v1 = model.vert(face[(j + 1) % 3]);
            
            // 因为模型空间取值范围是 [-1, 1]^3，我们要把模型坐标平移到屏幕坐标中
            // Viewport Transformation
            int x0 = (v0.x + 1) * width / 2;
            int y0 = (v0.y + 1) * height / 2;
            int x1 = (v1.x + 1) * width / 2;
            int y1 = (v1.y + 1) * height / 2;
            
            // 画线
            line(x0, y0, x1, y1, image, white);
        }
    }
    image.flip_vertically();
    image.write_tga_file("african_head.tga");
    system("timeout 1");
    return 0;
}
//g++ draw_head.cpp tgaimage.cpp model.cpp -o draw_head.exe; .\draw_head.exe; .\african_head.tga
