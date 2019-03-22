unit slm_runtime_easy;

{$IFDEF CompilerVersion > 14.5}
  // Delphi 6默认未处理FPC，故使用版本后区分，简化编译条件。 shijc 2016-8-8
  // 备注：原文无版本判断
  {$IFDEF FPC}
    {$WARNINGS OFF}
    {$HINTS OFF}
  {$ELSE FPC}
    {$WARN UNSAFE_CODE OFF}
    {$WARN UNSAFE_TYPE OFF}
    {$WARN UNSAFE_CAST OFF}
  {$ENDIF FPC}
{$ENDIF}


interface

uses
{$IF CompilerVersion >= 23}
  // XE2 及以上版本引用头文件
  Winapi.Windows, System.SysUtils, System.Classes,
  BTMemory_x86_64;
{$ELSE}
  // XE以下版本（包括Delphi7）
  windows, SysUtils, Classes,
  BTMemory_x86;
{$IFEND}

type
  DWORD       = LongWord;
  SLM_HANDLE_INDEX = LongWord;
  SS_CHAR     = ANSICHAR;
  SS_INT8     = BYTE;
  SS_BYTE     = BYTE;

  SS_INT16    = SMALLINT;
  SS_UINT16   = WORD;
  SS_INT32    = LONGINT;
  SS_UINT32   = LONGWORD;

// 不使用枚举定义声明，编译器会固定枚举变量的字节长度为1，而 slm_runtime_easy.dll 传参为4字节（整数）。
// 当枚举传参前进行任何代码操作，都可能影响枚举所在寄存器高字节内容（由编译器决定），
// 导致传参时强制转换为4字节时数据并非期望值，导致返回参数错误（2）。
// 故此处使用Integer类型代替枚举定义。shijc
type
  INFO_TYPE = SS_INT32;
  INFO_FORMAT_TYPE = SS_INT32;
  LIC_USER_DATA_TYPE = SS_INT32;

const
  LOCK_INFO = 1;
  SESSION_INFO = 2;
  LICENSE_INFO = 3;
  FILE_LIST = 4;

  JSON = 2;
  STRUCT = 3;
  STRING_KV = 4;
  CIPHER = 5;

  ROM = 0;
  RAW = 1;
  PUB = 2;

 {$A1+}
type
  ST_LOGIN_PARAM = packed record
    size : DWORD;
	  license_id : DWORD;
    timeout : DWORD;
    login_mode : DWORD;
    login_flag : DWORD;
    sn : array[1..16] of BYTE;
    server : array[1..32] of BYTE;
    access_token : array[1..64] of BYTE;
    cloud_server : array[1..100] of BYTE;
    snippet_seed : array[1..32] of BYTE;
    user_guid : array[1..128] of BYTE;

end;

