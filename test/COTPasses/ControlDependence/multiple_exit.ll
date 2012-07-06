; RUN: opt -load %projshlibdir/COTPasses.so \
; RUN:     -analyze -cdg                    \
; RUN:     -S -o - %s | FileCheck %s
; REQUIRES: loadable_module

target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @multiple_exit(i32 %a) nounwind uwtable {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  store i32 %a, i32* %2, align 4
  %3 = load i32* %2, align 4
  %4 = icmp ne i32 %3, 0
  br i1 %4, label %5, label %7

; <label>:5                                       ; preds = %0
  %6 = load i32* %2, align 4
  store i32 %6, i32* %1
  ret i32 %6

; <label>:7                                       ; preds = %0
  %8 = load i32* %2, align 4
  %9 = sub nsw i32 %8, 1
  store i32 %9, i32* %1
  ret i32 %9
}

;CHECK:      Printing analysis 'Control Dependency Graph Contruction' for function 'multiple_exit':
;CHECK-NEXT: =============================--------------------------------
;CHECK-NEXT: Control Dependency Graph: 
;CHECK-NEXT:    <<EntryNode>> { %0:0 }
;CHECK-NEXT:    %0 { %5:0 %7:0 }
;CHECK-NEXT:    %5 { }
;CHECK-NEXT:    %7 { }
