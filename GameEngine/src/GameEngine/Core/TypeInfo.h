#pragma once

#include <utility>
#include <string_view>

#define FUNCTION_SIG __FUNCSIG__
#define FUNCTION_PREFIX '<'
#define FUNCTION_SUFFIX '>'

namespace GE
{
	using id_type = uint64_t;

	namespace internal
	{
		template <typename T>
		[[nodiscard]] constexpr auto stripped_type_name() noexcept
		{
			std::string_view function{ FUNCTION_SIG };
			auto first = function.find_first_not_of(' ', function.find_first_of(FUNCTION_PREFIX) + 1);
			auto value = function.substr(first, function.find_last_of(FUNCTION_SUFFIX) - first);
			return value;
		}

		template <typename Type>
		[[nodiscard]] static std::string_view type_name(char) noexcept
		{
			static const auto value = stripped_type_name<Type>();
			return value;
		}

		template <typename Type>
		[[nodiscard]] static id_type type_hash(int) noexcept
		{
			static const auto value = std::hash<std::string_view>()._Do_hash(stripped_type_name<Type>());

			return value;
		}
	}

	template <typename Type, typename = void>
	struct type_hash final
	{
		[[nodiscard]] static constexpr id_type value() noexcept
		{
			return internal::type_hash<Type>(0);
		}

		[[nodiscard]] constexpr operator id_type() const noexcept
		{
			return value();
		}
	};

	template <typename Type, typename = void>
	struct type_name final
	{
		[[nodiscard]] static constexpr std::string_view value() noexcept
		{
			return internal::type_name<Type>(0);
		}

		[[nodiscard]] constexpr operator std::string_view() const noexcept
		{
			return value();
		}
	};

	struct type_info final
	{
		template <typename Type>
		constexpr type_info(std::in_place_type_t<Type>) noexcept
			: m_Identifier(type_hash<std::remove_cv_t<std::remove_reference_t<Type>>>::value()),
			  m_Alias{ type_name<std::remove_cv_t<std::remove_reference_t<Type>>>::value() }
		{
		}

		[[nodiscard]] constexpr id_type hash() const noexcept
		{
			return m_Identifier;
		}

		/*
		 * @brief Type name
		 * @return Type name
		 */
		[[nodiscard]] constexpr std::string_view name() const noexcept
		{
			return m_Alias;
		}



	private:
		id_type m_Identifier;
		std::string_view m_Alias;
	};

	[[nodiscard]] inline constexpr bool operator==(const type_info& lhs, const type_info& rhs) noexcept
	{
		return lhs.hash() == rhs.hash();
	}

	[[nodiscard]] inline constexpr bool operator!=(const type_info& lhs, const type_info& rhs) noexcept
	{
		return !(lhs == rhs);
	}

	[[nodiscard]] inline constexpr bool operator<(const type_info& lhs, const type_info& rhs) noexcept
	{
		return lhs.hash() < rhs.hash();
	}

	[[nodiscard]] inline constexpr bool operator>(const type_info& lhs, const type_info& rhs) noexcept
	{
		return !(lhs < rhs);
	}

	[[nodiscard]] inline constexpr bool operator<=(const type_info& lhs, const type_info& rhs) noexcept
	{
		return lhs.hash() <= rhs.hash();
	}

	[[nodiscard]] inline constexpr bool operator>=(const type_info& lhs, const type_info& rhs) noexcept
	{
		return !(lhs <= rhs);
	}

	template <typename Type>
	[[nodiscard]] const type_info& type_id() noexcept
	{
		if constexpr (std::is_same_v<Type, std::remove_cv_t<std::remove_reference_t<Type>>>)
		{
			static type_info instance{ std::in_place_type<Type> };
			return instance;
		}
		else
		{
			return type_id<std::remove_cv_t<std::remove_reference_t<Type>>>();
		}
	}
}

namespace std
{
	template <>
	struct hash<GE::type_info>
	{
		size_t operator()(const GE::type_info& info) const noexcept
		{
			return (size_t)info.hash();
		}
	};
}