var
   //argument for Functions
   slm_get_last_error         : function(): DWORD; stdcall;
   app_msg_get_last_status    : function() : DWORD; stdcall;
   app_msg_get_last_info      : function() : PChar; stdcall;
   slm_init_easy              : function(pAPIPasswor : PANSICHAR): DWORD; stdcall;
   slm_cleanup_easy           : function() : DWORD; stdcall;
   slm_is_debug_easy          : function() : DWORD; stdcall;
   slm_login_easy             : function(pLoginParam : PANSICHAR; FormatType : INFO_FORMAT_TYPE) : SLM_HANDLE_INDEX; stdcall;
   slm_logout_easy            : function(Handle : SLM_HANDLE_INDEX) : DWORD; stdcall;
   slm_find_license_easy      : function(LicenseID : DWORD; FormatType : INFO_FORMAT_TYPE) : Pointer; stdcall;
   slm_get_info_easy          : function(Handle : SLM_HANDLE_INDEX; InfoType : INFO_TYPE; FormatType : INFO_FORMAT_TYPE) : Pointer; stdcall;
   slm_encrypt_easy           : function(Handle : SLM_HANDLE_INDEX; pInBuff : PBYTE; pOutBuff : PBYTE; Len : DWORD) : DWORD; stdcall;
   slm_decrypt_easy           : function(Handle : SLM_HANDLE_INDEX; pInBuff : PBYTE; pOutBuff : PBYTE; Len : DWORD) : DWORD; stdcall;
   slm_user_data_getsize_easy : function(Handle : SLM_HANDLE_INDEX; DataType : LIC_USER_DATA_TYPE) : DWORD; stdcall;
   slm_user_data_read_easy    : function(Handle : SLM_HANDLE_INDEX; DataType : LIC_USER_DATA_TYPE; pReadBuff : PBYTE; Offset : DWORD; Len : DWORD) : DWORD; stdcall;
   slm_user_data_write_easy   : function(Handle : SLM_HANDLE_INDEX; pWriteBuff : PBYTE; Offset : DWORD; Len : DWORD) : DWORD; stdcall;
   slm_mem_alloc_easy         : function(Handle : SLM_HANDLE_INDEX; Size : DWORD) : DWORD; stdcall;
   slm_mem_free_easy          : function(Handle : SLM_HANDLE_INDEX; MemoryID : DWORD) : DWORD; stdcall;
   slm_mem_read_easy          : function(Handle : SLM_HANDLE_INDEX; MemoryID : DWORD; Offset : DWORD; Len : DWORD; pReadBuff : PBYTE; var pReadLen : DWORD) : DWORD; stdcall;
   slm_mem_write_easy         : function(Handle : SLM_HANDLE_INDEX; MemoryID : DWORD; Offset : DWORD; Len : DWORD; pWriteBuff : PBYTE) : DWORD; stdcall;
   slm_execute_dynamic_easy   : function(Handle : SLM_HANDLE_INDEX; ExfBuff : PANSICHAR; ExfSize : DWORD; pInBuff : PBYTE; InSize : DWORD; pOutBuff : PBYTE; OutSize : DWORD; var pRetSize : DWORD) : DWORD; stdcall;
   slm_execute_static_easy    : function(Handle : SLM_HANDLE_INDEX; ExfName : PANSICHAR; pInBuff : PBYTE; InSize : DWORD; pOutBuff : PBYTE; OutSize : DWORD; var pRetSize : DWORD) : DWORD; stdcall;
   slm_led_control_easy       : function(Handle : SLM_HANDLE_INDEX; Color : DWORD; State : DWORD; Interval : DWORD) : DWORD; stdcall;
   slm_get_version_easy       : function(var pApiVersion : DWORD; var pSSVersion : DWORD) : DWORD; stdcall;
   slm_error_format_easy      : function(ErrorCode : DWORD; Language : DWORD) : PAnsiChar; stdcall;
   slm_update_easy            : function(D2CPkg : PANSICHAR) : Pointer; stdcall;
   slm_update_ex_easy         : function(szLockSN : Pointer; D2CPkg : PANSICHAR) : Pointer; stdcall;
   slm_get_cloud_token_easy   : function() : PAnsiChar; stdcall;
   slm_keep_alive_easy        : function(Handle : SLM_HANDLE_INDEX) : DWORD; stdcall;
   slm_get_device_cert_easy   : function(Handle : SLM_HANDLE_INDEX; pDeviceCert : PBYTE; BuffSize : DWORD; var pRetSize : DWORD) : DWORD; stdcall;
   slm_sign_by_device_easy    : function(Handle : SLM_HANDLE_INDEX; pSignData : PBYTE; SignDataSize : DWORD; pSigned : PBYTE; MaxSize : DWORD; var RetSize : DWORD) : DWORD; stdcall;
   slm_free_easy              : procedure(Buffer : Pointer) ; stdcall;
   slm_check_module_easy      : function(Handle : SLM_HANDLE_INDEX; ModuleID : DWORD) : DWORD; stdcall;
   slm_get_developer_id_easy  : function(szDeveloperID : AnsiString) : DWORD; stdcall;
   slm_update_file_easy       : function(szLockSN : Pointer; D2CFile : PANSICHAR) : Pointer; stdcall;
   slm_execute_easy           : function(Handle : SLM_HANDLE_INDEX; CodeType : DWORD; pCodeBuffer : PBYTE; CodeSize : DWORD; pInput : PBYTE; InputSize : DWORD; pOutput : PBYTE; Output_size : DWORD; var pRetSize : DWORD) : DWORD; stdcall;
   slm_pub_data_getsize_easy  : function(Handle : SLM_HANDLE_INDEX; LicenseID : DWORD) : DWORD; stdcall;
   slm_pub_data_read_easy     : function(Handle : SLM_HANDLE_INDEX; LicenseID : DWORD; pReadBuffer : PBYTE; Offset : DWORD; Len : DWORD) :DWORD; stdcall;
   slm_enum_device_easy       : function() : Pointer; stdcall;
   slm_d2c_update_inside_easy : function(szLockSn : Pointer; szInsideFile : Pointer) : Pointer; stdcall;
   slm_enum_license_id_easy   : function(szDeviceInfo : Pointer) : Pointer; stdcall;
   slm_get_license_info_easy  : function(szDeviceInfo : Pointer; LicenseID : DWORD) : Pointer; stdcall;
   slm_license_sign_easy      : function(Handle : SLM_HANDLE_INDEX; pSignData : PBYTE; SignDataSize : DWORD; pSignature : PBYTE; MaxBufSize : DWORD) : DWORD; stdcall;
   slm_license_verify_easy    : function(pSignData : PBYTE; SignDataSize : DWORD; pSignature : PBYTE; SignatureLength : DWORD) : DWORD; stdcall;
   
implementation

resourcestring
  rsErrLoadMemLibary    = '加载服务库失败';

// 引用将 dll 转换为二进制数组的inc文件
// 安全升级方案：避免静态分析替换dll内容
//       可对 dll 转换后的二进制数据进行加密处理，然后在使用期间解密。
//       软件开发者可根据实际情况酌情考虑此方案。
const
{$I slm_runtime_easy.inc}

