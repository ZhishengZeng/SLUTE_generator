#include "Generator.h"

#include "Util.h"

namespace slut {

void Generator::init()
{
  // 初始化所有点集
  std::vector<Point> source_list;
  source_list.reserve(_point_num * _point_num);
  for (int i = 0; i < _point_num; i++) {
    for (int j = 0; j < _point_num; j++) {
      source_list.emplace_back(i, j);
    }
  }
  // 生成点的全组合
  std::vector<std::vector<Point>> result_list;
  // 从n个中取k个
  int n = source_list.size();
  int k = _point_num;
  // 初始化从0,1,2,3,...,(k-1)
  std::vector<int> idx_list;
  idx_list.resize(k);
  for (int i = 0; i < k; i++) {
    idx_list[i] = i;
  }

  int create_num = 0;

  while (true) {
    while (true) {
      std::vector<Point> temp_list;
      for (int i = 0; i < k; i++) {
        temp_list.push_back(source_list[idx_list[i]]);
      }
      // 过滤器
      if (Util::filterPointList(temp_list)) {
        result_list.push_back(temp_list);
        create_num++;
      }
      // 去重器 (压缩后出现大量重复)
      int new_num = 200000;  //<! 新增数量
      if (create_num % new_num == 0) {
        // 每新增二十万去一次重
        int delete_num = Util::UniquePointList(result_list);
        std::cout << "[INFO] Created " << result_list.size() << " combs (add "
                  << new_num - delete_num << " combs)" << std::endl;
      }

      if ((idx_list[k - 1] + 1) == n) {
        break;
      }
      idx_list[k - 1]++;
    }
    // 找到一个未达到上界的idx
    int nub_idx;
    for (nub_idx = (k - 1); nub_idx >= 0; nub_idx--) {
      if (idx_list[nub_idx] < (n - k + nub_idx)) {
        // 找到一个未达到上界 +1
        idx_list[nub_idx]++;
        // 将此上界右边的所有值设为逐项相加
        for (int i = (nub_idx + 1); i < k; i++) {
          idx_list[i] = idx_list[i - 1] + 1;
        }
        break;
      }
    }
    // 若都已达到上界,则退出
    if (nub_idx < 0) {
      break;
    }
  }

  // 全部去重
  Util::UniquePointList(result_list);
  std::cout << "[INFO] Created " << result_list.size() << " Combs" << std::endl;

  // 以point_list生成初始comb_list
  _comb_list.reserve(result_list.size());
  for (size_t i = 0; i < result_list.size(); i++) {
    _comb_list.emplace_back(_slut_file, result_list[i]);
  }
}

void Generator::process()
{
  int process_num = 0;
  for (size_t i = 0; i < _comb_list.size(); i++) {
    _comb_list[i].process();
    _comb_list[i].write();
    _comb_list[i].print();
    _comb_list[i].destroy();
    process_num++;
    std::cout << "[INFO] Processed " << process_num << "/" << _comb_list.size() << "("
              << process_num / 1.0 / _comb_list.size() * 100 << "%) combs" << std::endl;
  }
}

}  // namespace slut
