package miwan.game.rpgwar;

import java.util.HashMap;
import java.util.Map;

import android.os.Handler;
import android.os.Message;
import android.widget.Toast;

import com.skymobi.pay.sdk.SkyPayServer;

public class PayHandler extends Handler {
	@Override
	public void handleMessage(Message msg) {
		super.handleMessage(msg);

		if (msg.what == SkyPayServer.MSG_WHAT_TO_APP) {
			String retInfo = (String) msg.obj;
			Map<String, String> map = new HashMap<String, String>();

			//mHinTextView.setText(mHinTextView.getText() + "\n" + "\n"
					//+ retInfo);

			String[] keyValues = retInfo.split("&|=");
			for (int i = 0; i < keyValues.length; i = i + 2) {
				map.put(keyValues[i], keyValues[i + 1]);
			}

			int msgCode = Integer.parseInt(map.get("msg_code"));
			if (msgCode == 100) {
				if (map.get("3rdpay_status") != null) {
					// 绗笁鏂逛粯璐硅繑鍥�
					int payStatus = Integer.parseInt(map
							.get("3rdpay_status"));
					int payPrice = Integer.parseInt(map
							.get("pay_price"));

					if (payStatus == 201) {
						
						Toast.makeText(RpgWar.getInstance(),
								"上次付费尚未完成", Toast.LENGTH_LONG).show();
					} else if (payStatus == 203) {
						Toast.makeText(RpgWar.getInstance(),
								"内存不足", Toast.LENGTH_LONG).show();
					} else if (payStatus == 202) {
						Toast.makeText(RpgWar.getInstance(),
								"apk下载更新失败", Toast.LENGTH_LONG).show();
					} else if (payStatus == 204) {
						Toast.makeText(RpgWar.getInstance(),
								"网络超时", Toast.LENGTH_LONG).show();
					}
					else
					{
						Toast.makeText(RpgWar.getInstance(),
								"已取消支付", Toast.LENGTH_LONG).show();
					}
				}

			} else {
				// 瑙ｆ瀽閿欒鐮�
				int errcrCode = Integer
						.parseInt(map.get("error_code"));
				Toast.makeText(RpgWar.getInstance(),
						"错误", Toast.LENGTH_LONG).show();
			}
		}
	}
}