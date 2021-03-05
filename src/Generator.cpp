#include "Generator.h"

#include "Util.h"
namespace slut {

void Generator::run()
{
  std::cout << "[INFO] Generator Running..." << std::endl;
  // 生成点组合
  initCombs();
  // 处理每个点组合
  processCombs();
  std::cout << "[INFO] Generator End" << std::endl;
}

void Generator::initCombs()
{
  // 初始化所有点集
  std::vector<Point> source_point_list;
  source_point_list.reserve(_point_num * _point_num);
  for (int i = 0; i < _point_num; i++) {
    for (int j = 0; j < _point_num; j++) {
      source_point_list.emplace_back(i, j);
    }
  }
  // 生成点的全组合
  std::vector<std::vector<Point>> result_list;
  Util::combinePointList(
      result_list, source_point_list, _point_num, Util::filterPointList, Util::UniquePointList);
  // 以point_list生成初始comb_list
  _comb_list.reserve(result_list.size());
  for (size_t i = 0; i < result_list.size(); i++) {
    _comb_list.emplace_back(result_list[i]);
  }
}

void Generator::processCombs()
{
  int process_num = 0;
  for (size_t i = 0; i < _comb_list.size(); i++) {
    _comb_list[i].process();
    _comb_list[i].write();
    _comb_list[i].destroy();
    process_num++;
    std::cout << "[INFO] Processed " << process_num << "/" << _comb_list.size() << "("
              << process_num / 1.0 / _comb_list.size() * 100 << "%) combs" << std::endl;
  }
}

}  // namespace slut
