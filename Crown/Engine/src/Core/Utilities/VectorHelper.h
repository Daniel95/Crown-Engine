#pragma once

#include <vector>
#include <numeric>
#include <algorithm>

#include "Core/Utilities/Log.h"
#include "Core/Utilities/Utilities.h"
#include "Core/Utilities/Defines.h"

namespace Crown
{
	class VectorHelper
	{
	public:
		template<typename T>
		static bool Contains(const std::vector<T>& a_Vector, T a_Element);

		template<typename T>
		static void Remove(std::vector<T>& a_Vector, T a_Element);

		template<typename T>
		static T GetRandom(std::vector<T>& a_Vector);

		template<typename T>
		static void LogElements(std::vector<T>& a_Vector);

		template<typename T, class Function>
		static void ForEach(std::vector<T>& a_Vector, Function a_Function);
	};

	template <typename T>
	bool VectorHelper::Contains(const std::vector<T>& a_Vector, T a_Element)
	{
		return std::find(a_Vector.begin(), a_Vector.end(), a_Element) != a_Vector.end();
	}

	template <typename T>
	void VectorHelper::Remove(std::vector<T>& a_Vector, T a_Element)
	{
		CROWN_PROFILE_FUNCTION();
		ASSERT_ENGINE(Contains(a_Vector, a_Element), "The element you are trying to remove is not in the vector!");

		a_Vector.erase(std::remove(a_Vector.begin(), a_Vector.end(), a_Element), a_Vector.end());
	}

	template <typename T>
	T VectorHelper::GetRandom(std::vector<T>& a_Vector)
	{
		CROWN_PROFILE_FUNCTION();
		ASSERT_ENGINE(a_Vector.size() == 0, "Cannot GetRandom since size is 0!");

		int randomIndex = Random(0, static_cast<int>(a_Vector.size()) - 1);
		return a_Vector.at(randomIndex);
	}

	template <typename T>
	void VectorHelper::LogElements(std::vector<T>& a_Vector)
	{
		CROWN_PROFILE_FUNCTION();
		ForEach(a_Vector, [](const int x) { LOG_ENGINE(x); });
	}

	template <typename T, class Function>
	void VectorHelper::ForEach(std::vector<T>& a_Vector, Function a_Function)
	{
		CROWN_PROFILE_FUNCTION();
		std::for_each(a_Vector.begin(), a_Vector.end(), a_Function);
	}
}

