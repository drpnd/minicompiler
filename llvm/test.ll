target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.14.0"

@.str = private unnamed_addr constant [19 x i8] c"%ld : # args = %d\0A\00", align 1
@.str.usage = private unnamed_addr constant [17 x i8] c"Usage: %s <int>\0A\00", align 1

define i32 @main(i32, i8**) nounwind {
  %argcp = alloca i32, align 4
  %argvp = alloca i8**, align 8
  store i32 %0, i32* %argcp, align 4
  store i8** %1, i8*** %argvp, align 8
  %argc = load i32, i32* %argcp, align 4
  %argv = load i8**, i8*** %argvp, align 8

  %cmp = icmp sle i32 %argc, 1
  br i1 %cmp, label %if.then, label %if.else

if.then:
  %progp = getelementptr inbounds i8*, i8** %argv, i64 0
  %prog = load i8*, i8** %progp, align 8
  call void @usage(i8* %prog)
  ret i32 -1
if.else:
  br label %if.end
if.end:

  %valstrp = getelementptr inbounds i8*, i8** %argv, i64 1
  %valstr = load i8*, i8** %valstrp, align 8
  %rest = alloca i8*, align 8
  %val = call i64 @strtol(i8* %valstr, i8** %rest, i32 10)

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

  %argc2 = load i32, i32 * %argcp, align 4

  %10 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @.str, i32 0, i32 0), i64 %val, i32 %argc2)

  ret i32 0
}

define void @usage(i8*) nounwind {
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @.str.usage, i32 0, i32 0), i8* %0)
  ret void
}

declare i32 @printf(i8*, ...) #1
declare i64 @strtol(i8*, i8**, i32) #1

attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sahf,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
