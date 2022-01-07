#include <algorithm>
#include <iterator>
#include <vector>

namespace prgrmr::generic
{
template<typename iterator_type>
void append_values(const iterator_type first,
                   const iterator_type last,
                   std::vector<typename iterator_type::value_type>& values)
{
    values.reserve(values.capacity() + std::distance(first, last));
    std::copy(first, last, std::back_insert_iterator(values));
}

template<typename value_type, std::size_t capacity_limit>
class circular_buffer
{
    private:
        using buffer_type = std::vector<value_type>;

        buffer_type _buffer;
        std::size_t _current_index{ 0 };

    public:
        circular_buffer()
        {
            _buffer.reserve(capacity_limit);
        }

        void clear()
        {
            _buffer.clear();
            _current_index = 0;
        }

        auto size() const
        {
            return _buffer.size();
        }

        auto capacity() const
        {
            return _buffer.capacity();
        }

        auto empty() const
        {
            return _buffer.empty();
        }

        auto is_filled() const
        {
            return (size() == capacity());
        }

        void push_back(const value_type& value)
        {
            if (!is_filled())
            {
                _buffer.push_back(value);
            }
            else
            {
                if (_current_index == capacity())
                    _current_index = 0;

                _buffer[_current_index] = value;
            }

            ++_current_index;
        }

        auto latest(const std::size_t num_elements) const
        {
            std::vector<value_type> values;
            values.reserve(std::min(num_elements, size()));

            auto remaining_count = values.capacity();

            if (is_filled() && remaining_count > 0)
            {
                const auto first = _buffer.rend() - _current_index;
                const auto last = _buffer.rend();

                append_values(first, last, values);

                remaining_count -= values.size();
            }

            append_values(_buffer.rbegin(), _buffer.rbegin() + remaining_count, values);

            return values;
        }

        auto earliest(const std::size_t num_elements) const
        {
            std::vector<value_type> values;
            values.reserve(std::min(num_elements, size()));

            auto remaining_count = values.capacity();

            if (is_filled() && remaining_count > 0)
            {
                const auto first = _buffer.begin() + _current_index;
                const auto last = (size() - _current_index > remaining_count)
                                ? first + remaining_count
                                : _buffer.end();

                append_values(first, last, values);

                remaining_count -= values.size();
            }

            append_values(_buffer.begin(), _buffer.begin() + remaining_count, values);

            return values;
        }
};
}
