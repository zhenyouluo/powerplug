/* GPL 2015 Jake Sebastian-Jones */

#ifndef __INCLUDE_FACTORY_HPP__
#define __INCLUDE_FACTORY_HPP__

template<typename Interface> class Factory
{
public:
    virtual Interface *create() const = 0;
};

#endif  /* __INCLUDE_FACTORY_HPP__ */
