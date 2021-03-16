#include "Generator.h"
#include "Util.h"

using namespace slute;

int main()
{
  std::ofstream* slute_file = new std::ofstream("slute.txt");

  for (size_t i = 2; i <= 6; i++) {
    std::cout << "[INFO] Now create " << i << " points look-up table" << std::endl;
    double start, end;
    start = Util::microtime();

    Generator generator;
    generator.set_point_num(i);
    generator.set_slute_file(slute_file);
    generator.init();
    generator.process();

    end = Util::microtime();
    std::cout << "[INFO] Created " << i << " points look-up table : " << (end - start) << "s"
              << std::endl;
  }

  slute_file->close();
  return 0;
}