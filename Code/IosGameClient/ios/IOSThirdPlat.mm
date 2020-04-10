// 
//  FMLayerWebView.mm 
//  WebViewDemo 
// url=http://update.176sy.com/hx/ ipwai=221.123.179.26 ipnei=210.14.78.205nbcs
//  Created by Yanghui Liu on 12-6-5. 
//  Copyright (c) 2012?í FMinutes company. All rights reserved. 
//  
#import "IOSThirdPlat.h" 
//#import "PayPageViewController.h"
#include "IOSThirdPlat_delegate.h"
//#import <ShareSDK/ShareSDK.h>
#include "EAGLView.h"
#import <QuartzCore/CALayer.h>

#import "FMUIWebViewBridge.h"
#import "Utility.h"
#import "Constants.h"

//#define UC_SDK
//#define PPS_SDK
//#define N91_SDK
//#define TB_SDK
//#define DJ_SDK
//#define IAP_SDK
#define KY_SDK
//#define SYTX_SDK

#ifdef KY_SDK

//#import "MacAddressObject.h"
#import "ky_inst_check.h"
char * macaddress()

{
    
    int                    mib[6];
    
    size_t                len;
    
    char                *buf;
    
    unsigned char        *ptr;
    
    struct if_msghdr    *ifm;
    
    struct sockaddr_dl    *sdl;
    
    
    
    mib[0] = CTL_NET;
    
    mib[1] = AF_ROUTE;
    
    mib[2] = 0;
    
    mib[3] = AF_LINK;
    
    mib[4] = NET_RT_IFLIST;
    
    
    
    if ((mib[5] = if_nametoindex("en0")) == 0) {
        
        printf("Error: if_nametoindex error/n");
        
        return NULL;
        
    }
    
    
    
    if (sysctl(mib, 6, NULL, &len, NULL, 0) < 0) {
        
        printf("Error: sysctl, take 1/n");
        
        return NULL;
        
    }
    
    
    
    if ((buf = (char *)malloc(len)) == NULL) {
        
        printf("Could not allocate memory. error!/n");
        
        return NULL;
        
    }
    
    
    
    if (sysctl(mib, 6, buf, &len, NULL, 0) < 0) {
        
        printf("Error: sysctl, take 2");
        
        return NULL;
        
    }
    
    
    
    ifm = (struct if_msghdr *)buf;
    
    sdl = (struct sockaddr_dl *)(ifm + 1);
    
    ptr = (unsigned char *)LLADDR(sdl);
    
    //NSString *outstring = [NSString stringWithFormat:@"%02x:%02x:%02x:%02x:%02x:%02x", *ptr, *(ptr+1), *(ptr+2), *(ptr+3), *(ptr+4), *(ptr+5)];
    char *macbuf = (char *)malloc(18);
    sprintf(macbuf, "%02x:%02x:%02x:%02x:%02x:%02x", *ptr, *(ptr+1), *(ptr+2), *(ptr+3),*(ptr+4),*(ptr+5));
    free(buf);
    return macbuf;
    //return [outstring uppercaseString];
}

int IosThirdPlat::init_KY()
{
    char * appid = (char *)[[[NSBundle mainBundle] bundleIdentifier] UTF8String];
    char * appver = (char *)[[[[NSBundle mainBundle] infoDictionary] objectForKey:(NSString *)kCFBundleVersionKey] UTF8String];
    char * devUuid = NULL;
    //MacAddressObject * obj = [[MacAddressObject alloc] init];
    //char * devmac = (char *)[[obj macaddress] UTF8String];
    int ret;
    ret = ky_installed();
    char *devmac = macaddress();
    NSString *str = [[NSString stringWithCString:devmac encoding:NSUTF8StringEncoding] uppercaseString];
    free(devmac);
    char * strmac = (char *)[str UTF8String];

    
    NSLog(@"%@", str);
    ret = ky_report(appid, appver, devUuid, strmac);
    NSLog(@"ky_report result is %d, %s", ret, devmac);
    ret = ky_mark_reported();
    NSLog(@"ky_report_mark result is %d", ret);
    ret = ky_installed();
    NSLog(@"ky_installed result is %d", ret);
    return 0;
    
}
#endif

#ifdef DJ_SDK
#import <DownjoySDK_framework/DJPlatform.h>
//#import <DownjoySDK_framework/DJPlatformMemberInfo.h>
#import <DownjoySDK_framework/DJPlatformNotify.h>
#endif

