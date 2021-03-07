// Copyright 2020 Author ChibiMarukoZ
#ifndef INCLUDE_EDGE_H_
#define INCLUDE_EDGE_H_
#include "Point.h"
namespace slut {
class Edge
{
 private:
  Point _p1;
  Point _p2;

 public:
  Edge(Point& p1, Point& p2) : _p1(move(p1)), _p2(move(p2))
  {
#if OBJECT_CREATE_LOG
    cout << "call class<Edge> constructor.\n";
#endif
  }

  Edge(const Edge& other) : _p1(other._p1), _p2(other._p2)
  {
#if OBJECT_COPY_LOG
    cout << "call class<Edge> copy constructor.\n";
#endif
  }

  Edge(Edge&& other) : _p1(move(other._p1)), _p2(move(other._p2))
  {
#if OBJECT_MOVE_LOG
    cout << "call class<Edge> move constructor.\n";
#endif
  }

  ~Edge()
  {
#if OBJECT_DESTROY_LOG
    cout << "call class<Edge> destructor.\n";
#endif
  }

  Edge& operator=(const Edge& other)
  {
    _p1 = other._p1;
    _p2 = other._p2;
#if OBJECT_COPY_LOG
    cout << "call class<Edge> operator=(copy).\n";
#endif
    return (*this);
  }

  Edge& operator=(Edge&& other)
  {
    _p1 = move(other._p1);
    _p2 = move(other._p2);
#if OBJECT_MOVE_LOG

    cout << "call class<Edge> operator=(move).\n";
#endif
    return (*this);
  }

  bool operator==(Edge& other) { return (_p1 == other.get_p1() && _p2 == other.get_p2()); }
  bool operator!=(Edge& other) { return !((*this) == other); }

  // getter
  Point& get_p1() { return _p1; }
  Point& get_p2() { return _p2; }

  // setter

  // function
  void print()
  {
    _p1.print();
    _p2.print();
  }
};

}  // namespace slut
#endif  // INCLUDE_EDGE_H_
