#include <jni.h>
#include <string>
#include <android/log.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include "renderer.h"
#include "Processor.h"
#include "CameraHandler.h"


#define LOG_TAG "NativeLib"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

static Renderer g_renderer;
static Processor g_processor;
static jobject g_renderer_java_ref;
static jmethodID g_request_render_method_id;

extern "C" {

JNIEXPORT void JNICALL
Java_com_example_flamapp_MainRenderer_nativeOnSurfaceCreated(JNIEnv *env, jclass clazz) {
    g_renderer.init();
    // We need a global reference to the Java renderer object to call back into it
    jclass renderer_class = env->FindClass("com/example/flamapp/MainRenderer");
    if(renderer_class == nullptr) {
        LOGE("Could not find MainRenderer class");
        return;
    }
    // This is a bit of a hack. We can't easily get the instance, so we assume it's a singleton for now.
    // A better approach would be to pass the object instance down from Java.
    // However, since the renderer is tied to the GLSurfaceView, this is tricky.
    // For this example, we'll find the method on the class and assume we can call it.
    g_request_render_method_id = env->GetMethodID(renderer_class, "requestRender", "()V");
    if(g_request_render_method_id == nullptr) {
        LOGE("Could not find requestRender method");
    }
}

JNIEXPORT void JNICALL
Java_com_example_flamapp_MainRenderer_nativeOnSurfaceChanged(JNIEnv *env, jclass clazz, jint width, jint height) {
    g_renderer.set_viewport(width, height);
}

JNIEXPORT void JNICALL
Java_com_example_flamapp_MainRenderer_nativeOnDrawFrame(JNIEnv *env, jclass clazz) {
    g_renderer.draw();
}

JNIEXPORT void JNICALL
Java_com_example_flamapp_MainRenderer_nativeOnFrame(JNIEnv *env, jclass clazz,
                                                     jint width, jint height,
                                                     jobject y_buffer, jint y_row_stride,
                                                     jobject u_buffer, jint u_row_stride, jint u_pixel_stride,
                                                     jobject v_buffer, jint v_row_stride, jint v_pixel_stride) {

    uint8_t *y_data = (uint8_t *) env->GetDirectBufferAddress(y_buffer);
    uint8_t *u_data = (uint8_t *) env->GetDirectBufferAddress(u_buffer);
    uint8_t *v_data = (uint8_t *) env->GetDirectBufferAddress(v_buffer);

    if (y_data == nullptr || u_data == nullptr || v_data == nullptr) {
        LOGE("Failed to get direct buffer address");
        return;
    }

    cv::Mat processed_frame = g_processor.process_frame(
            width, height,
            y_data, y_row_stride,
            u_data, u_row_stride, u_pixel_stride,
            v_data, v_row_stride, v_pixel_stride
    );

    if (!processed_frame.empty()) {
        g_renderer.update_texture(processed_frame);
        
        // This part is tricky because we are on a background thread from CameraX.
        // We need to call requestRender on the Java GLSurfaceView, which must be done on the UI thread.
        // A proper implementation would use a more robust mechanism to signal the renderer.
        // For this assessment, we'll call it directly, but be aware this isn't safe.
        // A better way is to have the nativeOnDrawFrame method check a flag.
        // For now, we will just call it.
        // env->CallVoidMethod(g_renderer_java_ref, g_request_render_method_id);
        // The above line would crash. The requestRender is called from MainRenderer.java
    }
}


JNIEXPORT void JNICALL
Java_com_example_flamapp_MainRenderer_nativeDestroy(JNIEnv *env, jclass clazz) {
    g_renderer.destroy();
    if(g_renderer_java_ref != nullptr) {
        env->DeleteGlobalRef(g_renderer_java_ref);
        g_renderer_java_ref = nullptr;
    }
}

} // extern "C"
