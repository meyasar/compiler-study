file(MAKE_DIRECTORY "${TEST_DIR}")

function(check_program name source expected)
    set(input "${TEST_DIR}/${name}.toy")
    set(ir "${TEST_DIR}/${name}.ll")
    file(WRITE "${input}" "${source}")
    execute_process(COMMAND "${INTERPRETER}" --emit-ir "${input}"
        RESULT_VARIABLE status OUTPUT_VARIABLE output ERROR_VARIABLE error)
    if(NOT "${status}" STREQUAL "0" OR NOT "${error}" STREQUAL "")
        message(FATAL_ERROR "${name}: emission failed: ${status}: ${error}")
    endif()
    file(WRITE "${ir}" "${output}")
    execute_process(COMMAND "${OPT}" -passes=verify -disable-output "${ir}"
        RESULT_VARIABLE status ERROR_VARIABLE error)
    if(NOT "${status}" STREQUAL "0")
        message(FATAL_ERROR "${name}: invalid IR: ${error}")
    endif()
    execute_process(COMMAND "${LLI}" "${ir}"
        RESULT_VARIABLE status OUTPUT_VARIABLE actual ERROR_VARIABLE error)
    if(NOT "${status}" STREQUAL "0" OR NOT "${actual}" STREQUAL "${expected}"
        OR NOT "${error}" STREQUAL "")
        message(FATAL_ERROR "${name}: execution failed: ${status}: [${actual}] [${error}]")
    endif()
    execute_process(COMMAND "${INTERPRETER}" "${input}"
        RESULT_VARIABLE status OUTPUT_VARIABLE interpreted ERROR_VARIABLE error)
    if(NOT "${status}" STREQUAL "0" OR NOT "${interpreted}" STREQUAL "${actual}"
        OR NOT "${error}" STREQUAL "")
        message(FATAL_ERROR "${name}: interpreter and LLVM disagree")
    endif()
endfunction()

function(check_rejection name source expected)
    set(input "${TEST_DIR}/${name}.toy")
    file(WRITE "${input}" "${source}")
    execute_process(COMMAND "${INTERPRETER}" --emit-ir "${input}"
        RESULT_VARIABLE status OUTPUT_VARIABLE output ERROR_VARIABLE error)
    if(NOT "${status}" STREQUAL "1" OR NOT "${output}" STREQUAL ""
        OR NOT "${error}" STREQUAL "Error: ${expected}\n")
        message(FATAL_ERROR "${name}: incorrect rejection: ${status}: [${output}] [${error}]")
    endif()
endfunction()

check_program(number "print 42;" "42\n")
check_program(multiple "print 0; print (2147483647); print -2147483648;" "0\n2147483647\n-2147483648\n")
check_program(empty "" "")
check_rejection(binary "print 42; print 1 + 2;" "LLVM IR generation does not yet support this expression")
check_rejection(unary "print -5;" "LLVM IR generation does not yet support this expression")
check_rejection(declaration "let x = 1; print x;" "LLVM IR generation does not yet support this statement")
check_rejection(semantic "print 42; print missing;" "at 1:17: Undefined variable: missing")
