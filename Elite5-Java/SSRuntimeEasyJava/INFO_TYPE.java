package com.senseyun.openapi.SSRuntimeEasyJava;

public enum INFO_TYPE {
	LOCK_INFO(1),
	SESSION_INFO(2),
	LICENSE_INFO(3),
	FILE_LIST(4);
	
	private final int value;
	public int get()
	{
		return value;
	}
	INFO_TYPE(int value)
	{
		this.value = value;
	}
	
}
