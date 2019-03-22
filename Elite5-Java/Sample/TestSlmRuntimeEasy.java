import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;

import com.senseyun.openapi.SSRuntimeEasyJava.ErrorCode;
import com.senseyun.openapi.SSRuntimeEasyJava.INFO_FORMAT_TYPE;
import com.senseyun.openapi.SSRuntimeEasyJava.INFO_TYPE;
import com.senseyun.openapi.SSRuntimeEasyJava.LIC_USER_DATA_TYPE;
import com.senseyun.openapi.SSRuntimeEasyJava.SSDefines;
import com.senseyun.openapi.SSRuntimeEasyJava.SSRuntimeEasy;
import com.senseyun.openapi.SSRuntimeEasyJava.ST_LOGIN_PARAM;



public class TestSlmRuntimeEasy {

	String FileName = "test.d2c";
	public static void main(String arg[])
	{
		//test_all(arg);
		test_update();
	}
    
    private static byte charToByte(char c)
	{
		return (byte) "0123456789ABCDEF".indexOf(c);
	}
	
    // ��ʮ����������ת��Ϊʮ�������ַ���������0x12 --> "12"
	public static String hexByteToString(byte[] src)
	{
		StringBuffer stringBuffer = new StringBuffer();
		if(src == null || src.length <= 0)
		{
			return null;
		}
		
		for (int i = 0; i < src.length; i++)
		{
			int v = src[i] & 0xff;
			String hv = Integer.toHexString(v);
			if (hv.length() < 2)
			{
				stringBuffer.append(0);
			}
			stringBuffer.append(hv);
		}
		return stringBuffer.toString();
	}
	
    // ��ʮ�������ַ���ת��Ϊʮ���������飬����"12" --> 0x12
	public static byte[] stringToHexByte(String hexString)
	{
		if(hexString == null)
			return null;
		
		hexString = hexString.toUpperCase();
		int length = hexString.length() / 2;
		char[] hexChars = hexString.toCharArray();
		byte[] dst = new byte[length];
		
		for(int i = 0; i < length; i ++)
		{
			int pos = i * 2;
			dst[i] = (byte)((charToByte(hexChars[pos]) << 4) | (charToByte(hexChars[pos + 1])));
		}
		return dst;
	}
	
	public static void test_update()
	{
		long ret = 0;
		//test SlmUpdateEasy SlmGetLastError SlmFreeEasy SlmErrorFormatEasy
		Object MsgUpd = new Object();
		String str = "";
		File file = new File("10010.d2c");
		try
		{
			FileInputStream in = new FileInputStream(file);
			
			int size = in.available();
			byte[] buffer = new byte[size];
			in.read(buffer);
			in.close();
			
			str = new String(buffer, "US-ASCII");
		}
		catch(IOException e)
		{
			e.printStackTrace();
		}
		
		MsgUpd = SSRuntimeEasy.SlmUpdateEasy(str);
		ret = SSRuntimeEasy.SlmGetLastError();
		if(ret == ErrorCode.SS_OK)
		{
			System.out.println("SlmUpdateEasy success!");
		    System.out.println("[Update Msg]:" + MsgUpd);
		    SSRuntimeEasy.SlmFreeEasy(MsgUpd);
		}
		else
		{
			System.out.printf("SlmUpdateEasy failure : 0x%08X!  %s\n", ret, SSRuntimeEasy.SlmErrorFormatEasy(ret, 2));
		}
		
	}
	
