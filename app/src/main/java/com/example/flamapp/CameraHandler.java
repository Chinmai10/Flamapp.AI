package com.example.flamapp;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.util.Size;

import androidx.camera.core.Camera;
import androidx.camera.core.CameraSelector;
import androidx.camera.core.ImageAnalysis;
import androidx.camera.core.Preview;
import androidx.camera.lifecycle.ProcessCameraProvider;
import androidx.core.content.ContextCompat;
import androidx.lifecycle.LifecycleOwner;

import com.google.common.util.concurrent.ListenableFuture;

import java.nio.ByteBuffer;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class CameraHandler {

    private final Activity activity;
    private final MainRenderer renderer;
    private final ExecutorService cameraExecutor;
    private ProcessCameraProvider cameraProvider;

    public CameraHandler(Activity activity, MainRenderer renderer) {
        this.activity = activity;
        this.renderer = renderer;
        this.cameraExecutor = Executors.newSingleThreadExecutor();
    }

    public void startCamera() {
        ListenableFuture<ProcessCameraProvider> cameraProviderFuture = ProcessCameraProvider.getInstance(activity);
        cameraProviderFuture.addListener(() -> {
            try {
                cameraProvider = cameraProviderFuture.get();
                bindCameraUseCases();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }, ContextCompat.getMainExecutor(activity));
    }

    @SuppressLint("UnsafeExperimentalUsageError")
    private void bindCameraUseCases() {
        CameraSelector cameraSelector = new CameraSelector.Builder()
                .requireLensFacing(CameraSelector.LENS_FACING_BACK)
                .build();

        // Using a lower resolution for better performance
        Size targetResolution = new Size(640, 480);

        ImageAnalysis imageAnalysis = new ImageAnalysis.Builder()
                .setTargetResolution(targetResolution)
                .setBackpressureStrategy(ImageAnalysis.STRATEGY_KEEP_ONLY_LATEST)
                .build();

        imageAnalysis.setAnalyzer(cameraExecutor, image -> {
            // The image format from CameraX is YUV_420_888
            ImageAnalysis.PlaneProxy[] planes = image.getImage().getPlanes();
            ByteBuffer yBuffer = planes[0].getBuffer();
            ByteBuffer uBuffer = planes[1].getBuffer();
            ByteBuffer vBuffer = planes[2].getBuffer();

            int yRowStride = planes[0].getRowStride();
            int uvPixelStride = planes[1].getPixelStride();
            int uvRowStride = planes[1].getRowStride();

            MainRenderer.nativeOnFrame(
                    image.getWidth(), image.getHeight(),
                    yBuffer, yRowStride,
                    uBuffer, uvRowStride, uvPixelStride,
                    vBuffer, uvRowStride, uvPixelStride
            );
            image.close();
        });

        try {
            cameraProvider.unbindAll();
            cameraProvider.bindToLifecycle((LifecycleOwner) activity, cameraSelector, imageAnalysis);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void shutdown() {
        cameraExecutor.shutdown();
        if (cameraProvider != null) {
            cameraProvider.unbindAll();
        }
    }
}