#ifdef UC_SDK
#import <UCGameSdk/UCGameSdk.h>
#endif

#ifdef PPS_SDK
#import <PPAppPlatformKit/PPAppPlatformKit.h>
#endif

#ifdef TB_SDK
#import "TBPlatform.h"
#endif

#ifdef N91_SDK
//#include "ky_inst_check.h"
#include <NdComPlatform/NdComPlatform.h>

#endif



IosTPDelegate*  pTPDelegate;
extern int     g_bSpecialFlag;



#ifdef IAP_SDK
MyIAPStoreObServer*  pTPDelegateIAP;
#endif
IosThirdPlat::IosThirdPlat(){
    pTPDelegate = [IosTPDelegate alloc];
    nLoginStep=0;
    bCloseSubView   = false;
    
#ifdef DJ_SDK
    [[DJPlatform defaultDJPlatform] setAppId:@"407"];
    [[DJPlatform defaultDJPlatform] setAppKey:@"FmxV2r5D"];
    [[DJPlatform defaultDJPlatform] setMerchantId:@"254"];
    [[DJPlatform defaultDJPlatform] setServerId:@"1"];
    [[DJPlatform defaultDJPlatform] setTapBackgroundHideView:YES];
    
    [[NSNotificationCenter defaultCenter] addObserver:pTPDelegate selector:@selector(dealDJPlatformLoginRsltNotify:) name:kDJPlatformLoginResultNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:pTPDelegate selector:@selector(dealDJPlatformLogoutRsltNotify:) name:kDJPlatformLogotResultNotification object:nil];
    
    //[[DJPlatform defaultDJPlatform] DJLogin];
#endif

#ifdef UC_SDK
    UCGameSdk *sdk = [UCGameSdk defaultSDK];
    
    //sdk.cpId = 193;          // 将 UC 分配的 Cp ID 赋值给 sdk.cpId 属性
    //sdk.gameId = 64414;       // 将 UC 分配的 Game ID 赋值给 sdk.gameId 属性
    //sdk.serverId = 1017;     // 将 UC 分配的 Server ID 赋值给 sdk.serverId 属性
    
    sdk.cpId = 23047;
    sdk.gameId = 505469;
    sdk.serverId = 1810;
    
    sdk.isDebug = NO;  // 是否启用调试模式，如果启用调试模式，SDK访问的服务器将SDK的测试服务器
    sdk.logLevel = UCLOG_LEVEL_DEBUG;   // 日志级别，控制sdk自身日志的输出
    sdk.allowGameUserLogin = NO;  // 是否允许官方账号登录，如果允许，请传 YES
    //sdk.allowChangeAccount = YES;
    sdk.gameUserName = @"幻仙";           // 游戏名称
    sdk.gameName    = @"幻仙";
    sdk.orientation = UC_LANDSCAPE;
    
    [sdk initSDK];      // 开始初始化操作
#endif
    
#ifdef TB_SDK
    [TBPlatform defaultPlatform].appid  = @"100000";
#endif
    
#ifdef PPS_SDK
    [[PPAppPlatformKit sharedInstance] setAppId:492 AppKey:@"9c4a4f2a8c188fd0d958c2dad84a1e3"];
    //[[PPAppPlatformKit sharedInstance] setIsNSlogData:YES];
    //[[PPAppPlatformKit sharedInstance] setRechargeAmount:@"10"];
    [[PPAppPlatformKit sharedInstance] setIsGetBillNo:NO];
    //[[PPAppPlatformKit sharedInstance] setIsLogOutPushLoginView:YES];
    [[PPAppPlatformKit sharedInstance] setIsOpenRecharge:YES];
    [[PPAppPlatformKit sharedInstance] setCloseRechargeAlertMessahe:@"关闭充值提示语"];
    [PPUIKit sharedInstance];
    
    //[[NSNotificationCenter defaultCenter] addObserver:pTPDelegate selector:@selector(loginCallBack:)
                                                 //name:PP_CLIENT_LOGIN_NOTIFICATION object:nil];
#endif
    
#ifdef N91_SDK
    [[NdComPlatform defaultPlatform] setAppId:106141];
	[[NdComPlatform defaultPlatform] setAppKey:@"c6e70546cf02c13541532f702335ec278f18a8ff75d035b3"];
	
    //设置是否自动旋转
	[[NdComPlatform defaultPlatform] NdSetAutoRotation:NO];
	//设定平台为横屏或者竖屏
    [[NdComPlatform defaultPlatform] NdSetScreenOrientation:UIDeviceOrientationLandscapeLeft];
	//[[NdComPlatform defaultPlatform] NdSetDebugMode:0];
	
    
    //[[NdComPlatform defaultPlatform] NdAppVersionUpdate:0 delegate:self];
    
    [[NSNotificationCenter defaultCenter] addObserver:pTPDelegate  selector:@selector(SNSLoginResult:) name:(NSString *)kNdCPLoginNotification object:nil];

#endif
#ifdef KY_SDK
    init_KY();//songwei
    //[pTPDelegate init_KY];
#endif
    
#ifdef IAP_SDK
    //MyIAPStoreObServer *tempObserver = [[MyIAPStoreObServer alloc] init];
    pTPDelegateIAP = [[MyIAPStoreObServer alloc] init];
    //[pTPDelegateIAP loadStore];
    
    //[[SKPaymentQueue defaultQueue] addTransactionObserver:tempObserver];
    //[tempObserver release];
#endif

}




