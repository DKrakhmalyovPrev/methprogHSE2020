#pragma once
#include <iostream>
#include "TypeList.h"
#include <unordered_map>
#include <typeindex>

class Animal
{
	virtual void f() {};
};

class Dog
	: public virtual Animal
{
	virtual void f() override {};
};

class Cat
	: public virtual Animal
{
	virtual void f() override {};
};

class CatDog
	: public virtual Dog,
	public virtual Cat
{
	virtual void f() override {};
};


class Meeting
{
public:
	static void meet(Dog* one, Dog* two)
	{
		std::cout << "Bark Bark" << std::endl;
	}
	static void meet(Cat* one, Dog* two)
	{
		std::cout << "Meow Bark" << std::endl;
	}
	static void meet(Dog* one, Cat* two)
	{
		std::cout << "Meow Bark" << std::endl;
	}
	static void meet(Cat* one, Cat* two)
	{
		std::cout << "Meow Meow" << std::endl;
	}
	static void meet(Dog* one, CatDog* two)
	{
		std::cout << "Who are you?" << std::endl;
	}
	static void meet(CatDog* one, Dog* two)
	{
		std::cout << "Who are you?" << std::endl;
	}
	static void meet(Cat* one, CatDog* two)
	{
		std::cout << "Who are you?" << std::endl;
	}
	static void meet(CatDog* one, Cat* two)
	{
		std::cout << "Who are you?" << std::endl;
	}

	static void meet(CatDog* one, CatDog* two)
	{
		std::cout << "Who are you?" << std::endl;
	}


	static void meet(...)
	{
		std::cout << "Something bad happend" << std::endl;
	}
};

//
//void RawDoubleDispatch(Animal* one, Animal* two)
//{
//	if (typeid(*one) == typeid(CatDog))
//	{
//		CatDog* p = dynamic_cast<CatDog*>(one);
//		if (CatDog* q = dynamic_cast<CatDog*>(two))
//		{
//			Meeting::meet(dynamic_cast<CatDog*>(one), q);
//		}
//		else
//		if (Dog* q = dynamic_cast<Dog*>(two))
//		{
//			Meeting::meet(dynamic_cast<CatDog*>(one), q);
//		}
//		else
//			if (Cat* q = dynamic_cast<Cat*>(two))
//			{
//				Meeting::meet(dynamic_cast<CatDog*>(one), q);
//			}
//			else
//			{
//				std::cout << "No match types" << std::endl;
//			}
//	}
//	else
//	if (Dog* p = dynamic_cast<Dog*>(one))
//	{
//		if (CatDog* q = dynamic_cast<CatDog*>(two))
//		{
//			Meeting::meet(p, q);
//		}
//		else
//		if (Dog* q = dynamic_cast<Dog*>(two))
//		{
//			Meeting::meet(p, q);
//		}
//		else
//		if (Cat* q = dynamic_cast<Cat*>(two))
//		{
//			Meeting::meet(p, q);
//		}
//		else
//		{
//			std::cout << "No match types" << std::endl;
//		}
//	}
//	else
//	if (Cat* p = dynamic_cast<Cat*>(one))
//	{
//		if (CatDog* q = dynamic_cast<CatDog*>(two))
//		{
//			Meeting::meet(p, q);
//		}
//		else
//		if (Dog* q = dynamic_cast<Dog*>(two))
//		{
//			Meeting::meet(p, q);
//		}
//		else
//		if (Cat* q = dynamic_cast<Cat*>(two))
//		{
//			Meeting::meet(p, q);
//		}
//		else
//		{
//			std::cout << "No match types" << std::endl;
//		}
//	}
//	else
//	{
//		std::cout << "No match types" << std::endl;
//	}
//}


template
<
	class Executor,
	class BaseLhs,
	class TypesLhs,
	class BaseRhs = BaseLhs,
	class TypesRhs = TypesLhs,
	typename ResultType = void
>
class StaticDispatcher
{
	using Head = typename TypesLhs::Head;
	using Tail = typename TypesLhs::Tail;

public:

