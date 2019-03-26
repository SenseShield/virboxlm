using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SLM
{
    public class SSErrCode
    {
        //============================================================
        //              一般错误码
        //============================================================
        public const UInt32 SS_OK = 0x00000000;                            //  成功
        public const UInt32 SS_ERROR = 0x00000001;                         //  错误，原因未知 */ // TODO(zhaock) : 应该去
        public const UInt32 SS_ERROR_INVALID_PARAM = 0x00000002;           //  不合法的参数
        public const UInt32 SS_ERROR_MEMORY_FAIELD = 0x00000003;           //  内存错误
        public const UInt32 SS_ERROR_INSUFFICIENT_BUFFER = 0x00000004;     //  缓冲区大小不足
        public const UInt32 SS_ERROR_NOT_FOUND = 0x00000005;               //  没找到目标
        public const UInt32 SS_ERROR_EXISTED = 0x00000006;                 //  目标已存在
        public const UInt32 SS_ERROR_DATA_BROKEN = 0x00000007;             //  数据损坏
        public const UInt32 SS_ERROR_INVALID_HANDLE = 0x00000008;          //  无效的句柄
        public const UInt32 SS_ERROR_TIMEOUT = 0x00000009;                 //  操作超时
        public const UInt32 SS_ERROR_TARGET_NOT_IN_USE = 0x0000000A;       //  目标未在使用状态，如目标模块未开启或已关闭
        public const UInt32 SS_ERROR_DATA_CONFLICT = 0X0000000B;           //  不相容的数据同时存在
        public const UInt32 SS_ERROR_INVALID_TYPE = 0x0000000C;            //  无效类型
        public const UInt32 SS_ERROR_INVALID_LENGTH = 0x0000000D;          //  无效长度
        public const UInt32 SS_ERROR_USER_MOD_CRASH = 0x0000000E;          //  用户模块冲突
        public const UInt32 SS_ERROR_SERVER_IS_LOCAL = 0x0000000F;         //  查找的SS是本地
        public const UInt32 SS_ERROR_UNSUPPORT = 0x00000010;               //  不支持的操作
        public const UInt32 SS_ERROR_PORT_IN_USE = 0x00000011;             //  端口占用
        public const UInt32 SS_ERROR_NO_KEY = 0x00000013;                  //  没有密钥
        public const UInt32 SS_ERROR_SERVICE_TYPE_NOT_SUPPORT = 0x00000014;//  服务类型不支持操作
        public const UInt32 SS_ERROR_MULTICAST_ADDR_IN_USE = 0x00000015;   //  多播地址占用
        public const UInt32 SS_ERROR_MULTICAST_PORT_IN_USE = 0x00000016;   //  多播端口占用
        public const UInt32 SS_ERROR_MOD_FAIL_LIBSTRING = 0x00000020;      //  libstring错误
        public const UInt32 SS_ERROR_NET_ERROR = 0x00000040;               //  网络错误
        public const UInt32 SS_ERROR_IPC_ERROR = 0x00000041;               //  IPC 错误
        public const UInt32 SS_ERROR_INVALID_SESSION = 0x00000042;         //  会话失效

        //============================================================
        //          LM 模块(0x20): (runtime, control, develop)
        //============================================================
        public const UInt32 SS_ERROR_D2C_NO_PACKAGE = 0x13000000;                   //  D2C包中无签发内容
        public const UInt32 SS_ERROR_DEVELOPER_CERT_ALREADY_EXIST = 0x13000001;     //  开发商证书已存在
        public const UInt32 SS_ERROR_PARSE_CERT = 0x13000003;                       //  解析证书错误
        public const UInt32 SS_ERROR_D2C_PACKAGE_TOO_LARGE = 0x13000004;            //  D2C包过大
        public const UInt32 SS_ERROR_RESPONSE = 0x13000005;                         //  错误的数据响应
        public const UInt32 SS_ERROR_SEND_LM_REMOTE_REQUEST = 0x13000006;           //  发送LM远程请求失败
        public const UInt32 SS_ERROR_RUNTIME_NOT_INITIALIZE = 0x13000007;           //  未调用Runtime初始化函数
        public const UInt32 SS_ERROR_BAD_CONNECT = 0x13000008;                      //  获取连接失败
        public const UInt32 SS_ERROR_RUNTIME_VERSION = 0x13000009;                  //  版本不匹配
        public const UInt32 SS_ERROR_LIC_NOT_FOUND = 0x13000020;                    //  许可未找到
        public const UInt32 SS_ERROR_AUTH_ACCEPT_FAILED = 0x13000021;               //  认证错误
        public const UInt32 SS_ERROR_AUTH_HANDLE_FAILED = 0x13000022;               //  认证失败
        public const UInt32 SS_ERROR_DECODE_BUFFER = 0x13000023;                    //  解密错误
        public const UInt32 SS_ERROR_USER_DATA_TOO_SMALL = 0x13000024;              //  用户数据区太小
        public const UInt32 SS_ERROR_INVALID_LM_REQUEST = 0x13000025;               //  无效的LM请求
        public const UInt32 SS_ERROR_INVALID_SHORTCODE = 0x13000026;                //  无效的短码
        public const UInt32 SS_ERROR_INVALID_D2C_PACKAGE = 0x13000027;              //  错误的D2C升级包
        public const UInt32 SS_ERROR_CLOUD_RESPONSE = 0x13000028;                   //  云锁返回的数据错误
        public const UInt32 SS_ERROR_USER_DATA_TOO_LARGE = 0x13000029;              //  读写的数据过大
        public const UInt32 SS_ERROR_INVALID_MEMORY_ID = 0x1300002A;                //  无效的内存ID
        public const UInt32 SS_ERROR_INVALID_MEMORY_OFFSET = 0x1300002B;            //  无效的内存偏移
        public const UInt32 SS_ERROR_INVALID_CLOUD_SERVER = 0x1300002C;             //  无效的云锁服务器
        public const UInt32 SS_ERROR_UNCALIBRATED_TIMESTAMP = 0x1300002D;           //  时间戳未校准
        public const UInt32 SS_ERROR_GENERATE_GUID = 0x1300002F;                    //  生成GUID错误
        public const UInt32 SS_ERROR_NO_LOGGED_USER = 0x13000030;                   //  没有登录的用户
        public const UInt32 SS_ERROR_USER_AUTH_SERVER_NOT_RUNNING = 0x13000031;     //  用户认证服务未启动
        public const UInt32 SS_ERROR_MODULE_NOT_EXIST = 0x13000032;                 //  模块不存在
        public const UInt32 SS_ERROR_UNSUPPORTED_SNIPPET_CODE = 0x13000033;         //  不支持的代码片
        public const UInt32 SS_ERROR_INVALID_SNIPPET_CODE = 0x13000034;             //  无效的代码
        public const UInt32 SS_ERROR_EXECUTE_SNIPPET_CODE = 0x13000035;             //  执行碎片代码失败
        public const UInt32 SS_ERROR_SNIPPET_EXECUTE_LOGIN = 0x13000036;            //  碎片执行登录失败
        public const UInt32 SS_ERROR_LICENSE_MODULE_NOT_EXISTS = 0x13000037;        //  许可模块不存在

        //============================================================
        //              IPC 模块 (0x02)
        //============================================================
        
        public const UInt32 SS_ERROR_BAD_ADDR                          = 0x02000000;  //  错误的地址
        public const UInt32 SS_ERROR_BAD_NAME                          = 0x02000001;  //  错误的名称
        public const UInt32 SS_ERROR_IPC_FAILED                        = 0x02000002;  //  IPC 收发错误
        public const UInt32 SS_ERROR_IPC_CONNECT_FAILED                = 0x02000003;  //  连接失败
        public const UInt32 SS_ERROR_IPC_AUTH_INITIALIZE               = 0x02000004;  //  Auth失败
        public const UInt32 SS_ERROR_IPC_QUERY_STATE                   = 0x02000005;  //  查询SS状态失败
        public const UInt32 SS_ERROR_SERVICE_NOT_RUNNING               = 0x02000006;  //  SS未运行
        public const UInt32 SS_ERROR_IPC_DISCONNECT_FAILED             = 0x02000007;  //  断开连接失败
        public const UInt32 SS_ERROR_IPC_BUILD_SESSION_KEY             = 0x02000008;  //  会话密钥协商失败
        public const UInt32 SS_ERROR_REQUEST_OUTPUT_BUFFER_TOO_LARGE   = 0x02000009;  //  请求的最大缓冲区过大
        public const UInt32 SS_ERROR_IPC_AUTH_ENCODE                   = 0x0200000A;  //  Auth encode错误
        public const UInt32 SS_ERROR_IPC_AUTH_DECODE                   = 0x0200000B;  //  Auth decode错误
        
        //============================================================
        //              Net Agent 模块 (0x11)
        //============================================================
        
        
        
        
        //============================================================
        //              安全模块 (0x12)
        //============================================================
        
        public const UInt32 SS_ERROR_INIT_ANTIDEBUG                    = 0x12000005;
        public const UInt32 SS_ERROR_DEBUG_FOUNDED                     = 0x12000006;
        
        
        
        //============================================================
        //              LM Service (0x24)
        //============================================================
        
        public const UInt32 ERROR_LM_SVC_UNINTIALIZED                  = 0x24000001;  //  未初始化 service中的表项
        public const UInt32 ERROR_LM_SVC_INITIALIZING                  = 0x24000002;  //  正在初始化service表
        public const UInt32 ERROR_LM_SVC_INVALID_SESSION_INFO_SIZE     = 0x24000003;  //  传入session 大小不正确
        public const UInt32 ERROR_LM_SVC_KEEP_ALIVE_FAILED             = 0x24000004;  //  未知的keep alive 操作失败原因 
        public const UInt32 ERROR_LM_SVC_LICENSE_NOT_FOUND             = 0x24000005;  //  缓存中没有找到对应许可
        public const UInt32 ERROR_LM_SVC_SESSION_ALREADY_LOGOUT        = 0x24000006;  //  session已经退出 
        public const UInt32 ERROR_LM_SVC_SESSION_ID_NOT_FOUND          = 0x24000007;  //  不存在该session id
        public const UInt32 ERROR_LM_SVC_DEBUGGED                      = 0x24000008;  //  发现被调试
        public const UInt32 ERROR_LM_SVC_INVALID_DESCRIPTION           = 0x24000009;  //  无效的许可描述信息
        public const UInt32 ERROR_LM_SVC_HANDLE_NOT_FOUND              = 0x2400000A;  //  找不到指定句柄
        public const UInt32 ERROR_LM_SVC_CACHE_OVERFLOW                = 0x2400000B;  //  cache 缓存已满
        public const UInt32 ERROR_LM_SVC_SESSION_OVERFLOW              = 0x2400000C;  //  session 缓存已满
        public const UInt32 ERROR_LM_SVC_INVALID_SESSION               = 0x2400000D;  //  无效的session
        public const UInt32 ERROR_LM_SVC_SESSION_ALREADY_DELETED       = 0x2400000E;  //  session 已经被删除
        public const UInt32 ERROR_LM_SVC_LICENCE_EXPIRED               = 0x2400000F;  //  许可已经过期
        public const UInt32 ERROR_LM_SVC_SESSION_TIME_OUT              = 0x24000010;  //  session超时
        public const UInt32 ERROR_LM_SVC_NOT_ENOUGH_BUFF               = 0x24000011;  //  缓冲区大小不足
        public const UInt32 ERROR_LM_SVC_DESC_NOT_FOUND                = 0x24000012;  //  没找到该设备描述
        public const UInt32 ERROR_LM_INVALID_PARAMETER                 = 0x24000013;  //  LM service参数错误
        public const UInt32 ERROR_LM_INVALID_LOCK_TYPE                 = 0x24000014;  //	锁类型不支持
        public const UInt32 ERROR_LM_REMOTE_LOGIN_DENIED               = 0x24000015;  //  许可不允许远程登录
        public const UInt32 ERROR_LM_SVC_SESSION_INVALID_AUTHCODE      = 0x24000016;  //  session认证失败
        public const UInt32 ERROR_LM_SVC_ACCOUNT_NOT_BOUND             = 0x24000017;  //  账户未绑定
        
        
        //============================================================
        //              LM Native (0x21)
        //============================================================
        
        public const UInt32 SS_ERROR_UNSUPPORTED_ALGORITHM             = 0x21000000;  //  不支持的算法类型
        public const UInt32 SS_ERROR_INVAILD_HLC_HANDLE                = 0x21000001;  //  无效的HLC句柄
        public const UInt32 SS_ERROR_HLC_CHECK                         = 0x21000002;  //  HLC检查失败
        public const UInt32 SS_ERROR_LM_CHECK_READ                     = 0x21000003;  //  读标志位检查失败
        public const UInt32 SS_ERROR_LM_CHECK_LICENSE                  = 0x21000004;  //  输出缓冲区许可ID不匹配
        public const UInt32 SS_ERROR_LM_CHECKSUM                       = 0x21000005;  //  输出缓冲区校验失败
        public const UInt32 SS_ERROR_HLC_BUFFER_LEN                    = 0x21000006;  //  HLC数据加密大于最大的缓冲区
        public const UInt32 SS_ERROR_L2CWF_LEN                         = 0x21000007;  //  无效的加密长度
        public const UInt32 SS_ERROR_INVAILD_MAX_ENCRYPT_LENGTH        = 0x21000008;  //  无效的加密长度
        public const UInt32 SS_ERROR_INVAILD_ENUM_CRYPT_TYPE           = 0x21000009;  //  不支持的加密类型
        public const UInt32 SS_ERROR_NATIVE_INSUFFICIENT_BUFFER        = 0x2100000A;  //  缓冲区不足
        public const UInt32 SS_ERROR_NATIVE_LIST_FILE_FAILED           = 0x2100000B;  //  枚举锁内文件错误
        public const UInt32 SS_ERROR_INVALID_C2H_REQUEST               = 0x2100000C;  //  无效的云锁到硬件锁请求
        
        //============================================================
        //              LM Firmware (0x22)
        //============================================================
        
        public const UInt32 SS_ERROR_FIRM_INVALID_FILE_NAME               = 0x22000001;  // 文件名称无效                                        
        public const UInt32 SS_ERROR_FIRM_CHECK_BUFF_FAILED               = 0x22000002;  // 数据校验失败
        public const UInt32 SS_ERROR_FIRM_INVALID_BUFF_LEN                = 0x22000003;  // 传入数据长度错误
        public const UInt32 SS_ERROR_FIRM_INVALID_PARAM                   = 0x22000004;  // 参数有误
        public const UInt32 SS_ERROR_FIRM_INVALID_SESSION_INFO            = 0x22000005;  // session 信息错误
        public const UInt32 SS_ERROR_FIRM_INVALID_FILE_SIZE               = 0x22000006;  // 创建文件长度出错
        public const UInt32 SS_ERROR_FIRM_WRITE_FILE_FAILED               = 0x22000007;  // 写入文件数据出错
        public const UInt32 SS_ERROR_FIRM_INVALID_LICENCE_HEADER          = 0x22000008;  // 许可信息头部错误
        public const UInt32 SS_ERROR_FIRM_INVALID_LICENCE_SIZE            = 0x22000009;  // 许可信息数据错误  
        public const UInt32 SS_ERROR_FIRM_INVALID_LICENCE_INDEX           = 0x2200000A;  // 超过支持最大许可序号                                     
        public const UInt32 SS_ERROR_FIRM_LIC_NOT_FOUND                   = 0x2200000B;  // 没有找到对应的许可
        public const UInt32 SS_ERROR_FIRM_MEM_STATUS_INVALID              = 0x2200000C;  // 内存状态数据未初始化
        public const UInt32 SS_ERROR_FIRM_INVALID_LIC_ID                  = 0x2200000D;  // 不可用的许可号
        public const UInt32 SS_ERROR_FIRM_LICENCE_ALL_DISABLED            = 0x2200000E;  // 所有许可被禁用
        public const UInt32 SS_ERROR_FIRM_CUR_LICENCE_DISABLED            = 0x2200000F;  // 当前许可被禁用
        public const UInt32 SS_ERROR_FIRM_LICENCE_INVALID                 = 0x22000010;  // 当前许可不可用
        public const UInt32 SS_ERROR_FIRM_LIC_STILL_UNAVALIABLE           = 0x22000011;  // 许可尚不可用
        public const UInt32 SS_ERROR_FIRM_LIC_TERMINATED                  = 0x22000012;  // 许可已经到期
        public const UInt32 SS_ERROR_FIRM_LIC_RUNTIME_TIME_OUT            = 0x22000013;  // 运行时间用尽
        public const UInt32 SS_ERROR_FIRM_LIC_COUNTER_IS_ZERO             = 0x22000014;  // 次数用尽
        public const UInt32 SS_ERROR_FIRM_LIC_MAX_CONNECTION              = 0x22000015;  // 已达到最大并发授权
        public const UInt32 SS_ERROR_FIRM_INVALID_LOGIN_COUNTER           = 0x22000016;  // 不正确的登录计数
        public const UInt32 SS_ERROR_FIRM_REACHED_MAX_SESSION             = 0x22000017;  // 锁内已经到达最大会话数量
        public const UInt32 SS_ERROR_FIRM_INVALID_TIME_INFO               = 0x22000018;  // 通讯时间信息出错
        public const UInt32 SS_ERROR_FIRM_SESSION_SIZE_DISMATCH           = 0x22000019;  // session 信息大小不匹配
        public const UInt32 SS_ERROR_FIRM_NOT_ENOUGH_SHAREMEMORY          = 0x2200001A;  // 没有足够的共享内存
        public const UInt32 SS_ERROR_FIRM_INVALID_OPCODE                  = 0x2200001B;  // 不可用的操作码
        public const UInt32 SS_ERROR_FIRM_INVALID_DATA_LEN                = 0x2200001C;  // 错误的数据文件长度    
        public const UInt32 SS_ERROR_FIRM_DATA_FILE_NOT_FOUND             = 0x2200001E;  // 找不到对应的许可数据文件
        public const UInt32 SS_ERROR_FIRM_INVALID_PKG_TYPE                = 0x2200001F;  // 远程升级包类型错误
        public const UInt32 SS_ERROR_FIRM_INVALID_TIME_STAMP              = 0x22000020;  // 时间戳错误的远程升级包
        public const UInt32 SS_ERROR_FIRM_INVALID_UPD_LIC_ID              = 0x22000021;  // 错误的远程升级许可序号
        public const UInt32 SS_ERROR_FIRM_LIC_ALREADY_EXIST               = 0x22000022;  // 添加的许可已经存在
        public const UInt32 SS_ERROR_FIRM_LICENCE_SIZE_LIMITTED           = 0x22000023;  // 许可数量受限
        public const UInt32 SS_ERROR_FIRM_INVALID_DATA_FILE_OFFSET        = 0x22000024;  // 无效的许可数据文件偏移
        public const UInt32 SS_ERROR_FIRM_ZERO_INDEX_LIC_DESTROY          = 0x22000025;  // 零号许可损坏
        public const UInt32 SS_ERROR_FIRM_LIC_ALREADY_DISABLED            = 0x22000026;  // 许可原已经被禁止
        public const UInt32 SS_ERROR_FIRM_INVALID_UPD_OPCODE              = 0x22000027;  // 无效的远程升级操作码
        public const UInt32 SS_ERROR_FIRM_LIC_ALREADY_ENABLED             = 0x22000028;  // 许可原已经有效
        public const UInt32 SS_ERROR_FIRM_INVALID_PKG_SIZE                = 0x22000029;  // 远程升级包长度不正确
        public const UInt32 SS_ERROR_FIRM_LIC_COUNT_RETURN                = 0x2200002A;  // 返回了错误的许可计数
        public const UInt32 SS_ERROR_FIRM_INVALID_OPERATION               = 0x2200002B;  // 执行了不正确的操作
        public const UInt32 SS_ERROR_FIRM_SESSION_ALREADY_LOGOUT          = 0x2200002C;  // session已经退出登录
        public const UInt32 SS_ERROR_FIRM_EXCHANGE_KEY_TIMEOUT            = 0x2200002D;  // 交换密钥超时
        public const UInt32 SS_ERROR_FIRM_INVALID_EXCHANGE_KEY_MAGIC      = 0x2200002E;  // 错误的交换密钥魔数
        public const UInt32 SS_ERROR_FIRM_INVALID_AUTH_CODE               = 0x2200002F;  // 认证数据错误
        public const UInt32 SS_ERROR_FIRM_CONVERT_INDEX_TO_FILE           = 0x22000030;  // 转换lic序号到文件名称失败
        public const UInt32 SS_ERROR_FIRM_INVALID_USER_DATA_TYPE          = 0x22000031;  // 用户自定义字段类型错误
        public const UInt32 SS_ERROR_FIRM_INVALID_DATA_FILE_SIZE          = 0x22000032;  // 用户自定义区域过大
        public const UInt32 SS_ERROR_FIRM_INVALID_CCRNT_OPR_TYPE          = 0x22000033;  // 错误的并发计数操作类型
        public const UInt32 SS_ERROR_FIRM_ALL_LIC_TERMINATED              = 0x22000034;  // 所有许可时间到期不可用
        public const UInt32 SS_ERROR_FIRM_INVALID_CCRNT_VALUE             = 0x22000035;  // 错误的并发计数
        public const UInt32 SS_ERROR_FIRM_INVALID_UPD_FILE                = 0x22000036;  // 不可用的删除历史记录文件
        public const UInt32 SS_ERROR_FIRM_UPD_RECORD_FULL                 = 0x22000037;  // 更新记录达到最大值
        public const UInt32 SS_ERROR_FIRM_UPDATE_FAILED                   = 0x22000038;  // 远程升级失败
        public const UInt32 SS_ERROR_FIRM_LICENSE_BEING_WRITTING          = 0x22000039;  // 该许可正在被写入
        public const UInt32 SS_ERROR_FIRM_INVALID_PKG_FIELD_TYPE          = 0x2200003A;  // 升级包子类型错误
        public const UInt32 SS_ERROR_FIRM_LOAT_FSM_SALT                   = 0x2200003B;  // 加载盐值文件出错
        public const UInt32 SS_ERROR_FIRM_DATA_LENGTH_ALIGNMENT           = 0x2200003C;  // 加解密数据长度不对齐
        public const UInt32 SS_ERROR_FIRM_DATA_CRYPTION                   = 0x2200003D;  // 加解密数据错误
        public const UInt32 SS_ERROR_FIRM_SHORTCODE_UPDATE_NOT_SUPPORTED  = 0x2200003E;  // 不支持短码升级
        public const UInt32 SS_ERROR_FIRM_INVALID_SHORTCODE               = 0x2200003F;  // 不可用的短码
        public const UInt32 SS_ERROR_FIRM_LIC_USR_DATA_NOT_EXIST          = 0x22000040;  // 用户自定义数据不存在
        public const UInt32 SS_ERROR_FIRM_RCD_FILE_NOT_INITIALIZED        = 0x22000041;  // 删除记录文件未初始化
        public const UInt32 SS_ERROR_FIRM_AUTH_FILE_NOT_FOUND             = 0x22000042;  // 认证文件找不到
        public const UInt32 SS_ERROR_FIRM_SESSION_OVERFLOW                = 0x22000043;  // session会话数溢出（在不限制并发时导致超过最大计数）
        public const UInt32 SS_ERROR_FIRM_TIME_OVERFLOW                   = 0x22000044;  // 时间信息溢出，可能由于锁外pc时间被恶意修改所致
        public const UInt32 SS_ERROR_FIRM_REACH_FILE_LIS_END              = 0x22000045;  // 枚举到达最后一个文件
        public const UInt32 SS_ERROR_FIRM_ANTI_MECHANISM_ACTIVED          = 0x22000046;  // 惩罚计数触发锁定lm
        public const UInt32 SS_ERROR_FIRM_NO_BLOCK                        = 0x22000047;  // 获取block出错
        public const UInt32 SS_ERROR_FIRM_NOT_ENDED                       = 0x22000048;  // 数据未传输完毕   (特殊错误码)
        public const UInt32 SS_ERROR_FIRM_LIC_ALREADY_ACTIVE              = 0x22000049;  // 许可已经激活
        public const UInt32 SS_ERROR_FIRM_FILE_NOT_FOUND                  = 0x22000050;  // 找不到文件
        public const UInt32 SS_ERROR_FIRM_UNKNOW_USER_DATA_TYPE           = 0x22000051;  // 未知的用户数据类型
        public const UInt32 SS_ERROR_FIRM_INVALID_TF_CODE                 = 0x22000052;  // 错误的转移操作码
        public const UInt32 SS_ERROR_FIRM_UNMATCH_GUID                    = 0x22000053;  // 不匹配的GUID 
        public const UInt32 SS_ERROR_FIRM_UNABLE_TRANSFER                 = 0x22000054;  // 许可不可进行转移
        public const UInt32 SS_ERROR_FIRM_INVALID_TRANSCODE               = 0x22000055;  // 不能识别的随机码
        public const UInt32 SS_ERROR_FIRM_ACCOUNT_NAME_NOT_FOUND          = 0x22000056;  // 用户名未找到
        public const UInt32 SS_ERROR_FIRM_ACCOUNT_ID_NOT_FOUND            = 0x22000057;  // 账户id未找到
        public const UInt32 SS_ERROR_FIRM_INVALID_XKEY_STEP               = 0x22000058;  // 错误的秘钥交换过程
        public const UInt32 SS_ERROR_FIRM_INVLAID_DEVELOPER_ID            = 0x22000059;  // 无效的开发商ID
        public const UInt32 SS_ERROR_FIRM_CA_TYPE                         = 0x2200005A;  // CA类型错误
        public const UInt32 SS_ERROR_FIRM_LIC_TRANSFER_FAILURE            = 0x2200005B;  // 许可转移失败
        public const UInt32 SS_ERROR_FIRM_TF_PACKAGE_VERSION              = 0x2200005C;  // 转移包版本号错误
        public const UInt32 SS_ERROR_FIRM_BEYOND_PKG_ITEM_SIZE            = 0x2200005D;  // 升级包许可数量过大
        public const UInt32 SS_ERROR_FIRM_UNBOUND_ACCOUNT_INFO            = 0x2200005E;  // 账户未绑定
        
        //============================================================
        //              MODE LIC TRANS 模块()0x28
        //============================================================
        public const UInt32 SS_ERROR_LIC_TRANS_NO_SN_DESC              = 0x28000001;  // 未找到锁描述信息
        public const UInt32 SS_ERROR_LIC_TRANS_INVALID_DATA            = 0x28000002;  // 数据格式错误
        
        //============================================================
        //              AUTH SERVER 模块 (0x29)
        //============================================================
        
        public const UInt32 SS_ERROR_AUTH_SERVER_INVALID_TOKEN          = 0x29000001;  //无效的token
        public const UInt32 SS_ERROR_AUTH_SERVER_REFRESH_TOKEN          = 0x29000002;  //刷新token失败
        public const UInt32 SS_ERROR_AUTH_SERVER_LOGIN_CANCELED         = 0x29000003;  //用户取消登陆
        public const UInt32 SS_ERROR_AUTH_SERVER_GET_ALL_USER_INFO_FAIL = 0x29000004;  //获取所有用户信息失败
        
        //============================================================
        //              Cloud 模块 (0x30)
        //============================================================
        
        public const UInt32 SS_CLOUD_OK                                = 0x30000000;  //  成功
        public const UInt32 SS_ERROR_CLOUD_INVALID_PARAMETER           = 0x30000001;  //  参数错误
        public const UInt32 SS_ERROR_CLOUD_QUERY_UESR_INFO             = 0x30000002;  //  查询用户信息失败
        public const UInt32 SS_ERROR_CLOUD_INVALID_LICENSE_SESSION     = 0x30000003;  //  许可未登录或已超时
        public const UInt32 SS_ERROR_CLOUD_DATA_EXPIRED                = 0x30000004;  //  数据已过期
        public const UInt32 SS_ERROR_CLOUD_VERIFY_TIMESTAMP_SIGNATURE  = 0x30000005;  //  时间戳签名验证失败
        public const UInt32 SS_ERROR_CLOUD_AUTH_FAILED                 = 0x30000006;  //  端到端认证失败
        public const UInt32 SS_ERROR_CLOUD_NOT_BOUND                   = 0x30000007;  //  算法不存在或未绑定
        public const UInt32 SS_ERROR_CLOUD_EXECUTE_FAILED              = 0x30000008;  //  算法执行失败
        public const UInt32 SS_ERROR_CLOUD_INVALID_TOKEN               = 0x30000010;  //  不合法的token
        public const UInt32 SS_ERROR_CLOUD_LICENSE_ALREADY_LOGIN       = 0x30000011;  //  许可已登陆
        public const UInt32 SS_ERROR_CLOUD_LICENSE_EXPIRED             = 0x30000012;  //  许可已到期
        public const UInt32 SS_ERROR_CLOUD_SESSION_KICKED              = 0x30000013;  //  许可已被其它电脑登录
        public const UInt32 SS_ERROR_CLOUD_INVALID_SESSSION            = 0x30001002;  //  无效的session
        public const UInt32 SS_ERROR_CLOUD_SESSION_TIMEOUT             = 0x30001004;  //  会话超时
        public const UInt32 SS_ERROR_CLOUD_PARSE_PARAM                 = 0x30001007;  //  参数解析错误
        public const UInt32 SS_ERROR_CLOUD_LICENSE_LOGIN_SUCCESS       = 0x31001000;  //  许可登录成功
        public const UInt32 SS_ERROR_CLOUD_LICENSE_NOT_EXISTS          = 0x31001001;  //  许可不存在
        public const UInt32 SS_ERROR_CLOUD_LICENSE_NOT_ACTIVE          = 0x31001002;  //  许可未激活
        public const UInt32 SS_ERROR_CLOUD_LICENSE_EXPIRED2            = 0x31001003;  //  许可已过期
        public const UInt32 SS_ERROR_CLOUD_LICENSE_COUNTER_IS_ZERO     = 0x31001004;  //  许可无使用次数
        public const UInt32 SS_ERROR_CLOUD_LICENSE_RUNTIME_TIME_OUT    = 0x31001005;  //  许可无使用时间
        public const UInt32 SS_ERROR_CLOUD_LICENSE_MAX_CONNECTION      = 0x31001006;  //  许可并发量限制
        public const UInt32 SS_ERROR_CLOUD_LICENSE_LOCKED              = 0x31001007;  //  许可被锁定
        public const UInt32 SS_ERROR_CLOUD_LICENSE_DATA_NOT_EXISTS     = 0x31001008;  //  许可数据不存在
        public const UInt32 SS_ERROR_CLOUD_LICENSE_STILL_UNAVAILABLE   = 0x31001010;  //  许可未到开始使用时间
        public const UInt32 SS_ERROR_CLOUD_ZERO_LICENSE_NOT_EXISTS     = 0x31001011;  //  0号许可不存在
        public const UInt32 SS_ERROR_CLOUD_VERIFY_LICENSE              = 0x31001012;  //  许可验证失败
        public const UInt32 SS_ERROR_CLOUD_EXECUTE_FILE_NOT_EXISTS     = 0x31002000;  //  算法不存在
        public const UInt32 SS_ERROR_CLOUD_LICENSE_NOT_BOUND           = 0x31003001;  //  算法未绑定

        private const int H5ErrorFlag = 0x01000000;     
        /// <summary>
        /// 错误码分组对比
        /// </summary>
        /// <param name="resultCode">返回错误码</param>
        /// <param name="expectCode">期望错误码</param>
        /// <returns>错误码相同返回true，否则返回false</returns>
        public static bool Compare(UInt32 returnCode, UInt32 expectCode)
        {
            if ((returnCode & H5ErrorFlag) != 0)
            {//说明该错误码为H5错误码
                return expectCode == (0x00FFFFFF & returnCode);
            }
            else
            {
                return expectCode == returnCode;
            }
        }
    }
}
