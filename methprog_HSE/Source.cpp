#include <iostream>
#include "TypeList.h"
#include "ConversionSort.h"
#include "Hierarchy.h"
#include "MultiMethods.h"
#include "Functor.h"

using tltp = TypeList<int, int, float, double>;

class C
{

};


int f(C* x, char y)
{
	std::cout << y;
	return 2;// .* x;
}

class Dog1
{
public:
	int bark(char t)
	{
		std::cout << "Bark" << std::endl;
		return 5;
	}
};

int main()
{
	//tltp tl;
	//int lg = Length<tltp>::value;
	//Length<Nulltype>::value;
	//bool ib = isBase<BA, BA1>::res;

	//DerivedToFront<TypeList<BB, BA1, BB21, BA2, BA, BB2, BB1, BB>>::res dtf;
	using mgsh = GenScatterHierarchy <TypeList <int, float, double>, Holder>;

	mgsh sch;
	GenLinearHierarchy <TypeList <int, float, double>, DerHolder> lih;
	Field<0>(sch)._obj = 11;
	int q = Field<0>(sch)._obj;


	Animal* cat1 = new Cat();
	Animal* cat2 = new Cat();
	Animal* dog1 = new Dog();
	Animal* dog2 = new Dog();
	Animal* catdog2 = new CatDog();

	//RawDoubleDispatch(cat1, cat2);
	//RawDoubleDispatch(cat1, dog2);
	//RawDoubleDispatch(dog1, dog2);
	//RawDoubleDispatch(dog1, cat2);
	//RawDoubleDispatch(catdog2, dog2);
	//RawDoubleDispatch(dog1, catdog2);

	//void* vcat = new Cat();

	//Cat* p = dynamic_cast<Cat*>(reinterpret_cast<Animal*>(vcat));

	//StaticDispatcher<Meeting, Animal, TypeList<Dog, Cat, CatDog> >::Go(p, dog1);

	BasicDispatcher<Animal, Animal> bds;
	bds.add<Dog, Dog>(MeetDog);
	bds.add<Cat, Dog>(MeetCatDog);
	bds.add<Dog, Cat>(MeetCatDog);
	bds.add<Cat, Cat>(MeetCat);


	bds.Go(cat1, cat2);
	bds.Go(dog1, cat2);
	bds.Go(dog1, dog2);


	std::cout << typeid(Dog).hash_code() << std::endl; 
	std::cout << typeid(Cat).hash_code() << std::endl;

	functor<decltype(f)> pfunc(f);

	C c_;
	std::cout << std::endl << pfunc(&c_, 'r') << std::endl;

	functor<decltype(f)> pfunc2(pfunc);

	functor <int(Dog1, char)> pfunc3 = &Dog1::bark;

	Dog1 dg1;

	std::cout << pfunc3(dg1, 'd') << std::endl;

	// Доп задание
	// Реализовать расширение/свой функтор, который в конструкторе принимает часть аргументов функции
	// а остальные -- при вызове оператора ()
	return 0;
}
