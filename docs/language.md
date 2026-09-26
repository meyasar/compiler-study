# Toy interpreter: integer rules

Values are signed 32-bit integers, from -2147483648 to 2147483647.

- Decimal literals are checked during parsing, before any statement executes.
- A positive literal must be at most 2147483647. The magnitude 2147483648
  is accepted only as the immediate number token following unary minus.
  Thus `-2147483648`, `(-2147483648)`, and `- 2147483648` are valid;
  `2147483648` and `-(2147483648)` are rejected. Leading zeros are allowed.
- Unary minus binds more tightly than multiplication and division. Repeated
  minus signs nest from right to left; binary arithmetic associates left to right.
- Division truncates toward zero: `-7 / 2` is `-3`.
- Addition, subtraction, multiplication, division, and negation report runtime
  errors if their results do not fit int32. Values never wrap around.
- Division by zero is a runtime error. Negating the minimum value or dividing
  it by -1 is also a runtime error.
- Runtime errors point to the operator. Output from earlier statements remains
  visible. Syntax and name-analysis errors prevent all execution.

The interpreter widens binary operands to int64 **before** arithmetic and checks
the result before narrowing to int32. Even the product of two int32 values fits
int64. Negation checks the minimum value before applying unary minus.

Name analysis checks definitions without evaluating arithmetic. It therefore
does not pre-detect overflow or division by zero in constant expressions.
