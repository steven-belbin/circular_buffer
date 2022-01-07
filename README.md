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
Clears the buffer. Thus there are 0 values being preserved.

### size_t size() const
Number of slots which contain a valid value.
When all slots contains a valid value, then it's considered as being filled. 

### bool empty() const
Indicates when no slots have a valid value.

### bool is_filled() const
Indicates when all slots have a valid value such that it's at capacity limit.

### void push_back(const value_type& value)
Puts a valid value into the next available slot.

### std::vector<value_type> latest(const size_t num_elements) const
Returns the nth most recently added values.

When the number of requested elements exceeds the number of slots with a valid value, then all values are
returned. This includes the scenario when it's considered as full capacity.

#### Use cases
##### Scenario A
  Given that a circular buffer has a capacity to hold 100 values
    And the circular buffer contains these 7 values [10, 22, 35, 7, 91, 33, 87]

##### Use case requesting none.
 Given the scenario A
  When requesting for the 0 newest elements
  Then the resulting set of values shall be []

##### Use case requesting less than current size.
 Given the scenario A
  When requesting for the 3 newest elements
  Then the resulting set of values shall be [87, 33, 91]

##### Use case requesting more than current size.
Given the scenario A
 When requesting for 7 newest elements (or more)
 Then the resulting set of values shall be [87, 33, 91, 7, 35, 22, 10]


### std::vector<value_type> earliest(const size_t num_elements) const
Returns the nth earliest added values.

When the number of requested elements exceeds the number of slots with a valid value, then all values are
returned. This includes the scenario when it's considered as full capacity.

#### Use cases
##### Scenario A
  Given that a circular buffer has a capacity to hold 100 values
    And the circular buffer contains these 7 values [10, 22, 35, 7, 91, 33, 87]

##### Use case requesting none.
 Given the scenario A
  When requesting for the 0 earliest elements
  Then the resulting set of values shall be []

##### Use case requesting less than current size.
 Given the scenario A
  When requesting for the 3 earliest elements
  Then the resulting set of values shall be [10, 22, 35]

##### Use case requesting more than current size.
Given the scenario A
 When requesting for 7 earliest elements (or more)
 Then the resulting set of values shall be [10, 22, 35, 7, 91, 33, 87]


## Technical debt
Introduce actual iterators such as begin, end, cbegin, cend, rbegin, rend, .....

A key motivation is that it simplifies the implementation of latest & earliest methods

auto latest(const std::size_t num_elements) const
{
    const auto first = begin();
    const auto last = (num_elements > size()) ? end() : first + num_elements;

    std::vector<typename iterator_type::value_type> values;
    values.reserve(std::distance(first, last));

    std::copy(first, last, std::back_insert_iterator(values));

    return values;
}

auto earliest(const std::size_t num_elements) const
{
    const auto first = rbegin();
    const auto last = (num_elements > size()) ? rend() : first + num_elements;

    std::vector<typename iterator_type::value_type> values;
    values.reserve(std::distance(first, last));

    std::copy(first, last, std::back_insert_iterator(values));

    return values;
}