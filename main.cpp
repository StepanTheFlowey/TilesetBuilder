#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <cmath>
#include <iostream>
#include <filesystem>
#include <Windows.h>
#include <SFML\Graphics.hpp>

namespace fs = std::filesystem;

class BoolTable {
  bool *table_ = nullptr;
  uint16_t width_ = 0;
  uint16_t height_ = 0;
public:
  BoolTable() {

  }

  ~BoolTable() {
    destroy();
  }

  void create(uint16_t width, uint16_t height) {
    table_ = new bool[width * height];
    width_ = width;
    height_ = height;
  }

  void destroy() {
    if(table_) {
      delete[] table_;
      table_ = nullptr;
    }
  }

  void clear() {
    for(uint16_t i = 0; i < width_; i++) {
      for(uint16_t j = 0; j < height_; j++) {
        set(i, j, false);
      }
    }
  }

  void set(uint16_t x, uint16_t y, bool value) {
    table_[x + y * width_] = value;
  }

  bool get(uint16_t x, uint16_t y) {
    return table_[x + y * width_];
  }
};

int main() {
  uint16_t tableSize = 0;
  uint16_t pixelSize = 0;
  uint16_t inputSize = 0;

  for(uint8_t i = 4; i < 14; i++) {
    uint16_t pSize = 0;
    pSize = std::pow(2, i);
    std::wcout << i - 3 << L".\t" << pSize << L"x" << pSize << std::endl;
  }
  std::wcout << L"Select output image size: ";
  std::wcin >> tableSize;
  tableSize += 3;
  pixelSize = std::pow(2, tableSize);
  _wsystem(L"cls");

  for(uint8_t i = 1; i < 10; i++) {
    uint16_t pSize = 0;
    pSize = std::pow(2, i);
    std::wcout << i << L".\t" << pSize << L"x" << pSize << std::endl;
  }
  std::wcout << L"Select input image size: ";
  std::wcin >> inputSize;
  inputSize = std::pow(2, inputSize);
  tableSize = pixelSize / inputSize;
  _wsystem(L"cls");

  std::wcout << L"Table size: " << tableSize << std::endl;
  std::wcout << L"Pixel size: " << pixelSize << std::endl;
  std::wcout << L"Input size: " << inputSize << std::endl;
  std::wcout << L"Initializing" << std::endl;

  BoolTable table;
  table.create(tableSize, tableSize);
  table.clear();

  sf::Image input;
  sf::Image output;
  output.create(pixelSize, pixelSize, sf::Color::Transparent);

  bool found;
  uint16_t x = 0;
  uint16_t y = 0;
  for(const auto &entry : fs::directory_iterator(L"images\\")) {
    if(!fs::is_regular_file(entry)) {
      continue;
    }
    if(entry.path().extension().wstring() != L".png") {
      continue;
    }
    std::wcout << L"Loading: " << entry.path().wstring() << std::endl;
    input.loadFromFile(entry.path().string());

    std::wcout << L"Allocating" << std::endl;
    found = false;
    for(uint16_t i = 0; i < tableSize; i++) {
      for(uint16_t j = 0; j < tableSize; j++) {
        if(!table.get(j, i)) {
          table.set(j, i, true);
          x = j;
          y = i;
          found = true;
          break;
        }
      }
      if(found) {
        break;
      }
    }

    if(!found) {
      std::wcout << L"Not enough space!" << std::endl;
      break;
    }

    x *= inputSize;
    y *= inputSize;
    std::wcout << L"Coping to " << x << L"x" << y << std::endl;
    output.copy(input, x, y, sf::IntRect(0, 0, inputSize, inputSize));
  }
  std::wcout << L"Saving output" << std::endl;
  output.saveToFile("OUTPUT.PNG");
  _wsystem(L"pause");
}