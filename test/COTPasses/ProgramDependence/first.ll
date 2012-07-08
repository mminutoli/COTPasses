; RUN: opt -load %projshlibdir/COTPasses.so \
; RUN:     -analyze -pdg                    \
; RUN:     -S -o - %s | FileCheck %s
; REQUIRES: loadable_module

target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @first() nounwind uwtable {
  %A = alloca [10 x i32], align 16
  %i = alloca i32, align 4
  br label %1

; <label>:1                                       ; preds = %9, %0
  %2 = load i32* %i, align 4
  %3 = icmp slt i32 %2, 10
  br i1 %3, label %4, label %12

; <label>:4                                       ; preds = %1
  %5 = load i32* %i, align 4
  %6 = load i32* %i, align 4
  %7 = sext i32 %6 to i64
  %8 = getelementptr inbounds [10 x i32]* %A, i32 0, i64 %7
  store i32 %5, i32* %8, align 4
  br label %9

; <label>:9                                       ; preds = %4
  %10 = load i32* %i, align 4
  %11 = add nsw i32 %10, 1
  store i32 %11, i32* %i, align 4
  br label %1

; <label>:12                                      ; preds = %1
  ret i32 0
}

;CHECK:      Printing analysis 'Program Dependency Graph Construction' for function 'first':
;CHECK-NEXT: =============================--------------------------------
;CHECK-NEXT: Program Dependency Graph: 
;CHECK-NEXT:     %0 { %1:1 %4:1 %9:1 }
;CHECK-NEXT:     %1 { %4:0 %9:0 }
;CHECK-NEXT:     %4 { }
;CHECK-NEXT:     %9 { }