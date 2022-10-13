 ; ModuleID = 'test.c'
source_filename = "test.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@MIN=constant i32 10, align 4
@.str=private unnamed_addr constant [14 x i8] c"MAX+MIN = %d\0A\00", align 1
@.str.1=private unnamed_addr constant [16 x i8] c"SetBit(6) = %d\0A\00", align 1
@.str.2=private unnamed_addr constant [3 x i8] c"%d\00", align 1
@N = internal global i32 10, align 4


define dso_local i32 @roll(i32 %0) #0 {
  %2 = alloca i32, align 4 ;n
  %3 = alloca i32, align 4 ;i
  %4 = alloca i32, align 4 ;f
  store i32 %0, i32* %2, align 4
  store i32 2, i32* %3, align 4
  store i32 1, i32* %4, align 4
  %5 = load i32, i32* @N, align 4
  %6 = icmp sle i32 %0, %5
  br i1 %6, label %7, label %17

7: 
  %8 = load i32, i32* %3, align 4
  %9 = load i32, i32* %2, align 4
  %10 = icmp sle i32 %8, %9
  br i1 %10, label %11, label %17

11: 
  %12 = load i32, i32* %4, align 4
  %13 = load i32, i32* %3, align 4
  %14 = mul nsw i32 %12, %13
  store i32 %14, i32* %4, align 4
  %15 = load i32, i32* %3, align 4
  %16 = add nsw i32 %15, 1
  store i32 %16, i32* %3, align 4
  br label %7

17: 
  %18 = load i32, i32* %4, align 4
  ret i32 %18
}

define i32 @main() #0 {
  %1 = alloca i32, align 4  ;0
  %2 = alloca i32, align 4  ;n
  %3 = alloca i32, align 4  ;m
  store i32 0, i32* %1, align 4
  %4 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([14 x i8], [14 x i8]* @.str, i64 0, i64 0), i32 noundef 30)
  %5 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([16 x i8], [16 x i8]* @.str.1, i64 0, i64 0), i32 noundef 6)
  %6 = call i32 (i8*, ...) @__isoc99_scanf(i8* noundef getelementptr inbounds  ([3 x i8], [3 x i8]* @.str.2, i64 0, i64 0), i32* noundef %2)
  %7 = load i32, i32* %2, align 4
  %8 = call i32 @roll(i32 noundef %7)
  store i32 %8, i32* %3, align 4
  %9 = load i32, i32* %3, align 4
  %10 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([3 x i8], [3 x i8]* @.str.2, i64 0, i64 0), i32 noundef %9)
  ret i32 0
}

declare i32 @__isoc99_scanf(i8* noundef, ...) #1

declare i32 @printf(i8* noundef, ...) #1

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 1}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"Ubuntu clang version 14.0.0-1ubuntu1"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
