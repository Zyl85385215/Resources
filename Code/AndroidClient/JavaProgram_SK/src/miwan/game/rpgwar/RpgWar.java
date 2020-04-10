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
package miwan.game.rpgwar;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Random;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;
import java.net.HttpURLConnection;  
import java.net.MalformedURLException;
import java.net.URL;  
import java.net.URLConnection;  

import org.cocos2dx.lib.Cocos2dxActivity;

import com.skymobi.pay.app.PayApplication;
import com.skymobi.pay.sdk.SkyPayServer;
import com.skymobi.pay.sdk.SkyPaySignerInfo;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.media.AudioManager;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.os.SystemClock;
import android.telephony.TelephonyManager;
import android.util.Log;
import android.view.KeyEvent;
import android.view.WindowManager;

public class RpgWar extends Cocos2dxActivity{
	public static RpgWar instance;
	public static int	 m_nUnZipCnt	= 0;
	public static int	 m_nMarkCheck	= 0;
	public static SkyPayServer mSkyPayServer = null;
	public static PayHandler mPayHandler = null;
	protected void onCreate(Bundle savedInstanceState){
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		Log.d("111111","11111start!!!!");
		instance	= this;
		super.onCreate(savedInstanceState);
		
		PayApplication payApplication	= new PayApplication();
		payApplication.applicationOnCreat(getApplicationContext());
		
		initTp();
	}
	
    static {
    	System.loadLibrary("iconv");
        System.loadLibrary("game");
    }
    public static RpgWar getInstance()
    {
    	return instance;
    }
    
    public  boolean dispatchKeyEvent(KeyEvent event)
    {    
         if(event.getKeyCode() == KeyEvent.KEYCODE_BACK && event.getAction() == KeyEvent.ACTION_DOWN){
     		 final AlertDialog isExit = new AlertDialog.Builder(this).create();
     		 isExit.setTitle("游戏提示");
     		 isExit.setMessage("确定要退出吗");
     		 
     		 DialogInterface.OnClickListener listener = new DialogInterface.OnClickListener() {
     			 public void onClick(DialogInterface dialog, int which) {
     			       switch (which) {
     			             case AlertDialog.BUTTON1:// "确认"按钮退出程序
     			             
     			            	  finish();
                                   android.os.Process.killProcess(android.os.Process.myPid());
                                   break;   
     			             case AlertDialog.BUTTON2:// "取消"第二个按钮取消对话框
     			            	    isExit.cancel();
     			                    break;
     			             default:
     			                    break;
     			        }
     			 }
     			};
     			// 注册监听
     		 isExit.setButton("确定", listener);
             isExit.setButton2("取消", listener);
     			 // 显示对话框
     		 isExit.show();
     		 return true;
     			 
     	}
     	
     	if(event.getKeyCode() == KeyEvent.KEYCODE_HOME){
    		
    			 
    	    }	 
     	//媒体音量调节
     	AudioManager mAudioManager=(AudioManager)getSystemService(Context.AUDIO_SERVICE);
     	int currentVolume = mAudioManager.getStreamVolume(AudioManager.STREAM_MUSIC);
     	
     	if(event.getKeyCode() == KeyEvent.KEYCODE_VOLUME_UP){
     		
     		if(currentVolume < mAudioManager.getStreamMaxVolume(AudioManager.STREAM_MUSIC)){    
     			
     			mAudioManager.setStreamVolume(AudioManager.STREAM_MUSIC,currentVolume+1,1);
     		}
     		
     		return true; 
     	}
         if(event.getKeyCode()==KeyEvent.KEYCODE_VOLUME_DOWN){
         	if(currentVolume > 0)
         	         mAudioManager.setStreamVolume(AudioManager.STREAM_MUSIC,currentVolume-1,1);
         	return true; 
     	}
     	
     	return super.dispatchKeyEvent(event);
     }
    public String getDeviceId() {
		TelephonyManager tm = (TelephonyManager) this
				.getSystemService(Context.TELEPHONY_SERVICE);
		String deviceId = tm.getDeviceId();
		if(deviceId==null){
			Random random=new Random(System.currentTimeMillis());
			int min=9;
			String temp1=System.currentTimeMillis()+"";
			while(true){
				deviceId=Math.abs(random.nextInt())+"";
				if(deviceId.length()==min)
					break;
			}
			deviceId=deviceId+temp1.substring(0, 6);
		}
		return deviceId;
	}
    //jni code
    public static String getAndDevId() {
		return instance.getDeviceId();
	}
  //取sdcard路径
    public static String getSDPath(){
    	String sdDir=null;
    	sdDir=instance.getBaseContext().getFilesDir().getAbsolutePath();//	Context.getFilesDir();
    	/*
    	Log.d(sdDir,"11111122222");
    	boolean sdCardExist=Environment.getExternalStorageState().equals(android.os.Environment.MEDIA_MOUNTED);
    	if(sdCardExist){
    		sdDir=Environment.getExternalStorageDirectory().getAbsolutePath();	
    	}
    	else
    	{
    		sdDir="/mnt/sdcard2/";
    		File zipFolderT = new File(sdDir);  
            
            if (!zipFolderT.exists()) {  
            	sdDir=Environment.getExternalStorageDirectory().getAbsolutePath();
            }
    	}
    	*/
    	//判断最后一位是否有"/"
    	if(sdDir.substring(sdDir.length()-1).equals("/")){
    		sdDir=sdDir.substring(0, sdDir.length()-1);
        }
    	return sdDir;
    }
    
