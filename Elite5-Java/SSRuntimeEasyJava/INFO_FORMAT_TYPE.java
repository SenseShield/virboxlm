package com.senseyun.openapi.SSRuntimeEasyJava;

public enum INFO_FORMAT_TYPE {
	JSON(2), STRUCT(3), STRING_KV(4),CRPHER(5);
	
	private final int value;
	public int get()
	{
		return value;
	}
	INFO_FORMAT_TYPE(int value)
	{
		this.value = value;
	}
}
