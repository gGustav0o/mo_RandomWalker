#pragma once

/*
* Pure Обертка над чистой(почти) compile-time функцией.
* используется дл¤ гарантии:
*	отсутстви¤ побочных эффектов(по смыслу),
*	[[nodiscard]],
*	noexcept(если переданна¤ функци¤ не бросает),
*	perfect forwarding.
*/
template <typename>
struct Pure;

// частична¤ специализаци¤ по сигнатуре функции: Ret(Args...)
template <typename Ret, typename... Args>
struct Pure<Ret(Args...)>
{
	using FnType = Ret(*)(Args...);
	FnType fn = nullptr;

	constexpr Pure() noexcept = default;
	constexpr Pure(FnType f) noexcept : fn(f) {}
	template <typename F, std::enable_if_t<
		std::is_convertible_v<F, FnType>, int> = 0>
	constexpr Pure(F f) noexcept : fn(static_cast<FnType>(f)) {}

	template <typename... CallArgs>
	[[nodiscard]] auto operator()(CallArgs&&... args) const
		noexcept(noexcept(fn(std::forward<CallArgs>(args)...)))
		-> decltype(fn(std::forward<CallArgs>(args)...))
	{
		return fn(std::forward<CallArgs>(args)...);
	}
};

#define FORWARD_FN(name)                                                    \
    template <typename... Args>                                             \
    decltype(auto) name(Args&&... args) const                               \
        noexcept(noexcept(dialectTraits.name(std::forward<Args>(args)...))) \
    {                                                                       \
        return dialectTraits.name(std::forward<Args>(args)...);             \
    }

#define FORWARD_SUB_FN(outer, name)                                               \
    template <typename... Args>                                                   \
    decltype(auto) name(Args&&... args) const                                     \
        noexcept(noexcept(dialectTraits.outer.name(std::forward<Args>(args)...))) \
    {                                                                             \
        return dialectTraits.outer.name(std::forward<Args>(args)...);             \
    }

#define NAME_OF(var) #var

#define STR_ENUM(EnumName, ...) \
    enum class EnumName { __VA_ARGS__, Count }; \
    static constexpr const char* EnumName##_names[] = { #__VA_ARGS__ }; \
    static constexpr const char* EnumName##ToString(EnumName value) noexcept { \
        const size_t index = static_cast<size_t>(value); \
        return (index < std::size(EnumName##_names)) ? EnumName##_names[index] : "Unknown"; \
    }

#define DECLARE_FACTORY(ClassName) \
    template<typename... Args>     \
    ClassName* create##ClassName(Args&&... args);

#define IMPLEMENT_FACTORY(ClassName) \
    template<typename... Args>       \
    ClassName* create##ClassName(Args&&... args) { return new ClassName(std::forward<Args>(args)...); }