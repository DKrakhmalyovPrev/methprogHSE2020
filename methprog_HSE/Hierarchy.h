#pragma once
#include "TypeList.h"


template <typename T>
struct Holder
{
	T _obj;
	size_t sz = sizeof(T);
};

template <typename T, class Base>
struct DerHolder
	: public Base
{
	T _obj;
	size_t sz = sizeof(T);
};

template <class TList, template <class> class Unit>
struct GenScatterHierarchy;

template <class ...TList, template <class> class Unit>
struct GenScatterHierarchy<TypeList<TList...>, Unit>
	: Unit< typename TypeList<TList...>::Head >,
	GenScatterHierarchy< typename TypeList<TList...>::Tail, Unit >
{
	using TL = TypeList<TList...>;
	using leftBase = Unit< typename TypeList<TList...>::Head>;
	using rightBase = GenScatterHierarchy< typename TypeList<TList...>::Tail, Unit >;

	template <typename T> struct Rebind
	{
		using Result = Unit<T>;
	};
};

template <class TList, template <class> class Unit>
struct GenScatterHierarchy<TypeList<TList>, Unit>
	: Unit<TList>
{
	using TL = TList;
	using leftBase = Unit<TList>;
	using rightBase = Nulltype;

	template <typename T> struct Rebind
	{
		using Result = Unit<T>;
	};
};

template <template <class> class Unit>
struct GenScatterHierarchy<TypeList<Nulltype>, Unit>
{
	using TL = Nulltype;
	using leftBase = Nulltype;
	using rightBase = Nulltype;

	template <typename T> struct Rebind
	{
		using Result = Unit<T>;
	};
};

template <class TList, template <class T, class Base> class Unit, class Root = Nulltype>
struct GenLinearHierarchy;


template <class ...TList, template <class T, class Base> class Unit, class Root>
struct GenLinearHierarchy < TypeList<TList...>, Unit, Root>
	: public Unit< typename TypeList<TList...>::Head, GenLinearHierarchy<typename TypeList<TList...>::Tail, Unit, Root>>
{

};

template <class TList, template <class T, class Base> class Unit, class Root>
struct GenLinearHierarchy < TypeList<TList>, Unit, Root>
	: public Unit< TList, Root>
{

};

template <class H, size_t i> struct FieldHelper;

template <class H>
struct FieldHelper<H, 0>
{
	using ElementType = typename H::TL::Head;
	using UnitType = typename H::template Rebind<ElementType>::Result;

	static UnitType& Do(H& obj)
	{
		UnitType& leftBase = obj;
		return leftBase;
	}
};

template <class H, size_t i>
struct FieldHelper
{
	using ElementType = typename TypeAt<i, typename H::TL>::Result;
	using UnitType = typename H::template Rebind<ElementType>::Result;

	using rightBase = typename H::rightBase;

	static UnitType& Do(H& obj)
	{
		rightBase& RightBase = obj;
		return FieldHelper<rightBase, i - 1>::Do(RightBase);
	}
};


template< size_t i, class H>
typename FieldHelper<H, i>::UnitType&
Field(H& obj)
{
	return FieldHelper<H, i>::Do(obj);
}