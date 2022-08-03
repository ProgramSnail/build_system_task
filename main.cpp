#include "interactive_mode.hpp"
#include "file_mode.hpp"

int main(int argc, char** argv) {
  if (argc < 2) {
    interactiveMode();
  } else {
    fileMode(argv[1]);
  }
}
