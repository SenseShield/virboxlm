package com.senseyun.openapi.SSRuntimeEasyJava;

public class ST_LOGIN_PARAM {
  
    public int       size;  ///** �ṹ���С�����*/
 
    public int       license_id;   ///** Ҫ��¼�����ID*/
  
    public int       timeout;  ///** ��ɻỰ�ĳ�ʱʱ�䣨��λ���룩,��0��ʹ��Ĭ��ֵ��600��   */
  
    public int       login_mode;  ///** ��ɵ�¼��ģʽ�����أ�Զ�̣��ƣ���LOGIN_MODE_XXX)�������0����ʹ��SLM_LOGIN_MODE_AUTO*/
  
    public int       login_flag;  ///** ��ɵ�¼�ı�־����SLM_LOGIN_FLAG_XXX */
  
    public byte[]         sn = new byte[SSDefines.SLM_LOCK_SN_LENGTH];  ///** ��ɵ�¼ָ������Ψһ���кţ���ѡ��*/
  
    public char[]         server = new char[SSDefines.SLM_MAX_SERVER_NAME];  ///** ��������������ַ����ѡ������ʶ��IP��ַ */
  
    public char[]         access_token = new char[SSDefines.SLM_MAX_ACCESS_TOKEN_LENGTH];  ///** �����û�token����ѡ��*/
   
    public char[]         cloud_server = new char[SSDefines.SLM_MAX_CLOUD_SERVER_LENGTH]; ///** ������������ַ����ѡ��*/
    
    public byte[]         snippet_seed = new byte[SSDefines.SLM_SNIPPET_SEED_LENGTH];///** ��Ƭ�������ӣ���ѡ�������Ҫ֧����Ƭ����,login_flag��Ҫָ��ΪSLM_LOGIN_FLAG_SNIPPET*/
    
    public byte[]         user_guid = new byte[SSDefines.SLM_CLOUD_MAX_USER_GUID_SIZE];///** �ѵ�¼�û���ΨһID����ѡ�� */
    
    public int getSize()
    {
   	
    	return (4 * 5  + SSDefines.SLM_LOCK_SN_LENGTH + SSDefines.SLM_MAX_SERVER_NAME + SSDefines.SLM_MAX_ACCESS_TOKEN_LENGTH +
    			SSDefines.SLM_MAX_CLOUD_SERVER_LENGTH + SSDefines.SLM_SNIPPET_SEED_LENGTH + SSDefines.SLM_CLOUD_MAX_USER_GUID_SIZE);
    }
}
