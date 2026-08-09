define i32 @choose_value(i1 %condition, i32 %value) {
entry:
    br i1 %condition, label %then, label %else

then:
    %increased = add i32 %value, 1
    br label %merge

else:
    %decreased = sub i32 %value, 1
    br label %merge

merge:
    %result = phi i32 [ %increased, %then ], [ %decreased, %else ]
    ret i32 %result

unreachable_block:
    %dead = mul i32 %value, 2
    ret i32 %dead
}
