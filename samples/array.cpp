#include "../squall/squall_vmstd.hpp"
#include "../squall/squall_array_base.hpp"
#include <iostream>

int main() {
    try {
        squall::VMStd vm;
        vm.dofile("array.nut");

        squall::ArrayBase arr = vm.root_table().get<squall::ArrayBase>("arr");
        squall::TableBase color = arr.get<squall::TableBase>(0);

        std::cout << color.get<std::string>("color") << std::endl;
    }
    catch(squall::squirrel_error& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}



