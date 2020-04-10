package com.mejoygame.rpgwar.uc;

import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.TimeZone;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.widget.Toast;

public class AlarmReceiver extends BroadcastReceiver {

	private static String mWeek;
//	private static String mHour;
//	private static String mSecond;

	@Override
	public void onReceive(Context context, Intent intent) {
		final Calendar c = Calendar.getInstance();
		c.setTimeZone(TimeZone.getTimeZone("GMT+8:00"));
		mWeek = String.valueOf(c.get(Calendar.DAY_OF_WEEK));

		SimpleDateFormat fmt = new SimpleDateFormat("HH:mm");
		String date = fmt.format(new java.util.Date());
		
//		mHour = String.valueOf(c.get(Calendar.HOUR));
//		mSecond = String.valueOf(c.get(Calendar.SECOND));
//		String date = mHour + ":" + mSecond;
		
		if ("10:00".equals(date)) {
			Toast.makeText(context,
					"摧毁箭塔" + "\n" + "摧毁帝国箭塔，提升全民士气；推塔大战，一触即发！",
					Toast.LENGTH_LONG).show();
		}
		if ("12:00".equals(date)) {
			Toast.makeText(context,
					"天灾来袭" + "\n" + "一大波BOSS正在靠近！天灾来袭，正午经验大狂欢！",
					Toast.LENGTH_LONG).show();
		}
		if ("18:00".equals(date)) {
			Toast.makeText(context,
					"巫妖之怒" + "\n" + "巫妖重生，怒火燎原！最强BOSS，全服挑战！",
					Toast.LENGTH_LONG).show();
		}
		//------------------------------------周一--------------------------------
		if ("1".equals(mWeek)) {
			if ("16:00".equals(date)) {
				Toast.makeText(context,
						"守卫要塞" + "\n" + "艾泽拉斯我的家，保卫家园靠大家！放下仇恨，携手退魔！",
						Toast.LENGTH_LONG).show();
			}
			if ("20:00".equals(date)) {
				Toast.makeText(context,
						"全民国运" + "\n" + "劫镖容易，运镖不易，且运且珍惜！国运开启，步步惊心！",
						Toast.LENGTH_LONG).show();
			}
	    //-------------------------------------周二-------------------------------
		} else if ("2".equals(mWeek)) {
			if ("16:00".equals(date)) {
				Toast.makeText(context,
						"急速赛艇" + "\n" + "让我们荡起双桨，炼星石装入背囊。急速赛艇，狂飚开赛！",
						Toast.LENGTH_LONG).show();
			}
			if ("20:00".equals(date)) {
				Toast.makeText(context,
						"夺取情报" + "\n" + "激情碟中谍，卧底无间道！永恒之井夺取情报活动开启了！",
						Toast.LENGTH_LONG).show();
			}
		//-------------------------------------周三-------------------------------
		} else if ("3".equals(mWeek)) {
			if ("16:00".equals(date)) {
				Toast.makeText(context,
						"捕鱼大赛" + "\n" + "舌尖上的永恒之井：全鱼宴开播！捕鱼大赛，海量经验！",
						Toast.LENGTH_LONG).show();
			}
			if ("20:00".equals(date)) {
				Toast.makeText(context,
						"全民国运" + "\n" + "劫镖容易，运镖不易，且运且珍惜！国运开启，步步惊心！",
						Toast.LENGTH_LONG).show();
			}
		//-------------------------------------周四-------------------------------
		} else if ("4".equals(mWeek)) {
			if ("16:00".equals(date)) {
				Toast.makeText(context,
						"守卫要塞" + "\n" + "艾泽拉斯我的家，保卫家园靠大家！放下仇恨，携手退魔！",
						Toast.LENGTH_LONG).show();
			}
		//-------------------------------------周五-------------------------------
		} else if ("5".equals(mWeek)) {
			if ("16:00".equals(date)) {
				Toast.makeText(context,
						"急速赛艇" + "\n" + "让我们荡起双桨，炼星石装入背囊。急速赛艇，狂飚开赛！",
						Toast.LENGTH_LONG).show();
			}
			if ("20:00".equals(date)) {
				Toast.makeText(context,
						"全民国战" + "\n" + "为了联盟，为了部落，全民国战，一触即发！胜负揭晓！",
						Toast.LENGTH_LONG).show();
			}
		//-------------------------------------周六-------------------------------
		} else if ("6".equals(mWeek)) {
			if ("16:00".equals(date)) {
				Toast.makeText(context,
						"捕鱼大赛" + "\n" + "舌尖上的永恒之井：全鱼宴开播！捕鱼大赛，海量经验！",
						Toast.LENGTH_LONG).show();
			}
			if ("20:00".equals(date)) {
				Toast.makeText(context,
						"军团争霸" + "\n" + "军团争霸，谁与争锋，激情PK,尽在永恒之井军团争霸赛！",
						Toast.LENGTH_LONG).show();
			}
		//-------------------------------------周日-------------------------------
		} else if ("7".equals(mWeek)) {
			
			if ("16:00".equals(date)) {
				Toast.makeText(context,
						"捕鱼大赛" + "\n" + "舌尖上的永恒之井：全鱼宴开播！捕鱼大赛，海量经验！",
						Toast.LENGTH_LONG).show();
			}
			if ("20:00".equals(date)) {
				Toast.makeText(context,
						"甜心轰炸" + "\n" + "周末送福利，好礼捡出来！甜心轰炸，错过再等一周！",
						Toast.LENGTH_LONG).show();
			}
		}

	}

}
