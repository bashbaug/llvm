; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py
; RUN: llc -mcpu=pwr8 -ppc-asm-full-reg-names \
; RUN:   -mtriple=powerpc64le-unknown-unknown < %s | FileCheck %s
define dso_local void @test(void (i32)* nocapture %fp, i32 signext %Arg, i32 signext %Len) local_unnamed_addr #0 {
; CHECK-LABEL: test:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    mflr r0
; CHECK-NEXT:    .cfi_def_cfa_offset 64
; CHECK-NEXT:    .cfi_offset lr, 16
; CHECK-NEXT:    .cfi_offset r28, -32
; CHECK-NEXT:    .cfi_offset r29, -24
; CHECK-NEXT:    .cfi_offset r30, -16
; CHECK-NEXT:    std r28, -32(r1) # 8-byte Folded Spill
; CHECK-NEXT:    std r29, -24(r1) # 8-byte Folded Spill
; CHECK-NEXT:    std r30, -16(r1) # 8-byte Folded Spill
; CHECK-NEXT:    std r0, 16(r1)
; CHECK-NEXT:    stdu r1, -64(r1)
; CHECK-NEXT:    mr r29, r5
; CHECK-NEXT:    cmpwi cr1, r4, 11
; CHECK-NEXT:    mr r30, r3
; CHECK-NEXT:    extsw r28, r4
; CHECK-NEXT:    cmpwi r29, 1
; CHECK-NEXT:    cror 4*cr5+lt, lt, 4*cr1+lt
; CHECK-NEXT:    bc 12, 4*cr5+lt, .LBB0_3
; CHECK-NEXT:  # %bb.1: # %for.body.us.preheader
; CHECK-NEXT:    std r2, 24(r1)
; CHECK-NEXT:    .p2align 5
; CHECK-NEXT:  .LBB0_2: # %for.body.us
; CHECK-NEXT:  # =>This Inner Loop Header: Depth=1
; CHECK-NEXT:    mtctr r30
; CHECK-NEXT:    mr r3, r28
; CHECK-NEXT:    mr r12, r30
; CHECK-NEXT:    bctrl
; CHECK-NEXT:    ld 2, 24(r1)
; CHECK-NEXT:    addi r29, r29, -1
; CHECK-NEXT:    cmplwi r29, 0
; CHECK-NEXT:    bne cr0, .LBB0_2
; CHECK-NEXT:  .LBB0_3: # %for.cond.cleanup
; CHECK-NEXT:    mtctr r30
; CHECK-NEXT:    mr r3, r28
; CHECK-NEXT:    mr r12, r30
; CHECK-NEXT:    std r2, 24(r1)
; CHECK-NEXT:    bctrl
; CHECK-NEXT:    ld 2, 24(r1)
; CHECK-NEXT:    addi r1, r1, 64
; CHECK-NEXT:    ld r0, 16(r1)
; CHECK-NEXT:    mtlr r0
; CHECK-NEXT:    ld r30, -16(r1) # 8-byte Folded Reload
; CHECK-NEXT:    ld r29, -24(r1) # 8-byte Folded Reload
; CHECK-NEXT:    ld r28, -32(r1) # 8-byte Folded Reload
; CHECK-NEXT:    blr
entry:
  %cmp7 = icmp sgt i32 %Len, 0
  %cmp1 = icmp sgt i32 %Arg, 10
  %or.cond = and i1 %cmp7, %cmp1
  br i1 %or.cond, label %for.body.us, label %for.cond.cleanup

for.body.us:                                      ; preds = %entry, %for.body.us
  %i.08.us = phi i32 [ %inc.us, %for.body.us ], [ 0, %entry ]
  tail call void %fp(i32 signext %Arg) #1
  %inc.us = add nuw nsw i32 %i.08.us, 1
  %exitcond = icmp eq i32 %inc.us, %Len
  br i1 %exitcond, label %for.cond.cleanup, label %for.body.us

for.cond.cleanup:                                 ; preds = %for.body.us, %entry
  tail call void %fp(i32 signext %Arg) #1
  ret void
}
