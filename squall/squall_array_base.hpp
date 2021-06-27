#ifndef SQUALL_ARRAY_BASE_HPP_
#define SQUALL_ARRAY_BASE_HPP_

#include <squirrel.h>
#include "squall_stack_operation.hpp"

namespace squall {

class ArrayBase {
public:
    ArrayBase() : vm_(nullptr) {
        sq_resetobject(&arrayobj_);
    }

    ArrayBase(HSQUIRRELVM vm, const HSQOBJECT& to) : vm_(vm), arrayobj_(to) {
        if ( !sq_isnull(arrayobj_) ) {
            assert(vm_);
            sq_addref(vm_, &arrayobj_);
        }
    }

    ArrayBase(const ArrayBase& o) {
        vm_ = o.vm_;
        arrayobj_ = o.arrayobj_;
        if ( !sq_isnull(arrayobj_) ) {
            sq_addref(vm_, &arrayobj_);
        }
    }

    virtual ~ArrayBase() {
        if ( !sq_isnull(arrayobj_) ) {
            sq_release(vm_, &arrayobj_);
        }
    }

    ArrayBase& operator=(const ArrayBase& o) {
        if ( this != &o ) {
            HSQUIRRELVM tmpvm = vm_;
            HSQOBJECT tmpobj = arrayobj_;
            vm_ = o.vm_;
            arrayobj_ = o.arrayobj_;
            // thisが違っても、同じテーブルインスタンスをさしているかもしれない
            if ( !sq_isnull(arrayobj_) ) {
                sq_addref(vm_, &arrayobj_);
            }
            if ( !sq_isnull(tmpobj) ) {
                sq_release(tmpvm, &tmpobj);
            }
        }
        return *this;
    }

    template <class T>
    T get(SQInteger idx) {
        T r;
        if(get<T>(idx, r)) {
            return r;
        }
        throw squirrel_error("index '" + std::to_string(idx) + "'is out of range");
    }

    template <class T>
    bool get(SQInteger idx, T& r) {
        sq_pushobject(vm_, arrayobj_);
        sq_pushinteger(vm_, idx);
        if (!SQ_SUCCEEDED(sq_get(vm_, -2))) {
            return false;
        }
        r = detail::fetch<T, detail::FetchContext::ArrayEntry>(vm_, -1);
        sq_pop(vm_, 2);
        return true;
    }

    SQInteger size() const {
        sq_pushobject(vm_, arrayobj_);
        SQInteger r = sq_getsize(vm_, -1);
        sq_pop(vm_, 1);
        return r;
    }

protected:
    HSQUIRRELVM handle() { return vm_; }
    HSQOBJECT&  arrayobj() { return arrayobj_; }

    template <class T>
    bool get(SQInteger idx, T& r, SQObjectType type) {
        sq_pushobject(vm_, arrayobj_);
        sq_pushinteger(vm_, idx);
        if (!SQ_SUCCEEDED(sq_get(vm_, -2))) {
            return false;
        }
        r = detail::fetch<T, detail::FetchContext::ArrayEntry>(vm_, -1, type);
        sq_pop(vm_, 2);
        return true;
    }

private:
    HSQUIRRELVM vm_;
    HSQOBJECT   arrayobj_;

};

template <> inline
bool TableBase::get(const string& name, ArrayBase& r) {
    sq_pushobject(vm_, tableobj_);
    sq_pushstring(vm_, name.data(), name.length());
    if (!SQ_SUCCEEDED(sq_get(vm_, -2))) {
        return false;
    }
    HSQOBJECT o = detail::fetch<HSQOBJECT, detail::FetchContext::TableEntry>(vm_, -1, OT_ARRAY);
    sq_pop(vm_, 2);
    r = ArrayBase(vm_, o);
    return true;
}

template <> inline
ArrayBase TableBase::get(const string& name) {
    HSQOBJECT obj;
    if (get<HSQOBJECT>(name, obj, OT_ARRAY)) {
        return ArrayBase(vm_, obj);
    }
    throw squirrel_error("slot '" + name + "' not found");
}

template <> inline
TableBase ArrayBase::get(SQInteger idx) {
    HSQOBJECT obj;
    if (get<HSQOBJECT>(idx, obj, OT_TABLE)) {
        return TableBase(vm_, obj);
    }
    throw squirrel_error("index '" + std::to_string(idx) + "'is out of range");
}

template <> inline
ArrayBase ArrayBase::get(SQInteger idx) {
    HSQOBJECT obj;
    if (get<HSQOBJECT>(idx, obj, OT_ARRAY)) {
        return ArrayBase(vm_, obj);
    }
    throw squirrel_error("index '" + std::to_string(idx) + "'is out of range");
}

}

#endif // SQUALL_ARRAY_BASE_HPP_
