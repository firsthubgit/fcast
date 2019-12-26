//
//  Created by hongruichen on 18-3-1.
//  Copyright (c) 2018 hongruichen. All rights reserved.
//

#ifndef RENDER_ROOTVIEW_H__
#define RENDER_ROOTVIEW_H__

#include <TargetConditionals.h>
#if TARGET_OS_IOS
#import <UIKit/UIKit.h>
#else
#import <Cocoa/Cocoa.h>
#endif

#ifdef RENDER_IMPLEMENT
#define RENDER_EXPORT __attribute__((visibility("default")))
#else
#define RENDER_EXPORT
#endif

typedef struct render_surface_s *render_surface_t;

@protocol VideoView;

@protocol VideoViewDelegate<NSObject>

- (void)onDrawBegin:(id<VideoView>)view;
- (void)onDrawEnd:(id<VideoView>)view;
- (void)onImageSizeChange:(id<VideoView>)view size:(CGSize)size;

@end

@protocol VideoView<NSObject>

typedef enum _SizeType {
  SizeManual = 0,
  SizeMatchParent,
  SizeWrapContent,
} SizeType;

typedef enum _ScaleType {
  ScaleCenter = 0,
  ScaleCenterCrop,
  ScaleCenterInside,
} ScaleType;

typedef struct _Color {
  CGFloat r, g, b, a;
} Color;

@property(nonatomic, readonly) NSString *viewId;
@property(nonatomic, setter=setFrame:) CGRect frame;
@property(nonatomic, setter=setClip:) CGRect clip;
@property(nonatomic, setter=setSizeType:) SizeType sizeType;
@property(nonatomic, setter=setScaleType:) ScaleType scaleType;
@property(nonatomic) BOOL mirrorHorizontal;
@property(nonatomic) Color backgroundColor;
@property(nonatomic, assign) id<VideoViewDelegate> delegate;

@end

#if TARGET_OS_IOS
RENDER_EXPORT @interface RootView
    : UIView
#else
RENDER_EXPORT @interface RootView
    : NSView
#endif

      @property(nonatomic, readonly, getter = renderSurface) render_surface_t renderSurface;
@property(nonatomic, retain) NSString *viewId;
@property(nonatomic, readonly, getter=videoviews) NSArray<id<VideoView>> *videoviews;

- (instancetype)initWithCoder:(NSCoder *)decoder;
- (instancetype)initWithFrame:(CGRect)frame;
- (id<VideoView>)addVideoviewWithSource:(NSString *)srcId;
- (void)removeVideoviewWithSource:(NSString *)srcId;
- (void)removeAllVideoview;
- (void)switchWithFront:(NSString *)srcId;
- (void)switchWithBack:(NSString *)srcId;
- (void)bringToFront:(NSString *)srcId;
- (void)bringToBack:(NSString *)srcId;
- (id<VideoView>)getVideoviewWithSource:(NSString *)srcId;
- (id<VideoView>)videoviewHitTest:(CGPoint)point;
- (void)invalidate;

@end

#endif
