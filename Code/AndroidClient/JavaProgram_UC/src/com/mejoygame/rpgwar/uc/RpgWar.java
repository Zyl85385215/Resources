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
package com.mejoygame.rpgwar.uc;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.security.MessageDigest;
import java.util.Calendar;
import java.util.Enumeration;
import java.util.TimeZone;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;
import java.util.zip.ZipInputStream;
import java.util.zip.ZipOutputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import org.cocos2dx.lib.Cocos2dxActivity;
import org.json.JSONObject;

import cn.uc.gamesdk.UCCallbackListener;
import cn.uc.gamesdk.UCCallbackListenerNullException;
import cn.uc.gamesdk.UCFloatButtonCreateException;
import cn.uc.gamesdk.UCGameSDK;
import cn.uc.gamesdk.UCGameSDKStatusCode;
import cn.uc.gamesdk.UCLogLevel;
import cn.uc.gamesdk.UCLoginFaceType;
import cn.uc.gamesdk.UCOrientation;
import cn.uc.gamesdk.info.FeatureSwitch;
import cn.uc.gamesdk.info.GameParamInfo;
import cn.uc.gamesdk.info.OrderInfo;
import cn.uc.gamesdk.info.PaymentInfo;

import android.app.AlarmManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.content.res.Configuration;
import android.media.AudioManager;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.os.SystemClock;
import android.provider.Settings.Secure;
import android.util.Log;
import android.view.KeyEvent;
import android.view.WindowManager;

public class RpgWar extends Cocos2dxActivity {
	public static RpgWar instance;
	public static int m_nUnZipCnt = 0;
	public static int m_nMarkCheck = 0;
	public static int loginStep = 0;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		Log.d("111111", "11111start!!!!");
		instance = this;
		super.onCreate(savedInstanceState);

		initTp();

//		Intent intent = new Intent(instance, AlarmReceiver.class);  
//		PendingIntent sender = PendingIntent.getBroadcast(instance, 0, intent, 0);  
//
//		long firstTime = SystemClock.elapsedRealtime(); // 开机之后到现在的运行时间(包括睡眠时间)
//		
//		Calendar calendar = Calendar.getInstance();  
//		calendar.setTimeInMillis(System.currentTimeMillis());  
//		// 这里时区需要设置一下，不然会有8个小时的时间差  
//		calendar.setTimeZone(TimeZone.getTimeZone("GMT+8"));  
//		calendar.add(Calendar.SECOND, 1);
//		
//		// 进行闹铃注册  每一秒一周期
//		AlarmManager manager = (AlarmManager)getSystemService(ALARM_SERVICE);  
//		manager.setRepeating(AlarmManager.ELAPSED_REALTIME_WAKEUP,  
//		                firstTime, calendar.getTimeInMillis(), sender);  
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
		if (event.getKeyCode() == KeyEvent.KEYCODE_BACK
				&& event.getAction() == KeyEvent.ACTION_DOWN) {
			goQuit();
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
		Log.d("1111getVersion", "1111111Version");
		return 0;
	}

	/**
	 * 平台编号, UC为18
	 * 
	 * @return
	 */
	public static int getTPID() {
		Log.d("1111TPID", "1111111getTPID");
		return 18;
	}

	public static int getLoginStep() {
		Log.d("1111LoginStep", "1111111getLoginStep");
		return loginStep;
	}

	public static int getLoginID() {
		Log.d("1111UID", "1111111getUID");
		return 0;
	}

	public static String getLoginToken() {
		Log.d("1111Token", UCGameSDK.defaultSDK().getSid());
		return UCGameSDK.defaultSDK().getSid();
	}

	public String getDeviceId() {
		Log.d("1111NOMD5", "1111111DeviceId");
//		TelephonyManager tm = (TelephonyManager) this
//				.getSystemService(Context.TELEPHONY_SERVICE);
//		String deviceId = tm.getDeviceId();
//		Log.d("1111NOMD5", deviceId);
//		if (deviceId == null) {
//			Random random = new Random(System.currentTimeMillis());
//			int min = 9;
//			String temp1 = System.currentTimeMillis() + "";
//			while (true) {
//				deviceId = Math.abs(random.nextInt()) + "";
//				if (deviceId.length() == min)
//					break;
//			}
//			deviceId = deviceId + temp1.substring(0, 6);
//			Log.d("1111NOMD5", "1111111"+deviceId);
//		}
		 String androidId = Secure.getString(instance.getContentResolver(),Secure.ANDROID_ID); 
		return androidId;//getMacAddress();
	}

