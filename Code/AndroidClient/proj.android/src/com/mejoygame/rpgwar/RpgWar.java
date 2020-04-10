/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 ****************************************************************************/
package com.mejoygame.rpgwar;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.security.MessageDigest;
import java.util.Enumeration;
import java.util.Random;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;
import java.util.zip.ZipInputStream;
import java.util.zip.ZipOutputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import org.cocos2dx.lib.Cocos2dxActivity;
import org.json.JSONException;
import org.json.JSONObject;


import android.app.ActivityManager;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.media.AudioManager;
import android.net.Uri;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.os.Environment;
import android.os.StatFs;
import android.os.SystemClock;
import android.provider.Settings.Secure;
import android.telephony.TelephonyManager;
import android.text.TextUtils;
import android.util.Log;
import android.view.KeyEvent;
import android.view.WindowManager;
import android.widget.Toast;

public class RpgWar extends Cocos2dxActivity {
	public static RpgWar instance;
	public static int m_nUnZipCnt = 0;
	public static int m_nMarkCheck = 0;
	public static int loginStep = 0;
	
	private static String loginToken = null; 
	
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		Log.d("111111", "11111start!!!!");
		instance = this;
		super.onCreate(savedInstanceState);
		
		initTp();
	}

	static {
		System.loadLibrary("iconv");
		System.loadLibrary("game");
	}

	public static RpgWar getInstance() {
		return instance;
	}

	@Override
	public boolean dispatchKeyEvent(KeyEvent event) {
		 if(event.getKeyCode() == KeyEvent.KEYCODE_BACK && event.getAction() == KeyEvent.ACTION_DOWN){
     		 final AlertDialog isExit = new AlertDialog.Builder(this).create();
     		 isExit.setTitle("系统提示");
     		 isExit.setMessage("是否离开游戏");
     		 
     		 DialogInterface.OnClickListener listener = new DialogInterface.OnClickListener() {
     			 public void onClick(DialogInterface dialog, int which) {
     			       switch (which) {
     			             case AlertDialog.BUTTON1:// "纭"鎸夐挳閫�嚭绋嬪簭
     			             
     			            	  finish();
                                   android.os.Process.killProcess(android.os.Process.myPid());
                                   break;   
     			             case AlertDialog.BUTTON2:// "鍙栨秷"绗簩涓寜閽彇娑堝璇濇
     			            	    isExit.cancel();
     			                    break;
     			             default:
     			                    break;
     			        }
     			 }
     			};
     			// 娉ㄥ唽鐩戝惉
     		 isExit.setButton("确定", listener);
             isExit.setButton2("取消", listener);
     		 isExit.show();
     		 return true;
     			 
     	}

		if (event.getKeyCode() == KeyEvent.KEYCODE_HOME) {

		}
		// 媒体音量调节
		AudioManager mAudioManager = (AudioManager) getSystemService(Context.AUDIO_SERVICE);
		int currentVolume = mAudioManager
				.getStreamVolume(AudioManager.STREAM_MUSIC);

		if (event.getKeyCode() == KeyEvent.KEYCODE_VOLUME_UP) {

			if (currentVolume < mAudioManager
					.getStreamMaxVolume(AudioManager.STREAM_MUSIC)) {

				mAudioManager.setStreamVolume(AudioManager.STREAM_MUSIC,
						currentVolume + 1, 1);
			}

			return true;
		}
		if (event.getKeyCode() == KeyEvent.KEYCODE_VOLUME_DOWN) {
			if (currentVolume > 0)
				mAudioManager.setStreamVolume(AudioManager.STREAM_MUSIC,
						currentVolume - 1, 1);
			return true;
		}

		return super.dispatchKeyEvent(event);
	}

	public String getMacAddr() {
		WifiManager wifi = (WifiManager) getSystemService(Context.WIFI_SERVICE);
		WifiInfo info = wifi.getConnectionInfo();
		Log.d("1111Mac", info.getMacAddress());
		return info.getMacAddress();
	}

	/**
	 * mac地址
	 * 
	 * @return
	 */
	public static String getMacAddress() {
		return instance.getMacAddr();
	}

	/**
	 * android系统版本
	 * 
	 * @return
	 */
	public static int getSystemVersion() {
		return 0;
	}

	/**
	 * 平台编号
	 * 
	 * @return
	 */
	public static int getTPID() {
		
		return 0;
	}

	public static int getLoginStep() {
		return 1;
	}

	public static int getLoginID() {
		return 0;
	}

	public static String getLoginToken() {
		return loginToken;
	}

	public String getDeviceId() {
		String androidId = Secure.getString(instance.getContentResolver(), Secure.ANDROID_ID);
		return androidId;
	}

	/**
	 * 返回md5运算的devid
	 * 
	 * @return
	 */
	public static String getAndDevId() {
		String devId = instance.getDeviceId();
		String mySign = "";
		try {
			// 通过MD5计算出签名
			MessageDigest messageDigest = MessageDigest.getInstance("MD5");
			byte[] b = messageDigest.digest(devId.getBytes("utf-8"));

			String HEX_CHARS = "0123456789abcdef";
			for (byte aB : b) {
				mySign += HEX_CHARS.charAt(aB >>> 4 & 0x0F);
				mySign += HEX_CHARS.charAt(aB & 0x0F);
			}

			// 注意：最后需要转化成大写
			mySign = mySign.toUpperCase();
		} catch (Exception e) {
			throw new RuntimeException(e);
		}
		Log.d("1111MD5", "1111111" + mySign);
		return mySign;
	}

	/**
	 * 返回devid
	 * 
	 * @return
	 */
	public static String getAndIDFA() {
		return android.os.Build.MODEL + "_" + instance.getDeviceId();
	}

	// 取sdcard路径
	public static String sdDir = null;
	public static String getSDPath() {
		File file = new File(instance.getBaseContext().getFilesDir().getAbsolutePath() + "/rpgwar/res/Servercfg.ini");
		if(file.exists()){
			sdDir = instance.getBaseContext().getFilesDir().getAbsolutePath();
			return sdDir;
		}else{
			file = new File(Environment.getExternalStorageDirectory().getAbsolutePath() + "/rpgwar/res/Servercfg.ini");
			if(file.exists()){
				sdDir =Environment.getExternalStorageDirectory().getAbsolutePath();
				return sdDir;
			}
		}
		long unUseredMem = 0;
		ActivityManager am = (ActivityManager)
		instance.getSystemService(Context.ACTIVITY_SERVICE);
		ActivityManager.MemoryInfo mi = new ActivityManager.MemoryInfo();
		am.getMemoryInfo(mi);
		unUseredMem = mi.availMem / 1024 / 1024;
		Log.d("1111unUserdMen", "" + unUseredMem);
		if (unUseredMem >= 150) {
			sdDir = instance.getBaseContext().getFilesDir().getAbsolutePath();
			// 判断最后一位是否有"/"
			if (sdDir.substring(sdDir.length() - 1).equals("/")) {
				sdDir = sdDir.substring(0, sdDir.length() - 1);
			}
		} else if (unUseredMem < 150){
			// SD卡
			boolean sdCardExist =Environment.getExternalStorageState().equals(android.os.Environment.MEDIA_MOUNTED);
			if (sdCardExist) {
				sdDir =Environment.getExternalStorageDirectory().getAbsolutePath();
			} else {
				sdDir = "/mnt/sdcard2/";
				File zipFolderT = new File(sdDir);
				if (!zipFolderT.exists()) {
					sdDir =Environment.getExternalStorageDirectory().getAbsolutePath();
				}
			}
	        StatFs stat = new StatFs(sdDir);
	        long blockSize = stat.getBlockSize();
	        long totalBlocks = stat.getBlockCount();
	        long unUesredSD = totalBlocks * blockSize / 1024 / 1024;
	        Log.d("1111unUserdSD", "" + unUesredSD);
			if(unUesredSD < 150){
				Toast.makeText(instance, "空间不足，请清理", Toast.LENGTH_LONG).show();
				System.exit(0);
			}
		} 
		return sdDir;
	}

	public static int getUnZipCnt() {
		// if(m_nMarkCheck == 0)
		// goLogin();
		String targetPath = getSDPath() + "/rpgwar/res/Servercfg.ini";
		File zipFolder1 = new File(targetPath);
		if (!zipFolder1.exists() && m_nMarkCheck == 0) {
			m_nMarkCheck = 1;
			return 9999999;
		}

		return m_nUnZipCnt;
	}

	public static void UnZipNetFile() throws IOException {
		m_nUnZipCnt = 0;
		String targetPath = getSDPath() + "/rpgwar/res/";
		Log.d("1111", "111111NETStart");
		URL url = new URL("http://192.168.1.100:8080/path/rpgwar.zip");// 文件地址
		HttpURLConnection conn = (HttpURLConnection) url.openConnection();
		conn.setConnectTimeout(5 * 1000);
		conn.setRequestMethod("GET");
		if (conn.getResponseCode() != 200)
			throw new RuntimeException("请求url失败");
		Log.d("1111", "111111NETOK");

		InputStream is = conn.getInputStream();
		ZipInputStream zis = new ZipInputStream(is);
		ZipEntry entry = null;
		while ((entry = zis.getNextEntry()) != null) {

			String zipPath = entry.getName();
			Log.d(zipPath, "111111NETProcess");
			if (entry.isDirectory()) {
				File zipFolder = new File(targetPath + zipPath);

				if (!zipFolder.exists()) {
					zipFolder.mkdirs();
				}
			} else {
				File file = new File(targetPath + zipPath);
				if (!file.exists()) {
					File pathDir = file.getParentFile();
					pathDir.mkdirs();
					file.createNewFile();
				}
				m_nUnZipCnt++;
				FileOutputStream fos = new FileOutputStream(file);

				byte[] b = new byte[2048];
				int bread = 0;
				while ((bread = zis.read(b, 0, 2048)) != -1) {
					fos.write(b, 0, bread);

				}
				fos.close();
			}
		}
		zis.close();
		is.close();
		m_nUnZipCnt = -1;
	}

	public static void UnZipRes() {
		m_nUnZipCnt = 0;
		String targetPath = getSDPath() + "/rpgwar/res/";

		Log.d("1111", "111111UNZIPSTART");

		File zipFolder1 = new File(targetPath);

		if (!zipFolder1.exists()) {
			if (zipFolder1.mkdirs())
				Log.d(targetPath, "11111122222OK");
			else
				Log.d(targetPath, "11111122222FAIL");
		}

		try {
			InputStream is = instance.getAssets().open("rpgwar.zip");
			ZipInputStream zis = new ZipInputStream(is);
			ZipEntry entry = null;
			while ((entry = zis.getNextEntry()) != null) {
				String zipPath = entry.getName();
				try {

					if (entry.isDirectory()) {
						File zipFolder = new File(targetPath + zipPath);

						if (!zipFolder.exists()) {
							zipFolder.mkdirs();
						}
					} else {
						File file = new File(targetPath + zipPath);
						if (!file.exists()) {
							File pathDir = file.getParentFile();
							pathDir.mkdirs();
							file.createNewFile();
						}
						m_nUnZipCnt++;
						FileOutputStream fos = new FileOutputStream(file);

						byte[] b = new byte[2048];
						int bread = 0;
						while ((bread = zis.read(b, 0, 2048)) != -1) {
							fos.write(b, 0, bread);

						}
						fos.close();

					}

				} catch (Exception e) {
					continue;
				}
			}
			zis.close();
			is.close();
			m_nUnZipCnt = -1;
		} catch (Exception e) {
			e.printStackTrace();
			m_nUnZipCnt = -1;
		}
	}

	// 初始化第三方平台的东西
	public static int initTp() {
		// 初始化第三方平台信息,放在网络正常之后
		return 0;
	}
	//服务回调	
	
	
	public static int goLogin() {
		Log.d("111111AZ", "11111goLogin");
		
		return 0;
	}

	//退出接口
	public static void goQuit(){
    	Log.d("111111AZ", "11111goQuit");
    	
    	
    }
	/**
	 * 出用户中心界面
	 */
	public static void goTpCenter() {
		
	}

	public static void goPay(int nVal, int nSvrId, String szAcc, String szOrder) {
		Log.i("11111AZpay", "1111111START");
		
	}
	
	/**
	 * 提交游戏扩展数据功能，游戏SDK要求游戏在运行过程中，提交一些用于运营需要的扩展数据，这些数据通过扩展数据提交方法进行提交。
	 * 登录游戏角色成功后调用此段
	 */
	public static void TPSubmitUser(int uin, int nSvr, int nLv, String strName, String szZoneName) {
		return;
	}
	
	// 每次刷新调用，每30-50ms调用一次
	public static void update() {
	}
	
	public static void OtherEvent(int nVal) { // 暂时不用
	}
	public static int getLockInUpdate() { // 暂时不用
		return 0;
	}

	// 返回一个网络URL的文本内容
	public static String TPOpenVersionUrl(String szUrl) throws IOException {
		szUrl	= "http://192.168.1.100/path/ViersionList.txt";
		m_nUnZipCnt = 0;
		Log.d("1111openVersion", "1111START" + szUrl);
		if (szUrl.length() < 5) {
			return "";
		}
		StringBuffer buffer = new StringBuffer();
		URL url = new URL(szUrl);
		HttpURLConnection conn = (HttpURLConnection) url.openConnection();
		conn.setConnectTimeout(5 * 1000);
		conn.setRequestMethod("GET");
		if (conn.getResponseCode() != 200) {
			return "";
		} else {
			InputStreamReader read = new InputStreamReader(
					conn.getInputStream());
			BufferedReader bufferedReader = new BufferedReader(read);
			String s = null;
			while ((s = bufferedReader.readLine()) != null) {
				buffer.append(s + "\n");
			}
			read.close();
			bufferedReader.close();
		}
		Log.d("1111openVersion", "1111" + buffer.toString());
		return buffer.toString();
	}

	// 下载一个URL文件存在REs位置, 成功返回1
	public static void _httpDownZip(String szUrl, String szSaveName)
			throws IOException {
		String path = getSDPath() + "/rpgwar/res/";
		String targetPath = getSDPath() + "/rpgwar/res/" + szSaveName;// 文件保存路径
		URL url = new URL(szUrl);
		HttpURLConnection conn = (HttpURLConnection) url.openConnection();
		conn.setConnectTimeout(5 * 1000);
		conn.setRequestMethod("GET");
		if (conn.getResponseCode() != 200)
			return;
		else {
			File file = new File(path);
			if (!file.exists()) {
				file.mkdirs();
			}
			BufferedInputStream bis = new BufferedInputStream(
					conn.getInputStream());
			ZipInputStream zis = new ZipInputStream(bis);
			ZipEntry entry = null;
			ZipOutputStream zout = new ZipOutputStream(new FileOutputStream(
					targetPath));
			while ((entry = zis.getNextEntry()) != null) {
				String zipPath = entry.getName();
				zout.putNextEntry(new ZipEntry(zipPath));
				byte[] b = new byte[4096];
				int bread = 0;
				while ((bread = zis.read(b, 0, 4096)) != -1) {
					zout.write(b, 0, bread);
				}
				zout.closeEntry();
			}
			zout.close();
			zis.close();
			bis.close();
			m_nUnZipCnt++;
		}
	}

	public static int TPDownZipFile(final String szUrl, final String szSaveName)
			throws IOException {
		new Thread(new Runnable() {
			@Override
			public void run() {
				try {
					_httpDownZip(szUrl, szSaveName);
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}).start();
		return 1;
	}

	// 解压REs位置的这个文件
	public static int TPUnZipFile(String szName) {
		String targetPath = getSDPath() + "/rpgwar/res/";// 目标路径
		String path = getSDPath() + "/rpgwar/res/" + szName;// 要解压的文件路径
		Log.d("1111UNZIP", "1111UNZIPSTART");

		File zipFolder = new File(targetPath);

		if (!zipFolder.exists()) {
			if (zipFolder.mkdirs())
				Log.d(targetPath, "11111122222OK");
			else
				Log.d(targetPath, "11111122222FAIL");
		}
		try {
			BufferedInputStream bis = null;
			ZipFile zis = new ZipFile(path);
			Enumeration zList = zis.entries();
			ZipEntry entry = null;
			while (zList.hasMoreElements()) {
				entry = (ZipEntry) zList.nextElement();
				String zipPath = entry.getName();
				try {

					if (entry.isDirectory()) {
						File file = new File(targetPath + zipPath);
						if (!file.exists()) {
							file.mkdirs();
						}
					} else {
						File file = new File(targetPath + zipPath);
						if (!file.exists()) {
							File pathDir = file.getParentFile();
							pathDir.mkdirs();
							file.createNewFile();
						}
						m_nUnZipCnt++;
						bis = new BufferedInputStream(zis.getInputStream(entry));
						BufferedOutputStream fos = new BufferedOutputStream(
								new FileOutputStream(file));

						byte[] b = new byte[2048];
						int bread = 0;
						while ((bread = bis.read(b, 0, 2048)) != -1) {
							fos.write(b, 0, bread);
						}
						fos.close();
						bis.close();
					}

				} catch (Exception e) {
					Log.d("1111UNZIP", "FAIL" + zipPath);
					continue;
				}
			}
			zis.close();
			Log.d("1111UNZIP", "SUCCESS" + m_nUnZipCnt);
			m_nUnZipCnt = -1;
			deleteZip(path);
			return 1;
		} catch (Exception e) {
			Log.d("1111UNZIP", "FAIL");
			e.printStackTrace();
		}
		return 0;
	}

	public static boolean deleteZip(String path) {
		File file = new File(path);
		if (!file.exists()) {
			return true;
		} else {
			file.delete();
			return true;
		}
	}
	public static void initResToSDCard() {
		new Thread(new Runnable() {
			@Override
			public void run() {
				UnZipRes();
			}
		}).start();
	}

}
