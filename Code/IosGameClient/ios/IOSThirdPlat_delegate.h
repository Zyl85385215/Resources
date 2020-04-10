//  FMLayerWebView.h 
//  WebViewDemo
//  Created by Yanghui Liu on 12-6-5. 
//  Copyright (c) 2012Äê FMinutes company. All rights reserved. 
//

#import <UIKit/UIKit.h>
#import "DownjoyDelegate.h"
#import "Downjoy.h"
#include "PaymemtParam.h"

@interface  IosTPDelegate : UIViewController <DownjoyDelegate> {

    Downjoy *_demoVC;
    DownjoyLoginResult *_downjoyLoginResult;
    DownjoyMemberInfoResult *_downjoyMemberInfoResult;
}

@property(retain, nonatomic) Downjoy *demoVC;
@property(retain, nonatomic) DownjoyLoginResult *downjoyLoginResult;
@property(retain, nonatomic) DownjoyMemberInfoResult *downjoyMemberInfoResult;

@end