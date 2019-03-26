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
	
    // 将十六进制数组转换为十六进制字符串，即：0x12 --> "12"
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
	
    // 将十六进制字符串转换为十六进制数组，即："12" --> 0x12
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
		
		// 初始化函数，在使用深思RuntimeAPI其他接口之前，必须调用初始化函数
		ret = SSRuntimeEasy.SlmInitEasy(APIPsd);
		if(ret == ErrorCode.SS_OK)
		{
			System.out.println("SlmInitEasy success !");
		}
		else
		{
			System.out.printf("SlmInitEasy failure : 0x%08X!  %s\n", ret, SSRuntimeEasy.SlmErrorFormatEasy(ret, 2));
		}
		
		// 此接口用于获取开发商ID，可以通过此接口判断开发库是否与自己的一致
	    String[] DeveloperID = new String[1];
		ret = SSRuntimeEasy.SlmGetDeveloperIDEasy(DeveloperID);
		System.out.println("[Developer ID]:" + DeveloperID[0]);
		
		// 此接口可以获取到当前库的版本和Virbox许可服务的版本
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
		
		// 获取许可信息，此接口可以获取指定许可的详细信息
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
		
		// 登录许可，此过程十分重要，软件加密过程中所用到的最为重要的接口，
        // 只有登录了许可，才能访问加密锁中的重要数据
		long Handle = 0;
		
		//json 为确保安全，SDK版本号2.1.0.15128之后的许可登录将不再支持json登录的方法。
		String Param = "{\"license_id\":0}";
		
		//struct
		ST_LOGIN_PARAM stLogin = new ST_LOGIN_PARAM();
		stLogin.size = stLogin.getSize();
		stLogin.license_id = 0;
		//stLogin.user_guid = stringToHexByte("123456");  //(可选)
		//Handle = SSRuntimeEasy.SlmLoginEasy(Param, INFO_FORMAT_TYPE.JSON.get());     //login by json  （不支持）
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
		
		// 心跳保持测试
		ret = SSRuntimeEasy.SlmKeepAliveEasy(Handle);
		if(ret == ErrorCode.SS_OK)
		{
			System.out.println("SlmKeepAliveEasy success!");
		}
		else
		{
			System.out.printf("SlmKeepAliveEasy failure : ErrorCode = 0x%08X!  %s\n", ret, SSRuntimeEasy.SlmErrorFormatEasy(ret, 2));
		}
		
		// 根据INFO_TYPE可以获取当前登录许可的详细信息、会话的详细信息、加密锁的详细信息等
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
		
		// 可以控制加密锁LED灯光颜色或者闪烁频率
		ret = SSRuntimeEasy.SlmLedControlEasy(Handle, SSDefines.LED_COLOR_BLUE, SSDefines.LED_STATE_SHRINK, 500);
		if(ret == ErrorCode.SS_OK)
		{
			System.out.println("SlmLedControlEasy success!");
		}
		else
		{
			System.out.printf("SlmLedControlEasy  failure : ErrorCode = 0x%08X!  %s\n", ret, SSRuntimeEasy.SlmErrorFormatEasy(ret, 2));
		}
		
		// 使用许可对数据进行加密解密，此操作是许可与软件深度绑定的一个操作，安全性极高。
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
		
		// 使用内存托管的过程，将软件的内存安全的存放在许可服务中进行管理
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
		
		// 获取设备证书
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
		
		// 执行动态代码，代码移植相关操作
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
		
		// 执行锁内静态代码，代码移植相关操作
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
		
		// 可以根据LIC_USER_DATA_TYPE来获取公开区、读写区和只读取的数据区大小。根据数据区大小和数据偏移读取数据区内容。
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
			
		// 往读写区中写入数据，将软件运行过程中产生的一些配置信息或者重要数据写入加密锁中。
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
		
		// 判断该许可的某个功能模块是否存在
		ret = SSRuntimeEasy.SlmCheckModuleEasy(Handle, 1);
		if(ret == ErrorCode.SS_OK)
		{
			System.out.println("SlmCheckModuleEasy success!");
		}
		else
		{
			System.out.printf("SlmCheckModuleEasy failure : 0x%08X!  %s\n", ret, SSRuntimeEasy.SlmErrorFormatEasy(ret, 2));
		}
		
		// 此接口可以获取到当前计算机上所有插入的加密锁
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
				
        // 此接口可以获取加密锁中的所有许可ID数组
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
		
		
		
		// 当不再使用许可时，将许可登出，关闭会话
		ret = SSRuntimeEasy.SlmLogoutEasy(Handle);
		if(ret == ErrorCode.SS_OK)
		{
			System.out.println("SlmLogoutEasy success!");
		}
		else
		{
			System.out.printf("SlmLogoutEasy failure : 0x%08X!  %s\n", ret, SSRuntimeEasy.SlmErrorFormatEasy(ret, 2));
		}
		
		// 与SlmInitEasy相对应，清除运行环境，一般在程序退出时调用，也可以不调用。
		ret = SSRuntimeEasy.SlmCleanupEasy();
		if(ret == ErrorCode.SS_OK)
		{
			System.out.println("SlmCleanupEasy success!");
		}
		else
		{
			System.out.printf("SlmCleanupEasy failure : 0x%08X!  %s\n", ret, SSRuntimeEasy.SlmErrorFormatEasy(ret, 2));
		}
		
		// 数据升级操作
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
