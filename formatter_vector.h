#include <format>
#include <vector>

template<typename value_type, typename char_type>
struct std::formatter<std::vector<value_type>, char_type>
{
    constexpr auto parse(auto& ctx)
    {
        return std::end(ctx);
    }

    constexpr auto format(const std::vector<value_type>& values, auto& ctx)
    {
        format_to(ctx.out(), "[");
        format_entries(values, ctx);
        return format_to(ctx.out(), "]");
    }

private:

    constexpr auto format_entries(const auto& values, auto& ctx)
    {
        auto it { std::begin(values) };
        const auto& end { std::end(values) };

        if (it != end)
        {
            format_to(ctx.out(), "{}", *it);
            ++it;
        }

        for (; it != end; ++it)
            format_to(ctx.out(), ", {}", *it);

        return ctx;
    }
};
