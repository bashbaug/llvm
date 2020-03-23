; NOTE: Assertions have been autogenerated by utils/update_test_checks.py
; RUN: opt -instsimplify -S < %s | FileCheck %s

define i1 @test(i8 %p, i8* %pq) {
; CHECK-LABEL: @test(
; CHECK-NEXT:    ret i1 true
;
  %q = load i8, i8* %pq, !range !0 ; %q is known nonzero; no known bits
  %1 = shl i8 %p, %q              ; because %q is nonzero, %1[0] is known to be zero.
  %2 = and i8 %1, 1
  %x = icmp eq i8 %2, 0
  ret i1 %x
}

!0 = !{ i8 1, i8 5 }
