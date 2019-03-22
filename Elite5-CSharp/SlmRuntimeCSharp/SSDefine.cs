using System;
using System.Runtime.InteropServices;

namespace SLM
{
    /// <summary>
    /// 
    /// </summary>
    public enum INFO_FORMAT_TYPE : uint
    {
        JSON = 2,         //JSON格式
        STRUCT = 3,       //结构体格式
        STRING_KV = 4,    //字符串模式,遵行Key=value
        CIPHER = 5,       //加密二进制格式
    }

    /// <summary>
    /// 
    /// </summary>
    public enum LIC_USER_DATA_TYPE : uint
    {
        ROM = 0,       //只读区 
        RAW = 1,       //读写区
        PUB = 2,       //公开区
    }

    /// <summary>
    /// 
    /// </summary>
    public enum INFO_TYPE
    {
        LOCK_INFO = 1,       //锁信息
        SESSION_INFO = 2,    //会话信息
        LICENSE_INFO = 3,    //许可信息
        FILE_LIST = 4,       //文件列表
    }

    /// <summary>
    /// LED灯控制参数
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct ST_LED_CONTROL
    {
        public UInt32 index;               //0表示蓝色LED，1表示红色LED
        public UInt32 state;               //0代表关闭，1代表打开，2代表闪烁
        public UInt32 interval;            //当state为2时，表示闪烁间隔（毫秒）；state为其他值时，该字段无效。
    }

    /// <summary>
    /// 
    /// </summary>
    public class SSDefine
    {
        public const UInt32 LED_COLOR_BLUE = 0;         //闪灯颜色：蓝色
        public const UInt32 LED_COLOR_RED = 1;          //闪灯颜色：红色

        public const UInt32 LED_STATE_CLOSE = 0;        //闪灯控制：关闭
        public const UInt32 LED_STATE_OPEN = 1;         //闪灯控制：打开
        public const UInt32 LED_STATE_SHRINK = 2;       //闪灯控制：闪烁

        //============================================================
        //              回调消息 message 类型
        //============================================================
        public const UInt32 SS_ANTI_INFORMATION		    = 0x0101;  //  消息类型：信息提示
        public const UInt32 SS_ANTI_WARNING			    = 0x0102;  //  消息类型：警告
        public const UInt32 SS_ANTI_EXCEPTION		    = 0x0103;  //  消息类型：异常
        public const UInt32 SS_ANTI_IDLE			    = 0x0104;  //  消息类型：暂保留
        
        
        public const UInt32 SS_MSG_SERVICE_START        = 0x0200;  //  服务启动
        public const UInt32 SS_MSG_SERVICE_STOP         = 0x0201;  //  服务停止
        public const UInt32 SS_MSG_LOCK_AVAILABLE       = 0x0202;  //  锁可用（插入锁或SS启动时锁已初始化完成）回调函数wparam 代表锁号
        public const UInt32 SS_MSG_LOCK_UNAVAILABLE     = 0x0203;  //  锁无效（锁已拔出）回调函数wparam 代表锁号
        
        
        //============================================================
        //              回调消息 wparam 参数
        //============================================================
        public const UInt32 SS_ANTI_PATCH_INJECT		= 0x0201;  //  发现注入
        public const UInt32 SS_ANTI_MODULE_INVALID		= 0x0202;  //  模块检测失败
        public const UInt32 SS_ANTI_ATTACH_FOUND		= 0x0203;  //  发现被调试器附加
        public const UInt32 SS_ANTI_THREAD_INVALID		= 0x0204;  //  无效的线程
        public const UInt32 SS_ANTI_THREAD_ERROR		= 0x0205;  //  线程检测失败
        public const UInt32 SS_ANTI_CRC_ERROR			= 0x0206;  //  CRC检验失败
        public const UInt32 SS_ANTI_DEBUGGER_FOUND		= 0x0207;  //  发现调试器
        
        
        public const UInt32 SLM_FIXTIME_RAND_LENGTH     = 0x08;    //  时钟校准随机数种子长度
        
