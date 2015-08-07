/* GPL 2015 Jake Sebastian-Jones */

#ifndef __INCLUDE__BARCOMPONENT_HPP__
#define __INCLUDE__BARCOMPONENT_HPP__

#include "Factory.hpp"

class BarComponent
{
public:
    BarComponent();
    virtual ~BarComponent();
};

typedef Factory<BarComponent> BarComponentFactory;

#endif  /* __INCLUDE__BARCOMPONENT_HPP__ */
