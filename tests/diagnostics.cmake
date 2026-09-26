# Check both the exit status and exact diagnostic, not just any failure.
function(check_error name source expected)
    set(input "${TEST_DIR}/${name}.toy")
    file(WRITE "${input}" "${source}")
    execute_process(
        COMMAND "${INTERPRETER}" "${input}"
        RESULT_VARIABLE status
        OUTPUT_VARIABLE output
        ERROR_VARIABLE error
    )
    string(REPLACE "\r\n" "\n" error "${error}")
    if(NOT "${status}" STREQUAL "1")
        message(FATAL_ERROR "${name}: expected exit 1, got ${status}")
    endif()
    if(NOT "${output}" STREQUAL "")
        message(FATAL_ERROR "${name}: unexpected stdout: ${output}")
    endif()
    if(NOT "${error}" STREQUAL "Error: ${expected}\n")
        message(FATAL_ERROR "${name}: unexpected diagnostic: ${error}")
    endif()
endfunction()

file(MAKE_DIRECTORY "${TEST_DIR}")
check_error(duplicate "let x = 10;\nlet x = 20;\nprint x;\n"
    "at 2:5: Variable x already exists")
check_error(duplicate_before_initializer "let x = 10;\nlet x = 1 / 0;\n"
    "at 2:5: Variable x already exists")
check_error(undefined "\n  print missing;\n"
    "at 2:9: Undefined variable: missing")
check_error(division "let x = 1;\nprint x / 0;\n"
    "at 2:9: Division by zero")
check_error(expression "let x = 1;\nprint 1 + ;\n"
    "at 2:11: Expected expression. Found: ;")
check_error(eof "print 1\n  "
    "at 2:3: Expected ';' after print expression. Found: <end of file>")
check_error(unknown "\r\n\t@"
    "at 2:2: Expected statement. Found: @")
check_error(parenthesis "print (1 + 2;"
    "at 1:13: Expected ')' after expression. Found: ;")
check_error(large_integer "print 999999999999999999999999;"
    "at 1:7: Integer literal out of range: 999999999999999999999999")
check_error(analyze_before_execution "print 42;\nprint missing;\n"
        "at 2:7: Undefined variable: missing")
check_error(self_reference "let x = x + 1;\n"
        "at 1:9: Undefined variable: x")
