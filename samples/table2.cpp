#include "../squall/squall_vmstd.hpp"
#include "../squall/squall_table.hpp"
#include <iostream>

int main() {
	try {
		squall::VMStd vm;
		vm.dofile("table2.nut");

		squall::TableBase foo2;

		{
			squall::TableBase foo = vm.root_table().get<squall::TableBase>("foo");
			std::cout << foo.get<std::string>("bar") << std::endl;
			foo2 = foo;
		}
		std::cout << foo2.get<std::string>("bar") << std::endl;

		squall::ArrayBase fruits = foo2.get<squall::ArrayBase>("fruits");
		std::cout << fruits.get<std::string>(0) << std::endl;
		std::cout << fruits.get<std::string>(1) << std::endl;
		std::cout << fruits.get<std::string>(2) << std::endl;
	}
	catch(squall::squirrel_error& e) {
		std::cerr << e.what() << std::endl;
	}

    return 0;
}



