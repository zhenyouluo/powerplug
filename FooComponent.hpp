/* GPL 2015 Jake Sebastian-Jones */

#ifndef __INCLUDE__FOOCOMPONENT_HPP__
#define __INCLUDE__FOOCOMPONENT_HPP__

#include "Factory.hpp"

class FooComponent
{
public:
    FooComponent();
    virtual ~FooComponent();
};

typedef Factory<FooComponent> FooComponentFactory;

#endif  /* __INCLUDE__FOOCOMPONENT_HPP__ */
