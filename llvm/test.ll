target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.14.0"

@.str = private unnamed_addr constant [18 x i8] c"%s : # args = %d\0A\00", align 1

define i32 @main(i32, i8**) nounwind {
  %a = alloca i32, align 4
  store i32 5, i32* %a, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 1, i32* %3, align 4
  store i32 2, i32* %4, align 4
  %5 = load i32, i32* %3, align 4
  %6 = load i32, i32* %4, align 4
  %7 = add nsw i32 %5, %6
  %8 = load i32, i32* %a, align 4
  %9 = add nsw i32 %7, %8

  %argcp = alloca i32, align 4
  %argvp = alloca i8**, align 8
  store i32 %0, i32* %argcp, align 4
  store i8** %1, i8*** %argvp, align 8
  %argc = load i32, i32 * %argcp, align 4
  %argv = load i8**, i8*** %argvp, align 8
  %progp = getelementptr inbounds i8*, i8** %argv, i64 0
  %prog = load i8*, i8** %progp, align 8

  %10 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @.str, i32 0, i32 0), i8* %prog, i32 %argc)

  ret i32 0
}

declare i32 @printf(i8*, ...)

