package com.example.flamapp;

import android.opengl.GLSurfaceView;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
import java.nio.ByteBuffer;

public class MainRenderer implements GLSurfaceView.Renderer {

    private final MainGLSurfaceView glSurfaceView;

    static {
        System.loadLibrary("native-lib");
    }

    public MainRenderer(MainGLSurfaceView view) {
        this.glSurfaceView = view;
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        nativeOnSurfaceCreated();
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        nativeOnSurfaceChanged(width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        nativeOnDrawFrame();
    }

    public void requestRender() {
        glSurfaceView.requestRender();
    }

    // Native methods
    public static native void nativeOnSurfaceCreated();
    public static native void nativeOnSurfaceChanged(int width, int height);
    public static native void nativeOnDrawFrame();
    public static native void nativeOnFrame(int width, int height,
                                            ByteBuffer yBuffer, int yRowStride,
                                            ByteBuffer uBuffer, int uRowStride, int uPixelStride,
                                            ByteBuffer vBuffer, int vRowStride, int vPixelStride);
    public static native void nativeDestroy();
}