	/**
	 * 返回md5运算的devid
	 * 
	 * @return
	 */
	public static String getAndDevId() {
		Log.d("1111MD5", "1111111DeviceId");
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
		Log.d("1111MD5", "1111111"+mySign);
		return mySign;
	}

	/**
	 * 返回devid
	 * 
	 * @return
	 */
	public static String getAndIDFA() {
		Log.d("1111IDFA", "1111111getIDFA");
		return android.os.Build.MODEL + "_" + instance.getDeviceId();
	}

	/**
	 * 取sdcard路径
	 * 
	 * @return
	 */
	public static String getSDPath() {
		String sdDir = null;
		sdDir = instance.getBaseContext().getFilesDir().getAbsolutePath();
		// 判断最后一位是否有"/"
		if (sdDir.substring(sdDir.length() - 1).equals("/")) {
			sdDir = sdDir.substring(0, sdDir.length() - 1);
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
			byte[] b = new byte[4096];
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

						
						int bread = 0;
						while ((bread = zis.read(b, 0, 4096)) != -1) {
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
		// 监听用户注销登录消息
		// 九游社区-退出当前账号功能执行时会触发此监听
		try {
			UCGameSDK.defaultSDK().setLogoutNotifyListener(
					new UCCallbackListener<String>() {
						@Override
						public void callback(int statuscode, String msg) {
							// TODO 此处需要游戏客户端注销当前已经登录的游戏角色信息
							String s = "游戏接收到用户退出通知。" + msg + statuscode;
							Log.i("UCGameSDK", s);
							// 未成功初始化
							if (statuscode == UCGameSDKStatusCode.NO_INIT) {
								// 调用SDK初始化接口
								initTp();
							}
							// 未登录成功
							if (statuscode == UCGameSDKStatusCode.NO_LOGIN) {
								// 调用SDK登录接口
								goLogin();
							}
							// 退出账号成功
							if (statuscode == UCGameSDKStatusCode.SUCCESS) {
								// 执行销毁悬浮按钮接口
								ucSdkDestoryFloatButton();
								loginStep = 0;
								//goLogin();
							}
							// 退出账号失败
							if (statuscode == UCGameSDKStatusCode.FAIL) {
								// 调用SDK退出当前账号接口
								ucSdkLogout();
								loginStep = 0;
							}
						}
					});
		} catch (UCCallbackListenerNullException e) {
			e.printStackTrace();
		}

		try {
			GameParamInfo gpi = new GameParamInfo();
			gpi.setCpId(UCSdkConfig.cpId);
			gpi.setGameId(UCSdkConfig.gameId);
			gpi.setServerId(UCSdkConfig.serverId);
			// gpi.setChannelId(2); // 渠道号统一处理，已不需设置，此参数已废弃，服务端此参数请设置值为2

			// 在九游社区设置显示查询充值历史和显示切换账号按钮，
			// 在不设置的情况下，默认情况情况下，生产环境显示查询充值历史记录按钮，不显示切换账户按钮
			// 测试环境设置无效
			gpi.setFeatureSwitch(new FeatureSwitch(true, true));

			// 设置SDK登录界面为横屏，个人中心及充值页面默认为强制竖屏，无法修改
			UCGameSDK.defaultSDK().setOrientation(UCOrientation.LANDSCAPE);

			// 设置登录界面：
			// USE_WIDGET - 简版登录界面
			// USE_STANDARD - 标准版登录界面
			UCGameSDK.defaultSDK().setLoginUISwitch(UCLoginFaceType.USE_WIDGET);

			UCGameSDK.defaultSDK().initSDK(instance, UCLogLevel.DEBUG,
					UCSdkConfig.debugMode, gpi,
					new UCCallbackListener<String>() {
						@Override
						public void callback(int code, String msg) {
							Log.i("UCGameSDK", "UCGameSDK初始化接口返回数据 msg:" + msg
									+ ",code:" + code + ",debug:"
									+ UCSdkConfig.debugMode + "\n");
							switch (code) {
							// 初始化成功,可以执行后续的登录充值操作
							case UCGameSDKStatusCode.SUCCESS:
								// 调用sdk登录接口
								break;
							// 初始化失败
							case UCGameSDKStatusCode.INIT_FAIL:
								// 调用sdk初始化接口
								initTp();
							default:
								break;
							}
						}
					});
		} catch (UCCallbackListenerNullException e) {
			e.printStackTrace();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return 0;
	}

	/**
	 * SDK客户端登录成功后，游戏客户端通过getsid()方法获取SDK客户端的sid，发送给游戏服务器，
	 * 游戏服务器使用此sid进行服务端接口（ucid.user.sidInfo接口）调用，即可获取ucid及昵称，
	 * 随后游戏服务器向游戏客户端发送ucid及昵称即可。 （注：游戏客户端无法直接从SDK客户端获取UCID）
	 * 详细流程可见接入文档“02-技术文档-SDK总体机制\UC游戏_SDK_开发参考说明书_总体机制_vXXX.pdf”
	 */
	public static int goLogin() {
		Log.d("1111UC", "1111goLogin");
		instance.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				try {
					ucSdkCreateFloatButton();
					ucSdkShowFloatButton();
					loginStep = 1;
					UCGameSDK.defaultSDK().login(instance,
							new UCCallbackListener<String>() {
								@Override
								public void callback(int code, String msg) {
									Log.i("UCGameSDK", "UCGameSdk登录接口返回数据:code=" + code
											+ ",msg=" + msg);
									// 登录成功。此时可以获取sid。并使用sid进行游戏的登录逻辑
									// 客户端无法直接获取UCID
									if (code == UCGameSDKStatusCode.SUCCESS) {
										// 登录成功，可以执行后续操作
										loginStep = 2;
									}
									// 登录退出。该回调会在登录界面退出时执行。
									if (code == UCGameSDKStatusCode.LOGIN_EXIT) {
										// 登录界面关闭，游戏需判断此时是否已登录成功进行相应操作
									}
									// 登录失败。应该先执行初始化成功后再进行登录调用。
									if (code == UCGameSDKStatusCode.NO_INIT) {
										// 没有初始化就进行登录调用，需要游戏调用SDK初始化方法
										initTp();
									}
								}
							});
				} catch (UCCallbackListenerNullException e) {
					e.printStackTrace();
				}
			}
		});
		return 1;
	}
	/**
	 * 游戏可通过调用下面方法，退出当前登录的账号
	 * 通过退出账号侦听器（此侦听器在初始化前经由setLogoutNotifyListener方法设置）
	 * 把退出消息返回给游戏，游戏可根据状态码进行相应的处理。
	 */
	private static void ucSdkLogout() {
		try {
			UCGameSDK.defaultSDK().logout();
		} catch (UCCallbackListenerNullException e) {
			e.printStackTrace();
		}
	}
	