IosThirdPlat::~IosThirdPlat(){
    [pTPDelegate release];
}

void IosThirdPlat::InitKY()
{
    //if(ky_installed()==0)
    //ky_mark_reported();
    
}

void IosThirdPlat::TP_RqPay(int nVal,int nSerID)
{
    szOrder_No=NULL;
    nMoney  = nVal;
    
#ifdef IAP_SDK
    switch (nVal) {
        case 6:
            pTPDelegateIAP.szProductId  = @"com.aotian.IosVirSword.pay1";
            break;
        case 30:
            pTPDelegateIAP.szProductId  = @"com.aotian.IosVirSword.pay2";
            break;
        case 68:
            pTPDelegateIAP.szProductId  = @"com.aotian.IosVirSword.pay3";
            break;
        case 198:
            pTPDelegateIAP.szProductId  = @"com.aotian.IosVirSword.pay4";
            break;
        case 328:
            pTPDelegateIAP.szProductId  = @"com.aotian.IosVirSword.pay5";
            break;
        case 648:
            pTPDelegateIAP.szProductId  = @"com.aotian.IosVirSword.pay6";
            break;
    }
    
    [pTPDelegateIAP loadStore];
    
#endif
    
#ifdef N91_SDK
    CFUUIDRef theUUID = CFUUIDCreate(NULL);
    CFStringRef guid = CFUUIDCreateString(NULL, theUUID);
    CFRelease(theUUID);
    NSString *uuidString = [((NSString *)guid) stringByReplacingOccurrencesOfString:@"-" withString:@""];
    CFRelease(guid);
    
    [[NdComPlatform defaultPlatform] NdUniPayForCoin:[uuidString lowercaseString] needPayCoins:nVal payDescription:[NSString stringWithFormat:@"%d",nSerID]];
    //[[NdComPlatform defaultPlatform] NdEnterRecharge:0 content:@""];
    //[[NdComPlatform defaultPlatform] NdUniPayForCoin:[uuidString lowercaseString] needPayCoins:0 payDescription:@"支付描述内容(iOS)"];
#endif
    
#ifdef TB_SDK
    if([TBPlatform defaultPlatform].isLogined)
    {
        //[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(payStatusNotification:) name:TBPlatformPayStatusNotification object:nil];
        [[TBPlatform defaultPlatform] enterPlatform:@"test by server"];
    }
#endif
    
#ifdef UC_SDK
    NSString *strVal = [NSString stringWithFormat:@"%d", nVal];
    NSDictionary *dict = [NSDictionary dictionaryWithObjectsAndKeys:
                          [NSNumber numberWithBool:YES],
                          UCG_SDK_KEY_PAY_ALLOW_CONTINUOUS_PAY,
                          @"TEST", UCG_SDK_KEY_PAY_CUSTOM_INFO,
                          @"TEST", UCG_SDK_KEY_PAY_ROLE_ID,
                          @"TEST", UCG_SDK_KEY_PAY_ROLE_NAME,
                          @"TEST", UCG_SDK_KEY_PAY_GRADE,
                          strVal, UCG_SDK_KEY_PAY_AMOUNT, nil];
    [[UCGameSdk defaultSDK] payWithPaymentInfo:dict];
#endif
    
    
    ////[NSString initWithCString:szTmp encoding:NSASCIIStringEncoding];
    
#ifdef DJ_SDK
    NSString* str   = [NSString stringWithFormat:@"%lu",nSerID];
    //NSString *order=[_demoVC downjoyOrder:money productName:@"PayDownJoy" extInfo:str];
    [[DJPlatform defaultDJPlatform] DJPayment:nMoney productName:@"游戏充值" extInfo:str];
    //[[[[UIApplication sharedApplication] delegate] window] addSubview:pTPDelegate.view];
    //[pTPDelegate payDownjoy:nMoney serid:nSerID];
    //[[pTPDelegate view] removeFromSuperview];
    //bCloseSubView=true;
#endif
    
    
    //PPS
    //兑换道具处理
#ifdef PPS_SDK
    PPExchange *exchange = [[PPExchange alloc] init];
    //查询余额
    int money = [exchange ppSYPPMoneyRequest];
    if (money == -2) {
        [exchange release];
        return;
    }
    
    NSString* pPPSMoney   = [NSString stringWithFormat:@"%lu",nVal];
    NSString* pPPSServer   = [NSString stringWithFormat:@"%lu",nSerID];
    if(money >= nVal){
        
        //如果PP币余额大于道具的金额。就直接调用兑换接口进行兑换
        int time = [[NSDate date] timeIntervalSince1970];
        NSString *billNO = [[NSString alloc] initWithFormat:@"%d",time];
        
        //请求兑换道具
        [exchange ppExchangeToGameRequestWithBillNo:billNO Amount:pPPSMoney RoleId:pPPSServer ZoneId:0];
        
        [billNO release];
    }else {
        int time = [[NSDate date] timeIntervalSince1970];
        NSString *billNO = [[NSString alloc] initWithFormat:@"%d",time];
        
        //调出充值并且兑换接口
        [[PPWebView sharedInstance] rechargeAndExchangeWebShow:billNO BillNoTitle:@"pay" PayMoney:pPPSMoney
                                                        RoleId:pPPSServer ZoneId:0];
        [billNO release];
    }
    [exchange release];
#endif
//add songwei 快用支付
#ifdef KY_SDK
    NSString *szUrl = [[NSString alloc] autorelease];
    NSMutableDictionary *params = [[NSMutableDictionary alloc] init];
    NSString *strVal = [NSString stringWithFormat:@"%d", nVal];
    NSString *strSid = [NSString stringWithFormat:@"%d", nSerID];
    
    [params setObject:strVal forKey:@"fee"];
    [params setObject:strSid forKey:@"gamesvr"];
    [szUrl initWithString:[Utility getAcessUrlString:params]];
    FMLayerWebView::SFN_SetNextUrl((char *)[szUrl UTF8String]);
    
	FMLayerWebView* pWeb    = FMLayerWebView::create();
	cocos2d::CCScene* pSc	= cocos2d::CCDirector::sharedDirector ()->getRunningScene();
	pWeb->setPosition(CCPointZero);
	pSc->addChild(pWeb);
#endif
}

