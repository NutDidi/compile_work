; ModuleID = 'test1.c'
source_filename = "test1.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@.str = private unnamed_addr constant [13 x i8] c"hello,world\0A\00", align 1
@.str.1 = private unnamed_addr constant [36 x i8] c"MAX = %d , MIN = %d , MAX+MIN = %d\0A\00", align 1
@.str.2 = private unnamed_addr constant [33 x i8] c"SetBit(5) = %d , SetBit(6) = %d\0A\00", align 1
@.str.3 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@N = internal global i32 10, align 4

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 0, i32* %1, align 4
  %5 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([13 x i8], [13 x i8]* @.str, i64 0, i64 0))
  %6 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([36 x i8], [36 x i8]* @.str.1, i64 0, i64 0), i32 noundef 20, i32 noundef 10, i32 noundef 30)
  %7 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([33 x i8], [33 x i8]* @.str.2, i64 0, i64 0), i32 noundef 32, i32 noundef 64)
  %8 = call i32 (i8*, ...) @__isoc99_scanf(i8* noundef getelementptr inbounds ([3 x i8], [3 x i8]* @.str.3, i64 0, i64 0), i32* noundef %3)
  store i32 2, i32* %2, align 4
  store i32 1, i32* %4, align 4
  %9 = load i32, i32* %3, align 4
  %10 = load i32, i32* @N, align 4
  %11 = icmp sle i32 %9, %10
  br i1 %11, label %12, label %24

12:                                               ; preds = %0
  br label %13

13:                                               ; preds = %17, %12
  %14 = load i32, i32* %2, align 4
  %15 = load i32, i32* %3, align 4
  %16 = icmp sle i32 %14, %15
  br i1 %16, label %17, label %23

17:                                               ; preds = %13
  %18 = load i32, i32* %4, align 4
  %19 = load i32, i32* %2, align 4
  %20 = mul nsw i32 %18, %19
  store i32 %20, i32* %4, align 4
  %21 = load i32, i32* %2, align 4
  %22 = add nsw i32 %21, 1
  store i32 %22, i32* %2, align 4
  br label %13, !llvm.loop !6

23:                                               ; preds = %13
  br label %24

24:                                               ; preds = %23, %0
  %25 = load i32, i32* %4, align 4
  %26 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([3 x i8], [3 x i8]* @.str.3, i64 0, i64 0), i32 noundef %25)
  ret i32 0
}

declare i32 @printf(i8* noundef, ...) #1

declare i32 @__isoc99_scanf(i8* noundef, ...) #1

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
