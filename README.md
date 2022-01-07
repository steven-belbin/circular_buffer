# Circular Buffer
A circular buffer that has a fixed capacity size that shall never be grow once that sufficient number of
values have been added that meets the capacity size.

Thus once that the capacity has been reached, then the oldest values are discarded in favor of recent ones.

## Implementation
Basically a facade ontop of a std::vector<T> whose capacity has been defined at compile-time.

However, this circular buffer could be adaptable as to use other containers that could be considered
as being appropriate. Therefore, avoid relying too heavily of index idioms or pointers.

## Methods
### void clear() const
>Clears the buffer. Thus there are 0 values being preserved.

### size_t size() const
>Number of slots which contain a valid value.
>When all slots contains a valid value, then it's considered as being filled. 

### bool empty() const
>Indicates when no slots have a valid value.

### bool is_filled() const
>Indicates when all slots have a valid value such that it's at capacity limit.

### void push_back(const value_type& value)
>Puts a valid value into the next available slot.

### std::vector<value_type> latest(const size_t num_elements) const
>Returns the nth most recently added values.
>
>When the number of requested elements exceeds the number of slots with a valid value, then all values are
>returned. This includes the scenario when it's considered as full capacity.

#### Use cases
##### Scenario A
>Given that a circular buffer has a capacity to hold 100 values
>  And the circular buffer contains these 7 values [10, 22, 35, 7, 91, 33, 87]

##### Use case requesting none.
>Given the scenario A
> When requesting for the 0 newest elements
> Then the resulting set of values shall be []

##### Use case requesting less than current size.
> Given the scenario A
>  When requesting for the 3 newest elements
>  Then the resulting set of values shall be [87, 33, 91]

##### Use case requesting more than current size.
>Given the scenario A
> When requesting for 7 newest elements (or more)
> Then the resulting set of values shall be [87, 33, 91, 7, 35, 22, 10]


### std::vector<value_type> earliest(const size_t num_elements) const
>Returns the nth earliest added values.
>
>When the number of requested elements exceeds the number of slots >with a valid value, then all values are
>returned. This includes the scenario when it's considered as full capacity.

#### Use cases scenarios
1. Scenario A
  >Given that a circular buffer has a capacity to hold 100 values
  >  And the circular buffer contains these 7 values [10, 22, 35, 7, 91, 33, 87]

  - Use case requesting none.
    >Given the scenario A
    >When requesting for the 0 earliest elements
    >Then the resulting set of values shall be []

  - Use case requesting less than current size.
    >Given the scenario A
    > When requesting for the 3 earliest elements
    > Then the resulting set of values shall be [10, 22, 35]

  - Use case requesting more than current size.
    >Given the scenario A
    > When requesting for 7 earliest elements (or more)
    > Then the resulting set of values shall be [10, 22, 35, 7, 91, 33, 87]

## Technical debt
### Definiing custom iterators.
Introduce custom iterators into this circular buffer class such that it could be use as in
[C++ Boost circular_buffer class](https://www.boost.org/doc/libs/1_78_0/doc/html/circular_buffer.html)

Motivators is that it simplifies the implementation of latest & earliest methods.

>Reference would be the iterators defined in std::vector. With the difficulty being the exceeding the
>begin & end that will automatically need to wrap around.
>
>Adding idiom of `head` & `tail` as iterators will be useful, since those provide the  `begin` & `end`
>concepts of the circular especially once that the container has reached it's capacity limit.

```c++
auto latest(const std::size_t num_elements) const
{
    return copy_values(std::begin(*this),
                       std::min(num_elements, size()));
}

auto earliest(const std::size_t num_elements) const
{
    return copy_values(std::rbegin(*this),
                       std::min(num_elements, size()));
}

auto copy_values(const auto first,
                 const std::size_t count)
{
    std::vector<typename iterator_type::value_type> values;
    values.reserve(count);

    std::copy(first,
              first + count,
              std::back_insert_iterator(values));
}
```
>The above functions are generic algorithms since there are applicable for containers that could serve
>to provide queue paradigm such as `std::deque`, `std::queue`, `std::list`, `std::vector`, `std::array`, `std::stack`, etc...
