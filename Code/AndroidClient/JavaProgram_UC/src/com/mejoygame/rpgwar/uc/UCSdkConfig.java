package com.mejoygame.rpgwar.uc;

/**
 * 游戏参数配置
 * 
 */
public class UCSdkConfig {
	// 以下参数仅供测试。在正式集成SDK时，需要使用正式的id数据。
	// 游戏开发人员可从UC九游开放平台获取自己游戏的参数信息，参考http://game.open.uc.cn/doc/guide_gameid.html
	// 验收及对外发布时，要求必须使用生产环境模式。
	public static int cpId = 33271;//游戏合作厂商标识。 由 UC 游戏中心分配， 用于区别不同的游戏合作厂商
	public static int gameId = 538942;//游戏标识。由 UC 游戏中心分配，用于区别具体的游戏
	public static int serverId = 3125;//游戏服务器标识，又称游戏分区标识。由 UC 游戏中心分配，用于区别一款游戏中不同服务器或分区
	// 值为true时，为调试环境模式，当值为false时，是生产环境模式，验收及对外发布时，要求必须使用生产环境模式
	public static boolean debugMode = false;

}
