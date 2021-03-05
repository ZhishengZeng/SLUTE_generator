// Copyright 2020 Author ChibiMarukoZ
#ifndef INCLUDE_POINT_H_
#define INCLUDE_POINT_H_
#include <sys/resource.h>
#include <sys/time.h>
#include <time.h>

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include <vector>
#define PARALLEL_FLAG 1
#define OBJECT_CREATE_LOG 0
#define OBJECT_COPY_LOG 0
#define OBJECT_MOVE_LOG 0
#define OBJECT_DESTROY_LOG 0
using namespace std;

namespace slut {
class Point
{
 private:
  int _x;
  int _y;

 public:
  Point()
  {
#if OBJECT_CREATE_LOG
    cout << "call class<Point> constructor.\n";
#endif
  }
  Point(int x, int y) : _x(x), _y(y)
  {
#if OBJECT_CREATE_LOG
    cout << "call class<Point> constructor.\n";
#endif
  }

  Point(const Point& other) : _x(other._x), _y(other._y)
  {
#if OBJECT_COPY_LOG
    cout << "call class<Point> copy constructor.\n";
#endif
  }

  Point(Point&& other) : _x(move(other._x)), _y(move(other._y))
  {
#if OBJECT_MOVE_LOG
    cout << "call class<Point> move constructor.\n";
#endif
  }

  ~Point()
  {
#if OBJECT_DESTROY_LOG
    cout << "call class<Point> destructor.\n";
#endif
  }

  Point& operator=(const Point& other)
  {
    _x = other._x;
    _y = other._y;
#if OBJECT_COPY_LOG
    cout << "call class<Point> operator=(copy).\n";
#endif
    return (*this);
  }

  Point& operator=(Point&& other)
  {
    _x = move(other._x);
    _y = move(other._y);
#if OBJECT_MOVE_LOG
    cout << "call class<Point> operator=(move).\n";
#endif
    return (*this);
  }

  bool operator==(Point& other) { return _x == other.get_x() && _y == other.get_y(); }
  bool operator!=(Point& other) { return !((*this) == other); }

  // getter
  int get_x() const { return _x; }
  int get_y() const { return _y; }

  // setter
  void set_x(const int x) { _x = x; }
  void set_y(const int y) { _y = y; }

  // function
  void reduce_x(const int reduction) { _x -= reduction; }
  void reduce_y(const int reduction) { _y -= reduction; }
};

}  // namespace slut
#endif  // INCLUDE_POINT_H_
