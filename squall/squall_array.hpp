#ifndef SQUALL_ARRAY_HPP_
#define SQUALL_ARRAY_HPP_

#include "squall_array_base.hpp"

namespace squall {

class Array : public ArrayBase {
public:
    Array(VM& vm) : ArrayBase(vm.handle(), make_array(vm.handle())) {}
    ~Array() { sq_release(handle(), &arrayobj()); }

private:
    static HSQOBJECT make_array(HSQUIRRELVM vm) {
        HSQOBJECT arrayobj;
        sq_newarray(vm, 1);
        sq_getstackobj(vm, -1, &arrayobj);
        sq_addref(vm, &arrayobj);
        sq_pop(vm, 1);
        return arrayobj;
    }

};

}

#endif // SQUALL_ARRAY_HPP_
