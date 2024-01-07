#include "iostream"

#include "Window.hpp"

int main() {

  std::cout << "Hello World\n";

  Window window = Window();

  window.renderWindowTemplate();
  window.renderWindowList();
  window.renderWindowCursor();
  window.refreshFrames();
  window.processInput();


  for (;;) {
  }

  return 0;

}
