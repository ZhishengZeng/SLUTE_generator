#include "Generator.h"
#include "Util.h"

using namespace slut;

int main()
{
  std::ofstream* slut_file = new std::ofstream("slut.txt");

  for (size_t i = 2; i <= 9; i++) {
    std::cout << "[INFO] Now create " << i << " points look-up table" << std::endl;
    double start, end;
    start = Util::microtime();

    Generator generator;
    generator.set_point_num(i);
    generator.set_slut_file(slut_file);
    generator.init();
    // generator.process();

    end = Util::microtime();
    std::cout << "[INFO] Created " << i << " points look-up table : " << (end - start) << "s"
              << std::endl;
  }

  slut_file->close();
  return 0;
}