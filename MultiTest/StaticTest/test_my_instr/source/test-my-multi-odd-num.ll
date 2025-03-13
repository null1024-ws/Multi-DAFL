; ModuleID = 'test.c'
source_filename = "test.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@.str = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@__afl_area_ptr = external local_unnamed_addr global i8*
@__target_map_ptr = external local_unnamed_addr global i32**
@__afl_prev_loc = external thread_local local_unnamed_addr global i32
@str = private unnamed_addr constant [17 x i8] c"hello first else\00", align 1
@str.3 = private unnamed_addr constant [15 x i8] c"hello first if\00", align 1

; Function Attrs: nofree norecurse nounwind uwtable willreturn
define dso_local i32 @func1(i32 %0) local_unnamed_addr #0 !dbg !7 {
  %2 = load i32, i32* @__afl_prev_loc, align 4, !dbg !15, !nosanitize !2
  %3 = load i8*, i8** @__afl_area_ptr, align 8, !dbg !15, !nosanitize !2
  %4 = xor i32 %2, 9497, !dbg !15
  %5 = sext i32 %4 to i64, !dbg !15
  %6 = getelementptr i8, i8* %3, i64 %5, !dbg !15
  %7 = load i8, i8* %6, align 1, !dbg !15, !nosanitize !2
  %8 = add i8 %7, 1, !dbg !15
  store i8 %8, i8* %6, align 1, !dbg !15, !nosanitize !2
  store i32 4748, i32* @__afl_prev_loc, align 4, !dbg !15, !nosanitize !2
  call void @llvm.dbg.value(metadata i32 %0, metadata !12, metadata !DIExpression()), !dbg !15
  %9 = mul nsw i32 %0, %0, !dbg !16
  call void @llvm.dbg.value(metadata i32 %9, metadata !13, metadata !DIExpression()), !dbg !15
  call void @llvm.dbg.value(metadata i32 0, metadata !14, metadata !DIExpression()), !dbg !15
  %10 = and i32 %9, 1, !dbg !17
  %11 = icmp eq i32 %10, 0, !dbg !17
  %12 = load i8*, i8** @__afl_area_ptr, align 8, !dbg !15
  %13 = select i1 %11, i64 57571, i64 36346, !dbg !19
  %14 = select i1 %11, i32 31031, i32 20411, !dbg !19
  %15 = xor i1 %11, true, !dbg !19
  %16 = zext i1 %15 to i32, !dbg !19
  %17 = getelementptr i8, i8* %12, i64 %13, !dbg !15
  %18 = load i8, i8* %17, align 1, !dbg !15
  %19 = add i8 %18, 1, !dbg !15
  store i8 %19, i8* %17, align 1, !dbg !15
  store i32 %14, i32* @__afl_prev_loc, align 4, !dbg !15
  %20 = load i8*, i8** @__afl_area_ptr, align 8, !dbg !15, !nosanitize !2
  %21 = xor i32 %14, 57119, !dbg !15
  %22 = zext i32 %21 to i64, !dbg !15
  %23 = getelementptr i8, i8* %20, i64 %22, !dbg !15
  %24 = load i8, i8* %23, align 1, !dbg !15, !nosanitize !2
  %25 = add i8 %24, 1, !dbg !15
  store i8 %25, i8* %23, align 1, !dbg !15, !nosanitize !2
  store i32 28559, i32* @__afl_prev_loc, align 4, !dbg !15, !nosanitize !2
  call void @llvm.dbg.value(metadata i32 %16, metadata !14, metadata !DIExpression()), !dbg !15
  ret i32 %16, !dbg !20
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.lifetime.start.p0i8(i64 immarg, i8* nocapture) #1

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.lifetime.end.p0i8(i64 immarg, i8* nocapture) #1

