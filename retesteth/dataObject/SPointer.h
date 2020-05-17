#pragma once
#include <cassert>
namespace dataobject
{
template <class T>
class GCP_SPointer;
class GCP_SPointerBase
{
private:
    int _nRef;
    bool _isEmpty;
    void AddRef() { _nRef++; }
    int DelRef()
    {
        _nRef--;
        return _nRef;
    }

public:
    GCP_SPointerBase() : _nRef(0), _isEmpty(false) {}
    template <class T>
    friend class GCP_SPointer;
};

template <class T>
class GCP_SPointer
{
private:
    T* _pointee;
    void release()
    {
        if (_pointee != nullptr)
        {
            assert(_pointee->_nRef > 0);
            if (_pointee->DelRef() == 0)
            {
                delete _pointee;
                _pointee = nullptr;
            }
        }
    }

public:
    explicit GCP_SPointer() : _pointee(nullptr) {}
    GCP_SPointer(int n) : _pointee(nullptr) {}
    explicit GCP_SPointer(T* pointee) : _pointee(pointee)
    {
        if (pointee != nullptr)
        {
            if (!(_pointee->_isEmpty))
                _pointee->AddRef();
            else
                _pointee = nullptr;
        }
    };
    GCP_SPointer(GCP_SPointer const& pnt) : _pointee(nullptr)
    {
        release();
        _pointee = pnt.getPointer();
        if (_pointee != nullptr)
        {
            if (!(_pointee->_isEmpty))
                _pointee->AddRef();
            else
                _pointee = nullptr;
        }
    }

    // !!! Unsafe function
    T* getPointer() const { return _pointee; }
    const T* getCPtr() const { return _pointee; }

    // Replace one pointer with another
    GCP_SPointer& operator=(GCP_SPointer const& rhs)
    {
        // If pointers are different. release our current pointer and add ref to another (rhs)
        if (_pointee != rhs.getCPtr())
        {
            release();
            _pointee = rhs.getPointer();
            _pointee->AddRef();
        }
        return *this;
    }

    GCP_SPointer& operator=(const int rhs)
    {
        _pointee = nullptr;
        return *this;
    }

    ~GCP_SPointer() { release(); };

    // !!! Really strange function
    void setEmpty(bool empty)
    {
        // Mark pointer as a ghost. Everyone will think that it is deleted, but it is not
        if (_pointee != nullptr)
        {
            if (_pointee->_nRef > 0)
                _pointee->_isEmpty = empty;
        }
    }

    ///
    int getRefCount() const
    {
        if (_pointee)
            return _pointee->_nRef;
        return -1;
    }

    ///
    bool isEmpty() const
    {
        if (_pointee != nullptr)
            return _pointee->_isEmpty;

        return true;
    }


    // Types convertion
    T& operator*() const { return *_pointee; }
    T* operator->() const { return _pointee; }
    operator T*() { return _pointee; }
    // operator GCP_SPointer<T>() { return GCP_SPointer<T>(this); }

    /*template <class D>
      operator GCP_SPointer<D> ()
      {
      GCP_SPointer<D> sliced = GCP_SPointer<D>(static_cast<D>_pointee);
      return sliced;
      }*/

    // Pointers address (pointer's pointer)
    // T** operator& () {	return &_pointee;	}

    bool operator!() const { return _pointee == 0; }
    // inline friend bool operator != (const GCP_SPointer& lhs, const GCP_SPointer& rhs) { return
    // lhs.pointee_ != rhs.pointee_;} inline friend bool operator != (const GCP_SPointer& lhs, int
    // rhs) { return lhs.pointee_ != rhs;}
    inline friend bool operator==(const GCP_SPointer& lhs, const GCP_SPointer& rhs)
    {
        return lhs._pointee == rhs._pointee;
    }
    // inline friend bool operator == (const SmartPtr& lhs, const T* rhs) { return lhs.pointee_ ==
    // rhs;} inline friend bool operator == (const T* lhs, const SmartPtr* rhs) { return lhs ==
    // rhs.pointee_;} inline friend bool operator != (const SmartPtr& lhs, const T* rhs) { return
    // lhs.pointee_ != rhs;} inline friend bool operator != (const T* lhs, const SmartPtr& rhs) {
    // return lhs != rhs.pointee_;}
};
}  // namespace dataobject