    public static int  getUnZipCnt()
    {
    	String targetPath	= getSDPath()+"/rpgwar/res/Servercfg.ini";
    	File zipFolder1 = new File(targetPath);  
        if (!zipFolder1.exists() && m_nMarkCheck==0) {
        	m_nMarkCheck	= 1;
            return 9999999;
        }
    	
    	return m_nUnZipCnt;
    }
    
    
    public static void UnZipNetFile() throws IOException
    {
    	m_nUnZipCnt			= 0;
    	String targetPath	= getSDPath()+"/rpgwar/res/";
    	Log.d("1111","111111NETStart");
    	URL url = new URL("http://192.168.1.100:8080/path/rpgwar.zip");//文件地址  
    	HttpURLConnection conn = (HttpURLConnection) url.openConnection();  
    	conn.setConnectTimeout(5* 1000);  
    	conn.setRequestMethod("GET");  
    	if (conn.getResponseCode() != 200) throw new RuntimeException("请求url失败");  
    	Log.d("1111","111111NETOK");
    	InputStream is = conn.getInputStream();  
    	ZipInputStream zis = new ZipInputStream(is);  
        ZipEntry entry = null;  
        while ((entry = zis.getNextEntry()) != null) {  
        	
            String zipPath = entry.getName();  
            Log.d(zipPath,"111111NETProcess");
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
                while ((bread = zis.read(b,0,2048)) != -1) {  
                    fos.write(b,0,bread); 
                    
                }
                fos.close();  
            }  
        }  
        zis.close();  
        is.close();  
        m_nUnZipCnt	= -1;
    }
    
    public static void UnZipRes(){
    	m_nUnZipCnt			= 0;
    	String targetPath	= getSDPath()+"/rpgwar/res/";
    	
    	Log.d("1111","111111UNZIPSTART");
        
        File zipFolder1 = new File(targetPath);  
        
        if (!zipFolder1.exists()) {  
            if(zipFolder1.mkdirs())
            	Log.d(targetPath,"11111122222OK");
            else
            	Log.d(targetPath,"11111122222FAIL");
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
                        while ((bread = zis.read(b,0,2048)) != -1) {  
                            fos.write(b,0,bread); 
                            
                        }
                        fos.close();  
    
                    }  
  
    
                } catch (Exception e) {  
                    continue;  
                }  
            }  
            zis.close();  
            is.close();  
            m_nUnZipCnt	= -1;
        } catch (Exception e) {  
            e.printStackTrace();  
        }
    }
    
    public static int	getTPID()
    {
    	return 11;
    }
    public static String	getLoginID()
    {
    	return "";
    }
    public static String	getLoginKey()
    {
    	return "";
    }
    
    //初始化第三方平台的东西
    public static int  initTp()
    {
    	//初始化第三方平台信息,放在网络正常之后
		mPayHandler = new PayHandler();
		mSkyPayServer = SkyPayServer.getInstance();

		String startUpInfo = "payMethod=3rd&appId=" + PartnerConfig.APPID;

		mSkyPayServer.startUp(instance, startUpInfo);
		int initRet = mSkyPayServer.init(mPayHandler);
		if (SkyPayServer.PAY_RETURN_SUCCESS == initRet) {
			Log.i("skymobipay", "1111111OK!!!!!!");
			return 1;
		} else if (SkyPayServer.PAY_RETURN_PAYING_ERROR == initRet) {
			Log.i("skymobipay", "1111111ERROR!!!!!");
		} else if (SkyPayServer.PAY_RETURN_FAILED == initRet) {
			Log.i("skymobipay", "1111111FAILED!!!!!");
		}
		return 0;
    }
    //账号登陆接口
    public static void	goLogin()
    {
    	
    }
  //账号登陆接口
    public static void	goBBS(String szID,String szPswd)
    {
    	Uri uri = Uri.parse("http://www.baidu.com");  
    	Intent intent = new Intent(Intent.ACTION_VIEW, uri);  
    	//Intent intent = new Intent(); 
		//intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
    	//intent.setClass(instance, SKPayActivity.class);
    	instance.startActivity(intent);
    }
    //付费接口
    public static void	goPay(int nVal,int nSvrId,String szAcc)
    {
    	Log.i("11111skymobipay", "1111111START");
    	String paymethod = "3rd";

		String orderId = SystemClock.elapsedRealtime() + "";

		SkyPaySignerInfo skyPaySignerInfo = new SkyPaySignerInfo();

		skyPaySignerInfo.setMerchantPasswd(PartnerConfig.MERCHANTPASSWD);
		skyPaySignerInfo.setMerchantId(PartnerConfig.MERCHANTID);
		skyPaySignerInfo.setAppId(PartnerConfig.APPID);
		skyPaySignerInfo.setNotifyAddress(PartnerConfig.NOTIFYADDRESS);
		skyPaySignerInfo.setAppName(PartnerConfig.APPNAME);
		skyPaySignerInfo.setAppVersion(PartnerConfig.APPVERSION);

		skyPaySignerInfo.setPayType(PartnerConfig.PAYTYPE);
		String strPrice="";
		strPrice	= strPrice.format("%d", nVal);
		String strSvrID="";
		strSvrID	= strSvrID.format("%d", nSvrId);
		Log.i("skymobipay", "1111111AAA"+strPrice+"BBB"+strSvrID);
		skyPaySignerInfo.setPrice(strPrice+"00");
		skyPaySignerInfo.setOrderId(orderId);

		skyPaySignerInfo.setReserved1(szAcc, false);
		skyPaySignerInfo.setReserved2(strSvrID, false);
		skyPaySignerInfo.setReserved3("ok", true);
		String signOrderInfo = RpgWar.mSkyPayServer
				.getSignOrderString(skyPaySignerInfo);

		String mOrderInfo = "payMethod" + "=" + paymethod + "&"
				+ "systemId" + "=" + PartnerConfig.SYSTEMID + "&" + signOrderInfo;

		String orderDesc = "";

		if ("3rd".equals(paymethod)) {
			orderDesc = "test";
			mOrderInfo += "&" + "productName" + "=" + "充值"+strPrice+"元" + "&"
					+ "orderDesc" + "=" + orderDesc;
		}

		String channelId = "1_zhiyifu_";
		if (channelId != null) {
			mOrderInfo += "&" + "channelId" + "=" + channelId;
		}

		mOrderInfo += "&" + "testEnvironment" + "=" + PartnerConfig.TEST_ENV;
		Log.i("skymobipay", "1111111try"+mOrderInfo);
		int payRet = RpgWar.mSkyPayServer.startActivityAndPay(
			instance, mOrderInfo);

		if (SkyPayServer.PAY_RETURN_SUCCESS == payRet) {
			Log.i("skymobipay", "1111111222222222OK!");
		} else if (SkyPayServer.PAY_RETURN_UNINIT_ERROR == payRet) {
			Log.i("skymobipay", "111111122222ERROR");
		} else if (SkyPayServer.PAY_RETURN_FAILED == payRet) {
			Log.i("skymobipay", "111111222221FAILED!");
		} else if (SkyPayServer.PAY_RETURN_PAYING_ERROR == payRet) {
			Log.i("skymobipay", "111111122222PAYERROR");
		}
    	//Uri uri = Uri.parse("http://www.baidu.com");  
    	//Intent intent = new Intent(Intent.ACTION_VIEW, uri);  
    	
    	//Intent intent = new Intent(); 
		//intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
    	//intent.setClass(instance, SKPayActivity.class);
    	//instance.startActivity(intent);
    }
    public static void initResToSDCard()
    {
    	new Thread(new Runnable() {			
			@Override
			public void run() {
				UnZipRes();
				/*
				try {
					UnZipNetFile();
					
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}*/
			}
    	}).start();
    }

}