bool IosThirdPlat::TP_OPenCenter()
{//Open center
#ifdef UC_SDK
    //[UCGameSdk defaultSDK].isHidePayHistoryEntrance = NO;
    // 是否允许用户快速换号(即在SDK的个人中心中直接注销当前用户，重新显示登录界面)，如果允许，请监听用户注销消息
    //[UCGameSdk defaultSDK].allowChangeAccount = NO;
    
    // 进入个人中心
    [[UCGameSdk defaultSDK] enterUserCenter];
    return true;
#endif
#ifdef N91_SDK
    [[NdComPlatform defaultPlatform] NdEnterPlatform:0];
    //[[NdComPlatform defaultPlatform] NdEnterAppBBS];
    return true;
#endif
    
    return false;
}

int IosThirdPlat::TP_GetPlat()
{//Open center
#ifdef DJ_SDK
    return 1;
#endif
#ifdef UC_SDK
    return 2;
#endif
#ifdef PPS_SDK
    return 3;
#endif
#ifdef KY_SDK
    return 5;
#endif
#ifdef N91_SDK
    return 6;
#endif
#ifdef IAP_SDK
    return 10;
#endif
#ifdef SYTX_SDK
    return 11;
#endif
    
    return 0;
}

//#import "UMSocialSnsService.h"
void IosThirdPlat::TP_ShareSDK(char* szTxt)
{
    /*
    NSString* str   = [NSString stringWithUTF8String: szTxt];
    
    id<ISSContent> publishContent = [ShareSDK content:str defaultContent:@"111" image:nil title:@"2222" url:@"http://www.baidu.com" description:@"555555" mediaType:SSPublishContentMediaTypeNews];
    
    id<ISSContainer> container = [ShareSDK container];
    [container setIPadContainerWithView:[EAGLView sharedEGLView] rect:CGRectMake(0,0,1024,300) arrowDirect:UIPopoverArrowDirectionUp];
    
    [ShareSDK showShareActionSheet:container shareList:nil content:publishContent statusBarTips:YES authOptions:nil shareOptions:nil result:^(ShareType type, SSPublishContentState state, id<ISSStatusInfo> statusInfo, id<ICMErrorInfo> error, BOOL end) {
        if(state == SSPublishContentStateSuccess)
        {
            
        }
        else if(state == SSPublishContentStateFail)
        {
            
        }
    }];
    */
    return;
}