; Function Attrs: nofree nounwind uwtable
define dso_local i32 @main() local_unnamed_addr #2 !dbg !21 {
  %1 = load i32, i32* @__afl_prev_loc, align 4, !nosanitize !2
  %2 = load i8*, i8** @__afl_area_ptr, align 8, !nosanitize !2
  %3 = xor i32 %1, 35549
  %4 = sext i32 %3 to i64
  %5 = getelementptr i8, i8* %2, i64 %4
  %6 = load i8, i8* %5, align 1, !nosanitize !2
  %7 = add i8 %6, 1
  store i8 %7, i8* %5, align 1, !nosanitize !2
  store i32 17774, i32* @__afl_prev_loc, align 4, !nosanitize !2
  %8 = alloca i32, align 4
  %9 = bitcast i32* %8 to i8*, !dbg !26
  call void @llvm.lifetime.start.p0i8(i64 4, i8* nonnull %9) #6, !dbg !26
  call void @llvm.dbg.value(metadata i32* %8, metadata !25, metadata !DIExpression(DW_OP_deref)), !dbg !27
  %10 = call i32 (i8*, ...) @__isoc99_scanf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str, i64 0, i64 0), i32* nonnull %8), !dbg !28
  %11 = load i32, i32* %8, align 4, !dbg !29, !tbaa !31
  call void @llvm.dbg.value(metadata i32 %11, metadata !25, metadata !DIExpression()), !dbg !27
  %12 = and i32 %11, 1, !dbg !35
  %13 = icmp eq i32 %12, 0, !dbg !35
  %14 = load i32, i32* @__afl_prev_loc, align 4, !dbg !36
  %15 = load i8*, i8** @__afl_area_ptr, align 8, !dbg !36
  br i1 %13, label %23, label %16, !dbg !37

16:                                               ; preds = %0
  %17 = xor i32 %14, 26414, !dbg !38
  %18 = sext i32 %17 to i64, !dbg !38
  %19 = getelementptr i8, i8* %15, i64 %18, !dbg !38
  %20 = load i8, i8* %19, align 1, !dbg !38, !nosanitize !2
  %21 = add i8 %20, 1, !dbg !38
  store i8 %21, i8* %19, align 1, !dbg !38, !nosanitize !2
  store i32 13207, i32* @__afl_prev_loc, align 4, !dbg !38, !nosanitize !2
  %22 = load i32**, i32*** @__target_map_ptr, align 8, !dbg !38, !nosanitize !2
  store i32 11451, i32** %22, align 4, !dbg !38, !nosanitize !2
  br label %29, !dbg !40

23:                                               ; preds = %0
  %24 = xor i32 %14, 36263, !dbg !41
  %25 = sext i32 %24 to i64, !dbg !41
  %26 = getelementptr i8, i8* %15, i64 %25, !dbg !41
  %27 = load i8, i8* %26, align 1, !dbg !41, !nosanitize !2
  %28 = add i8 %27, 1, !dbg !41
  store i8 %28, i8* %26, align 1, !dbg !41, !nosanitize !2
  store i32 18131, i32* @__afl_prev_loc, align 4, !dbg !41, !nosanitize !2
  br label %29

29:                                               ; preds = %23, %16
  %30 = phi i8* [ getelementptr inbounds ([17 x i8], [17 x i8]* @str, i64 0, i64 0), %23 ], [ getelementptr inbounds ([15 x i8], [15 x i8]* @str.3, i64 0, i64 0), %16 ]
  %31 = call i32 @puts(i8* nonnull dereferenceable(1) %30), !dbg !36
  %32 = load i32, i32* @__afl_prev_loc, align 4, !dbg !43, !nosanitize !2
  %33 = load i8*, i8** @__afl_area_ptr, align 8, !dbg !43, !nosanitize !2
  %34 = xor i32 %32, 44750, !dbg !43
  %35 = sext i32 %34 to i64, !dbg !43
  %36 = getelementptr i8, i8* %33, i64 %35, !dbg !43
  %37 = load i8, i8* %36, align 1, !dbg !43, !nosanitize !2
  %38 = add i8 %37, 1, !dbg !43
  store i8 %38, i8* %36, align 1, !dbg !43, !nosanitize !2
  store i32 22375, i32* @__afl_prev_loc, align 4, !dbg !43, !nosanitize !2
  %39 = load i32, i32* %8, align 4, !dbg !43, !tbaa !31
  call void @llvm.dbg.value(metadata i32 %39, metadata !25, metadata !DIExpression()), !dbg !27
  %40 = srem i32 %39, 3, !dbg !45
  %41 = icmp eq i32 %40, 0, !dbg !45
  br i1 %41, label %73, label %42, !dbg !46