        public const UInt32 SLM_CALLBACK_VERSION02      = 0x02;    //  SS_CALL_BACK的版本 （支持开发商API密码的版本）
        
        
        public const UInt32  SLM_MEM_MAX_SIZE           = 2048;  //  内存托管最大字节
        public const UInt32 SLM_MAX_INPUT_SIZE          = 1758;  //  代码执行，最大输入缓冲区大小（字节）
        public const UInt32 SLM_MAX_OUTPUT_SIZE         = 1758;  //  代码执行，最大输出缓冲区大小（字节）
        public const UInt32 SLM_MAX_USER_CRYPT_SIZE     = 1520;  //  加解密最大缓冲区大小（字节）
                                                        
        public const UInt32 SLM_MAX_USER_DATA_SIZE      = 2048;  //  用户数据区最大长度（字节）
        public const UInt32 SLM_MAX_WRITE_SIZE          = 1904;  //  用户数据区写入最大长度（字节）

        public const String SLM_VERIFY_DEVICE_PREFIX   = "SENSELOCK";  //  请求签名的数据前缀

        public const UInt32 SLM_VERIFY_DATA_SIZE        = 41 ;  //  请求签名的数据大小，见slm_verify_device
        public const UInt32 SLM_LOCK_SN_LENGTH          = 16 ;  //  锁号的长度
        public const UInt32 SLM_DEVELOPER_ID_SIZE       = 8  ;  //  开发商ID长度
        public const UInt32 SLM_MAX_SERVER_NAME         = 32 ;  //  服务器名称最大长度
        public const UInt32 SLM_MAX_ACCESS_TOKEN_LENGTH = 64 ;  //  云锁用户token最大长度
        public const UInt32 SLM_MAX_CLOUD_SERVER_LENGTH = 100;  //  云锁服务器地址最大长度
        public const UInt32 SLM_SNIPPET_SEED_LENGTH     = 32 ;  //  碎片代码种子长度
        public const UInt32 SLM_DEV_PASSWORD_LENGTH     = 16 ;  //  开发商密码长度

        public const UInt32 SLM_CLOUD_MAX_USER_GUID_SIZE 	       = 128;   //  最大用户GUID长度 
                                                                   
        public const UInt32 SLM_FILE_TYPE_BINARY                   = 0;     //  文件类型：数据文件
        public const UInt32 SLM_FILE_TYPE_EXECUTIVE                = 1;     //  文件类型：可执行文件文件
        public const UInt32 SLM_FILE_TYPE_KEY                      = 2;     //  文件类型：密钥文件 
                                                                  
        public const UInt32 SLM_FILE_PRIVILEGE_FLAG_READ           = 0x01;  //  可读
        public const UInt32 SLM_FILE_PRIVILEGE_FLAG_WRITE          = 0x02;  //  可写
        public const UInt32 SLM_FILE_PRIVILEGE_FLAG_USE            = 0x04;  //  （密钥文件）可使用
        public const UInt32 SLM_FILE_PRIVILEGE_FLAG_UPDATE         = 0x08;  //  可远程升级
 
        public const UInt32 SLM_FILE_PRIVILEGE_FLAG_ENTRY_READ     = 0x10;  //  可读
        public const UInt32 SLM_FILE_PRIVILEGE_FLAG_ENTRY_WRITE    = 0x20;  //  可写
        public const UInt32 SLM_FILE_PRIVILEGE_FLAG_ENTRY_USE      = 0x40;  //  （密钥文件）可使用
        public const UInt32 SLM_FILE_PRIVILEGE_FLAG_ENTRY_UPDATE   = 0x80;  //  可远程升级

        public const UInt32 SLM_LOGIN_MODE_AUTO             = 0x0000;  //  自动登录模式
        public const UInt32 SLM_LOGIN_MODE_LOCAL            = 0x0001;  //  指定登录本地USE锁
        public const UInt32 SLM_LOGIN_MODE_REMOTE           = 0x0002;  //  指定登录远程USE锁 
        public const UInt32 SLM_LOGIN_MODE_CLOUD            = 0x0004;  //  指定登录云锁  
                                                            
