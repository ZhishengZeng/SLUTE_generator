#include "Generator.h"
#include "Util.h"

using namespace slut;

int main()
{
  for (size_t i = 4; i <= 8; i++) {
    std::cout << "[INFO] Now create " << i << " points look-up table" << std::endl;
    double start, end;
    start = Util::microtime();

    Generator generator;
    generator.set_point_num(i);
    generator.run();

    end = Util::microtime();
    std::cout << "[INFO] Created " << i << " points look-up table : " << (end - start) << "s"
              << std::endl;
  }
  return 0;
}