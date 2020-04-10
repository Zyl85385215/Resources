package miwan.game.rpgwar;

import java.util.HashMap;
import java.util.Map;

import android.os.Handler;
import android.os.Message;

import com.skymobi.pay.sdk.SkyPayServer;

public class PartnerConfig {

	/*
	 * 应用在斯凯这申请到的APPID，请准确填写，这个影响到后期结算， 填错了，就给别人打工了哦，亲~
	 */
	public static final String APPID = "300001";
	
	/*
	 * 请填写现网merchantPasswd和merchantId
	 * Test env Id && password change it Official when your app release
	 */
	public static final String MERCHANTPASSWD = "zz$r0oiljy";
	public static final String MERCHANTID = "11422";
	
	/*
	 * 系统号 system=300020 Android支付请求（冒泡堂）
	 * 
	 * system=300021 冒泡堂
	 * 
	 * system=300022 逗斗
	 * 
	 * system=300023 开放平台
	 * 
	 * system=300024 支付接入
	 * 
	 * system=300025 公司自研
	 */
	public static final String SYSTEMID = "300024";

	/*
	 * 应用名称和版本号
	 * Test env Id && password change it Official when your app release
	 */
	public static final String APPNAME = "永恒之井";
	public static final String APPVERSION = "1000";
	
	/*
	 * 应用服务端通知地址 第三方付费时必须填写
	 */
	public static final String NOTIFYADDRESS = "http://115.29.233.18:8089";
	
	/*
	 * 0=注册 1=道具 2=积分 3=充值，50=网游小额支付（如果不填，默认是道具）
	 */
	public static final String PAYTYPE = "1";
	
	/*
	 * 是否使用测试环境 release版本，此参数无效
	 */
	public static final String TEST_ENV = "true";

}
