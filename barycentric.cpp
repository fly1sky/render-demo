#include "tgaimage.h"
#include "model.h"
#include "geometry.h"
#include <time.h>
#include <stdlib.h>


void line(int x1, int y1, int x2, int y2, TGAImage& image, const TGAColor& color) {
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

Vec3f barycentric(const std::vector<Vec2i> &triangle, const Vec2i& point) {
    const Vec3i x = Vec3i(triangle[1].x - triangle[0].x, triangle[2].x - triangle[0].x, triangle[0].x - point.x);
    const Vec3i y = Vec3i(triangle[1].y - triangle[0].y, triangle[2].y - triangle[0].y, triangle[0].y - point.y);
    const Vec3i xy = x^y;
    if(std::abs(xy.z) == 0) { // triangle is actually a line
        return Vec3f(-1, 1, 1);
    }
    const float by = xy.x/(float)xy.z, bz = xy.y/(float)xy.z;
    const float bx = 1-by-bz;
    return Vec3f(bx, by, bz);
}

std::pair<Vec2i, Vec2i> bounding_box(const std::vector<Vec2i>& triangle) {
    int minx = std::min(triangle[0].x, triangle[1].x);
    minx = std::min(minx, triangle[2].x);
    int miny = std::min(triangle[0].y, triangle[1].y);
    miny = std::min(miny, triangle[2].y);

    int maxx = std::max(triangle[0].x, triangle[1].x);
    maxx = std::max(maxx, triangle[2].x);
    int maxy = std::max(triangle[0].y, triangle[1].y);
    maxy = std::max(maxy, triangle[2].y);
    return std::make_pair(Vec2i{minx, miny}, Vec2i{maxx, maxy});
}

void fill(const std::vector<Vec2i>& triangle, TGAImage& image, const TGAColor& color) {
    const auto &&[minP, maxP] = bounding_box(triangle);
    for(int x = minP.x; x <= maxP.x; ++x) {
        for(int y = minP.y; y <= maxP.y; ++y) {
            const Vec3f&& bc = barycentric(triangle, {(int)x,(int)y});
            if(bc.x > 0 && bc.y > 0 && bc.z > 0)  
                image.set(x, y, color);
        }
    }
}

int main() {
    srand(time(0));
    const int width = 1920, height = 1080;
    const TGAColor&& white = TGAColor(255, 255, 255, 255);
    TGAImage image(width, height, TGAImage::RGB);  
    // 循环模型里的所有三角形
    Model model = Model("african_head.obj");
    for(int i = 0; i < model.nfaces(); i++) {
        const std::vector<int>& face = model.face(i);
        std::vector<Vec2i> triangle;
        // 循环三角形三个顶点，每两个顶点连一条线
        for(int j = 0; j < 3; j++) {
            const Vec3f&& v0 = model.vert(face[j]);
            const Vec3f&& v1 = model.vert(face[(j + 1) % 3]);
            // 因为模型空间取值范围是 [-1, 1]^3，我们要把模型坐标平移到屏幕坐标中
            // Viewport Transformation
            int x0 = (v0.x + 1) * width / 2;
            int y0 = (v0.y + 1) * height / 2;
            int x1 = (v1.x + 1) * width / 2;
            int y1 = (v1.y + 1) * height / 2;
            // 画线
            // line(x0, y0, x1, y1, image, white);
            triangle.push_back({x0, y0});
        }
        const TGAColor rcolor = TGAColor(rand()%256, rand()%256, rand()%256, rand()%256);
        fill(triangle, image, rcolor);
    }
    image.flip_vertically();
    image.write_tga_file("african_shaded_head.tga");
    system("timeout 1");
    return 0;
}
/*
g++ -Wall -Wextra -Wshadow barycentric.cpp tgaimage.cpp model.cpp -o barycentric.exe ; .\barycentric.exe ; .\african_shaded_head.tga
*/