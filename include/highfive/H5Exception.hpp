#ifndef H5EXCEPTION_HPP
#define H5EXCEPTION_HPP

#include <stdexcept>
#include <string>
#include <boost/shared_ptr.hpp>

namespace HighFive{


///
/// \brief Basic HighFive Exception class
///
///
class Exception : public std::exception{
public:
    Exception(const std::string & err_msg) : _errmsg(err_msg), _next(), _err_major(0), _err_minor(0) {}

    virtual ~Exception() throw(){ }


    ///
    /// \brief get the current exception error message
    /// \return
    ///
    inline virtual const char* what() const throw(){
        return _errmsg.c_str();
    }

    ///
    /// \brief define the error message
    /// \param errmsg
    ///
    inline virtual void setErrorMsg(const std::string & errmsg){
        _errmsg = errmsg;
    }

    ///
    /// \brief nextException
    /// \return pointer to the next exception in the chain, or NULL if not existing
    ///
    inline Exception* nextException() const{
        return _next.get();
    }


    ///
    /// \brief HDF5 library error mapper
    /// \return HDF5 major error number
    ///
    inline hid_t getErrMajor() const{
        return _err_major;
    }

    ///
    /// \brief HDF5 library error mapper
    /// \return HDF5 minor error number
    ///
    inline hid_t getErrMinor() const{
        return _err_minor;
    }



protected:
    std::string _errmsg;
    boost::shared_ptr<Exception> _next;
    hid_t _err_major, _err_minor;


    friend struct HDF5ErrMapper;

};


///
/// \brief Exception specific to HighFive Object interface
///
class ObjectException : public Exception{
public:
    ObjectException(const std::string & err_msg) : Exception(err_msg){}
};

///
/// \brief Exception specific to HighFive File interface
///
class FileException : public Exception{
public:
    FileException(const std::string & err_msg) : Exception(err_msg){}

};


///
/// \brief Exception specific to HighFive DataSpace interface
///
class DataSpaceException : public Exception{
public:
    DataSpaceException(const std::string & err_msg) : Exception(err_msg){}

};



///
/// \brief Exception specific to HighFive DataSet interface
///
class DataSetException : public Exception{
public:
    DataSetException(const std::string & err_msg) : Exception(err_msg){}

};


}

#include "bits/H5Exception_misc.hpp"

#endif // H5EXCEPTION_HPP
