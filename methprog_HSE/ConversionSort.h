#pragma once

class A
{
public:
	int q = 10;
};



class B
{
	float b = 12.5;
};

class BA : public B
{

};

class BA1 : public BA
{
	virtual void goo() {};
};

class BA2 : public BA
{

};

class BA11 : public BA1
{

};
class BB : public B
{

};

class BB1 : public BB
{
	virtual void foo() {};
};

class BB2 : public BB
{

};

class BB21 : public BB2
{

};

//...................................................................//

template <class T, class U>
class Conversion
{
	using Small = char;
	struct Big { char x[2]; };

	static Small Test(const U&);
	static Big Test(...);
	static T makeT();

public:
	static constexpr bool exists = sizeof(Small) == sizeof(Test(makeT()));
	static constexpr bool equal = false;
};

template <class T>
class Conversion<T, T>
{
public:
	static constexpr bool exists = true;
	static constexpr bool equal = true;
};

template <bool one, bool two>
struct ANDNOT
{
	static constexpr bool res = false;
};

template <>
struct ANDNOT<true, false>
{
	static const bool res = true;
};

template <typename T, typename U>
struct isBase
{
	static const bool res = ANDNOT< Conversion<T, U>::exists, Conversion<T, U>::equal>::res;
};
//
////...................................................................//
//
//template <class TList, class T>
//struct MostDerived;
//
//template <class T>
//struct MostDerived<Nulltype, T>
//{
//	using res = T;
//};
//
//template<class ...TList, class T>
//struct MostDerived<TypeList<TList...>, T>
//{
//	using Cand = typename MostDerived<typename TypeList<TList...>::Tail, T>::res;
//
//	using res = Select<isBase<typename TypeList<TList...>::Head, Cand>::res,
//		typename TypeList<TList...>::Head, Cand>::res;
//};
////
//template<class T> struct DerivedToFront
//{
//	using res = Nulltype;
//};
//
//template<> struct DerivedToFront<TypeList<Nulltype>>
//{
//	using res = Nulltype;
//};
//
//template<class T>
//struct DerivedToFront<TypeList<T>>
//{
//	using res = T;
//};
//
//template <typename ...TList>
//struct DerivedToFront<TypeList<TList...>>
//{
//private:
//
//	using TheMostDerived = typename MostDerived<typename TypeList<TList...>::Tail, typename TypeList<TList...>::Head>::res;
//
//	using Temp = typename Replace< typename TypeList<TList...>::Tail, TheMostDerived, typename TypeList<TList...>::Head>::res;
//
//	using L = typename DerivedToFront<Temp>::res;
//
//public:
//	using res = TypeList<TheMostDerived, L>;
//};