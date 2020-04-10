#import "IOSThirdPlat_delegate.h"

@interface IosTPDelegate()

@end


@implementation IosTPDelegate



//实现Downjoy协议

-(void) loginSuccess:(DownjoyLoginResult *)loginResult{
    //self.dicDownjoyLoginInfo=dictParm;
    self.downjoyLoginResult=loginResult;
    NSString *content = [NSString stringWithFormat:@"登陆/注册成功\n乐号：%@\n用户名：%@\n昵称：%@\nstate参数：%@\n令牌：%@",
                         loginResult.memberId, loginResult.username,
                         loginResult.nickname, loginResult.state, loginResult.token];
    //[SINGLETON myAlertView:nil message:content delegate:self buttons:nil];
    [self dismissModalViewControllerAnimated:YES];
    
}

//登陆出错或取消回调
-(void) loginError:(DownjoyLoginResult *)loginResult {
    /*传回来的dicParm有五个参数
     error_code：错误代码
     error_msg：错误描述
     state：登陆时客户端传给Downjoy的数据，登陆成功后会原样传回
     */
    //self.dicDownjoyLoginInfo = dicParm;
    NSString *content = [NSString stringWithFormat:@"登陆出错\n错误码：%@\n错误描述：%@\nstate参数：%@",
                         loginResult.errorCode,loginResult.errorMsg,loginResult.state];
    //[SINGLETON myAlertView:nil message:content delegate:self buttons:nil];
    [self dismissModalViewControllerAnimated:YES];
}

//获取用户信息成功回调
- (void) readMemberInfo: (DownjoyMemberInfoResult *)memberInfo {
    /*1.获取用户信息成功时，传回来的dicMemberInfo有五个参数
     avatar_url：用户头像地址
     created_date：建立日期
     error_code：状态码
     username：用户名
     nickname：昵称
     memberId：乐号
     gender：性别
     level：级别
     token：安全令牌
     
     2.获取用户信息失败时，传回来的dicMemberInfo有两个参数
     error_code：错误代码
     error_msg：错误描述
     */
    self.downjoyMemberInfoResult = memberInfo;
    NSString *content=[NSString stringWithFormat:@"mid:%@,username:%@,nickname:%@,token:%@", _downjoyMemberInfoResult.memberId, _downjoyMemberInfoResult.username, _downjoyMemberInfoResult.nickname, _downjoyMemberInfoResult.token];
    
    //NSString *unicodeStr = //[SINGLETON unicodeToUtf8:content];
    //[SINGLETON myAlertView:nil message:unicodeStr delegate:self buttons:nil];
}

//注销回调
- (void) logoutSuccess {
    NSString *content=[NSString stringWithFormat:@"logout ok"];
    //[SINGLETON myAlertView:nil message:content delegate:self buttons:nil];
}

-(void) logoutError:(NSString *)errorCode errorMsg:(NSString *)errorMsg{
    NSString *content=[NSString stringWithFormat:@"logout error:%@, %@", errorCode, errorMsg];
    //[SINGLETON myAlertView:nil message:content delegate:self buttons:nil];
}

//支付取消回调
- (void) payBack {
    [self dismissModalViewControllerAnimated:YES];
}

-(void) memberCenterError:(NSString *)errorCode errorMsg:(NSString *)errorMsg{
    NSString *content = [NSString stringWithFormat:@"进入个人中心出错\n错误码：%@\n错误描述：%@\n",errorCode,errorMsg];
    //[SINGLETON myAlertView:nil message:content delegate:self buttons:nil];
    [self dismissModalViewControllerAnimated:YES];
    
}

//钱包回调
-(void) orderSuccess:(NSString *)orderNo{
    NSString *content = [NSString stringWithFormat:@"支付成功，订单号为:%@\n",orderNo];
    //[SINGLETON myAlertView:nil message:content delegate:self buttons:nil];
    [self dismissModalViewControllerAnimated:YES];
}

-(void) orderError:(NSString *)errorCode errorMsg:(NSString *)errorMsg orderNo:(NSString *)orderNo{
    NSString *content = [NSString stringWithFormat:@"支付失败\n订单号:%@\n错误码：%@\n错误描述：%@\n",orderNo, errorCode,errorMsg];
    //[SINGLETON myAlertView:nil message:content delegate:self buttons:nil];
    [self dismissModalViewControllerAnimated:YES];
}


-(void) loginDownjoy{
    _demoVC =[[Downjoy alloc] initWithBaseInfo:@"407" appKey :@"FmxV2r5D" delegate :self];
    [self presentModalViewController:_demoVC animated:YES];
    [_demoVC downjoyLogin:@"1" state:@"aotianhuanxian"];
    [_demoVC release];
}

-(void) payDownjoy:(int) money serid:(NSString*)exid{
    _demoVC =[[Downjoy alloc] initWithBaseInfo:@"407" appKey :@"FmxV2r5D" delegate :self];
    _demoVC.merchantId=@"254";
    _demoVC.serverSeqNum=@"1";
    [self presentModalViewController:_demoVC animated:YES];
    NSString *order=[_demoVC downjoyOrder:money productName:@"PayDownJoy" extInfo:exid];
    NSLog(@"order:%@", order);
    [_demoVC release];
    
}


@end