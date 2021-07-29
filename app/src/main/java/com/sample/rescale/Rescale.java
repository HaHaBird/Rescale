package com.sample.rescale;

import android.graphics.Bitmap;

public class Rescale {
    static {
        System.loadLibrary("Rescale");
    }

    public static Bitmap rescale(Bitmap src, Bitmap dst, AlgoParametrized1 algo, double p0, double p1) {
        return native_scale(src, dst, algo.flag, p0, p1);
    }

    private static native Bitmap native_scale(Bitmap src, Bitmap dst, int algo, double p0, double p1);

    public enum AlgoParametrized1 {
        /* swscale.h: For SWS_GAUSS param[0] tunes the exponent and thus cutoff frequency */
        GAUSS(0x80),
        /* swscale.h: For SWS_LANCZOS param[0] tunes the width of the window function */
        LANCZOS(0x200);

        public final int flag;

        AlgoParametrized1(int flag) {
            this.flag = flag;
        }
    }
}
