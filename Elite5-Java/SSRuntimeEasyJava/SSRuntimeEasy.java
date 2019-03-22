package com.senseyun.openapi.SSRuntimeEasyJava;

public class SSRuntimeEasy {
	static{
		try{
			System.loadLibrary("SSRuntimeEasyJava");
		}catch(UnsatisfiedLinkError e){
			System.err.println("Native code library failed to load.\n" + e);
			System.exit(1);
		}
	}
	
	
	public final static native long SlmGetLastError();
	public final static native long AppMsgGetLastStatus();
	public final static native String AppMsgGetLastInfo();
	
	public final static native long SlmInitEasy(String APIPsd);
	public final static native long SlmCleanupEasy();
	public final static native long SlmIsDebugEasy();
	public final static native long SlmLoginEasy(Object Param, long FmtType);
	public final static native long SlmLogoutEasy(long Handle);
	public final static native Object SlmFindLicenseEasy(long LicenseID, long FmtType);
	public final static native Object SlmGetInfoEasy(long Handle, long InfoType, long FmtType);
	public final static native long SlmKeepAliveEasy(long Handle);
	public final static native long SlmCheckModuleEasy(long Handle, long Mod);
	public final static native long SlmEncryptEasy(long Handle, byte[] InBuffer, byte[] OutBuffer, long Len);
	public final static native long SlmDecryptEasy(long Handle, byte[] InBuffer, byte[] OutBuffer, long Len);
	public final static native long SlmUserDataGetsizeEasy(long Handle, long DataType);
	public final static native long SlmUserDataReadEasy(long Handle, long DataType, byte[] ReadBuffer, long Offset, long Len);
	public final static native long SlmUserDataWriteEasy(long Handle, byte[] WriteBuffer, long Offset, long Len);
	public final static native long SlmPubDataGetsizeEasy(long Handle, long LicenseID);
	public final static native long SlmPubDataReadEasy(long Handle, long LicenseID, byte[] ReadBuffer, long Offset, long Len);
	public final static native long SlmMemAllocEasy(long Handle, long Size);
	public final static native long SlmMemFreeEasy(long Handle, long MemID);
	public final static native long SlmMemReadEasy(long Handle, long MemID, long Offset, long Len, byte[] ReadBuffer);
	public final static native long SlmMemWriteEasy(long Handle, long MemID, long Offset, long Len, byte[] WriteBuffer);
	public final static native long SlmExectueDynamicEasy(long Handle, byte[] ExfBuffer, long ExfSize, byte[] InBuffer, long InSize, byte[] OutBuffer, long OutSize, long[] RetLen);
	public final static native long SlmExectueStaticEasy(long Handle, String ExfName, byte[] InBuffer, long InSize, byte[] OutBuffer, long OutSize, long[] RetLen);
	public final static native long SlmGetDeviceCertEasy(long Handle, byte[] DeviceCert, long CertSize, long[] RetLen);
	public final static native long SlmSignByDeviceEasy(long Handle, byte[] SignData, long DataSize, byte[] SignedData, long MaxSize, long[] RetLen);
	public final static native long SlmLedControlEasy(long Handle, long Color, long State, long Interval);
	public final static native long SlmGetVersionEasy(long[] APIVersion, long[] SSVersion);
	public final static native String SlmErrorFormatEasy(long ErrorCode, long Language);
	public final static native Object SlmUpdateEasy(String D2CPkg);
	public final static native Object SlmUpdateEasyEx(String LockSn, String D2CPkg);
	public final static native Object SlmUpdateFileEasy(String LockSn, String D2CFile);
	public final static native Object SlmEnumDeviceEasy();
	public final static native void SlmFreeEasy(Object obj);
	public final static native Object SlmGetCloudTokenEasy();
	public final static native long SlmGetDeveloperIDEasy(String[] DeveloperID);
	public final static native long SlmD2CUpdateInsideEasy(String LockSn, String InsideFile);
	public final static native Object SlmEnumLicenseIDEasy(String DeviceInfo);
	public final static native Object SlmGetLicenseInfoEasy(String DeviceInfo, long LicenseID);
	public final static native long SlmLicenseSignEasy(long Handle, byte[] SignData, long SignLength, byte[] Signature, long MaxBufSize);
	public final static native Object SlmLicenseVerifyEasy(byte[] SignData, long SignLength, byte[] Signature, long SignatureLength);
	
}
