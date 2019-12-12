#ifndef __PQ_H__
#define __PQ_H__
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>

template <class T,
          class Container = std::vector<T>,
          class Compare = std::less<typename Container::value_type>>
class movable_priority_queue : public std::priority_queue<T, Container, Compare>
{
public:
    T top_and_pop()
    {
        std::pop_heap(this->c.begin(), this->c.end(), this->comp);
        T value = std::move(this->c.back());
        this->c.pop_back();
        return value;
    }
};

#endif