var
{$IF CompilerVersion >= 23}
  LibModule: TMemoryModule;
{$ELSE}
  LibModule: TBTMemoryModule;
{$IFEND}
  
  LibInited: Boolean;

procedure Init;
  function GetFuncAddress(const AProcName: PChar): Pointer;
  begin
  {$IF CompilerVersion >= 23}
    Result := MemoryGetProcAddress(LibModule, PAnsiChar(AProcName));
  {$ELSE}
    Result := BTMemoryGetProcAddress(LibModule, PChar(AProcName));
  {$IFEND}
  end;
begin
  if LibInited then
    Exit;

{$IF CompilerVersion >= 23}
  LibModule := MemoryLoadLibary(@data);
{$ELSE}
  LibModule := BTMemoryLoadLibary(@data, Length(data));
{$IFEND}
  if LibModule = nil then
    Exit;

  @slm_get_last_error         :=  GetFuncAddress(PChar(1));
  @app_msg_get_last_status    :=  GetFuncAddress(PChar(2));
  @app_msg_get_last_info      :=  GetFuncAddress(PChar(3));
  @slm_init_easy              :=  GetFuncAddress(PChar(4));
  @slm_cleanup_easy           :=  GetFuncAddress(PChar(5));
  @slm_is_debug_easy          :=  GetFuncAddress(PChar(6));
  @slm_login_easy             :=  GetFuncAddress(PChar(7));
  @slm_logout_easy            :=  GetFuncAddress(PChar(8));
  @slm_find_license_easy      :=  GetFuncAddress(PChar(9));
  @slm_get_info_easy          :=  GetFuncAddress(PChar(10));
  @slm_encrypt_easy           :=  GetFuncAddress(PChar(11));
  @slm_decrypt_easy           :=  GetFuncAddress(PChar(12));
  @slm_user_data_getsize_easy :=  GetFuncAddress(PChar(13));
  @slm_user_data_read_easy    :=  GetFuncAddress(PChar(14));
  @slm_user_data_write_easy   :=  GetFuncAddress(PChar(15));
  @slm_mem_alloc_easy         :=  GetFuncAddress(PChar(16));
  @slm_mem_free_easy          :=  GetFuncAddress(PChar(17));
  @slm_mem_read_easy          :=  GetFuncAddress(PChar(18));
  @slm_mem_write_easy         :=  GetFuncAddress(PChar(19));
  @slm_execute_dynamic_easy   :=  GetFuncAddress(PChar(20));
  @slm_execute_static_easy    :=  GetFuncAddress(PChar(21));
  @slm_led_control_easy       :=  GetFuncAddress(PChar(22));
  @slm_get_version_easy       :=  GetFuncAddress(PChar(23));
  @slm_error_format_easy      :=  GetFuncAddress(PChar(24));
  @slm_update_easy            :=  GetFuncAddress(PChar(25));
  @slm_update_ex_easy         :=  GetFuncAddress(PChar(26));
  @slm_get_cloud_token_easy   :=  GetFuncAddress(PChar(27));
  @slm_keep_alive_easy        :=  GetFuncAddress(PChar(28));
  @slm_get_device_cert_easy   :=  GetFuncAddress(PChar(29));
  @slm_sign_by_device_easy    :=  GetFuncAddress(PChar(30));
  @slm_free_easy              :=  GetFuncAddress(PChar(31));
  @slm_check_module_easy      :=  GetFuncAddress(PChar(32));
  @slm_get_developer_id_easy  :=  GetFuncAddress(PChar(33));
  @slm_update_file_easy       :=  GetFuncAddress(PChar(34));
  @slm_execute_easy           :=  GetFuncAddress(PChar(35));
  @slm_pub_data_getsize_easy  :=  GetFuncAddress(PChar(36));
  @slm_pub_data_read_easy     :=  GetFuncAddress(PChar(37));
  @slm_enum_device_easy       :=  GetFuncAddress(PChar(38));
  @slm_d2c_update_inside_easy :=  GetFuncAddress(PChar(39));
  @slm_enum_license_id_easy   :=  GetFuncAddress(PChar(40));
  @slm_get_license_info_easy  :=  GetFuncAddress(PChar(41));
  @slm_license_sign_easy      :=  GetFuncAddress(PChar(42));
  @slm_license_verify_easy    :=  GetFuncAddress(PChar(43));

  LibInited := True;
end;

procedure Done;
begin
  if not LibInited then
    Exit;
     //TODO Pointer for Function
    @slm_init_easy := nil;

  if LibModule <> nil then
  {$IF CompilerVersion >= 23}
    MemoryFreeLibrary(LibModule);
  {$ELSE}
    BTMemoryFreeLibrary(LibModule);
  {$IFEND}
  
  LibModule := nil;

  LibInited := False;
end;

////////////////////////////////////////////////////////////////////////////////
//功能：初始化、析构
//参数：
////////////////////////////////////////////////////////////////////////////////
initialization
  Init;

finalization
  Done;

end.
