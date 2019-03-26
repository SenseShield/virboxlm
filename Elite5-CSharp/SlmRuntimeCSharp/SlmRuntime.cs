using System;
using System.Runtime.InteropServices;

using SLM_HANDLE_INDEX = System.UInt32;

namespace SLM
{
    /// <summary>
    /// 
    /// </summary>
    internal class SlmRuntime
    {
        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        [DllImport("slm_runtime_easy.dll", EntryPoint = "#1", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 SlmGetLastError();

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        [DllImport("slm_runtime_easy.dll", EntryPoint = "#2", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 AppMsgGetLastStatus();

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        [DllImport("slm_runtime_easy.dll", EntryPoint = "#3", CallingConvention = CallingConvention.StdCall)]
        public static extern IntPtr AppMsgGetLastInfo();
        
        /// <summary>
        /// 
        /// </summary>
        /// <param name="DevPsd">开发商api密码</param>
        /// <returns></returns>
        [DllImport("slm_runtime_easy.dll", EntryPoint = "#4", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 SlmInitEasy(
            [In, MarshalAs(UnmanagedType.LPStr)] string DevPsd);

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        [DllImport("slm_runtime_easy.dll", EntryPoint = "#5", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 SlmCleanupEasy();

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        [DllImport("slm_runtime_easy.dll", EntryPoint = "#6", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 SlmIsDebugEasy();

        /// <summary>
        /// 
        /// </summary>
        /// <param name="LicenseParam"></param>
        /// <param name="ParamFormat"></param>
        /// <returns>SLM_HANDLE_INDEX</returns>
        [DllImport("slm_runtime_easy.dll", EntryPoint = "#7", CallingConvention = CallingConvention.StdCall)]
        public static extern SLM_HANDLE_INDEX SlmLoginEasy(
            ref ST_LOGIN_PARAM            LicenseParam,
            INFO_FORMAT_TYPE              ParamFormat);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="SlmHandle"></param>
        /// <returns></returns>
        [DllImport("slm_runtime_easy.dll", EntryPoint = "#8", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 SlmLogoutEasy(
            SLM_HANDLE_INDEX SlmHandle);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="LicenseID"></param>
        /// <param name="Format"></param>
        /// <returns>"IntPtr" 需要调用SlmFreeEasy 释放</returns>
        [DllImport("slm_runtime_easy.dll", EntryPoint = "#9", CallingConvention = CallingConvention.StdCall)]
        public static extern IntPtr SlmFindLicenseEasy(
            UInt32            LicenseID,
            INFO_FORMAT_TYPE  Format);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="SlmHandle"></param>
        /// <returns></returns>
        [DllImport("slm_runtime_easy.dll", EntryPoint = "#28", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 SlmKeepAliveEasy(
            SLM_HANDLE_INDEX SlmHandle);
        
        /// <summary>
        /// 
        /// </summary>
        /// <param name="SlmHandle">登录许可句柄</param>
        /// <param name="InfoType">信息类型</param>
        /// <param name="Format">信息格式仅支持JSON</param>
        /// <returns>IntPtr 需要调用SlmFreeEasy 释放</returns>
        [DllImport("slm_runtime_easy.dll", EntryPoint = "#10", CallingConvention = CallingConvention.StdCall)]
        public static extern IntPtr SlmGetInfoEasy(
            SLM_HANDLE_INDEX SlmHandle,
            INFO_TYPE        InfoType,
            INFO_FORMAT_TYPE Format);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="SlmHandle"></param>
        /// <param name="ModuleID"></param>
        /// <returns></returns>
        [DllImport("slm_runtime_easy.dll", EntryPoint = "#32", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 SlmCheckModuleEasy(
            SLM_HANDLE_INDEX   SlmHandle,
            UInt32             ModuleID);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="SlmHandle"></param>
        /// <param name="InputBuff"></param>
        /// <param name="OutBuff"></param>
        /// <param name="Len"></param>
        /// <returns></returns>
        [DllImport("slm_runtime_easy.dll", EntryPoint = "#11", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 SlmEncryptEasy(
            SLM_HANDLE_INDEX                              SlmHandle,
            [In, MarshalAs(UnmanagedType.LPArray)] byte[] InputBuff,
            [In, Out, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 3)] byte[] OutBuff,
            UInt32                                        Len);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="SlmHandle"></param>
        /// <param name="InputBuff"></param>
        /// <param name="OutBuff"></param>
        /// <param name="Len"></param>
        /// <returns></returns>
        [DllImport("slm_runtime_easy.dll", EntryPoint = "#12", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 SlmDecryptEasy(
            SLM_HANDLE_INDEX                              SlmHandle,
            [In, MarshalAs(UnmanagedType.LPArray)] byte[] InputBuff,
            [In, Out, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 3)] byte[] OutBuff,
            UInt32                                        Len);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="SlmHandle"></param>
        /// <param name="Type"></param>
        /// <param name="pMemSize"></param>
        /// <returns></returns>
        [DllImport("slm_runtime_easy.dll", EntryPoint = "#13", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 SlmUserDataGetSizeEasy(
            SLM_HANDLE_INDEX   SlmHandle,
            LIC_USER_DATA_TYPE Type);

        /// <summary>
        /// 读许可数据，可以读取RW和ROM
        /// </summary>
        /// <param name="SlmHandle"></param>
        /// <param name="Type"></param>
        /// <param name="ReadBuff"></param>
        /// <param name="Offset"></param>
        /// <param name="Len"></param>
        /// <returns></returns>
        [DllImport("slm_runtime_easy.dll", EntryPoint = "#14", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 SlmUserDataReadEasy(
            SLM_HANDLE_INDEX       SlmHandle,
            LIC_USER_DATA_TYPE     Type,
            [Out, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 4)] byte[] ReadBuff,
            UInt32                 Offset,
            UInt32                 Len);

        /// <summary>
        /// 写许可的读写数据区 ,数据区操作之前请先确认内存区的大小，可以使用slm_user_data_getsize获得
        /// </summary>
        /// <param name="SlmHandle"></param>
        /// <param name="WriteBuff"></param>
        /// <param name="Offset"></param>
        /// <param name="Len"></param>
        /// <returns></returns>
        [DllImport("slm_runtime_easy.dll", EntryPoint = "#15", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 SlmUserDataWriteEasy(
            SLM_HANDLE_INDEX      SlmHandle,
            [In, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 3)] byte[] WriteBuff,
            UInt32                Offset,
            UInt32                Len);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="SlmHandle"></param>
        /// <param name="LicenseID"></param>
        /// <returns>返回数据区大小</returns>
        [DllImport("slm_runtime_easy.dll", EntryPoint = "#36", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 SlmPubDataGetSizeEasy(
            SLM_HANDLE_INDEX       SlmHandle,
            UInt32                 LicenseID);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="SlmHandle"></param>
        /// <param name="LicenseID"></param>
        /// <param name="ReadBuff"></param>
        /// <param name="Offset"></param>
        /// <param name="Len"></param>
        /// <returns></returns>
        [DllImport("slm_runtime_easy.dll", EntryPoint = "#37", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 SlmPubDataReadEasy(
            SLM_HANDLE_INDEX SlmHandle,
            UInt32           LicenseID,
            [Out, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 4)] byte[] ReadBuff,
            UInt32                 Offset,
            UInt32                 Len);

        /// <summary>
        /// SS内存托管内存申请
        /// </summary>
        /// <param name="SlmHandle"></param>
        /// <param name="Size"></param>
        /// <returns>UInt32 返回MemID</returns>
        [DllImport("slm_runtime_easy.dll", EntryPoint = "#16", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 SlmMemAllocEasy(
            SLM_HANDLE_INDEX SlmHandle,
            UInt32           Size);

        /// <summary>
        /// 释放托管内存
        /// </summary>
        /// <param name="SlmHandle"></param>
        /// <param name="MemID"></param>
        /// <returns></returns>
        [DllImport("slm_runtime_easy.dll", EntryPoint = "#17", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 SlmMemFreeEasy(
            SLM_HANDLE_INDEX   SlmHandle,
            UInt32             MemID);

        /// <summary>
        /// SS内存托管读
        /// </summary>
        /// <param name="SlmHandle"></param>
        /// <param name="MemID"></param>
        /// <param name="Offset"></param>
        /// <param name="Len"></param>
        /// <param name="readbuff"></param>
        /// <param name="ReadLen"></param>
        /// <returns></returns>
        [DllImport("slm_runtime_easy.dll", EntryPoint = "#18", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 SlmMemReadEasy(
            SLM_HANDLE_INDEX          SlmHandle,
            UInt32                    MemID,
            UInt32                    Offset,
            UInt32                    Len,
            [Out, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 3)] byte[] readbuff);

        /// <summary>
        /// SS内存托管内存写入
        /// </summary>
        /// <param name="SlmHandle"></param>
        /// <param name="MemID"></param>
        /// <param name="Offset"></param>
        /// <param name="Len"></param>
        /// <param name="writebuff"></param>
        /// <returns>UInt32 返回写入数据长度</returns>
        [DllImport("slm_runtime_easy.dll", EntryPoint = "#19", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 SlmMemWriteEasy(
            SLM_HANDLE_INDEX   SlmHandle,
            UInt32             MemID,
            UInt32             Offset,
            UInt32             Len,
            [In, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 3)] byte[] writebuff
            );

        /// <summary>
        /// 执行锁内代码
        /// </summary>
        /// <param name="SlmHandle"></param>
        /// <param name="ExfName"></param>
        /// <param name="InBuff"></param>
        /// <param name="InSize"></param>
        /// <param name="OutBuff"></param>
        /// <param name="OutSize"></param>
        /// <param name="RetSize"></param>
        /// <returns></returns>
        [DllImport("slm_runtime_easy.dll", EntryPoint = "#21", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 SlmExecuteStaticEasy(
            SLM_HANDLE_INDEX                             SlmHandle,
            [In, MarshalAs(UnmanagedType.LPStr)] string ExfName,
            [In, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 3)] byte[] InBuff,
            UInt32                                       InSize,
            [Out, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 5)] byte[] OutBuff,
            UInt32                                       OutSize,
            ref UInt32                                   RetSize);

        /// <summary>
        /// 许可动态执行代码，由开发商API gen_dynamic_code生成
        /// </summary>
        /// <param name="SlmHandle"></param>
        /// <param name="ExfBuff"></param>
        /// <param name="ExfSize"></param>
        /// <param name="InBuff"></param>
        /// <param name="InSize"></param>
        /// <param name="OutBuff"></param>
        /// <param name="OutSize"></param>
        /// <param name="RetSize"></param>
        /// <returns></returns>
        [DllImport("slm_runtime_easy.dll", EntryPoint = "#20", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 SlmExecuteDynamicEasy(
            SLM_HANDLE_INDEX      SlmHandle,
            [In, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 2)] byte[] ExfBuff,
            UInt32                ExfSize,
            [In, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 4)] byte[] InBuff,
            UInt32                InSize,
            [Out, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 6)] byte[] OutBuff,
            UInt32                OutSize,
            ref UInt32            RetSize);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="SlmHandle"></param>
        /// <param name="CodeType"></param>
        /// <param name="CodeBuff"></param>
        /// <param name="CodeSize"></param>
        /// <param name="InBuff"></param>
        /// <param name="InSize"></param>
        /// <param name="OutBuff"></param>
        /// <param name="OutSize"></param>
        /// <param name="RetSize"></param>
        /// <returns></returns>
        [DllImport("slm_runtime_easy.dll", EntryPoint = "#35", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 SlmExecuteEasy(
            SLM_HANDLE_INDEX SlmHandle,
            UInt32           CodeType,
            [In, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 3)] byte[] CodeBuff,
            UInt32           CodeSize,
            [In, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 5)] byte[] InBuff,
            UInt32           InSize,
            [Out, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 7)] byte[] OutBuff,
            UInt32           OutSize,
            ref UInt32       RetSize);

        /// <summary>
        /// 获取锁的设备证书
        /// </summary>
        /// <param name="SlmHandle"></param>
        /// <param name="DeviceCert"></param>
        /// <param name="CertSize"></param>
        /// <param name="RetSize"></param>
        /// <returns></returns>
        [DllImport("slm_runtime_easy.dll", EntryPoint = "#29", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 SlmGetDeviceCertEasy(
            SLM_HANDLE_INDEX           SlmHandle,
            [In, Out, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 2)] byte[] DeviceCert,
            UInt32                     CertSize,
            ref UInt32                 RetSize);

        /// <summary>
        /// 设备正版验证
        /// </summary>
        /// <param name="SlmHandle"></param>
        /// <param name="SignData"></param>
        /// <param name="SignSize"></param>
        /// <param name="Signature"></param>
        /// <param name="BuffSize"></param>
        /// <param name="SignatureSize"></param>
        /// <returns></returns>
        [DllImport("slm_runtime_easy.dll", EntryPoint = "#30", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 SlmSignByDeviceEasy(
            SLM_HANDLE_INDEX               SlmHandle,
            [In, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 2)] byte[] SignData,
            UInt32                         SignSize,
            [Out, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 4)] byte[] Signature,
            UInt32                         BuffSize,
            ref UInt32                     SignatureSize);

        /// <summary>
        /// 闪烁指示灯
        /// </summary>
        /// <param name="SlmHandle"></param>
        /// <param name="Color"></param>
        /// <param name="State"></param>
        /// <param name="Interval"></param>
        /// <returns></returns>
        [DllImport("slm_runtime_easy.dll", EntryPoint = "#22", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 SlmLedControlEasy(
            SLM_HANDLE_INDEX     SlmHandle,
            UInt32               Color,
            UInt32               State,
            UInt32               Interval);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="APIVersion"></param>
        /// <param name="SSVersion"></param>
        /// <returns></returns>
        [DllImport("slm_runtime_easy.dll", EntryPoint = "#23", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 SlmGetVersionEasy(
            ref UInt32 APIVersion, 
            ref UInt32 SSVersion);

        /// <summary>
        /// 通过错误码获得错误信息
        /// </summary>
        /// <param name="ErrorCode">错误码</param>
        /// <param name="Language">要返回字符串的语言，见 LANGUAGE_XXXX_ASCII</param>
        /// <returns>成功返回错误码信息（不需要调用slm_free释放），失败返回空指针NULL</returns>
        [DllImport("slm_runtime_easy.dll", EntryPoint = "#24", CallingConvention = CallingConvention.StdCall)]
        public static extern IntPtr SlmErrorFormatEasy(
            UInt32 ErrorCode, 
            UInt32 Language);

        /// <summary>
        /// 枚举已登录的用户token
        /// </summary>
        /// <returns>错误通过get_last_error 返回;SS_OK，如果oauth后台进程未启动，则返回SS_ERROR_BAD_CONNECT，
        ///          返回值调用slm_free_easy释放</returns>
        [DllImport("slm_runtime_easy.dll", EntryPoint = "#27", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 SlmGetCloudTokenEays();

        /// <summary>
        /// 将D2C包进行升级
        /// </summary>
        /// <param name="d2c_pkg"></param>
        /// <returns>IntPtr 返回错误信息，使用需要调用slm_free_easy释放</returns>
        [DllImport("slm_runtime_easy.dll", EntryPoint = "#25", CallingConvention = CallingConvention.StdCall)]
        public static extern IntPtr SlmUpdateEasy(
            [In, MarshalAs(UnmanagedType.LPStr)] string d2c_pkg);

        /// <summary>
        /// 将D2C包进行升级，需指定加密锁芯片号,可以并行操作
        /// </summary>
        /// <param name="szLockSN"></param>
        /// <param name="D2CPkg"></param>
        /// <returns>IntPtr 返回错误信息，使用需要调用SlmFreeEasy释放</returns>
        [DllImport("slm_runtime_easy.dll", EntryPoint = "#26", CallingConvention = CallingConvention.StdCall)]
        public static extern IntPtr SlmUpdateEasyEx(
            [In, MarshalAs(UnmanagedType.LPStr)] string szLockSN,
            [In, MarshalAs(UnmanagedType.LPStr)] string D2CPkg);

        /// <summary>
        /// 将D2C包进行升级，需指定加密锁芯片号和D2C包文件路径。
        /// </summary>
        /// <param name="szLockSN"></param>
        /// <param name="D2CFile"></param>
        /// <returns>IntPtr 返回错误信息，使用需要调用SlmFreeEasy释放</returns>
        [DllImport("slm_runtime_easy.dll", EntryPoint = "#34", CallingConvention = CallingConvention.StdCall)]
        public static extern IntPtr SlmUpdateFileEasy(
            [In, MarshalAs(UnmanagedType.LPStr)] string szLockSN,
            [In, MarshalAs(UnmanagedType.LPStr)] string D2CFile);

        /// <summary>
        /// 
        /// </summary>
        /// <returns>不需要时，需要调用SlmFreeEasy释放</returns>
        [DllImport("slm_runtime_easy.dll", EntryPoint = "#38", CallingConvention = CallingConvention.StdCall)]
        public static extern IntPtr SlmEnumDeviceEasy();

        /// <summary>
        /// 
        /// </summary>
        /// <param name="Buff"></param>
        [DllImport("slm_runtime_easy.dll", EntryPoint = "#31", CallingConvention = CallingConvention.StdCall)]
        public static extern void SlmFreeEasy(IntPtr Buff);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="DeveloperID"></param>
        /// <returns></returns>
        [DllImport("slm_runtime_easy.dll", EntryPoint = "#33", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 SlmGetDeveloperIDEasy(
            [Out, MarshalAs(UnmanagedType.LPArray)] byte[] DeveloperID);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="szLockSN"></param>
        /// <param name="szInsideFile"></param>
        /// <returns></returns>
        [DllImport("slm_runtime_easy.dll", EntryPoint = "#39", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 SlmD2CUpdateInsideEasy(
            [In, MarshalAs(UnmanagedType.LPStr)] string szLockSN,
            [In, MarshalAs(UnmanagedType.LPStr)] string szInsideFile);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="szDeviceInfo"></param>
        /// <returns>返回许可ID json结构，需要调用SlmFreeEasy释放</returns>
        [DllImport("slm_runtime_easy.dll", EntryPoint = "#40", CallingConvention = CallingConvention.StdCall)]
        public static extern IntPtr SlmEnumLicenseIDEasy(
            [In, MarshalAs(UnmanagedType.LPStr)] string szDeviceInfo);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="szDeviceInfo"></param>
        /// <param name="LicenseID"></param>
        /// <returns>返回许可信息 json结构，需要调用SlmFreeEasy释放</returns>
        [DllImport("slm_runtime_easy.dll", EntryPoint = "#41", CallingConvention = CallingConvention.StdCall)]
        public static extern IntPtr SlmGetLicenseInfoEasy(
            [In, MarshalAs(UnmanagedType.LPStr)] string szDeviceInfo,
            UInt32 LicenseID);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="slm_handle"></param>
        /// <param name="SignData"></param>
        /// <param name="SignLength"></param>
        /// <param name="Signature"></param>
        /// <param name="MaxBuffSize"></param>
        /// <returns>返回签名结果长度</returns>
        [DllImport("slm_runtime_easy.dll", EntryPoint = "#42", CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 SlmLicenseSignEasy(
            SLM_HANDLE_INDEX slm_handle,
            [In, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 2)] byte[] SignData,
            UInt32 SignLength,
            [Out, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 4)] byte[] Signature,
            UInt32 MaxBuffSize);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="SignData"></param>
        /// <param name="SignLength"></param>
        /// <param name="Signature"></param>
        /// <param name="MaxBuffSize"></param>
        /// <returns>返回签名信息json结构，需要调用SlmFreeEasy释放 </returns>
        [DllImport("slm_runtime_easy.dll", EntryPoint = "#43", CallingConvention = CallingConvention.StdCall)]
        public static extern IntPtr SlmLicenseVerifyEasy(
            [In, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 1)] byte[] SignData,
            UInt32 SignLength,
            [In, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 3)] byte[] Signature,
            UInt32 MaxBuffSize
            );
    }
}
