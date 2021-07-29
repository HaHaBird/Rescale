//
// Created by WangGuoku on 2021/5/19.
//
#include <stddef.h>
#include <stdint.h>
#include <jni.h>

#include <android/log.h>
#include <android/bitmap.h>

#include <libswscale/swscale.h>
#include <libavutil/pixfmt.h>
#include "com_sample_rescale_Rescale.h"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,"wgk",__VA_ARGS__)

struct bitmap {
    jobject           jbitmap;
    AndroidBitmapInfo info;
    uint8_t           *buffer;
};
static int lock_bitmap(JNIEnv *env, struct bitmap *bm) {
    int res = AndroidBitmap_getInfo(env, bm->jbitmap, &bm->info);
    if(ANDROID_BITMAP_RESULT_SUCCESS != res) return res;
    else return AndroidBitmap_lockPixels(env, bm->jbitmap, (void **)&bm->buffer);
}

static int unlock_bitmap(JNIEnv *env, struct bitmap *bm) {
    const static struct bitmap null_bm;
    int res = AndroidBitmap_unlockPixels(env, bm->jbitmap);
    *bm = null_bm;
    return res;
}
static inline enum AVPixelFormat pix_fmt(enum AndroidBitmapFormat fmt) {
    /* bitmap formats directly correspond to SkColorType values */
    switch (fmt) {
        case ANDROID_BITMAP_FORMAT_RGBA_8888:
            return AV_PIX_FMT_RGBA;;
        case ANDROID_BITMAP_FORMAT_RGB_565:
            return AV_PIX_FMT_RGB565;
        case ANDROID_BITMAP_FORMAT_A_8:
        default:
            return AV_PIX_FMT_NONE;
    }
}

JNIEXPORT jobject JNICALL Java_com_sample_rescale_Rescale_native_1scale(JNIEnv *env, jclass clazz,
        jobject srcBitmap, jobject dstBitmap, jint sws_algo, jdouble p0, jdouble p1) {
    struct bitmap src = { .jbitmap = srcBitmap };
    struct bitmap dst = { .jbitmap = dstBitmap };
    jobject ret = NULL;

    if(ANDROID_BITMAP_RESULT_SUCCESS == lock_bitmap(env, &src)
            && ANDROID_BITMAP_RESULT_SUCCESS == lock_bitmap(env, &dst)) {
        const uint8_t *src_planes[] = { src.buffer };
        const int src_strides[] = { src.info.stride };
        uint8_t *dst_planes[] = { dst.buffer };
        const int dst_strides[] = { dst.info.stride };
        const double params[] = { p0, p1 };
        struct SwsContext *ctx;

        ctx = sws_getContext(src.info.width, src.info.height, pix_fmt(src.info.format),
                             dst.info.width, dst.info.height, pix_fmt(dst.info.format),
                             sws_algo, NULL, NULL, params);
        if(ctx) {
            int res = sws_scale(ctx, src_planes, src_strides, 0, src.info.height, dst_planes, dst_strides);
            sws_freeContext(ctx);
            if(res > 0) {
                ret = dstBitmap;
            }
        }
    }
    unlock_bitmap(env, &src);
    unlock_bitmap(env, &dst);
    return ret;
}
