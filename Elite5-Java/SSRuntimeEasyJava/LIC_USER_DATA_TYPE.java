package com.senseyun.openapi.SSRuntimeEasyJava;

public enum LIC_USER_DATA_TYPE {
	ROM(0), RAW(1), PUB(2);
	
	private final int value;
	public int get()
	{
		return value;
	}
	LIC_USER_DATA_TYPE(int value)
	{
		this.value = value;
	}
}
