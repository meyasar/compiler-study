; ModuleID = 'llvm-ir/examples/cfg_anomalies.ll'
source_filename = "llvm-ir/examples/cfg_anomalies.ll"

define i32 @choose_value(i1 %condition, i32 %value) {
entry:
  br i1 %condition, label %then, label %else

then:                                             ; preds = %entry
  %increased = add i32 %value, 1
  br label %merge

else:                                             ; preds = %entry
  %decreased = sub i32 %value, 1
  br label %merge

merge:                                            ; preds = %else, %then
  %result = phi i32 [ %increased, %then ], [ %decreased, %else ]
  ret i32 %result
}
