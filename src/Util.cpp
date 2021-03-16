#include "Util.h"

namespace slute {

double Util::microtime()
{
  struct timeval  tv;
  struct timezone tz;
  gettimeofday(&tv, &tz);
  return tv.tv_sec + tv.tv_usec / 1000000.00;
}

bool Util::comparePointASCByX(Point& a, Point& b)
{
  return a.get_x() != b.get_x() ? a.get_x() < b.get_x() : a.get_y() < b.get_y();
}

bool Util::comparePointASCByY(Point& a, Point& b)
{
  return a.get_y() != b.get_y() ? a.get_y() < b.get_y() : a.get_x() < b.get_x();
}

bool Util::comparePointListASCByX(std::vector<Point>& a_list, std::vector<Point>& b_list)
{
  if (a_list.size() == b_list.size()) {
    for (size_t i = 0; i < a_list.size(); i++) {
      if (a_list[i] != b_list[i]) {
        return comparePointASCByX(a_list[i], b_list[i]);
      }
    }
    return false;
  } else {
    return a_list.size() < b_list.size();
  }
}

bool Util::compareEqualPointList(std::vector<Point>& point_list_a, std::vector<Point>& point_list_b)
{
  if (point_list_a.size() != point_list_b.size()) {
    return false;
  }
  for (size_t i = 0; i < point_list_a.size(); i++) {
    if (point_list_a[i] != point_list_b[i]) {
      return false;
    }
  }
  return true;
}

bool Util::compareEdgeASCByX(Edge& a, Edge& b)
{
  // 先比较p1
  if (a.get_p1() != b.get_p1()) {
    return comparePointASCByX(a.get_p1(), b.get_p1());
  } else {
    // p1相等比较p2
    return comparePointASCByX(a.get_p2(), b.get_p2());
  }
}

bool Util::compareEdgeListASCByX(std::vector<Edge>& a_list, std::vector<Edge>& b_list)
{
  if (a_list.size() == b_list.size()) {
    for (size_t i = 0; i < a_list.size(); i++) {
      if (a_list[i] != b_list[i]) {
        return compareEdgeASCByX(a_list[i], b_list[i]);
      }
    }
    return false;

  } else {
    return a_list.size() < b_list.size();
  }
}

bool Util::compareEqualEdgeList(std::vector<Edge>& edge_list_a, std::vector<Edge>& edge_list_b)
{
  if (edge_list_a.size() != edge_list_b.size()) {
    return false;
  }
  for (size_t i = 0; i < edge_list_a.size(); i++) {
    if (edge_list_a[i] != edge_list_b[i]) {
      return false;
    }
  }
  return true;
}

void Util::compressGeometryByX(std::vector<Point>& temp_list)
{
  // reduction减少量,正数
  int base = 0, reduction = 0;
  if (temp_list[0].get_x() != 0) {
    reduction += temp_list[0].get_x();
    base = temp_list[0].get_x();
    temp_list[0].reduce_x(reduction);
  }

  for (size_t j = 1; j < temp_list.size(); j++) {
    if (base == temp_list[j].get_x()) {
      temp_list[j].reduce_x(reduction);
    } else {
      reduction += (temp_list[j].get_x() - base - 1);
      base = temp_list[j].get_x();
      temp_list[j].reduce_x(reduction);
    }
  }
}

void Util::compressGeometryByY(std::vector<Point>& temp_list)
{
  int reduction = 0, base = 0;
  if (temp_list[0].get_y() != 0) {
    reduction += temp_list[0].get_y();
    base = temp_list[0].get_y();
    temp_list[0].reduce_y(reduction);
  }

  for (size_t j = 1; j < temp_list.size(); j++) {
    if (base == temp_list[j].get_y()) {
      temp_list[j].reduce_y(reduction);
    } else {
      reduction += (temp_list[j].get_y() - base - 1);
      base = temp_list[j].get_y();
      temp_list[j].reduce_y(reduction);
    }
  }
}
// filterCombs:按照x排序 x压缩 按照y排序 y压缩
bool Util::filterPointList(std::vector<Point>& temp_list)
{
  std::sort(temp_list.begin(), temp_list.end(), Util::comparePointASCByX);
  Util::compressGeometryByX(temp_list);
  std::sort(temp_list.begin(), temp_list.end(), Util::comparePointASCByY);
  Util::compressGeometryByY(temp_list);
  return true;
}
// UniqueCombs:按照x排序 每新增二十万个去重一次
int Util::UniquePointList(std::vector<std::vector<Point>>& result_list)
{
  // 先排序
  for (size_t i = 0; i < result_list.size(); i++) {
    std::sort(result_list[i].begin(), result_list[i].end(), Util::comparePointASCByX);
  }
  std::sort(result_list.begin(), result_list.end(), Util::comparePointListASCByX);

  // 后去重
  int    delete_num = 0;
  size_t save_id    = 0;
  size_t sentry_id  = 0;
  size_t soldier_id = sentry_id + 1;
  while (sentry_id < result_list.size()) {
    std::vector<Point>& sentry_point_list = result_list[sentry_id];
    while (soldier_id < result_list.size()) {
      std::vector<Point>& soldier_point_list = result_list[soldier_id];
      if (!Util::compareEqualPointList(sentry_point_list, soldier_point_list)) {
        break;
      }
      ++delete_num;
      ++soldier_id;
    }
    result_list[save_id] = sentry_point_list;
    ++save_id;
    if (!(soldier_id < result_list.size())) {
      break;
    }
    sentry_id  = soldier_id;
    soldier_id = sentry_id + 1;
  }
  result_list.erase(result_list.begin() + save_id, result_list.end());
  return delete_num;
};
// filterPreTrees:可能会导致出度边重叠
bool Util::filterEdgeList(std::vector<Edge>& temp_list)
{
  std::sort(temp_list.begin(), temp_list.end(), Util::compareEdgeASCByX);
  // 判断边是否有重叠 有重叠去重

  size_t save_id    = 0;
  size_t sentry_id  = 0;
  size_t soldier_id = sentry_id + 1;
  while (sentry_id < temp_list.size()) {
    Edge& sentry_edge = temp_list[sentry_id];
    while (soldier_id < temp_list.size()) {
      Edge& soldier_edge = temp_list[soldier_id];
      if (sentry_edge != soldier_edge) {
        break;
      }
      ++soldier_id;
    }
    temp_list[save_id] = sentry_edge;
    ++save_id;
    if (!(soldier_id < temp_list.size())) {
      break;
    }
    sentry_id  = soldier_id;
    soldier_id = sentry_id + 1;
  }

  temp_list.erase(temp_list.begin() + save_id, temp_list.end());

  return true;
}
// UniquePreTrees:出度边重叠会导致PreTree重复
int Util::UniqueEdgeList(std::vector<std::vector<Edge>>& result_list)
{
  // 先排序
  for (size_t i = 0; i < result_list.size(); i++) {
    std::sort(result_list[i].begin(), result_list[i].end(), Util::compareEdgeASCByX);
  }
  std::sort(result_list.begin(), result_list.end(), Util::compareEdgeListASCByX);

  // 后去重
  int delete_num = 0;

  size_t save_id    = 0;
  size_t sentry_id  = 0;
  size_t soldier_id = sentry_id + 1;
  while (sentry_id < result_list.size()) {
    std::vector<Edge>& sentry_edge_list = result_list[sentry_id];
    while (soldier_id < result_list.size()) {
      std::vector<Edge>& soldier_edge_list = result_list[soldier_id];
      if (!Util::compareEqualEdgeList(sentry_edge_list, soldier_edge_list)) {
        break;
      }
      ++delete_num;
      ++soldier_id;
    }
    result_list[save_id] = sentry_edge_list;
    ++save_id;
    if (!(soldier_id < result_list.size())) {
      break;
    }
    sentry_id  = soldier_id;
    soldier_id = sentry_id + 1;
  }

  result_list.erase(result_list.begin() + save_id, result_list.end());

  return delete_num;
}
// 匹配边数
bool Util::isMappingEdgeNum(int&              diff_count,
                            std::vector<int>& diff_num_list,
                            std::vector<int>& powv_edge_num_list,
                            std::vector<int>& pt_edge_num_list)
{
  diff_count = 0;
  diff_num_list.resize(powv_edge_num_list.size());
  for (size_t i = 0; i < powv_edge_num_list.size(); i++) {
    int diff = powv_edge_num_list[i] - pt_edge_num_list[i];
    if (diff < 0) {
      return false;
    }
    diff_count += diff;
    diff_num_list[i] = diff;
  }
  return true;
}
// 计算点的出度
void Util::countPointDegree(std::vector<std::pair<Point, int>>& point_num_list, Point& point)
{
  int is_push = 1;
  for (size_t i = 0; i < point_num_list.size(); i++) {
    if (point_num_list[i].first == point) {
      is_push = 0;
      point_num_list[i].second++;
      break;
    }
  }
  if (is_push == 1) {
    point_num_list.push_back(std::make_pair(point, 1));
  }
}
// 判断边的连通性
bool Util::isConnected(std::vector<Point>&                 point_list,
                       std::vector<Edge>&                  edge_list,
                       std::vector<std::pair<Point, int>>& point_num_list)
{
  // 初始化点访问记录
  std::vector<std::pair<Point, int>> point_visit_list;
  for (size_t i = 0; i < point_num_list.size(); i++) {
    point_visit_list.push_back(std::make_pair(point_num_list[i].first, 0));
  }

  // 初始化边访问记录
  std::vector<std::pair<Edge, int>> edge_visit_list;
  for (size_t i = 0; i < edge_list.size(); i++) {
    edge_visit_list.push_back(std::make_pair(edge_list[i], 0));
  }

  // 找一个度为1的点开始
  Point curr_point;
  for (size_t i = 0; i < point_num_list.size(); i++) {
    if (point_num_list[i].second == 1) {
      curr_point                 = point_num_list[i].first;
      point_visit_list[i].second = 1;
      break;
    }
  }

  // dfs
  std::stack<Point> point_stack;
  point_stack.push(curr_point);
  while (true) {
    // 从当前点出发,找到存在当前点且未访问的边 将此边置为visit
    // 若已经访问则跳过,若遍历完无可访问的边,则此点出栈 并将他的父节点设为当前节点
    int find_flag = 0;
    for (size_t i = 0; i < edge_visit_list.size(); i++) {
      if (edge_visit_list[i].second == 0) {
        Edge  edge = edge_visit_list[i].first;
        Point p1   = edge.get_p1();
        Point p2   = edge.get_p2();
        if (p1 == curr_point) {
          curr_point = p2;
          point_stack.push(curr_point);
          edge_visit_list[i].second = 1;
          find_flag                 = 1;
          break;
        } else if (p2 == curr_point) {
          curr_point = p1;
          point_stack.push(curr_point);
          edge_visit_list[i].second = 1;
          find_flag                 = 1;
          break;
        }
      }
    }

    if (find_flag == 0) {
      // 此点弹出
      point_stack.pop();
      // 若堆栈不为空
      if (!point_stack.empty()) {
        // 将父节点设为当前点
        curr_point = point_stack.top();
      } else {
        // 判断给定点是否都已遍历
        for (size_t i = 0; i < point_list.size(); i++) {
          for (size_t j = 0; j < point_visit_list.size(); j++) {
            if (point_list[i] == point_visit_list[j].first && point_visit_list[j].second == 0) {
              return false;
            }
          }
        }
        return true;
      }
    } else if (find_flag == 1) {
      // 查看备选点是否已经被访问 没被访问则将此点设为当前节点 否则存在环,返回flase
      for (size_t i = 0; i < point_visit_list.size(); i++) {
        if (curr_point == point_visit_list[i].first) {
          if (point_visit_list[i].second == 1) {
            return false;
          } else {
            point_visit_list[i].second = 1;
            break;
          }
        }
      }
    }
  }
}
// 过滤POST 判断是否连接给定点 是否有多余边 是否是树
bool Util::filterPOST(std::vector<Point>& point_list, std::vector<Edge>& temp_list)
{
  // 判断是否有重复的边
  sort(temp_list.begin(), temp_list.end(), compareEdgeASCByX);

  for (size_t i = 1; i < temp_list.size(); i++) {
    if (temp_list[i - 1] == temp_list[i]) {
      return false;
    }
  }

  std::vector<std::pair<Point, int>> point_num_list;
  // 判断点的度是否满足
  point_num_list.reserve(point_list.size());
  for (size_t i = 0; i < point_list.size(); i++) {
    point_num_list.push_back(std::make_pair(point_list[i], 0));
  }

  for (size_t i = 0; i < temp_list.size(); i++) {
    countPointDegree(point_num_list, temp_list[i].get_p1());
    countPointDegree(point_num_list, temp_list[i].get_p2());
  }

  for (size_t i = 0; i < point_num_list.size(); i++) {
    Point& point = point_num_list[i].first;
    if (isExistIn(point_list, point)) {
      // 对于给定点 度不能为0
      if (point_num_list[i].second == 0) {
        return false;
      }
    } else {
      // 对于非给定点 度不能为1
      if (point_num_list[i].second == 1) {
        return false;
      }
    }
  }

  // 无论连通与否 判断"边数==点数-1"是否成立
  if ((point_num_list.size() - 1) != temp_list.size()) {
    return false;
  }

  // 判断是否连通
  return isConnected(point_list, temp_list, point_num_list);
}
// 去重POST PreTree会导致重复的POST
int Util::UniquePOST(std::vector<std::vector<Edge>>& result_list)
{
  // 先排序
  for (size_t i = 0; i < result_list.size(); i++) {
    std::sort(result_list[i].begin(), result_list[i].end(), Util::compareEdgeASCByX);
  }
  std::sort(result_list.begin(), result_list.end(), Util::compareEdgeListASCByX);
  // 后去重
  int delete_num = 0;

  size_t save_id    = 0;
  size_t sentry_id  = 0;
  size_t soldier_id = sentry_id + 1;
  while (sentry_id < result_list.size()) {
    std::vector<Edge>& sentry_edge_list = result_list[sentry_id];
    while (soldier_id < result_list.size()) {
      std::vector<Edge>& soldier_edge_list = result_list[soldier_id];
      if (!Util::compareEqualEdgeList(sentry_edge_list, soldier_edge_list)) {
        break;
      }
      ++delete_num;
      ++soldier_id;
    }
    result_list[save_id] = sentry_edge_list;
    ++save_id;
    if (!(soldier_id < result_list.size())) {
      break;
    }
    sentry_id  = soldier_id;
    soldier_id = sentry_id + 1;
  }
  result_list.erase(result_list.begin() + save_id, result_list.end());
  return delete_num;
}
}  // namespace slute
