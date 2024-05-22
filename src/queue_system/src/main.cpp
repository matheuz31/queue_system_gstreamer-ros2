#include <gst/gst.h>
#include <iostream>

static void on_pad_added(GstElement *element, GstPad *pad, gpointer data) {
    GstPad *sinkpad;
    GstElement *converter = (GstElement *)data;
    sinkpad = gst_element_get_static_pad(converter, "sink");

    if (gst_pad_is_linked(sinkpad) == FALSE) {
        gst_pad_link(pad, sinkpad);
    }
    gst_object_unref(sinkpad);
}

int main(int argc, char *argv[]) {
    GstElement *pipeline, *source, *video_sink, *decoder, *converter;
    GMainLoop *loop;
    GstBus *bus;
    GstMessage *msg;
    GError *error = NULL;

    gst_init(&argc, &argv);

    if(argc =! 2){
        std::cerr << "you need <Video File Path> as" << argv[0] << std::endl;

    }

    source = gst_element_factory_make("filesrc", "source");
    decoder = gst_element_factory_make("decodebin", "decoder");
    converter = gst_element_factory_make("videoconvert", "converter");
    video_sink = gst_element_factory_make("autovideosink", "sink");

    pipeline = gst_pipeline_new("pipeline_ros2");

    if(!pipeline || !source || !decoder || !video_sink || converter){
        g_printerr("Not all elements could be created");
    }

    g_object_set(G_OBJECT(source), "location", argv[1], NULL);

    gst_bin_add_many(GST_BIN(pipeline), source, decoder, converter, video_sink);

    gst_element_link(source, decoder);
    g_signal_connect(decoder, "pad-added", G_CALLBACK(on_pad_added), converter);
    gst_element_link(converter, video_sink);

    loop = g_main_loop_new(NULL, FALSE);

    gst_element_set_state(pipeline, GST_STATE_PLAYING);
  
    g_main_loop_run(loop);

    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(GST_OBJECT(pipeline));
    g_main_loop_unref(loop);

    return 0;
}