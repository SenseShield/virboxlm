package com.senseyun.openapi.SSRuntimeEasyJava;

public class ST_LOGIN_PARAM {
  
    public int       size;  ///** 结构体大小（必填）*/
 
    public int       license_id;   ///** 要登录的许可ID*/
  
    public int       timeout;  ///** 许可会话的超时时间（单位：秒）,填0则使用默认值：600秒   */
  
    public int       login_mode;  ///** 许可登录的模式：本地，远程，云（见LOGIN_MODE_XXX)，如果填0，则使用SLM_LOGIN_MODE_AUTO*/
  
    public int       login_flag;  ///** 许可登录的标志：见SLM_LOGIN_FLAG_XXX */
  
    public byte[]         sn = new byte[SSDefines.SLM_LOCK_SN_LENGTH];  ///** 许可登录指定的锁唯一序列号（可选）*/
  
    public char[]         server = new char[SSDefines.SLM_MAX_SERVER_NAME];  ///** 网络锁服务器地址（可选），仅识别IP地址 */
  
    public char[]         access_token = new char[SSDefines.SLM_MAX_ACCESS_TOKEN_LENGTH];  ///** 云锁用户token（可选）*/
   
    public char[]         cloud_server = new char[SSDefines.SLM_MAX_CLOUD_SERVER_LENGTH]; ///** 云锁服务器地址（可选）*/
    
    public byte[]         snippet_seed = new byte[SSDefines.SLM_SNIPPET_SEED_LENGTH];///** 碎片代码种子（可选），如果要支持碎片代码,login_flag需要指定为SLM_LOGIN_FLAG_SNIPPET*/
    
    public byte[]         user_guid = new byte[SSDefines.SLM_CLOUD_MAX_USER_GUID_SIZE];///** 已登录用户的唯一ID（可选） */
    
    public int getSize()
    {
   	
    	return (4 * 5  + SSDefines.SLM_LOCK_SN_LENGTH + SSDefines.SLM_MAX_SERVER_NAME + SSDefines.SLM_MAX_ACCESS_TOKEN_LENGTH +
    			SSDefines.SLM_MAX_CLOUD_SERVER_LENGTH + SSDefines.SLM_SNIPPET_SEED_LENGTH + SSDefines.SLM_CLOUD_MAX_USER_GUID_SIZE);
    }
}
