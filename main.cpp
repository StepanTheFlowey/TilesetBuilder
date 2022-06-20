#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include <cmath>
#include <vector>
#include <iostream>
#include <filesystem>
#include <Windows.h>
#include <SFML\Graphics.hpp>

namespace fs = std::filesystem;

class BoolTable {
  std::vector<bool> storage;
  uint32_t width_ = 0;
  uint32_t height_ = 0;
public:

  BoolTable() = default;

  ~BoolTable() = default;

  void create(const uint32_t width, const uint32_t height) {
    storage.resize(static_cast<size_t>(width) * height);
    width_ = width;
    height_ = height;
  }

  void destroy() {
    storage.clear();
    storage.shrink_to_fit();
  }

  void clear() {
    std::fill(storage.begin(), storage.end(), false);
  }

  void set(const uint32_t x, const uint32_t y, const bool value) {
    storage[x + static_cast<size_t>(y) * width_] = value;
  }

  bool get(const uint32_t x, const uint32_t y) const {
    return storage[x + static_cast<size_t>(y) * width_];
  }
};

int main() {
  uint32_t tableSize = 0;
  uint32_t pixelSize = 0;
  uint32_t inputSize = 0;

  for(uint8_t i = 4; i < 14; ++i) {
    uint32_t pSize = 0;
    pSize = std::pow(2, i);
    std::wcout << i - 3 << L".\t" << pSize << L"x" << pSize << std::endl;
  }
  std::wcout << L"Select output image size: ";
  std::wcin >> tableSize;
  tableSize += 3;
  pixelSize = std::pow(2, tableSize);
  _wsystem(L"cls");

  for(uint8_t i = 1; i < 10; ++i) {
    uint32_t pSize = 0;
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
  uint32_t x = 0;
  uint32_t y = 0;
  for(const auto& entry : fs::recursive_directory_iterator("images\\")) {
    if(!fs::is_regular_file(entry)) {
      continue;
    }
    if(entry.path().extension().string() != ".png") {
      continue;
    }
    std::wcout << L"Loading: " << entry.path().wstring() << std::endl;
    input.loadFromFile(entry.path().string());

    std::wcout << L"Allocating" << std::endl;
    found = false;
    for(uint32_t i = 0; i < tableSize; ++i) {
      for(uint32_t j = 0; j < tableSize; ++j) {
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
      std::wcout << L"\nNot enough space!\n" << std::endl;
      break;
    }

    x *= inputSize;
    y *= inputSize;

    std::wcout << L"Coping to " << x << L"x" << y << std::endl;

    output.copy(input, x, y, sf::IntRect(0, 0, inputSize, inputSize));
  }
  std::wcout << L"Saving output" << std::endl;
  output.saveToFile("OUTPUT.PNG");
  output.create(0, 0);

  _wsystem(L"pause");
}