42:                                               ; preds = %29
  %43 = load i8*, i8** @__afl_area_ptr, align 8, !dbg !47, !nosanitize !2
  %44 = getelementptr i8, i8* %43, i64 5174, !dbg !47
  %45 = load i8, i8* %44, align 1, !dbg !47, !nosanitize !2
  %46 = add i8 %45, 1, !dbg !47
  store i8 %46, i8* %44, align 1, !dbg !47, !nosanitize !2
  store i32 8616, i32* @__afl_prev_loc, align 4, !dbg !47, !nosanitize !2
  %47 = load i32**, i32*** @__target_map_ptr, align 8, !dbg !47, !nosanitize !2
  %48 = getelementptr i32*, i32** %47, i64 1, !dbg !47
  store i32 34567, i32** %48, align 4, !dbg !47, !nosanitize !2
  %49 = load i32, i32* @__afl_prev_loc, align 4, !dbg !49, !nosanitize !2
  %50 = load i8*, i8** @__afl_area_ptr, align 8, !dbg !49, !nosanitize !2
  %51 = xor i32 %49, 9497, !dbg !49
  %52 = sext i32 %51 to i64, !dbg !49
  %53 = getelementptr i8, i8* %50, i64 %52, !dbg !49
  %54 = load i8, i8* %53, align 1, !dbg !49, !nosanitize !2
  %55 = add i8 %54, 1, !dbg !49
  store i8 %55, i8* %53, align 1, !dbg !49, !nosanitize !2
  store i32 4748, i32* @__afl_prev_loc, align 4, !dbg !49, !nosanitize !2
  call void @llvm.dbg.value(metadata i32 %39, metadata !12, metadata !DIExpression()), !dbg !49
  %56 = mul nsw i32 %39, %39, !dbg !51
  call void @llvm.dbg.value(metadata i32 %56, metadata !13, metadata !DIExpression()), !dbg !49
  call void @llvm.dbg.value(metadata i32 0, metadata !14, metadata !DIExpression()), !dbg !49
  %57 = and i32 %56, 1, !dbg !52
  %58 = icmp eq i32 %57, 0, !dbg !52
  %59 = load i8*, i8** @__afl_area_ptr, align 8, !dbg !49
  %60 = getelementptr i8, i8* %59, i64 36346, !dbg !53
  %61 = getelementptr i8, i8* %59, i64 57571, !dbg !53
  %62 = select i1 %58, i8* %61, i8* %60, !dbg !53
  %63 = select i1 %58, i8* %61, i8* %60, !dbg !53
  %64 = select i1 %58, i32 31031, i32 20411, !dbg !53
  %65 = load i8, i8* %62, align 1, !dbg !49
  %66 = add i8 %65, 1, !dbg !49
  store i8 %66, i8* %63, align 1, !dbg !49
  store i32 %64, i32* @__afl_prev_loc, align 4, !dbg !49
  %67 = load i8*, i8** @__afl_area_ptr, align 8, !dbg !49, !nosanitize !2
  %68 = xor i32 %64, 57119, !dbg !49
  %69 = zext i32 %68 to i64, !dbg !49
  %70 = getelementptr i8, i8* %67, i64 %69, !dbg !49
  %71 = load i8, i8* %70, align 1, !dbg !49, !nosanitize !2
  %72 = add i8 %71, 1, !dbg !49
  store i8 %72, i8* %70, align 1, !dbg !49, !nosanitize !2
  store i32 28559, i32* @__afl_prev_loc, align 4, !dbg !49, !nosanitize !2
  call void @llvm.dbg.value(metadata i32 undef, metadata !14, metadata !DIExpression()), !dbg !49
  br label %73, !dbg !54

73:                                               ; preds = %42, %29
  %74 = phi i64 [ 4093, %42 ], [ 14101, %29 ]
  %75 = load i8*, i8** @__afl_area_ptr, align 8, !dbg !55, !nosanitize !2
  %76 = getelementptr i8, i8* %75, i64 %74, !dbg !55
  %77 = load i8, i8* %76, align 1, !dbg !55, !nosanitize !2
  %78 = add i8 %77, 1, !dbg !55
  store i8 %78, i8* %76, align 1, !dbg !55, !nosanitize !2
  store i32 12345, i32* @__afl_prev_loc, align 4, !dbg !55, !nosanitize !2
  call void @llvm.lifetime.end.p0i8(i64 4, i8* nonnull %9) #6, !dbg !55
  ret i32 0, !dbg !56
}

; Function Attrs: nofree nounwind
declare dso_local noundef i32 @__isoc99_scanf(i8* nocapture noundef readonly, ...) local_unnamed_addr #3

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare void @llvm.dbg.value(metadata, metadata, metadata) #4

; Function Attrs: nofree nounwind
declare noundef i32 @puts(i8* nocapture noundef readonly) local_unnamed_addr #5

