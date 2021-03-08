#include "Comb.h"

#include "Util.h"
namespace slut {

void Comb::process()
{
  // 初始化点组合信息
  initCombInfo();
  // 初始化预备树
  initPreTree();
  // 初始化powv
  createPowv();
}

void Comb::initCombInfo()
{
  // 初始化powv_h与powv_v
  initPowvInfo();
  // 初始化powv总和上界
  initUpBound();
}

void Comb::initPowvInfo()
{
  for (size_t j = 0; j < _point_list.size(); j++) {
    int x   = _point_list[j].get_x();
    int y   = _point_list[j].get_y();
    _powv_h = _powv_h < x ? x : _powv_h;
    _powv_v = _powv_v < y ? y : _powv_v;
  }
}

void Comb::initUpBound()
{
  initBalancePoint();
  initHTreeLength();
  initVTreeLength();
  _upper_bound = std::min(_h_tree_length, _v_tree_length);
}

void Comb::initBalancePoint()
{
  double balance_x = 0, balance_y = 0;
  for (size_t i = 0; i < _point_list.size(); i++) {
    balance_x += _point_list[i].get_x();
    balance_y += _point_list[i].get_y();
  }
  balance_x /= _point_list.size();
  balance_y /= _point_list.size();
  // 四舍五入取整数点
  balance_x = std::round(balance_x);
  balance_y = std::round(balance_y);

  _balance_point.set_x(balance_x);
  _balance_point.set_y(balance_y);
}

void Comb::initHTreeLength()
{
  // 计算H树长度
  double pre_x = 0;
  std::sort(_point_list.begin(), _point_list.end(), Util::comparePointASCByX);
  int    balance_y = _balance_point.get_y();
  double min_y     = balance_y;
  double max_y     = balance_y;
  for (size_t j = 0; j < _point_list.size(); j++) {
    if (pre_x != _point_list[j].get_x()) {
      _h_tree_length += std::abs(max_y - min_y);
      pre_x = _point_list[j].get_x();
      min_y = balance_y;
      max_y = balance_y;
    }
    min_y = _point_list[j].get_y() < min_y ? _point_list[j].get_y() : min_y;
    max_y = max_y < _point_list[j].get_y() ? _point_list[j].get_y() : max_y;
  }
  _h_tree_length += std::abs(max_y - min_y);
  _h_tree_length += _powv_h;
}

void Comb::initVTreeLength()
{
  // 计算V树长度
  double pre_y = 0;
  std::sort(_point_list.begin(), _point_list.end(), Util::comparePointASCByY);
  int    balance_x = _balance_point.get_x();
  double min_x     = balance_x;
  double max_x     = balance_x;
  for (size_t j = 0; j < _point_list.size(); j++) {
    if (pre_y != _point_list[j].get_y()) {
      _v_tree_length += std::abs(max_x - min_x);
      pre_y = _point_list[j].get_y();
      min_x = balance_x;
      max_x = balance_x;
    }
    min_x = _point_list[j].get_x() < min_x ? _point_list[j].get_x() : min_x;
    max_x = max_x < _point_list[j].get_x() ? _point_list[j].get_x() : max_x;
  }
  _v_tree_length += std::abs(max_x - min_x);
  _v_tree_length += _powv_v;
}

void Comb::initPreTree()
{
  // 生成每个给定点的出度边集合
  std::vector<std::vector<Edge>> source_list;
  initDegreeEdgeList(source_list);
  std::vector<std::vector<Edge>> result_list;
  // 初始化全零开始
  std::vector<int> idx_list;
  idx_list.resize(source_list.size());
  // 生成idx序列
  while (true) {
    // 将最后一位加至上界
    while (true) {
      std::vector<Edge> edge_list;
      edge_list.reserve(idx_list.size());
      for (size_t i = 0; i < idx_list.size(); i++) {
        // 拷贝边
        edge_list.push_back(source_list[i][idx_list[i]]);
      }
      // 使用过滤器 过滤器可对结果进行修改
      if (Util::filterEdgeList(edge_list)) {
        result_list.push_back(move(edge_list));
      }
      // 若已达到上界,终止累加
      if ((idx_list[source_list.size() - 1] + 1)
          == (int) source_list[source_list.size() - 1].size()) {
        break;
      }
      idx_list[source_list.size() - 1]++;
    }
    // 找到一个未达到上界的idx
    int nub_idx;
    for (nub_idx = (idx_list.size() - 1); nub_idx >= 0; nub_idx--) {
      if ((idx_list[nub_idx] + 1) < (int) source_list[nub_idx].size()) {
        // 找到一个未达到上界 +1
        idx_list[nub_idx]++;
        // 将此上界右边的所有值设为0
        for (size_t i = (nub_idx + 1); i < idx_list.size(); i++) {
          idx_list[i] = 0;
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
  Util::UniqueEdgeList(result_list);
  // 建立PreTree
  _pre_tree_list.reserve(result_list.size());
  for (size_t i = 0; i < result_list.size(); i++) {
    PreTree pre_tree(result_list[i]);
    pre_tree.createEdgeNumList(_powv_h, _powv_v);
    _pre_tree_list.push_back(std::move(pre_tree));
  }
}

void Comb::initDegreeEdgeList(std::vector<std::vector<Edge>>& degree_edge_list)
{
  degree_edge_list.reserve(_point_list.size());
  for (size_t i = 0; i < _point_list.size(); i++) {
    std::vector<Edge> temp_list;
    temp_list.reserve(4);

    int x = _point_list[i].get_x();
    int y = _point_list[i].get_y();

    // 中心点
    Point m_point(x, y);
    // 右点
    Point r_point(x + 1, y);
    // 左点
    Point l_point(x - 1, y);
    // 上点
    Point t_point(x, y + 1);
    // 下点
    Point b_point(x, y - 1);

    if (_powv_h > 0) {
      if (x == 0) {
        temp_list.emplace_back(m_point, r_point);
      }
      if (0 < x && x < _powv_h) {
        temp_list.emplace_back(m_point, r_point);
        temp_list.emplace_back(l_point, m_point);
      }
      if (x == _powv_h) {
        temp_list.emplace_back(l_point, m_point);
      }
    }
    if (_powv_v > 0) {
      if (y == 0) {
        temp_list.emplace_back(m_point, t_point);
      }
      if (0 < y && y < _powv_v) {
        temp_list.emplace_back(m_point, t_point);
        temp_list.emplace_back(b_point, m_point);
      }
      if (y == _powv_v) {
        temp_list.emplace_back(b_point, m_point);
      }
    }
    degree_edge_list.push_back(move(temp_list));
  }
}

/**
 * @brief 热创建powv(每创建一个powv即马上创建对应的post)
 *             ***暴力傻瓜式解决多重集合的组合问题***
 *              _ _ _ _ 上限是 2 2 3 3
 *              那么 source_list 为 0011222333
 *              从 source_list 中取r个数,即为多重集合的r组合
 *              取出的r组合内有重复的组合,需要去重(效率低)
 */
void Comb::createPowv()
{
  // 最大增量 在11111....上增加的数
  int max_increment = _upper_bound - (_powv_h + _powv_v);
  // 初始化source_list
  std::vector<int> source_list;
  for (int i = 0; i < (_powv_h + _powv_v); i++) {
    if (i < _powv_h) {
      // 这里不是(_powv_v+1)而是_powv_v,因为是在111...的基础上追加的
      for (int j = 0; j < _powv_v; j++) {
        source_list.push_back(i);
      }
    } else {
      for (int j = 0; j < _powv_h; j++) {
        source_list.push_back(i);
      }
    }
  }

  for (int r = 0; r <= max_increment; r++) {
    // 初始化source_idx_list 从0,1,2,3,...,(r-1)
    std::vector<int> source_idx_list;
    source_idx_list.resize(r);
    for (int i = 0; i < r; i++) {
      source_idx_list[i] = i;
    }
    std::set<std::vector<int>> edge_num_memo_set;
    while (true) {
      while (true) {
        // 生成基本POWV edge_num_list(11111...)
        std::vector<int> edge_num_list(_powv_h + _powv_v, 1);
        // 生成追加索引序列
        std::vector<int> edge_num_idx_list;
        for (int i = 0; i < r; i++) {
          edge_num_idx_list.push_back(source_list[source_idx_list[i]]);
        }
        // 在基本的POWV上加r
        for (int i = 0; i < r; i++) {
          edge_num_list[edge_num_idx_list[i]]++;
        }
        // 检查是否出现此组合
        std::set<std::vector<int>>::iterator iter = edge_num_memo_set.find(edge_num_list);
        if (iter == edge_num_memo_set.end()) {
          // 报告时输出
          std::vector<int> report_temp = edge_num_list;
          // 未出现此组合 加入到备忘录中
          edge_num_memo_set.insert(edge_num_list);
          POWV powv(edge_num_list);
          // 获取可匹配的PreTree 并创建POST
          powv.createPost(_powv_h, _powv_v, _point_list, _pre_tree_list);
          // 如果此POWV出现有效POST则记录,重置增量,本轮为最后一轮
          if (powv.get_post_list().size() > 0) {
            reportEdgeNumList(report_temp, true);
            _powv_list.push_back(move(powv));
            max_increment = r;
          } else {
            reportEdgeNumList(report_temp, false);
          }
          // 如果此POWV出现有效POST则记录,直接结束
          if (powv.get_post_list().size() > 0) {
            _powv_list.push_back(move(powv));
            return;
          }
        }
        if (r == 0 || (source_idx_list[r - 1] + 1) == (int) source_list.size()) {
          break;
        }
        source_idx_list[r - 1]++;
      }
      // 找到一个未达到上界的idx
      int nub_idx;
      for (nub_idx = (r - 1); nub_idx >= 0; nub_idx--) {
        if (source_idx_list[nub_idx] < (int) (source_list.size() - r + nub_idx)) {
          // 找到一个未达到上界 +1
          source_idx_list[nub_idx]++;
          // 将此上界右边的所有值设为逐项相加
          for (int i = (nub_idx + 1); i < r; i++) {
            source_idx_list[i] = source_idx_list[i - 1] + 1;
          }
          break;
        }
      }
      // 若都已达到上界,则退出
      if (nub_idx < 0) {
        break;
      }
    }
  }
}

void Comb::reportEdgeNumList(std::vector<int>& edge_num_list, bool yes)
{
  if ((int) edge_num_list.size() != (_powv_h + _powv_v)) {
    std::cout << "[WRAN] edge_num_list.size() != (powv_h + powv_v) !!";
  }

  // print edge_num_list
  std::cout << "[INFO] [ ";
  for (int i = 0; i < _powv_h; i++) {
    std::cout << edge_num_list[i] << " ";
  }
  std::cout << "/ ";
  for (int i = _powv_h; i < (_powv_h + _powv_v); i++) {
    std::cout << edge_num_list[i] << " ";
  }
  std::cout << "] ";
  if (yes) {
    std::cout << "yes" << std::endl;
  } else {
    std::cout << "no" << std::endl;
  }
}

void Comb::write()
{
  std::ofstream& slut_file = (*_slut_file);
  // y坐标升序
  std::sort(_point_list.begin(), _point_list.end(), Util::comparePointASCByY);

  // 坐标xyxy
  slut_file << "$";
  for (size_t i = 0; i < _point_list.size(); i++) {
    slut_file << _point_list[i].get_x() << _point_list[i].get_y();
  }
  slut_file << "\n";
  // powv
  for (size_t i = 0; i < _powv_list.size(); i++) {
    if (_powv_list[i].get_post_list().size() == 0) {
      // 若此powv没有post 则跳过
      continue;
    }
    slut_file << "#";
    std::vector<int>& edge_num_list = _powv_list[i].get_edge_num_list();
    for (size_t j = 0; j < edge_num_list.size(); j++) {
      slut_file << edge_num_list[j];
    }
    slut_file << "\n";
    // post
    std::vector<POST> post_list = _powv_list[i].get_post_list();
    for (size_t j = 0; j < post_list.size(); j++) {
      std::vector<std::vector<int>>& edge_idx_list = post_list[j].get_edge_idx_list();
      for (size_t k = 0; k < edge_idx_list.size(); k++) {
        for (size_t l = 0; l < edge_idx_list[k].size(); l++) {
          slut_file << edge_idx_list[k][l];
        }
      }
      slut_file << "\n";
    }
  }
}

void Comb::print()
{
  printCombInfo();
  // printPreTrees();
  printPowvs();
}

void Comb::printCombInfo()
{
  std::cout << "[INFO] point_list:";
  for (size_t i = 0; i < _point_list.size(); i++)
    _point_list[i].print();
  std::cout << " powv_h:" << _powv_h;
  std::cout << " powv_v:" << _powv_v;
  std::cout << " balance_point:";
  _balance_point.print();
  std::cout << " h_tree_length:" << _h_tree_length;
  std::cout << " v_tree_length:" << _v_tree_length;
  std::cout << " upper_bound:" << _upper_bound;
  std::cout << std::endl;
}

void Comb::printPreTrees()
{
  if (_pre_tree_list.size() > 0) {
    std::cout << "[INFO] pre_tree_list:\n";
    for (size_t i = 0; i < _pre_tree_list.size(); i++)
      _pre_tree_list[i].print(_powv_h, _powv_v);
  }
}

void Comb::printPowvs()
{
  if (_powv_list.size() > 0) {
    std::cout << "[INFO] powv_list:\n";
    std::cout << "[INFO] ";
    for (size_t i = 0; i < _powv_list.size(); i++)
      _powv_list[i].print(_powv_h, _powv_v);
    std::cout << std::endl;
  }
}

void Comb::destroy()
{
  _point_list.clear();
  _pre_tree_list.clear();
  _powv_list.clear();
}

}  // namespace slut
