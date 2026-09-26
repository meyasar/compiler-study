function(check_case name source expected_status expected_output expected_error)
    set(input "${TEST_DIR}/${name}.toy")
    file(WRITE "${input}" "${source}")
    execute_process(COMMAND "${INTERPRETER}" "${input}"
        RESULT_VARIABLE status OUTPUT_VARIABLE output ERROR_VARIABLE error)
    string(REPLACE "\r\n" "\n" output "${output}")
    string(REPLACE "\r\n" "\n" error "${error}")
    if(NOT "${status}" STREQUAL "${expected_status}"
        OR NOT "${output}" STREQUAL "${expected_output}"
        OR NOT "${error}" STREQUAL "${expected_error}")
        message(FATAL_ERROR "${name}: exit=${status}, stdout=[${output}], stderr=[${error}]")
    endif()
endfunction()

file(MAKE_DIRECTORY "${TEST_DIR}")
check_case(values "let x = 5; print -5; print -x; print -(2 + 3); print --5; print -0;"
    0 "-5\n-5\n-5\n5\n0\n" "")
check_case(precedence "print -2 * 3 + 10; print 2 * -3; print 10 - -3; print -8 / 2; print 8 / -2; print 10 - 3 - 2;"
    0 "4\n-6\n13\n-4\n-4\n5\n" "")
check_case(undefined "print 42;\nprint -missing;" 1 ""
    "Error: at 2:8: Undefined variable: missing\n")
check_case(missing_operand "print -;" 1 ""
    "Error: at 1:8: Expected expression. Found: ;\n")
check_case(overflow "let x = -2147483647 - 1;\nprint -x;" 1 ""
    "Error: at 2:7: Integer overflow in unary minus\n")

check_case(bounds "print 2147483647; print -2147483648; print (-2147483648); print -0002147483648;"
    0 "2147483647\n-2147483648\n-2147483648\n-2147483648\n" "")
check_case(truncation "print -7 / 2; print 7 / -2; print -7 / -2; print -1 / 2;"
    0 "-3\n-3\n3\n0\n" "")
check_case(valid_boundary_arithmetic "print 2147483646 + 1; print -2147483647 - 1; print -1073741824 * 2; print -2147483648 / 1;"
    0 "2147483647\n-2147483648\n-2147483648\n-2147483648\n" "")
check_case(add_overflow "print 2147483647 + 1;" 1 ""
    "Error: at 1:18: Integer overflow\n")
check_case(add_underflow "print -2147483648 + -1;" 1 ""
    "Error: at 1:19: Integer overflow\n")
check_case(subtract_underflow "print -2147483648 - 1;" 1 ""
    "Error: at 1:19: Integer overflow\n")
check_case(subtract_overflow "print 2147483647 - -1;" 1 ""
    "Error: at 1:18: Integer overflow\n")
check_case(multiply_overflow "print 2147483647 * 2147483647;" 1 ""
    "Error: at 1:18: Integer overflow\n")
check_case(multiply_underflow "print -2147483648 * 2;" 1 ""
    "Error: at 1:19: Integer overflow\n")
check_case(divide_overflow "print -2147483648 / -1;" 1 ""
    "Error: at 1:19: Integer overflow\n")
check_case(divide_zero "print -2147483648 / 0;" 1 ""
    "Error: at 1:19: Division by zero\n")
check_case(negate_min "print --2147483648;" 1 ""
    "Error: at 1:7: Integer overflow in unary minus\n")
check_case(positive_literal "print 42; print 2147483648;" 1 ""
    "Error: at 1:17: Integer literal out of range: 2147483648\n")
check_case(negative_literal "print -2147483649;" 1 ""
    "Error: at 1:8: Integer literal out of range: 2147483649\n")
check_case(parenthesized_magnitude "print -(2147483648);" 1 ""
    "Error: at 1:9: Integer literal out of range: 2147483648\n")
check_case(runtime_output "print 42;\nprint 2147483647 + 1;" 1 "42\n"
    "Error: at 2:18: Integer overflow\n")
