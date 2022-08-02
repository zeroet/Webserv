#ifndef ITERATOR_TRAITS_HPP
#define ITERATOR_TRAITS_HPP

//#include "iterator.hpp"
#include "../src/HttpBlock.hpp"
#include <iterator>
#include <typeinfo> 
#ifdef __APPLE__
#else
#include <uchar.h> 
#endif

namespace ft
{
	/* enable_if */
	template<bool B, class T = void>
	struct enable_if {};
	template<class T>
	struct enable_if<true, T> { typedef T type; };

	/* context_constant */
	template <class T, T v>
	struct context_constant 
	{
		const static T		value = v;
		typedef T 			value_type;
		typedef context_constant<T,v> 	type;
		operator T() const { return v; }
	};
	typedef	context_constant<bool, true> true_context;
	typedef	context_constant<bool, false> false_context;

	/* is_context */
	template <typename T>
	struct is_context : false_context {};
	template <>
	struct is_context<HttpBlock> : true_context {};
	template <>
	struct is_context<ServerBlock> : true_context {};
	template <>
	struct is_context<LocationBlock> : true_context {};

};

#endif