	/**
	 * 当游戏退出前必须调用该方法，进行清理工作。建议在游戏退出事件中进行调用，必须在游戏退出前执行
	 * 如果游戏直接退出，而不调用该方法，可能会出现未知错误，导致程序崩溃
	 */
	public static void goQuit() {
		Log.d("1111UC", "1111goQuit");
		UCGameSDK.defaultSDK().exitSDK(instance,
				new UCCallbackListener<String>() {
					@Override
					public void callback(int code, String msg) {
						if (UCGameSDKStatusCode.SDK_EXIT_CONTINUE == code) {
							// 此加入继续游戏的代码
						} else if (UCGameSDKStatusCode.SDK_EXIT == code) {
							// 在此加入退出游戏的代码
							Log.i("UCGameSDK", "退出SDK");
							loginStep = 0;
							ucSdkDestoryFloatButton();
							System.exit(0);
						}
					}
				});
	}

	/**
	 * 出用户中心界面
	 */
	public static void goTpCenter() {
		try {
			UCGameSDK.defaultSDK().enterUserCenter(instance,
					new UCCallbackListener<String>() {
						@Override
						public void callback(int statuscode, String data) {
							switch (statuscode) {
							case UCGameSDKStatusCode.SUCCESS:
								// 用户管理界面正常关闭，返回游戏的界面逻辑
								break;
							// 未初始化成功。应该先调用初始化
							case UCGameSDKStatusCode.NO_INIT:
								initTp();
								break;
							// 未登录。应该先调用登录功能进行登录
							case UCGameSDKStatusCode.NO_LOGIN:
								goLogin();
								break;
							default:
								break;
							}
						}
					});
		} catch (UCCallbackListenerNullException e) {
			e.printStackTrace();
		}
	}