        public const UInt32 SLM_LOGIN_FLAG_FIND_ALL         = 0x0001;  //  查找所有的锁，如果发现多个重名许可则不登录，提供选择，否则找到符合条件的锁直接登录
        public const UInt32 SLM_LOGIN_FLAG_VERSION          = 0x0004;  //  指定许可版本
        public const UInt32 SLM_LOGIN_FLAG_LOCKSN           = 0x0008;  //  指定锁号（USB）
        public const UInt32 SLM_LOGIN_FLAG_SERVER           = 0x0010;  //  指定服务器
        public const UInt32 SLM_LOGIN_FLAG_SNIPPET          = 0x0020;  //  指定碎片代码

        public const UInt32 LANGUAGE_CHINESE_ASCII              = 0x0001;  //  语言ID：简体中文
        public const UInt32 LANGUAGE_ENGLISH_ASCII              = 0x0002;  //  语言ID：英语
        public const UInt32 LANGUAGE_TRADITIONAL_CHINESE_ASCII  = 0x0003;  //  语言ID：繁体中文

        public const UInt32 SLM_INIT_FLAG_NOTIFY          = 0x01;  //  表示将收到SS的消息通知
    }

    //[StructLayout(LayoutKind.Sequential, Pack=1)]
    //public struct ST_INIT_PARAM 
    //{
    //    /** 版本－用来兼容，当前使用 SLM_CALLBACK_VERSION02 */
    //    public UInt32       version;
    //        /** 如果需要接收SS通知，填 SLM_INIT_FLAG_NOTIFY */
    //    public UInt32       flag;
    //    /** 回调函数指针*/
    //    public UInt32       pfn;
    //    /** 通信连接超时时间（毫秒），如果填0，则使用默认超时时间（7秒）*/
    //    public UInt32       timeout;
    //    /** API密码，可从开发者网站上获取*/
    //    [MarshalAs(UnmanagedType.ByValArray, SizeConst = (int)SSDefine.SLM_DEV_PASSWORD_LENGTH)]
    //    public char[]          password;
    //} 

    [StructLayout(LayoutKind.Sequential, Pack=1)]
    public struct ST_LOGIN_PARAM
    { 
        /** 结构体大小（必填）*/
        public UInt32       size;
        /** 要登录的许可ID*/
        public UInt32       license_id;
        /** 许可会话的超时时间（单位：秒）,填0则使用默认值：600秒   */
        public UInt32       timeout;
        /** 许可登录的模式：本地，远程，云（见LOGIN_MODE_XXX)，如果填0，则使用SLM_LOGIN_MODE_AUTO*/
        public UInt32       login_mode;
        /** 许可登录的标志：见SLM_LOGIN_FLAG_XXX */
        public UInt32       login_flag;
        /** 许可登录指定的锁唯一序列号（可选）*/
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = (int)SSDefine.SLM_LOCK_SN_LENGTH)]
        public byte[]         sn;
        /** 网络锁服务器地址（可选），仅识别IP地址 */
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = (int)SSDefine.SLM_MAX_SERVER_NAME)]
        public char[]         server;
        /** 云锁用户token（可选）*/
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = (int)SSDefine.SLM_MAX_ACCESS_TOKEN_LENGTH)]
        public char[]         access_token;
        /** 云锁服务器地址（可选）*/
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = (int)SSDefine.SLM_MAX_CLOUD_SERVER_LENGTH)]
        public char[]         cloud_server;
        /** 碎片代码种子（可选），如果要支持碎片代码,login_flag需要指定为SLM_LOGIN_FLAG_SNIPPET*/
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = (int)SSDefine.SLM_SNIPPET_SEED_LENGTH)]
        public byte[]         snippet_seed;
        /** 已登录用户的guid（可选） */
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = (int)SSDefine.SLM_CLOUD_MAX_USER_GUID_SIZE)]
        public byte[]         user_guid;
    }

 }
