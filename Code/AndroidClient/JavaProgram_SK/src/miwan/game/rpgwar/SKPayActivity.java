package miwan.game.rpgwar;

import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.os.SystemClock;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;

import com.skymobi.pay.sdk.SkyPayServer;
import com.skymobi.pay.sdk.SkyPaySignerInfo;

public class SKPayActivity extends Activity  {
	private TextView mHinTextView = null;
	private PayHandler mPayHandler = null;
	private Button mPayButton = null;
	private String mOrderInfo = "";
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		Log.i("skymobipay", "1111111222222222");
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		mHinTextView = (TextView) findViewById(R.id.hint_textview);
		mPayButton = (Button) findViewById(R.id.button1);
		
		
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

		skyPaySignerInfo.setPrice("200");
		skyPaySignerInfo.setOrderId(orderId);

		String reserved1 = "1";
		String reserved2 = "2";
		String reserved3 = "1|=2/3";

		skyPaySignerInfo.setReserved1(reserved1, false);
		skyPaySignerInfo.setReserved2(reserved2, false);
		skyPaySignerInfo.setReserved3(reserved3, true);
		String signOrderInfo = RpgWar.mSkyPayServer
				.getSignOrderString(skyPaySignerInfo);

		Log.i("skymobipay", "11111signOrderInfo:" + signOrderInfo);
		mOrderInfo = "payMethod" + "=" + paymethod + "&"
				+ "systemId" + "=" + PartnerConfig.SYSTEMID + "&" + signOrderInfo;

		String orderDesc = "";

		if ("3rd".equals(paymethod)) {
			orderDesc = "test";
			mOrderInfo += "&" + "productName" + "=" + "充值6元" + "&"
					+ "orderDesc" + "=" + orderDesc;
		}

		String channelId = "1_zhiyifu_";
		if (channelId != null) {
			mOrderInfo += "&" + "channelId" + "=" + channelId;
		}

		mOrderInfo += "&" + "testEnvironment" + "=" + PartnerConfig.TEST_ENV;
		
		
		mPayButton.setOnClickListener(new OnClickListener() {

			public void onClick(View v) {
				
				//Intent intent = new Intent();
		    	//Uri uri = Uri.parse("http://www.baidu.com");  
		    	//Intent it = new Intent(Intent.ACTION_VIEW, uri);  
		    	//this.startActivity(it);
		    	Log.i("1111111skymobipay", "111111"+mOrderInfo);
				int payRet = RpgWar.mSkyPayServer.startActivityAndPay(
						SKPayActivity.this, mOrderInfo);
				Log.i("111skymobipay", "111111payRet:" + payRet);

				if (SkyPayServer.PAY_RETURN_SUCCESS == payRet) {
					// 鎺ュ彛璋冪敤鎴愬姛
					Log.i("skymobipay", "1111111222222222OK!");
				} else if (SkyPayServer.PAY_RETURN_UNINIT_ERROR == payRet) {
					// 鏈垵濮嬪寲锛坕nit鎺ュ彛鏈皟鐢ㄦ垨璋冪敤澶辫触锛�
					Log.i("skymobipay", "111111122222ERROR");
				} else if (SkyPayServer.PAY_RETURN_FAILED == payRet) {
					// 浼犲叆鍙傛暟鏈夎
					Log.i("skymobipay", "111111222221FAILED!");
				} else if (SkyPayServer.PAY_RETURN_PAYING_ERROR == payRet) {
					// 鏈嶅姟姝ｅ浜庝粯璐圭姸鎬�
					Log.i("skymobipay", "111111122222PAYERROR");
				}
			}
		});
		

	}
	
}