	/**
	 * 悬浮按钮创建及显示 悬浮按钮必须保证在SDK进行初始化成功之后再进行创建需要在UI线程中调用
	 */
	private static void ucSdkCreateFloatButton() {
		instance.runOnUiThread(new Runnable() {
			public void run() {
				try {
					// 创建悬浮按钮。
					// 创建完成后，并不自动显示，需要调用showFloatButton()方法进行显示或隐藏。
					UCGameSDK.defaultSDK().createFloatButton(instance,
							new UCCallbackListener<String>() {

								@Override
								public void callback(int statuscode, String data) {
									Log.d("SelectServerActivity`floatButton Callback",
											"statusCode == " + statuscode
													+ "  data == " + data);
								}
							});

				} catch (UCCallbackListenerNullException e) {
					e.printStackTrace();
				} catch (UCFloatButtonCreateException e) {
					e.printStackTrace();
				}
			}
		});
	}

	/**
	 * 悬浮按钮显示 悬浮按钮显示需要在UI线程中调用
	 */
	private static void ucSdkShowFloatButton() {
		instance.runOnUiThread(new Runnable() {
			public void run() {
				// 显示悬浮图标，游戏可在某些场景选择隐藏此图标，避免影响游戏体验
				try {
					UCGameSDK.defaultSDK().showFloatButton(instance, 100, 50,
							true);
				} catch (UCCallbackListenerNullException e) {
					e.printStackTrace();
				}
			}
		});
	}

	/**
	 * 悬浮按钮销毁 悬浮按钮销毁需要在UI线程中调用
	 */
	private static void ucSdkDestoryFloatButton() {
		instance.runOnUiThread(new Runnable() {
			public void run() {
				// 悬浮按钮销毁功能
				UCGameSDK.defaultSDK().destoryFloatButton(instance);
			}
		});
	}

