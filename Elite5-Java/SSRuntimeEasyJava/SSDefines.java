package com.senseyun.openapi.SSRuntimeEasyJava;

public interface SSDefines {
    int LED_COLOR_BLUE = 0;         //闪灯颜色：蓝色
    int LED_COLOR_RED = 1;          //闪灯颜色：红色

    int LED_STATE_CLOSE = 0;        //闪灯控制：关闭
    int LED_STATE_OPEN = 1;         //闪灯控制：打开
    int LED_STATE_SHRINK = 2;       //闪灯控制：闪烁

    //============================================================
    //              回调消息 message 类型
    //============================================================
    int SS_ANTI_INFORMATION		    = 0x0101;  //  消息类型：信息提示
    int SS_ANTI_WARNING			    = 0x0102;  //  消息类型：警告
    int SS_ANTI_EXCEPTION		    = 0x0103;  //  消息类型：异常
    int SS_ANTI_IDLE			    = 0x0104;  //  消息类型：暂保留
    
    
    int SS_MSG_SERVICE_START        = 0x0200;  //  服务启动
    int SS_MSG_SERVICE_STOP         = 0x0201;  //  服务停止
    int SS_MSG_LOCK_AVAILABLE       = 0x0202;  //  锁可用（插入锁或SS启动时锁已初始化完成）回调函数wparam 代表锁号
    int SS_MSG_LOCK_UNAVAILABLE     = 0x0203;  //  锁无效（锁已拔出）回调函数wparam 代表锁号
    
    
    //============================================================
    //              回调消息 wparam 参数
    //============================================================
    int SS_ANTI_PATCH_INJECT		= 0x0201;  //  发现注入
    int SS_ANTI_MODULE_INVALID		= 0x0202;  //  模块检测失败
    int SS_ANTI_ATTACH_FOUND		= 0x0203;  //  发现被调试器附加
    int SS_ANTI_THREAD_INVALID		= 0x0204;  //  无效的线程
    int SS_ANTI_THREAD_ERROR		= 0x0205;  //  线程检测失败
    int SS_ANTI_CRC_ERROR			= 0x0206;  //  CRC检验失败
    int SS_ANTI_DEBUGGER_FOUND		= 0x0207;  //  发现调试器
    
    
    int SLM_FIXTIME_RAND_LENGTH     = 0x08;    //  时钟校准随机数种子长度
    
    int SLM_CALLBACK_VERSION02      = 0x02;    //  SS_CALL_BACK的版本 （支持开发商API密码的版本）
    
    
    int  SLM_MEM_MAX_SIZE           = 2048;  //  内存托管最大字节
    int SLM_MAX_INPUT_SIZE          = 1758;  //  代码执行，最大输入缓冲区大小（字节）
    int SLM_MAX_OUTPUT_SIZE         = 1758;  //  代码执行，最大输出缓冲区大小（字节）
    int SLM_MAX_USER_CRYPT_SIZE     = 1520;  //  加解密最大缓冲区大小（字节）
                                                    
    int SLM_MAX_USER_DATA_SIZE      = 2048;  //  用户数据区最大长度（字节）
    int SLM_MAX_WRITE_SIZE          = 1904;  //  用户数据区写入最大长度（字节）

    String SLM_VERIFY_DEVICE_PREFIX   = "SENSELOCK";  //  请求签名的数据前缀

    int SLM_VERIFY_DATA_SIZE        = 41 ;  //  请求签名的数据大小，见slm_verify_device
    int SLM_LOCK_SN_LENGTH          = 16 ;  //  锁号的长度
    int SLM_DEVELOPER_ID_SIZE       = 8  ;  //  开发商ID长度
    int SLM_MAX_SERVER_NAME         = 32 ;  //  服务器名称最大长度
    int SLM_MAX_ACCESS_TOKEN_LENGTH = 64 ;  //  云锁用户token最大长度
    int SLM_MAX_CLOUD_SERVER_LENGTH = 100;  //  云锁服务器地址最大长度
    int SLM_SNIPPET_SEED_LENGTH     = 32 ;  //  碎片代码种子长度
    int SLM_DEV_PASSWORD_LENGTH     = 16 ;  //  开发商密码长度

    int SLM_CLOUD_MAX_USER_GUID_SIZE 	       = 128;   //  最大用户GUID长度 
                                                               
    int SLM_FILE_TYPE_BINARY                   = 0;     //  文件类型：数据文件
    int SLM_FILE_TYPE_EXECUTIVE                = 1;     //  文件类型：可执行文件文件
    int SLM_FILE_TYPE_KEY                      = 2;     //  文件类型：密钥文件 
                                                              
    int SLM_FILE_PRIVILEGE_FLAG_READ           = 0x01;  //  可读
    int SLM_FILE_PRIVILEGE_FLAG_WRITE          = 0x02;  //  可写
    int SLM_FILE_PRIVILEGE_FLAG_USE            = 0x04;  //  （密钥文件）可使用
    int SLM_FILE_PRIVILEGE_FLAG_UPDATE         = 0x08;  //  可远程升级

    int SLM_FILE_PRIVILEGE_FLAG_ENTRY_READ     = 0x10;  //  可读
    int SLM_FILE_PRIVILEGE_FLAG_ENTRY_WRITE    = 0x20;  //  可写
    int SLM_FILE_PRIVILEGE_FLAG_ENTRY_USE      = 0x40;  //  （密钥文件）可使用
    int SLM_FILE_PRIVILEGE_FLAG_ENTRY_UPDATE   = 0x80;  //  可远程升级

    int SLM_LOGIN_MODE_AUTO             = 0x0000;  //  自动登录模式
    int SLM_LOGIN_MODE_LOCAL            = 0x0001;  //  指定登录本地USE锁
    int SLM_LOGIN_MODE_REMOTE           = 0x0002;  //  指定登录远程USE锁 
    int SLM_LOGIN_MODE_CLOUD            = 0x0004;  //  指定登录云锁  
                                                        
    int SLM_LOGIN_FLAG_FIND_ALL         = 0x0001;  //  查找所有的锁，如果发现多个重名许可则不登录，提供选择，否则找到符合条件的锁直接登录
    int SLM_LOGIN_FLAG_VERSION          = 0x0004;  //  指定许可版本
    int SLM_LOGIN_FLAG_LOCKSN           = 0x0008;  //  指定锁号（USB）
    int SLM_LOGIN_FLAG_SERVER           = 0x0010;  //  指定服务器
    int SLM_LOGIN_FLAG_SNIPPET          = 0x0020;  //  指定碎片代码

    int LANGUAGE_CHINESE_ASCII              = 0x0001;  //  语言ID：简体中文
    int LANGUAGE_ENGLISH_ASCII              = 0x0002;  //  语言ID：英语
    int LANGUAGE_TRADITIONAL_CHINESE_ASCII  = 0x0003;  //  语言ID：繁体中文

    int SLM_INIT_FLAG_NOTIFY          = 0x01;  //  表示将收到SS的消息通知
}
