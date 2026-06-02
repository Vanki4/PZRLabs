#include <iostream>
#include "splay_tree_api.h"
#include <string>

int main() {
	SplayTree<std::string,double> a;
	a.insert("cat",10.0);
	a.insert("dog",15.0);
	a.insert("pig",20.0);
	a.insert("DaniilMandrikin",100.0);
	auto i = a.search("dog");
	if (i)
	{
		std::cout << "Найдено: " << *i << std::endl;
	}
	else std::cout << "Ошибка" << std::endl;
	a.remove("DaniilMandrikin");
	std::cout << "Размер: " << a.size() << std::endl;
	if (a.isValidBST())
	{
		std::cout << "ВАЛИДНО" << std::endl;
	}
	return 0;	
}
