#pragma once

//Quickly add weak comparisons defined in terms of strong comparisons
//	allows defining ==, <, > to automatically define !=, >=, and <= respectively

template<typename T>
struct FullComparor
{
	//FullComparor() = default;
	template<typename OTy>
	inline bool operator!=(const OTy& other) const {
		return !(static_cast<const T&>(*this) == other);
	}
	
	template<typename OTy>
	inline bool operator>=(const OTy& other) const {
		return !(static_cast<const T&>(*this) < other);
	};
	
	template<typename OTy>
	inline bool operator<=(const OTy& other) const {
		return !(static_cast<const T&>(*this) > other);
	}
private:
	FullComparor() = default;
	friend T;
};


