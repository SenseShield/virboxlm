using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

using SLM;

using SLM_HANDLE_INDEX = System.UInt32;

namespace SlmRuntimeCSharp
{
    class Program
    {
        public static void WriteLineGreen(string s)
        {
            Console.ForegroundColor = ConsoleColor.Green;
            Console.WriteLine(s);
            Console.ResetColor();
        }
        public static void WriteLineRed(string s)
        {
            Console.ForegroundColor = ConsoleColor.Red;
            Console.WriteLine(s);
            Console.ResetColor();
        }
        public static void WriteLineYellow(string s)
        {
            Console.ForegroundColor = ConsoleColor.Yellow;
            Console.WriteLine(s);
            Console.ResetColor();
        }
        public static byte[] StringToHex(string HexString)
        {
            byte[] returnBytes = new byte[HexString.Length / 2];
            for (int i = 0; i < returnBytes.Length; i++)
                returnBytes[i] = Convert.ToByte(HexString.Substring(i * 2, 2), 16);

            return returnBytes;
        }

        static void Main(string[] args)
        {
            UInt32 Ret = SSErrCode.SS_OK;
            string StrMsg = string.Empty;
            SLM_HANDLE_INDEX Handle = 0;

            //
            //01. INIT   03...09
            //
            string psd = "DB3B838B2E4F08F5C9EFCD1A5DD16341";     
            //string psd = "34710D2A4BCF82E89F70D39A6E04DFB8";
            Ret = SlmRuntime.SlmInitEasy(psd);
            if (Ret != SSErrCode.SS_OK)
            {
                StrMsg = string.Format("SlmInitEasy Failure:0x{0:X8}", Ret);
                WriteLineRed(StrMsg);
            }
            else
            {
                WriteLineGreen("SlmInitEasy Success!");
            }

            //
            //02. FIND LICENSE
            //
            IntPtr FindLic = SlmRuntime.SlmFindLicenseEasy(0, INFO_FORMAT_TYPE.JSON);
            Ret = SlmRuntime.SlmGetLastError();
            if (Ret != SSErrCode.SS_OK)
            {
                StrMsg = string.Format("SlmFindLicenseEasy Failure:0x{0:X8}", Ret);
                WriteLineRed(StrMsg);
            }
            else
            {
                string StrPrint = Marshal.PtrToStringAnsi(FindLic);
                WriteLineYellow(StrPrint);
                WriteLineGreen("SlmFindLicenseEasy Success!");
            }

            //
            //03. LOGIN
            //
            // json方式  为确保安全，SDK版本号2.1.0.15128之后的许可登录将不再支持json登录的方法。
            String jsLogin = "{\"license_id\":0,\"time_out\":86400}";
            System.IntPtr jsLoginPtr = Marshal.StringToHGlobalAnsi(jsLogin);   // 需要将字符串转换为IntPtr

            // struct方式 
            ST_LOGIN_PARAM stLogin = new ST_LOGIN_PARAM();
            stLogin.size = (UInt32)Marshal.SizeOf(stLogin);
            stLogin.license_id = 0;
            stLogin.login_flag = SSDefine.SLM_LOGIN_FLAG_LOCKSN;   //(可选)
            stLogin.sn = StringToHex("9733c80100070205106100030015000c");   //（可选）

            //Handle = SlmRuntime.SlmLoginEasy(jsLoginPtr, INFO_FORMAT_TYPE.JSON);  //不支持
            Handle = SlmRuntime.SlmLoginEasy(ref stLogin, INFO_FORMAT_TYPE.STRUCT);
            Ret = SlmRuntime.SlmGetLastError();
            if (Ret != SSErrCode.SS_OK)
            {
                StrMsg = string.Format("SlmLoginEasy Failure:0x{0:X8}", Ret);
                WriteLineRed(StrMsg);
                System.Diagnostics.Debug.Assert(true);
            }
            else
            {
                WriteLineGreen("SlmLoginEasy Success!");
            }

            //04. KEEP ALIVE
            Ret = SlmRuntime.SlmKeepAliveEasy(Handle);
            if(Ret != SSErrCode.SS_OK)
            {
                StrMsg = string.Format("SlmKeepAliveEasy Failure:0x{0:X8}", Ret);
                WriteLineRed(StrMsg);
                System.Diagnostics.Debug.Assert(true);
            }
            else
            {
                WriteLineGreen("SlmKeepAliveEasy Success!");
            }

            //
            //05. 06. GET INFO
            //
            IntPtr License = SlmRuntime.SlmGetInfoEasy(Handle, INFO_TYPE.LICENSE_INFO, INFO_FORMAT_TYPE.JSON);
            Ret = SlmRuntime.SlmGetLastError();
            if (Ret != SSErrCode.SS_OK)
            {
                StrMsg = string.Format("SlmGetInfoEasy [LICENSE] Failure:0x{0:X8}", Ret);
                WriteLineRed(StrMsg);
                System.Diagnostics.Debug.Assert(true);
            }
            else
            {
                string StrPrint = Marshal.PtrToStringAnsi(License);
                WriteLineYellow(StrPrint);
                WriteLineGreen("SlmGetInfoEasy [LICENSE] Success!");
                SlmRuntime.SlmFreeEasy(License);
            }

            IntPtr Session = SlmRuntime.SlmGetInfoEasy(Handle, INFO_TYPE.SESSION_INFO, INFO_FORMAT_TYPE.JSON);
            Ret = SlmRuntime.SlmGetLastError();
            if (Ret != SSErrCode.SS_OK)
            {
                StrMsg = string.Format("SlmGetInfoEasy [SESSION] Failure:0x{0:X8}", Ret);
                WriteLineRed(StrMsg);
                System.Diagnostics.Debug.Assert(true);
            }
            else
            {
                string StrPrint = Marshal.PtrToStringAnsi(Session);
                WriteLineYellow(StrPrint);
                WriteLineGreen("SlmGetInfoEasy [SESSION] Success!");
                SlmRuntime.SlmFreeEasy(Session);
            }

            IntPtr LockInfo = SlmRuntime.SlmGetInfoEasy(Handle, INFO_TYPE.LOCK_INFO, INFO_FORMAT_TYPE.JSON);
            Ret = SlmRuntime.SlmGetLastError();
            if (Ret != SSErrCode.SS_OK)
            {
                StrMsg = string.Format("SlmGetInfoEasy [LOCKINFO] Failure:0x{0:X8}", Ret);
                WriteLineRed(StrMsg);
                System.Diagnostics.Debug.Assert(true);
            }
            else
            {
                string StrPrint = Marshal.PtrToStringAnsi(LockInfo);
                WriteLineYellow(StrPrint);
                WriteLineGreen("SlmGetInfoEasy [LOCKINFO] Success!");
                SlmRuntime.SlmFreeEasy(LockInfo);
            }
            IntPtr FileList = SlmRuntime.SlmGetInfoEasy(Handle, INFO_TYPE.FILE_LIST, INFO_FORMAT_TYPE.JSON);
            Ret = SlmRuntime.SlmGetLastError();
            if (Ret != SSErrCode.SS_OK)
            {
                StrMsg = string.Format("SlmGetInfoEasy [FILELIST] Failure:0x{0:X8}", Ret);
                WriteLineRed(StrMsg);
                System.Diagnostics.Debug.Assert(true);
            }
            else
            {
                string StrPrint = Marshal.PtrToStringAnsi(FileList);
                WriteLineYellow(StrPrint);
                WriteLineGreen("SlmGetInfoEasy [FILELIST] Success!");
                SlmRuntime.SlmFreeEasy(FileList);
            }

            //
            //07. 08. ENCRYPT DECRYPT
            //
            string StrData = "test data.......";
            byte[] Data = System.Text.ASCIIEncoding.Default.GetBytes(StrData);
            byte[] Enc = new byte[StrData.Length];
            byte[] Dec = new byte[StrData.Length];

            WriteLineYellow(string.Format("[DATA]:{0}", StrData));
            Ret = SlmRuntime.SlmEncryptEasy(Handle, Data, Enc, (UInt32)StrData.Length);
            if (Ret != SSErrCode.SS_OK)
            {
                StrMsg = string.Format("SlmEncryptEasy Failure:0x{0:X8}", Ret);
                WriteLineRed(StrMsg);
                System.Diagnostics.Debug.Assert(true);
            }
            else
            {
                string StrEnc = string.Format("[ENCRYPT]:{0}",System.Text.ASCIIEncoding.Default.GetString(Enc));
                WriteLineYellow(StrEnc);
                WriteLineGreen("SlmEncryptEasy Success!");
            }

            Ret = SlmRuntime.SlmDecryptEasy(Handle, Enc, Dec, (UInt32)StrData.Length);
            if (Ret != SSErrCode.SS_OK)
            {
                StrMsg = string.Format("SlmDecryptEasy Failure:0x{0:X8}", Ret);
                WriteLineRed(StrMsg);
                System.Diagnostics.Debug.Assert(true);
            }
            else
            {
                string StrDec = string.Format("[DECRYPT]:{0}",System.Text.ASCIIEncoding.Default.GetString(Dec));
                WriteLineYellow(StrDec);
                WriteLineGreen("SlmDecryptEasy Success!");
            }

            //
            //09. 10. 11.  USER DATA GETSIZE - USER DATA READ - USER DATA WRITE
            //
            UInt32 MemSize = 0;

            MemSize = SlmRuntime.SlmUserDataGetSizeEasy(Handle, LIC_USER_DATA_TYPE.PUB);
            Ret = SlmRuntime.SlmGetLastError();
            if (Ret == SSErrCode.SS_OK)
            {
                WriteLineGreen("SlmUserDataGetSizeEasy Success!");
                if(MemSize > 0)
                {
                    byte[] ReadBuff = new byte[MemSize];
                    Ret = SlmRuntime.SlmUserDataReadEasy(Handle, LIC_USER_DATA_TYPE.PUB, ReadBuff, 0, MemSize);
                    if (Ret != SSErrCode.SS_OK)
                    {
                        StrMsg = string.Format("SlmUserDataReadEasy Failure:0x{0:X8}", Ret);
                        WriteLineRed(StrMsg);
                        System.Diagnostics.Debug.Assert(true);
                    }
                    else
                    {
                        WriteLineGreen("SlmUserDataReadEasy Success!");
                    }
                }
            }
            else
            {
                StrMsg = string.Format("SlmUserDataGetSizeEasy Failure:0x{0:X8}", Ret);
                WriteLineRed(StrMsg);
                System.Diagnostics.Debug.Assert(true);
            }

            //
            //12. 13. 14. 15. 16 MEM ALLOC - MEM WRITE - MEM READ - MEM FREE
            //
            UInt32 MemID = SlmRuntime.SlmMemAllocEasy(Handle, 1024);
            Ret = SlmRuntime.SlmGetLastError();
            if (Ret != SSErrCode.SS_OK)
            {
                StrMsg = string.Format("SlmMemAllocEasy Failure:0x{0:X8}", Ret);
                WriteLineRed(StrMsg);
                System.Diagnostics.Debug.Assert(true);
            }
            else
            {
                WriteLineYellow(string.Format("[Mem ID]:{0}", MemID));
                WriteLineGreen("SlmDecryptEasy Success!");
            }

            string StrBuff = "test memory data...";
            UInt32 Size = (UInt32)StrBuff.Length;
            UInt32 Len = 0;
            byte[] WriteBuf = System.Text.ASCIIEncoding.Default.GetBytes(StrBuff);
            byte[] ReadBuf = new byte[Size];


            Len = SlmRuntime.SlmMemWriteEasy(Handle, MemID, 0, Size, WriteBuf);
            Ret = SlmRuntime.SlmGetLastError();
            if (Ret != SSErrCode.SS_OK)
            {
                StrMsg = string.Format("SlmMemWriteEasy Failure:0x{0:X8}", Ret);
                WriteLineRed(StrMsg);
                System.Diagnostics.Debug.Assert(true);
            }
            else
            {
                WriteLineYellow(string.Format("[Mem Write]:{0}", StrBuff));
                WriteLineGreen("SlmMemWriteEasy Success!");
            }

            Len = SlmRuntime.SlmMemReadEasy(Handle, MemID, 0, Size, ReadBuf);
            Ret = SlmRuntime.SlmGetLastError();
            if (Ret != SSErrCode.SS_OK)
            {
                StrMsg = string.Format("SlmMemReadEasy Failure:0x{0:X8}", Ret);
                WriteLineRed(StrMsg);
                System.Diagnostics.Debug.Assert(true);
            }
            else
            {
                string StrPrint = string.Format("[Mem Read]:{0}", System.Text.ASCIIEncoding.Default.GetString(ReadBuf));
                WriteLineYellow(StrPrint);
                WriteLineGreen("SlmMemReadEasy Success!");
            }

            Ret = SlmRuntime.SlmMemFreeEasy(Handle, MemID);
            if (Ret != SSErrCode.SS_OK)
            {
                StrMsg = string.Format("SlmMemFreeEasy Failure:0x{0:X8}", Ret);
                WriteLineRed(StrMsg);
                System.Diagnostics.Debug.Assert(true);
            }
            else
            {
                WriteLineGreen("SlmMemFreeEasy Success!");
            }

            //
            //17. LOGOUT
            //
            Ret = SlmRuntime.SlmLogoutEasy(Handle);

            //
            //18. GET ERROR FORMAT
            //
            IntPtr ErrFmt = SlmRuntime.SlmErrorFormatEasy(2, SSDefine.LANGUAGE_ENGLISH_ASCII);
            WriteLineYellow(Marshal.PtrToStringAnsi(ErrFmt));

            //
            //19. GET DEVELOPER ID
            //
            byte[] DeveloperID = new byte[16];
            Ret = SlmRuntime.SlmGetDeveloperIDEasy(DeveloperID);
            if (Ret != SSErrCode.SS_OK)
            {
                StrMsg = string.Format("SlmGetDeveloperIDEasy Failure:0x{0:X8}", Ret);
                WriteLineRed(StrMsg);
                System.Diagnostics.Debug.Assert(true);
            }
            else
            {
                WriteLineYellow(System.Text.ASCIIEncoding.Default.GetString(DeveloperID));
                WriteLineGreen("SlmGetDeveloperIDEasy Success!");
            }

            //
            //20. GET VERSION 
            UInt32 APIVersion = 0;
            UInt32 SSVersion = 0;
            Ret = SlmRuntime.SlmGetVersionEasy(ref APIVersion, ref SSVersion);
            if (Ret != SSErrCode.SS_OK)
            {
                StrMsg = string.Format("SlmGetVersionEasy Failure:0x{0:X8}", Ret);
                WriteLineRed(StrMsg);
                System.Diagnostics.Debug.Assert(true);
            }
            else
            {
                WriteLineYellow(string.Format("[API Version]:{0}\n[ SS Version]:{1}", APIVersion, SSVersion));
                WriteLineGreen("SlmGetVersionEasy Success!");
            }

            //
            //21. ENUM DEVICE 
            //
            IntPtr DeviceInfo = SlmRuntime.SlmEnumDeviceEasy();
            Ret = SlmRuntime.SlmGetLastError();
            if (Ret != SSErrCode.SS_OK)
            {
                StrMsg = string.Format("SlmEnumDeviceEasy Failure:0x{0:X8}", Ret);
                WriteLineRed(StrMsg);
                System.Diagnostics.Debug.Assert(true);
            }
            else
            {
                WriteLineYellow(Marshal.PtrToStringAnsi(DeviceInfo));
                WriteLineGreen("SlmEnumDeviceEasy Success!");
            }

            //
            //22. ENUM LICENSE ID ,GET LICENSE INFO
            //
            string strDeviceInfo = Marshal.PtrToStringAnsi(DeviceInfo);
            JArray arrDeviceInfo = (JArray)JsonConvert.DeserializeObject(strDeviceInfo);

            for(int i = 0; i < arrDeviceInfo.Count; i++)
            {
                string Info = arrDeviceInfo[i].ToString();
                IntPtr LicenseID = SlmRuntime.SlmEnumLicenseIDEasy(Info);
                Ret = SlmRuntime.SlmGetLastError();
                if (Ret != SSErrCode.SS_OK)
                {
                    StrMsg = string.Format("SlmEnumLicenseIDEasy Failure:0x{0:X8}", Ret);
                    WriteLineRed(StrMsg);
                    System.Diagnostics.Debug.Assert(true);
                }
                else
                {
                    WriteLineYellow(Marshal.PtrToStringAnsi(LicenseID));
                    WriteLineGreen("SlmEnumLicenseIDEasy Success!");
                    SlmRuntime.SlmFreeEasy(LicenseID);
                }

                IntPtr LicenseInfo = SlmRuntime.SlmGetLicenseInfoEasy(Info, 0);  // 获取零号许可信息
                Ret = SlmRuntime.SlmGetLastError();
                if (Ret != SSErrCode.SS_OK)
                {
                    StrMsg = string.Format("SlmGetLicenseInfoEasy Failure:0x{0:X8}", Ret);
                    WriteLineRed(StrMsg);
                    System.Diagnostics.Debug.Assert(true);
                }
                else
                {
                    WriteLineYellow(Marshal.PtrToStringAnsi(LicenseInfo));
                    WriteLineGreen("SlmGetLicenseInfoEasy Success!");
                    SlmRuntime.SlmFreeEasy(LicenseInfo);
                }
            }

            SlmRuntime.SlmFreeEasy(DeviceInfo);

            //
            //22. CLEANUP
            //
            Ret = SlmRuntime.SlmCleanupEasy();

            WriteLineGreen("test finish ...!");
            Console.ReadLine();
        }
    }
}