void IosThirdPlat::TP_Login()
{
    //UMSocialIconActionSheet * iconActionSheet   = [[UMSocialControllerService defaultControllerService] getSocialIconActionSheetInController:pTPDelegate];
    //[iconActionSheet showInView: pTPDelegate.view];
    nLoginStep	= 1;
    //szToken=NULL;
    nLoginMID=0;
    
#ifdef TB_SDK
    [[TBPlatform defaultPlatform] login:YES];
#endif
    
#ifdef PPS_SDK
    [[PPLoginView sharedInstance] showLoginViewByRight];
#endif
    
#ifdef N91_SDK
    //[[NdComPlatform defaultPlatform] NdSetScreenOrientation:UIDeviceOrientationLandscapeRight];
    [[NdComPlatform defaultPlatform] NdSwitchAccount];
#endif
    
#ifdef UC_SDK
    g_bSpecialFlag  = 1;
    [[UCGameSdk defaultSDK] login];
    g_bSpecialFlag  = 0;
#endif

#ifdef DJ_SDK
    pTPDelegate.nStep   = 0;
    [[DJPlatform defaultDJPlatform] DJLogin];
#endif
    
#ifdef KY_SDK
    
#endif
}
void IosThirdPlat::TP_CheckCloseView()
{
#ifdef DJ_SDK
    //if(nLoginStep >= 1 || bCloseSubView)
        //[[pTPDelegate view] removeFromSuperview];
#endif
    
#ifdef IAP_SDK
    int nVal    = pTPDelegateIAP.nStep;
    if(nVal == 2)
    {//PAY OK
        nPayStep    = 2;
        szOrder_No  = pTPDelegateIAP.szOrder.cString;
        pTPDelegateIAP.nStep = 3;
    }
    if(nVal == 9)
    {
        nPayStep    = 9;
        pTPDelegateIAP.nStep = 3;
    }
    
#endif
    
    if(nLoginStep == 1)
    {
#ifdef UC_SDK
        NSString* pUCStr    = [[UCGameSdk defaultSDK] sid];
        if(pUCStr && pUCStr.length > 5)
        {
            nLoginMID   = 0;
            szToken  = strdup([pUCStr cString]);
            nLoginStep  = 2;
        }
#endif
        
#ifdef N91_SDK
      if([NdComPlatform defaultPlatform].isLogined)
      {
          NSString* pN91Str     = [NdComPlatform defaultPlatform].sessionId;
          szToken       = strdup([pN91Str cString]);
          if(strlen(szToken)> 5)
          {
              NSString* pN91ID     = [NdComPlatform defaultPlatform].loginUin;
              nLoginMID   = pN91ID.intValue;
              nLoginStep  = 2;
          }
      }
#endif
        
#ifdef TB_SDK
        if([TBPlatform defaultPlatform].isLogined)
        {
            TBPlatformUserInfo* info    = [[TBPlatform defaultPlatform] getUserInfo];
            
        }
#endif
        
#ifdef PPS_SDK
        //if([[PPAppPlatformKit sharedInstance] currentUserId])
        //{
            //nLoginMID   = [[PPAppPlatformKit sharedInstance] currentUserId];
            
            //nLoginStep = 2;
        //}
#endif
        
#ifdef DJ_SDK
        //if([[DJPlatform defaultDJPlatform] DJIsLogin])
        if(pTPDelegate.nStep == 1)
        {
            nLoginMID   = [pTPDelegate nMemID];
            if(pTPDelegate.szToken)
            {
                szToken  = strdup([[pTPDelegate szToken] cString]);
                if(strlen(szToken) > 5)
                    nLoginStep  = 2;
            }


        }
#endif
    }
}


