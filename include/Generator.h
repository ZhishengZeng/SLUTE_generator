// Copyright 2020 Author ChibiMarukoZ
#ifndef INCLUDE_GENERATOR_H_
#define INCLUDE_GENERATOR_H_

#include "Comb.h"

namespace slute {
class Generator
{
 private:
  int               _point_num;
  std::vector<Comb> _comb_list;
  std::ofstream*    _slute_file;

 public:
  Generator()
  {
#if OBJECT_CREATE_LOG
    std::cout << "call class<Generator> constructor.\n";
#endif
  }
  Generator(const Generator& other) : _point_num(other._point_num), _comb_list(other._comb_list)
  {
#if OBJECT_COPY_LOG
    cout << "call class<Generator> copy constructor.\n";
#endif
  }

  Generator(Generator&& other) : _point_num(other._point_num), _comb_list(move(other._comb_list))
  {
#if OBJECT_MOVE_LOG
    cout << "call class<Generator> move constructor.\n";
#endif
  }

  ~Generator()
  {
#if OBJECT_DESTROY_LOG
    cout << "call class<Generator> destructor.\n";
#endif
  }

  Generator& operator=(const Generator& other)
  {
    _point_num = other._point_num;
    _comb_list = other._comb_list;
#if OBJECT_COPY_LOG
    cout << "call class<Generator> operator=(copy).\n";
#endif
    return (*this);
  }

  Generator& operator=(Generator&& other)
  {
    _point_num = other._point_num;
    _comb_list = move(other._comb_list);
#if OBJECT_MOVE_LOG
    cout << "call class<Generator> operator=(move).\n";
#endif
    return (*this);
  }

  // getter

  // setter
  void set_point_num(const int point_num) { _point_num = point_num; }
  void set_slute_file(std::ofstream* slute_file) { _slute_file = slute_file; }
  // function
  void init();
  void process();
};

}  // namespace slute
#endif  // INCLUDE_GENERATOR_H_
