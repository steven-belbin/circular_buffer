#include <algorithm>
#include <iterator>
#include <vector>

namespace prgrmr::generic
{
template<typename value_type, std::size_t capacity_limit>
class circular_buffer
{
    private:
        using buffer_type = std::vector<value_type>;

        buffer_type _buffer;
        buffer_type::iterator _tail; // Tail is the earliest entry.
        buffer_type::iterator _head; // Head is the beyond the most recent entry.

    public:
        circular_buffer()
        {
            _buffer.reserve(capacity_limit);
            _tail = std::begin(_buffer);
            _head = std::end(_buffer);
        }

        void clear()
        {
            _buffer.clear();
            _tail = std::begin(_buffer);
            _head = std::end(_buffer);
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
                _head = std::end(_buffer);
            }
            else
            {
                if (_head == std::end(_buffer))
                    _head = std::begin(_buffer);

                *_head = value;

                _tail = _head;
                ++_head;
            }
        }

        auto latest(const std::size_t num_elements) const
        {
            std::vector<value_type> values;
            values.reserve(std::min(num_elements, size()));

            const auto first = std::rbegin(_buffer);
            auto last = first + values.capacity();
            const auto back_inserter = std::back_insert_iterator(values);

            if (is_filled() && first != last)
            {
                const auto start = std::rend(_buffer) + distance_from_begin();
                const auto finish = std::rend(_buffer);

                std::copy(start, finish, back_inserter);

                last -= values.size();
            }

            std::copy(first, last, back_inserter);

            return values;
        }

        auto earliest(const std::size_t num_elements) const
        {
            std::vector<value_type> values;
            values.reserve(std::min(num_elements, size()));

            const auto first = std::begin(_buffer);
            auto last = first + values.capacity();
            const auto back_inserter = std::back_insert_iterator(values);

            if (is_filled() && first != last)
            {
                const auto available_count =
                    std::min(distance_from_end(), values.capacity());

                const auto start = _head;
                const auto finish = _head + available_count;

                std::copy(start, finish, back_inserter);
                
                last -= values.size();
            }

            std::copy(first, last, back_inserter);

            return values;
        }

    private:
        auto distance_from_begin() const
        {
            using iter_type = buffer_type::const_iterator;

            const auto distance = std::distance<iter_type>(_head, std::begin(_buffer));

            return static_cast<std::size_t>(distance);
        }

        auto distance_from_end() const
        {
            using iter_type = buffer_type::const_iterator;

            const auto distance = std::distance<iter_type>(_head, std::end(_buffer));

            return static_cast<std::size_t>(distance);
        }
};
}
