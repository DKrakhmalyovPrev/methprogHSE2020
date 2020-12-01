#pragma once

template <typename T>
class functor;

template <typename ResultType, typename ... ArgTypes>
class functor <ResultType(ArgTypes...)>
{
public:
	functor() : m_invoker() {};

	template <typename FuncT>
	functor(FuncT fun) : m_invoker(new free_functor_holder<FuncT>(fun)) {};

	functor(const functor& other) : m_invoker(other.m_invoker->clone()) {};

	template <typename FuncT, typename ClassT>
	functor(FuncT ClassT::* fun) : m_invoker(new member_functor_holder<FuncT, ArgTypes...>(fun)) {};

	functor& operator = (const functor& other)
	{
		m_invoker = other.m_invoker->clone();
	}

	ResultType operator ()(ArgTypes... args)
	{
		return m_invoker->invoke(args...);
	}

private:

	class functor_holder
	{
	public:
		functor_holder() {}
		virtual ~functor_holder() {}
		virtual ResultType invoke(ArgTypes...) = 0;

		virtual std::shared_ptr<functor_holder> clone() = 0;

	private:
		functor_holder(const functor_holder&);
		void operator =(const functor_holder&);
	};

	using invokerT = std::shared_ptr<functor_holder>;

	template <typename FuncT>
	class free_functor_holder : public functor_holder
	{
	public:
		free_functor_holder(FuncT fun) : functor_holder(), m_function(fun) {}

		virtual ResultType invoke(ArgTypes... args) override
		{
			return m_function(args...);
		}

		virtual invokerT clone() override
		{
			return invokerT(new free_functor_holder<FuncT>(m_function));
		}

	private:
		FuncT m_function;
	};

	template <typename FuncT, class ClassT, typename ... RestArgTypes>
	class member_functor_holder : public functor_holder
	{
	public:
		using mem_FuncT = FuncT ClassT::*;

		member_functor_holder(mem_FuncT fun) : functor_holder(), m_function(fun) {}

		virtual ResultType invoke(ClassT obj, RestArgTypes ... args) override
		{
			return (obj.*m_function)(args...);
		}

		virtual invokerT clone() override
		{
			return invokerT(new member_functor_holder(m_function));
		}

	private:
		mem_FuncT m_function;
	};


	invokerT m_invoker;
};