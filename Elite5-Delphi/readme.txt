
本例演示使用内存加载的方式在 Delphi 中使用 slm_runtime_easy 接口，实现许可相关功能接口及硬件锁闪灯控制等功能。

文件说明：
BTMemory_x86.pas BTMemory，支持Delphi XE及之前版本（至Delphi7可用）。
BTMemory_x86_64.pas BTMemory，支持Delphi XE2及之后版本。
slm_runtime_easy.pas runtime_easy API接口定义头文件，文件中引用 slm_runtime_easy.inc 文件。
slm_runtime_easy.inc 记录 slm_runtime_easy 动态库文件转换成二进制的数组数据，编译 slm_runtime_easy.pas 时必备的数据文件。
slm_runtime_easy_dll2inc.exe slm_runtime_easy 动态库文件转换成 inc 数据文件工具，工具使用方法详情见下文"DLL转INC工具使用说明"。

使用说明：
1.新建工程，拷贝 slm_runtime_easy.pas BTMemory_x86.pas BTMemory_x86_64.pas 至项目目录，引用 slm_runtime_easy。
2.根据构建程序平台（32-bit 或 64-bit）分别从 {SDK安装目录}\SDK\C\dll\x86 (或x64) 目录拷贝 slm_runtime_easy_dev.dll 和 slm_runtime_easy.dll 到当前项目目录，使用 slm_runtime_easy_dll2inc.exe 工具将 DLL 转换为 inc 文件。

注意事项：
1.切换构建选项Debug、Release、32-bit、64-bit时，需要手动删除项目生成目录下的*.dcu文件，否则会因为程序默认引用已生成的dcu导致执行结果与期望不一致的情况。若未作上述才操作最严重会导致正式发版安装包为可调试版，降低程序安全性。
2.软件正式发版时 slm_runtime_easy.inc 必须是使用 slm_runtime_easy.dll 转换生成后的数据文件，若inc数据是由 slm_runtime_easy_dev.dll 文件生成，则不具备反调试功能，同注意事项1。

常见问题：
1.切换构建程序平台（32-bit -> 64-bit）时提示以下错误：
[DCC Fatal Error] F2048 Bad unit format: 'slm_runtime_easy.dcu' - Expected version: 23.0, Windows Unicode(x64) Found version: 23.0, Windows Unicode(x86)
解决方案：删除工程目录下已经生成的.dcu文件，重新编译即可。
2.运行程序时崩溃，程序弹窗提示访问异常地址，错误码 $C0000005。拷贝动态库与当前构建程序平台不一致（32-bit与64-bit），按照"使用说明"-"步骤2"和"注意事项"进行操作，重新构建程序即可。
3.安装SDK后，使用 slm_runtime_easy_dll2inc.exe 生成 inc 文件没有反应，没有文件产生。SDK默认安装至C盘，一般情况下C盘必须是管理员权限才能正常运行，非管理员权限运行时会因为权限不足出现文件写入失败的情况。检查传参DLL文件路径是否需要管理员权限，若需要需要管理员权限，请将 dll 文件拷贝至桌面等不需要管理员权限位置进行转换，将生成后的 inc 文件拷贝至示例程序目录；或使用管理员权限启动命令提示符，重新执行转换命令。

DLL转INC工具使用说明：
slm_runtime_easy_dll2inc.exe 参数说明：
  参数1（必填）：需要转换的DLL文件路径
  参数2（可选）：指定生成INC文件路径
  示例1：使用默认输出文件路径(${filename}.inc) slm_runtime_easy_dll2inc.exe slm_runtime_easy_dev.dll，其中 ${filename} 表示输入文件路径。
  示例2：指定输出文件路径 slm_runtime_easy_dll2inc.exe slm_runtime_easy_dev.dll slm_runtime_easy.inc
  注意：编译 slm_runtime_easy.pas 所需要的 inc 文件名必须是 slm_runtime_easy.inc，否则会编译失败
使用工具的两种方式：
1.拖拽文件到 slm_runtime_easy_dll2inc.exe 程序图标上执行，会使用默认输出文件路径转换数据文件。
2.使用命令提示符工具，执行CMD命令。