attributes #0 = { nofree norecurse nounwind uwtable willreturn "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { argmemonly nofree nosync nounwind willreturn }
attributes #2 = { nofree nounwind uwtable "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nofree nounwind "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { nofree nosync nounwind readnone speculatable willreturn }
attributes #5 = { nofree nounwind }
attributes #6 = { nounwind }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!3, !4, !5}
!llvm.ident = !{!6}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "Ubuntu clang version 12.0.1-++20211029101322+fed41342a82f-1~exp1~20211029221816.4", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, enums: !2, splitDebugInlining: false, nameTableKind: None)
!1 = !DIFile(filename: "test.c", directory: "/home/moran/Workspace/Project/DirectedFuzz/afl/MultiTest/DAFL/MultiTest/StaticTest/test_my_instr/source")
!2 = !{}
!3 = !{i32 7, !"Dwarf Version", i32 4}
!4 = !{i32 2, !"Debug Info Version", i32 3}
!5 = !{i32 1, !"wchar_size", i32 4}
!6 = !{!"Ubuntu clang version 12.0.1-++20211029101322+fed41342a82f-1~exp1~20211029221816.4"}
!7 = distinct !DISubprogram(name: "func1", scope: !1, file: !1, line: 5, type: !8, scopeLine: 5, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !11)
!8 = !DISubroutineType(types: !9)
!9 = !{!10, !10}
!10 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!11 = !{!12, !13, !14}
!12 = !DILocalVariable(name: "x", arg: 1, scope: !7, file: !1, line: 5, type: !10)
!13 = !DILocalVariable(name: "pow", scope: !7, file: !1, line: 6, type: !10)
!14 = !DILocalVariable(name: "retcode", scope: !7, file: !1, line: 7, type: !10)
!15 = !DILocation(line: 0, scope: !7)
!16 = !DILocation(line: 6, column: 17, scope: !7)
!17 = !DILocation(line: 8, column: 13, scope: !18)
!18 = distinct !DILexicalBlock(scope: !7, file: !1, line: 8, column: 9)
!19 = !DILocation(line: 8, column: 9, scope: !7)
!20 = !DILocation(line: 13, column: 5, scope: !7)
!21 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 17, type: !22, scopeLine: 17, flags: DIFlagAllCallsDescribed, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !24)
!22 = !DISubroutineType(types: !23)
!23 = !{!10}
!24 = !{!25}
!25 = !DILocalVariable(name: "x", scope: !21, file: !1, line: 18, type: !10)
!26 = !DILocation(line: 18, column: 5, scope: !21)
!27 = !DILocation(line: 0, scope: !21)
!28 = !DILocation(line: 19, column: 5, scope: !21)
!29 = !DILocation(line: 20, column: 8, scope: !30)
!30 = distinct !DILexicalBlock(scope: !21, file: !1, line: 20, column: 8)
!31 = !{!32, !32, i64 0}
!32 = !{!"int", !33, i64 0}
!33 = !{!"omnipotent char", !34, i64 0}
!34 = !{!"Simple C/C++ TBAA"}
!35 = !DILocation(line: 20, column: 10, scope: !30)
!36 = !DILocation(line: 0, scope: !30)
!37 = !DILocation(line: 20, column: 8, scope: !21)
!38 = !DILocation(line: 21, column: 6, scope: !39)
!39 = distinct !DILexicalBlock(scope: !30, file: !1, line: 20, column: 14)
!40 = !DILocation(line: 22, column: 5, scope: !39)
!41 = !DILocation(line: 23, column: 2, scope: !42)
!42 = distinct !DILexicalBlock(scope: !30, file: !1, line: 22, column: 12)
!43 = !DILocation(line: 26, column: 9, scope: !44)
!44 = distinct !DILexicalBlock(scope: !21, file: !1, line: 26, column: 9)
!45 = !DILocation(line: 26, column: 11, scope: !44)
!46 = !DILocation(line: 26, column: 9, scope: !21)
!47 = !DILocation(line: 27, column: 6, scope: !48)
!48 = distinct !DILexicalBlock(scope: !44, file: !1, line: 26, column: 15)
!49 = !DILocation(line: 0, scope: !7, inlinedAt: !50)
!50 = distinct !DILocation(line: 27, column: 6, scope: !48)
!51 = !DILocation(line: 6, column: 17, scope: !7, inlinedAt: !50)
!52 = !DILocation(line: 8, column: 13, scope: !18, inlinedAt: !50)
!53 = !DILocation(line: 8, column: 9, scope: !7, inlinedAt: !50)
!54 = !DILocation(line: 28, column: 5, scope: !48)
!55 = !DILocation(line: 31, column: 1, scope: !21)
!56 = !DILocation(line: 30, column: 5, scope: !21)
