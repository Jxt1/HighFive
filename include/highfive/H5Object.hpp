#ifndef H5OBJECT_HPP
#define H5OBJECT_HPP


#include <H5Ipublic.h>

namespace HighFive{

class Object{
public:

    ///
    /// \brief isValid
    /// \return true if current Object is a valid HDF5Object
    ///
    bool isValid() const;


    hid_t getId()  const;


protected:
    // empty constructor
    Object();

    // copy constructor, increase reference counter
    Object(const Object & other);

    // decrease reference counter
    virtual ~Object();

    // override object destruction phase
    // needed for specific HDF5 type
    virtual void destroy();

    hid_t _hid;
};


}



#include "bits/H5Object_misc.hpp"

#endif // H5OBJECT_HPP
