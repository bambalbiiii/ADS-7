// Copyright 2022 NNTU-CS
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <algorithm>
#include "train.h"
#include "lodepng.h"
#include <iostream>

const int W = 900, H = 600;
const int ML = 80, MR = 30, MT = 30, MB = 60;

struct Color { unsigned char r, g, b; };

void setPixel(std::vector<unsigned char>& img, int x, int y, Color c) {
  if (x < 0 || x >= W || y < 0 || y >= H) return;
  int idx = (y * W + x) * 4;
  img[idx] = c.r; img[idx+1] = c.g; img[idx+2] = c.b; img[idx+3] = 255;
}

void drawLine(std::vector<unsigned char>& img,
              int x0, int y0, int x1, int y1, Color c) {
  int dx = abs(x1 - x0), dy = abs(y1 - y0);
  int sx = x0 < x1 ? 1 : -1, sy = y0 < y1 ? 1 : -1, err = dx - dy;
  while (true) {
    setPixel(img, x0, y0, c);
    if (x0 == x1 && y0 == y1) break;
    int e2 = 2 * err;
    if (e2 > -dy) {
      err -= dy;
      x0 += sx;
    }
    if (e2 < dx) {
      err += dx;
      y0 += sy;
    }
  }
}

void drawChar(std::vector<unsigned char>& img,
              int x, int y, char ch, Color c) {
  static const unsigned char font[10][5] = {
    {0x7E, 0x42, 0x42, 0x42, 0x7E}, {0x00, 0x44, 0x7E, 0x40, 0x00},
    {0x72, 0x52, 0x52, 0x52, 0x4E}, {0x24, 0x42, 0x52, 0x52, 0x3C},
    {0x1E, 0x10, 0x10, 0x7E, 0x10}, {0x2E, 0x4A, 0x4A, 0x4A, 0x32},
    {0x3C, 0x52, 0x52, 0x52, 0x20}, {0x02, 0x62, 0x12, 0x0A, 0x06},
    {0x3C, 0x52, 0x52, 0x52, 0x3C}, {0x0C, 0x52, 0x52, 0x52, 0x3C}
  };
  if (ch >= '0' && ch <= '9') {
    auto& g = font[ch - '0'];
    for (int col = 0; col < 5; col++) {
      for (int row = 0; row < 8; row++) {
        if (g[col] & (1 << row)) setPixel(img, x + col, y + row, c);
      }
    }
  }
}

void drawText(std::vector<unsigned char>& img,
              int x, int y, const std::string& s, Color c) {
  for (char ch : s) {
    drawChar(img, x, y, ch, c);
    x += 7;
  }
}

int main() {
  std::srand(static_cast<unsigned int>(time(nullptr)));

  std::vector<int> ns;
  std::vector<int> opFalseV, opTrueV, opRandV;

  for (int n = 2; n <= 100; n++) {
    int opFalse, opTrue, opRandom;
    {
      Train t;
      for (int i = 0; i < n; i++) t.addCar(false);
      t.getLength();
      opFalse = t.getOpCount();
    }
    {
      Train t;
      for (int i = 0; i < n; i++) t.addCar(true);
      t.getLength();
      opTrue = t.getOpCount();
    }
    {
      Train t;
      for (int i = 0; i < n; i++) t.addCar(std::rand() % 2);
      t.getLength();
      opRandom = t.getOpCount();
    }
    ns.push_back(n);
    opFalseV.push_back(opFalse);
    opTrueV.push_back(opTrue);
    opRandV.push_back(opRandom);
  }

  std::vector<unsigned char> img(W * H * 4, 255);
  int pw = W - ML - MR, ph = H - MT - MB;

  drawLine(img, ML, MT, ML, MT + ph, {0, 0, 0});
  drawLine(img, ML, MT + ph, ML + pw, MT + ph, {0, 0, 0});

  int vmax = *std::max_element(opTrueV.begin(), opTrueV.end());
  int xmin = ns.front(), xmax = ns.back();

  auto toX = [&](int n) {
    return ML + (n - xmin) * pw / (xmax - xmin);
  };
  auto toY = [&](int v) {
    return MT + ph - v * ph / vmax;
  };

  for (int i = 1; i < static_cast<int>(ns.size()); i++) {
    drawLine(img, toX(ns[i-1]), toY(opFalseV[i-1]),
             toX(ns[i]), toY(opFalseV[i]), {220, 50, 50});
    drawLine(img, toX(ns[i-1]), toY(opTrueV[i-1]),
             toX(ns[i]), toY(opTrueV[i]), {50, 150, 50});
    drawLine(img, toX(ns[i-1]), toY(opRandV[i-1]),
             toX(ns[i]), toY(opRandV[i]), {50, 50, 220});
  }

  for (int i = 0; i < static_cast<int>(ns.size()); i += 20) {
    int x = toX(ns[i]);
    drawLine(img, x, MT + ph, x, MT + ph + 4, {0, 0, 0});
    drawText(img, x - 7, MT + ph + 8, std::to_string(ns[i]), {0, 0, 0});
  }

  lodepng::encode("../result/plot.png", img, W, H);
  std::cout << "plot.png saved\n";
  return 0;
}
