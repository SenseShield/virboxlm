# Virbox SDK Samples -- C

-------------------------
本目录下列举了 Virbox LM 许可管理的常见场景程序示例，包含精锐5加密锁，云锁，软锁等。**示例中的代码主要为了展示 Virbox LM API 的使用方式，仅供参考，在实际产品中使用请自行调整。**

## sample_01 	
最简单的许可使用Demo。演示许可访问API的基本调用方法和注意事项，提供 VS2015 及以上版本编译失败（`__iob_func`）处理方案。

## sample_02 	
runtime 基本 API 使用方法。演示许可访问 API 的常用功能，注册回调函数获取加密锁拔插消息。

## sample_03 
runtime 数据区操作。演示数据区读写所有用法和注意事项。

## sample_04 	
runtime 设备操作。枚举当前计算机插入硬件锁、获取锁内许可ID 和查看许可内容。

## sample_05 	
runtime 数据区绑定 PC 信息。演示如何使用 API 实现将加密锁与某一台计算机绑定。

## sample_06 	
control 设备操作(枚举设备、获取设备信息、获取许可信息)。演示获取不区分开发商的所有加密锁信息和网络锁许可会话的方法。

## sample_07   
证书签发许可。演示使用加密锁证书签发许可（只能升级给指定加密锁），包括加密锁证书获取、合成证书链(p7b)以及使用证书链签发的方法。

## sample_08  	
加密锁数据升级。演示通过 API 将加密锁数据包升级到锁内的方法。

## sample_09  	
许可管理。演示签发不同类型的许可管理升级包修改加密锁许可状态，实现删除许可(单条/所有) 锁定/解锁许可操作。

## sample_10  	
种子码签发文件。演示使用 API 签发可升级到加密锁内文件升级包，种子码签发的升级包可以对任意加密锁升级。

## sample_11  	
恢复出厂设置。演示使用 API 签发清空加密锁数据升级包的方法。

## sample_12  	
许可增量更新。演示更新许可部分内容的方法，例如延长许可使用截止时间、取消时间跨度限制、修改数据区内容和大小。

## sample_13  	
`keep_alive` 单独演示demo。演示使用云锁时调用 `slm_keep_alive` 返回状态码的处理方法。

## sample_14  	
丢锁补锁 & 丢锁补锁与普通锁的转换。演示将硬件加密锁设置为定期激活锁和恢复为普通锁的方法。

## 特别说明
runtime 示例程序中依赖 `slm_runtime.dll（或.lib）`，开发阶段使用 `slm_runtime_dev.dll`（将文件名拷贝至程序目录下并改名为 `slm_runtime.dl`l ），调试代码时不会触发反调试而造成程序闪退；当软件正式发版时必须使用 `slm_runtime.dll`（非 `slm_runtime_dev.dll`），若发版使用可调试版本（`slm_runtime_dev.dll`）将导致正式发版能够被调试器附加调试可调试，极大程度降低发售软件的安全性。


**备注：** 反调试，全称反调试保护，所有调用 runtime 库的应用程序在执行许可登录（`slm_login`）后会受到 Virbox许可服务（的反调试安全保护，当黑客使用调试器分析被保护应用程序逻辑时，深思许可服务会检测到并在10秒后终止被调试进程，提高安全性。