	static ResultType Go(BaseLhs* lhs, BaseRhs* rhs)
	{
		if (typeid(*lhs) == typeid(Head))
		{
			Head* p = dynamic_cast<Head*>(lhs);
			return StaticDispatcher< Executor, BaseLhs, TypesLhs, BaseRhs, TypesRhs, ResultType>::DispatchRhs(p, rhs);
		}
		else
		{
			return StaticDispatcher< Executor, BaseLhs, Tail, BaseRhs, TypesRhs, ResultType>::Go(lhs, rhs);
		}
	}

	template <class SomeLhs>
	static ResultType DispatchRhs(SomeLhs* lhs, BaseRhs* rhs)
	{
		using RHead = typename TypesRhs::Head;
		using RTail = typename TypesRhs::Tail;

		if (typeid(*rhs) == typeid(RHead))
		{
			RHead* p = dynamic_cast<RHead*>(rhs);
			return Executor::meet(lhs, p);
		}
		else
		{
			return StaticDispatcher< Executor, BaseLhs, TypesLhs, BaseRhs, RTail, ResultType>::DispatchRhs(lhs, rhs);
		}
	}
};


template
<
	class Executor,
	class BaseLhs,
	class BaseRhs,
	class TypesRhs,
	typename ResultType
>
class StaticDispatcher< Executor, BaseLhs, Nulltype, BaseRhs, TypesRhs, ResultType>
{
public:

	static ResultType Go(BaseLhs* lhs, BaseRhs* rhs)
	{
		return ResultType();
	}

	template <class SomeLhs>
	static ResultType DispatchRhs(SomeLhs* lhs, BaseRhs* rhs)
	{
		return ResultType();
	}
};

template
<
	class Executor,
	class BaseLhs,
	class TypesLhs,
	class BaseRhs,
	typename ResultType
>
class StaticDispatcher< Executor, BaseLhs, TypesLhs, BaseRhs, Nulltype, ResultType>
{
public:

	static ResultType Go(BaseLhs* lhs, BaseRhs* rhs)
	{
		return ResultType();
	}

	template <class SomeLhs>
	static ResultType DispatchRhs(SomeLhs* lhs, BaseRhs* rhs)
	{
		return ResultType();
	}
};


void MeetDog(Animal* one, Animal* two)
{
	std::cout << "Bark Bark";
}

void MeetCat(Animal* one, Animal* two)
{
	std::cout << "Meow Meow";
}

void MeetCatDog(Animal* one, Animal* two)
{
	std::cout << "Bark Meow";
}

struct CompPair
{
	size_t operator()(const std::pair< std::type_index, std::type_index>& pr) const
	{
		static std::hash<std::type_index> z;

		return z(pr.first) * 31 + z(pr.second);
	}
};

template
<
	class BaseLhs,
	class BaseRhs,
	typename ResultType = void,
	typename CallbackType = ResultType (*)(BaseLhs*, BaseRhs*)
>
class BasicDispatcher
{
	using KeyType = std::pair<std::type_index, std::type_index>;
	using MappedType = CallbackType;
	using MapType = std::unordered_map<KeyType, MappedType, CompPair>;

	MapType callbackMap;

public:

	template <class TypeLhs, class TypeRhs>
	void add(CallbackType fun)
	{
		callbackMap.emplace(KeyType(typeid(TypeLhs), typeid(TypeRhs)), fun);
	}

	template <class TypeLhs, class TypeRhs>
	void remove()
	{
		callbackMap.erase(KeyType(typeid(TypeLhs), typeid(TypeRhs)));
	}

	ResultType Go(BaseLhs* one, BaseRhs* two)
	{
		KeyType k(typeid(*one), typeid(*two));
		typename MapType::iterator it = callbackMap.find(k);

		if (it == callbackMap.end())
		{
			std::cerr << "Error. Types not found";
			return (ResultType)nullptr;
		}

		return (it->second)(one, two);
	}
};