	/**
	 * 调用SDK支付功能 如你在调用支付页面时，没有显示正确的支付页面，请检查以下几点：
	 * 1、是否已经提交对应环境的支付回调地址给技术接口人，是否配置到对应的环境中 2、检查pInfo.setServerId()传入的值是否正确。
	 * 在联调环境中进行支付，可使用无效卡进行支付，只需符合卡号卡密长度位数即可
	 * 当卡号卡密全部输入为1时，是支付失败的订单，服务器将会收到订单状态为F的订单信息
	 */
	// int nVal, int nSvrId, String szAcc, String szOrder
	public static void goPay(int nVal, int nSvrId, String szAcc, String szOrder) {
		Log.i("1111UCpay", "1111START");

		PaymentInfo pInfo = new PaymentInfo(); // 创建Payment对象，用于传递充值信息

		// 设置充值自定义参数，此参数不作任何处理，
		// 在充值完成后，sdk服务器通知游戏服务器充值结果时原封不动传给游戏服务器传值，字段为服务端回调的callbackInfo字段
	    pInfo.setCustomInfo(nSvrId + "#" + szOrder);
		//pInfo.setCustomInfo("callback");
		// 当传入一个amount作为金额值进行调用支付功能时，SDK会根据此amount可用的支付方式显示充值渠道
		// 如你传入6元，则不显示充值卡选项，因为市面上暂时没有6元的充值卡，建议使用可以显示充值卡方式的金额
		pInfo.setAmount(nVal);// 设置充值金额，此为可选参数

		try {
			UCGameSDK.defaultSDK().pay(instance, pInfo,
					new UCCallbackListener<OrderInfo>() {
						@Override
						public void callback(int statudcode, OrderInfo orderInfo) {
							if (statudcode == UCGameSDKStatusCode.NO_INIT) {
								// 没有初始化就进行登录调用，需要游戏调用SDK初始化方法
							}
							if (statudcode == UCGameSDKStatusCode.SUCCESS) {
								// 成功充值
								if (orderInfo != null) {
									String ordereId = orderInfo.getOrderId();// 获取订单号
									float orderAmount = orderInfo
											.getOrderAmount();// 获取订单金额
									int payWay = orderInfo.getPayWay();// 获取充值类型，具体可参考支付通道编码列表
									String payWayName = orderInfo
											.getPayWayName();// 充值类型的中文名称
									System.out.print(ordereId + ","
											+ orderAmount + "," + payWay + ","
											+ payWayName);
								}
							}
							if (statudcode == UCGameSDKStatusCode.PAY_USER_EXIT) {
								// 用户退出充值界面。
							}
						}

					});
		} catch (UCCallbackListenerNullException e) {
			e.printStackTrace();
		}
	}
	/**
	 * 提交游戏扩展数据功能，游戏SDK要求游戏在运行过程中，提交一些用于运营需要的扩展数据，这些数据通过扩展数据提交方法进行提交。
	 * 登录游戏角色成功后调用此段
	 */
	public static void TPSubmitUser( int uin,int nSvr,int nLv, String strName, String szZoneName){
		try {
			JSONObject jsonExData = new JSONObject();
			jsonExData.put("roleId", uin);// 玩家角色ID
			jsonExData.put("roleName", strName);// 玩家角色名
			jsonExData.put("roleLevel", nLv);// 玩家角色等级
			jsonExData.put("zoneId", nSvr);// 游戏区服ID
			jsonExData.put("zoneName", szZoneName);// 游戏区服名称
			UCGameSDK.defaultSDK()
					.submitExtendData("loginGameRole", jsonExData);
			Log.i("UCGameSDK", "提交游戏扩展数据功能调用成功");
		} catch (Exception e) {
			e.printStackTrace();
		}
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
		m_nUnZipCnt	= 0;
		Log.d("1111openVersion", "1111START"+szUrl);
		if(szUrl.length() < 5){
			return "";
		}
		StringBuffer buffer = new StringBuffer();
		URL url = new URL(szUrl);
		HttpURLConnection conn = (HttpURLConnection) url.openConnection();
		conn.setConnectTimeout(5 * 1000);
		conn.setRequestMethod("GET");
		if (conn.getResponseCode() != 200)
		{
			return "";
		}
		else {
			InputStreamReader read = new InputStreamReader(conn.getInputStream());
			BufferedReader bufferedReader = new BufferedReader(read);
			String s = null;
			while ((s = bufferedReader.readLine()) != null) {
				buffer.append(s+"\n");
			}
			read.close();
			bufferedReader.close();
		}
		Log.d("1111openVersion", "1111"+buffer.toString());
		return buffer.toString();
	}

	// 下载一个URL文件存在REs位置, 成功返回1
	public static void _httpDownZip(String szUrl,String szSaveName) throws IOException {
		String path = getSDPath() + "/rpgwar/res/" ;
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
			BufferedInputStream bis = new BufferedInputStream(conn.getInputStream());
			ZipInputStream zis = new ZipInputStream(bis);
			ZipEntry entry = null;
			ZipOutputStream zout = new ZipOutputStream(new FileOutputStream(targetPath));
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
	
	public static int TPDownZipFile(final String szUrl,final String szSaveName) throws IOException {
		new Thread(new Runnable() {
			@Override
			public void run() {
				try {
					_httpDownZip(szUrl,szSaveName);
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
						bis  = new BufferedInputStream(zis.getInputStream(entry));
						BufferedOutputStream fos = new BufferedOutputStream(new FileOutputStream(file));

						byte[] b = new byte[2048];
						int bread = 0;
						while ((bread = bis.read(b, 0, 2048)) != -1) {
							fos.write(b, 0, bread);
						}
						fos.close();
						bis.close();
					}

				} catch (Exception e) {
					Log.d("1111UNZIP", "FAIL"+zipPath);
					continue;
				}
			}
			zis.close();
			Log.d("1111UNZIP", "SUCCESS"+m_nUnZipCnt);
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
		if(!file.exists()){
			return true;
		}else{
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

	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		super.onConfigurationChanged(newConfig);
		try {
			UCGameSDK.defaultSDK().showFloatButton(this, 100, 50, true);
		} catch (UCCallbackListenerNullException e) {
			e.printStackTrace();
		}
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();
		UCGameSDK.defaultSDK().destoryFloatButton(this);
	}
}
