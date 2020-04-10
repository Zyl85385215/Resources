package com.mejoygame.rpgwar;

public class JniCall {
	public static native void OpenForm(final String szForm,final boolean bOpen);
	public static native void SetLoginStep(final int nStep);
	public static native void SetLoginMID(final int nMID);
	public static native void SetSetToken(final String szToken);
}
