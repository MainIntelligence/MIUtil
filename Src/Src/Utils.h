#pragma once
#include <memory>//literally just for std::move (try and do something about it later maybe, idk)


//copy NumToCopy elements from the memory location at src to that of dest
template <typename T>
void inline pcpy(T* dest, size_t NumToCopy, const T* src) noexcept {
	for (; NumToCopy > 0; NumToCopy--, dest++, src++) { *dest = *src; }
}

//advance the pointers at src and dest while copying data from src to dest
template <typename T>
void inline pbuildcpy(T*& dest, size_t NumToCopy, const T*& src) noexcept {
	for (; NumToCopy > 0; NumToCopy--, dest++, src++) { *dest = *src; }
}

//advance the src pointer, keep the 
template <typename T>
void inline pparsecpy(T* dest, size_t NumToCopy, const T*& src) noexcept {
	for (; NumToCopy > 0; NumToCopy--, dest++, src++) { *dest = *src; }
}

/*swap the the value of the first argument with that of the second
	(if types are different, conversion is used in place of assignment)*/
template <typename T>
void inline Swap(T& thing1, T& thing2) noexcept {
	const T temp = std::move(thing1);
	thing1 = std::move(thing2);
	thing2 = std::move(temp);
};

template <typename T>
inline const T& Maximum(const T& thing1, const T& thing2) noexcept {
	return (thing1 >= thing2) ? thing1 : thing2;
};

template <typename T>
inline const T& Minimum(const T& thing1, const T& thing2) noexcept {
	return (thing1 <= thing2) ? thing1 : thing2;
};

/*calls the callable (fn), with the arguments specified after the callable/function parameter.
	Typical usage would be with implementing callback systems. ie. bind function, forward args. */
template <typename Func, typename ...Args>
void inline Call(Func func, Args&&... args) {
	// Forward the arguments to the function.
	func(static_cast<decltype(args)>(args)...);
};

