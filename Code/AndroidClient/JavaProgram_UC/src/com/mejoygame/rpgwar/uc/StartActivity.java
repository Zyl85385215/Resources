package com.mejoygame.rpgwar.uc;


import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;

public class StartActivity extends Activity{
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		this.setContentView(R.layout.splashscreen);
		Runnable runnable = new Runnable() {
			@Override
			public void run() {
				Intent intent = new Intent(StartActivity.this, RpgWar.class);
				startActivity(intent);
				finish();
			}
		};

		Handler handler = new Handler();
		handler.postDelayed(runnable, 3000);
	}
}
