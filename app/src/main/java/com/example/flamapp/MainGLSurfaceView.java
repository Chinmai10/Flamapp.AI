package com.example.flamapp;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;

public class MainGLSurfaceView extends GLSurfaceView {

    private final MainRenderer renderer;

    public MainGLSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
        setEGLContextClientVersion(2);
        renderer = new MainRenderer(this);
        setRenderer(renderer);
        setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
    }

    public MainRenderer getRenderer() {
        return renderer;
    }
}