	public static void test_all(String arg[])
	{
		String APIPsd = "DB3B838B2E4F08F5C9EFCD1A5DD16341";    //03...09
		long ret = 0;
		
		// ��ʼ����������ʹ����˼RuntimeAPI�����ӿ�֮ǰ��������ó�ʼ������
		ret = SSRuntimeEasy.SlmInitEasy(APIPsd);
		if(ret == ErrorCode.SS_OK)
		{
			System.out.println("SlmInitEasy success !");
		}
		else
		{
			System.out.printf("SlmInitEasy failure : 0x%08X!  %s\n", ret, SSRuntimeEasy.SlmErrorFormatEasy(ret, 2));
		}
		
		// �˽ӿ����ڻ�ȡ������ID������ͨ���˽ӿ��жϿ������Ƿ����Լ���һ��
	    String[] DeveloperID = new String[1];
		ret = SSRuntimeEasy.SlmGetDeveloperIDEasy(DeveloperID);
		System.out.println("[Developer ID]:" + DeveloperID[0]);
		
		// �˽ӿڿ��Ի�ȡ����ǰ��İ汾��Virbox��ɷ���İ汾
		long[] api = new long[1];
		long[] ss = new long[1];
		ret = SSRuntimeEasy.SlmGetVersionEasy(api, ss);
		if(ret == ErrorCode.SS_OK)
		{
			System.out.println("SlmGetVersionEasy success!");
			System.out.printf("[API VERSION]:0x%08X\n[ SS VERSION]:0x%08X\n", api[0], ss[0]);
		}
		else
		{
			System.out.printf("SlmGetVersionEasy failure : 0x%08X!  %s\n", ret, SSRuntimeEasy.SlmErrorFormatEasy(ret, 2));
		}
		
		// ��ȡ�����Ϣ���˽ӿڿ��Ի�ȡָ����ɵ���ϸ��Ϣ
		Object findLic = new Object();
		findLic = SSRuntimeEasy.SlmFindLicenseEasy(1, INFO_FORMAT_TYPE.JSON.get());
		ret = SSRuntimeEasy.SlmGetLastError();
		if(ret == ErrorCode.SS_OK)
		{
			System.out.println("SlmFindLicenseEasy success!");
			System.out.println("[FindLicense]:" + findLic);
			SSRuntimeEasy.SlmFreeEasy(findLic);
		}
		else
		{
			System.out.printf("SlmFindLicenseEasy failure : 0x%08X!  %s\n", ret, SSRuntimeEasy.SlmErrorFormatEasy(ret, 2));
		}
		
		// ��¼��ɣ��˹���ʮ����Ҫ��������ܹ��������õ�����Ϊ��Ҫ�Ľӿڣ�
        // ֻ�е�¼����ɣ����ܷ��ʼ������е���Ҫ����
		long Handle = 0;
		
		//json Ϊȷ����ȫ��SDK�汾��2.1.0.15128֮�����ɵ�¼������֧��json��¼�ķ�����
		String Param = "{\"license_id\":0}";
		
		//struct
		ST_LOGIN_PARAM stLogin = new ST_LOGIN_PARAM();
		stLogin.size = stLogin.getSize();
		stLogin.license_id = 0;
		//stLogin.user_guid = stringToHexByte("123456");  //(��ѡ)
		//Handle = SSRuntimeEasy.SlmLoginEasy(Param, INFO_FORMAT_TYPE.JSON.get());     //login by json  ����֧�֣�
		Handle = SSRuntimeEasy.SlmLoginEasy(stLogin, INFO_FORMAT_TYPE.STRUCT.get());   //login by struct
		ret = SSRuntimeEasy.SlmGetLastError();
		if(ret == ErrorCode.SS_OK && Handle != 0)
		{
			System.out.println("SlmLoginEasy success!");
			System.out.printf("[SLM Handle]: %d\n", Handle);
		}
		else
		{
			System.out.printf("SlmLoginEasy failure : ErrorCode = 0x%08X!   SLM Handle = %d\n", ret, Handle);
		}
		
		// �������ֲ���
		ret = SSRuntimeEasy.SlmKeepAliveEasy(Handle);
		if(ret == ErrorCode.SS_OK)
		{
			System.out.println("SlmKeepAliveEasy success!");
		}
		else
		{
			System.out.printf("SlmKeepAliveEasy failure : ErrorCode = 0x%08X!  %s\n", ret, SSRuntimeEasy.SlmErrorFormatEasy(ret, 2));
		}
		
		// ����INFO_TYPE���Ի�ȡ��ǰ��¼��ɵ���ϸ��Ϣ���Ự����ϸ��Ϣ������������ϸ��Ϣ��
		Object lic_info = new Object();
		lic_info = SSRuntimeEasy.SlmGetInfoEasy(Handle, INFO_TYPE.LICENSE_INFO.get(), INFO_FORMAT_TYPE.JSON.get());
		ret = SSRuntimeEasy.SlmGetLastError();
		if(ret == ErrorCode.SS_OK)
		{
			System.out.println("SlmGetInfoEasy [LICENSE_INFO] success!");
			System.out.println("[LICENSE INFO]:" + lic_info);
			SSRuntimeEasy.SlmFreeEasy(lic_info);
		}
		else
		{
			System.out.printf("SlmGetInfoEasy [LICENSE_INFO]  failure : ErrorCode = 0x%08X!  %s\n", ret, SSRuntimeEasy.SlmErrorFormatEasy(ret, 2));
		}
		
		// ���Կ��Ƽ�����LED�ƹ���ɫ������˸Ƶ��
		ret = SSRuntimeEasy.SlmLedControlEasy(Handle, SSDefines.LED_COLOR_BLUE, SSDefines.LED_STATE_SHRINK, 500);
		if(ret == ErrorCode.SS_OK)
		{
			System.out.println("SlmLedControlEasy success!");
		}
		else
		{
			System.out.printf("SlmLedControlEasy  failure : ErrorCode = 0x%08X!  %s\n", ret, SSRuntimeEasy.SlmErrorFormatEasy(ret, 2));
		}
		
		// ʹ����ɶ����ݽ��м��ܽ��ܣ��˲���������������Ȱ󶨵�һ����������ȫ�Լ��ߡ�
		String data = "test data.......";
		System.out.println("[Original Data]:" + data);
		
		byte[] EncData = new byte[16];
		ret = SSRuntimeEasy.SlmEncryptEasy(Handle, data.getBytes(), EncData, 16);
		if(ret == ErrorCode.SS_OK)
		{
			System.out.println("SlmEncryptEasy success!");
			String Msg = new String(EncData);
			System.out.println("[Encrypt]:"+ Msg);
		}
		else
		{
			System.out.printf("SlmEncryptEasy failure : 0x%08X!  %s\n", ret, SSRuntimeEasy.SlmErrorFormatEasy(ret, 2));
		}
		
		byte[] DecData = new byte[16];
		ret = SSRuntimeEasy.SlmDecryptEasy(Handle, EncData, DecData, 16);
		if(ret == ErrorCode.SS_OK)
		{
			System.out.println("SlmDecryptEasy success!");
		    String Msg = new String(DecData);
		    System.out.println("[Decrypt]:" + Msg);
		}
		else
		{
			System.out.printf("SlmDecryptEasy failure : 0x%08X!  %s\n", ret, SSRuntimeEasy.SlmErrorFormatEasy(ret, 2));
		}
		
		// ʹ���ڴ��йܵĹ��̣���������ڴ氲ȫ�Ĵ������ɷ����н��й���
		long MemID = 0;
		MemID = SSRuntimeEasy.SlmMemAllocEasy(Handle, 16);
		ret = SSRuntimeEasy.SlmGetLastError();
		if(ret == ErrorCode.SS_OK)
		{
			System.out.println("SlmMemAllocEasy success!");
		}
		else
		{
			System.out.printf("SlmMemAllocEasy failure : 0x%08X!  %s\n", ret, SSRuntimeEasy.SlmErrorFormatEasy(ret, 2));
		}
		
		System.out.println("[Write Data]:" + data);
		long write = SSRuntimeEasy.SlmMemWriteEasy(Handle, MemID, 0, 16, data.getBytes());
		ret = SSRuntimeEasy.SlmGetLastError();
		if(ret == ErrorCode.SS_OK)
		{
			System.out.println("SlmMemWriteEasy success!");
		}
		else
		{
			System.out.printf("SlmMemWriteEasy failure : 0x%08X!  %s\n", ret, SSRuntimeEasy.SlmErrorFormatEasy(ret, 2));
		}
		
		byte[] ReadBuffer = new byte[16];
		long read = SSRuntimeEasy.SlmMemReadEasy(Handle, MemID, 0, 16, ReadBuffer);
		ret = SSRuntimeEasy.SlmGetLastError();
		if(ret == ErrorCode.SS_OK)
		{
			System.out.println("SlmMemReadEasy success!");
		    String Msg = new String(ReadBuffer);
		    System.out.println("[Read Data]:" + Msg);
		}
		else
		{
			System.out.printf("SlmMemReadEasy failure : 0x%08X!  %s\n", ret, SSRuntimeEasy.SlmErrorFormatEasy(ret, 2));
		}
		
		ret = SSRuntimeEasy.SlmMemFreeEasy(Handle, MemID);
		if(ret == ErrorCode.SS_OK)
		{
			System.out.println("SlmMemFreeEasy success!");
		}
		else
		{
			System.out.printf("SlmMemFreeEasy failure : 0x%08X!  %s\n", ret, SSRuntimeEasy.SlmErrorFormatEasy(ret, 2));
		}
		
		// ��ȡ�豸֤��
		byte[] DeviceCert = new byte[2048];
		long[] RetLen = new long[1];
		ret = SSRuntimeEasy.SlmGetDeviceCertEasy(Handle, DeviceCert, 2048, RetLen);
		if(ret == ErrorCode.SS_OK)
		{
			System.out.println("SlmGetDeviceCertEasy success!");
			System.out.println("[Return Len]:" + RetLen[0]);
		}
		else
		{
			System.out.printf("SlmGetDeviceCertEasy failure : 0x%08X!  %s\n", ret, SSRuntimeEasy.SlmErrorFormatEasy(ret, 2));
		}
		
		// ִ�ж�̬���룬������ֲ��ز���
		byte[] ExfBuffer = new byte[1];
		long ExfSize = 1;
		byte[] InBuffer = "1234567890".getBytes();
		long InSize = 10;
		byte[] OutBuffer = new byte[1024];
		long OutSize = 1024;
		long[] RetSize = new long[1];
		ret = SSRuntimeEasy.SlmExectueDynamicEasy(Handle, ExfBuffer, ExfSize, InBuffer, InSize, OutBuffer, OutSize, RetSize);
		if(ret == ErrorCode.SS_OK)
		{
			System.out.println("SlmExectueDynamicEasy success!");
			System.out.println("[Return Len]:" + RetSize[0]);
		}
		else
		{
			System.out.printf("SlmExectueDynamicEasy failure : 0x%08X!  %s\n", ret, SSRuntimeEasy.SlmErrorFormatEasy(ret, 2));
		}
		
		// ִ�����ھ�̬���룬������ֲ��ز���
		byte[] SignData = new byte[32 + 9];
		SignData = "SENSELOCK2222".getBytes();
	    byte[] SignedData = new byte[0x100];
	    long MaxSize = 0x100;
	    long[] SignedSize = new long[1];

	    ret = SSRuntimeEasy.SlmSignByDeviceEasy(Handle, SignData, 32+9, SignedData, MaxSize, SignedSize);
		if(ret == ErrorCode.SS_OK)
		{
			System.out.println("SlmSignByDeviceEasy success!");
			System.out.println("[Signed Len]:" + SignedSize[0]);
		}
		else
		{
			System.out.printf("SlmSignByDeviceEasy failure : 0x%08X!  %s\n", ret, SSRuntimeEasy.SlmErrorFormatEasy(ret, 2));
		}
		
		// ���Ը���LIC_USER_DATA_TYPE����ȡ����������д����ֻ��ȡ����������С��������������С������ƫ�ƶ�ȡ���������ݡ�
		byte[] ReadBuff;
		long DataSize = 0;
		DataSize = SSRuntimeEasy.SlmUserDataGetsizeEasy(Handle, LIC_USER_DATA_TYPE.PUB.get());
		ret = SSRuntimeEasy.SlmGetLastError();
		if(ret ==ErrorCode.SS_OK)
		{
			System.out.println("SlmUserDataGetsizeEasy success!");
			if(DataSize != 0)
			{
				ReadBuff = new byte[(int)DataSize];
				ret = SSRuntimeEasy.SlmUserDataReadEasy(Handle, LIC_USER_DATA_TYPE.PUB.get(), ReadBuff, 0, DataSize);
				if(ret ==ErrorCode.SS_OK)
				{
					System.out.println("SlmUserDataReadEasy success!");
				}
				else
				{
					System.out.printf("SlmUserDataReadEasy failure : 0x%08X!  %s\n", ret, SSRuntimeEasy.SlmErrorFormatEasy(ret, 2));
				}

			}
			else
			{
				System.out.println("No data!");
			}
		}
		else
		{
			System.out.printf("SlmUserDataGetsizeEasy failure : 0x%08X!  %s\n", ret, SSRuntimeEasy.SlmErrorFormatEasy(ret, 2));
		}
			
		// ����д����д�����ݣ���������й����в�����һЩ������Ϣ������Ҫ����д��������С�
		String WriteData = "test 123...";
	    ret = SSRuntimeEasy.SlmUserDataWriteEasy(Handle, WriteData.getBytes(), 0, WriteData.length());
		if(ret ==ErrorCode.SS_OK)
		{
			System.out.println("SlmUserDataWriteEasy success!");
		}
		else
		{
			System.out.printf("SlmUserDataWriteEasy failure : 0x%08X!  %s\n", ret, SSRuntimeEasy.SlmErrorFormatEasy(ret, 2));
		}
		
		// �жϸ���ɵ�ĳ������ģ���Ƿ����
		ret = SSRuntimeEasy.SlmCheckModuleEasy(Handle, 1);
		if(ret == ErrorCode.SS_OK)
		{
			System.out.println("SlmCheckModuleEasy success!");
		}
		else
		{
			System.out.printf("SlmCheckModuleEasy failure : 0x%08X!  %s\n", ret, SSRuntimeEasy.SlmErrorFormatEasy(ret, 2));
		}
		
		// �˽ӿڿ��Ի�ȡ����ǰ����������в���ļ�����
		Object obj = new Object();
		obj = SSRuntimeEasy.SlmEnumDeviceEasy();
		ret = SSRuntimeEasy.SlmGetLastError();
		if(ret == ErrorCode.SS_OK)
		{
			System.out.println("SlmEnumDeviceEasy success!");
			System.out.println(obj);
		}
		else
		{
			System.out.printf("SlmEnumDeviceEasy failure : 0x%08X!  %s\n", ret, SSRuntimeEasy.SlmErrorFormatEasy(ret, 2));
		}
				
        // �˽ӿڿ��Ի�ȡ�������е��������ID����
		Object LicID = SSRuntimeEasy.SlmEnumLicenseIDEasy(new String());
		ret = SSRuntimeEasy.SlmGetLastError();
		if(ret == ErrorCode.SS_OK)
		{
			System.out.println("SlmEnumLicenseIDEasy success!");
			System.out.println(LicID);
		    SSRuntimeEasy.SlmFreeEasy(LicID);
		}
		else
		{
			System.out.printf("SlmEnumLicenseIDEasy failure : 0x%08X!  %s\n", ret, SSRuntimeEasy.SlmErrorFormatEasy(ret, 2));
		}
		   
		SSRuntimeEasy.SlmFreeEasy(obj);
		
		
		
		// ������ʹ�����ʱ������ɵǳ����رջỰ
		ret = SSRuntimeEasy.SlmLogoutEasy(Handle);
		if(ret == ErrorCode.SS_OK)
		{
			System.out.println("SlmLogoutEasy success!");
		}
		else
		{
			System.out.printf("SlmLogoutEasy failure : 0x%08X!  %s\n", ret, SSRuntimeEasy.SlmErrorFormatEasy(ret, 2));
		}
		
		// ��SlmInitEasy���Ӧ��������л�����һ���ڳ����˳�ʱ���ã�Ҳ���Բ����á�
		ret = SSRuntimeEasy.SlmCleanupEasy();
		if(ret == ErrorCode.SS_OK)
		{
			System.out.println("SlmCleanupEasy success!");
		}
		else
		{
			System.out.printf("SlmCleanupEasy failure : 0x%08X!  %s\n", ret, SSRuntimeEasy.SlmErrorFormatEasy(ret, 2));
		}
		
		// ������������
		Object MsgUpd = new Object();
		MsgUpd = SSRuntimeEasy.SlmUpdateEasy(new String());
		ret = SSRuntimeEasy.SlmGetLastError();
		if(ret == ErrorCode.SS_OK)
		{
			System.out.println("SlmUpdateEasy success!");
		    System.out.println("[Update Msg]:" + MsgUpd);
		    SSRuntimeEasy.SlmFreeEasy(MsgUpd);
		}
		else
		{
			System.out.printf("SlmUpdateEasy failure : 0x%08X!  %s\n", ret, SSRuntimeEasy.SlmErrorFormatEasy(ret, 2));
		}

		
		System.out.println("test over......");
	}

}
