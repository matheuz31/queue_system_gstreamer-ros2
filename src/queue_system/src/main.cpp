#include <gst/gst.h>
#include <iostream>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>

class SimpleImagePublisher : public rclcpp::Node {
public:
    SimpleImagePublisher()
    : Node("simple_image_publisher") {
        publisher_ = this->create_publisher<sensor_msgs::msg::Image>("camera/image", 10);
    }

    void publish_image(const uint8_t* data, size_t width, size_t height) {
        auto message = sensor_msgs::msg::Image();
        message.header.stamp = this->now();
        message.header.frame_id = "camera_frame";
        message.height = height;
        message.width = width;
        message.encoding = "rgb8";
        message.step = width * 3; 
        message.data = std::vector<uint8_t>(data, data + (width * height * 3));
        publisher_->publish(message);
    }

private:
    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr publisher_;
};

static void on_pad_added(GstElement *element, GstPad *pad, gpointer data) {
    GstPad *sinkpad;
    GstElement *converter = (GstElement *)data;
    sinkpad = gst_element_get_static_pad(converter, "sink");

    if (gst_pad_is_linked(sinkpad) == FALSE) {
        gst_pad_link(pad, sinkpad);
    }
    gst_object_unref(sinkpad);
}

static GstPadProbeReturn probe_callback(GstPad *pad, GstPadProbeInfo *info, gpointer user_data){

    return GST_PAD_PROBE_PASS;
}

int main(int argc, char *argv[]) {
    GstElement *pipeline, *source, *video_sink, *decoder, *converter;
    GMainLoop *loop;
    GstBus *bus;
    GstMessage *msg;
    GstPad *srcpad;
    GError *error = NULL;

    gst_init(&argc, &argv);

    rclcpp::init(argc, argv);

    auto node = std::make_shared<SimpleImagePublisher>();

    if(argc != 3){
        std::cerr << "you need <Video File Path> as" << argv[0] << std::endl;

    }

    source = gst_element_factory_make("filesrc", "source");
    decoder = gst_element_factory_make("decodebin", "decoder");
    converter = gst_element_factory_make("videoconvert", "converter");
    video_sink = gst_element_factory_make("autovideosink", "sink");

    pipeline = gst_pipeline_new("pipeline_ros2");

    if(!pipeline || !source || !decoder || !video_sink || !converter){
        g_printerr("Not all elements could be created");
    }
    g_object_set(G_OBJECT(source), "location", argv[1], NULL);

    gst_bin_add(GST_BIN(pipeline), source);
    gst_bin_add(GST_BIN(pipeline), decoder);
    gst_bin_add(GST_BIN(pipeline), converter);
    
    gst_bin_add(GST_BIN(pipeline), video_sink);
    gst_element_link(source, decoder);

    g_signal_connect(decoder, "pad-added", G_CALLBACK(on_pad_added), converter);

    std::cout << "teste" << std::endl;
    gst_element_link(converter, video_sink);

    srcpad = gst_element_get_static_pad(converter, "src");
    gst_pad_add_probe(srcpad, GST_PAD_PROBE_TYPE_BUFFER, probe_callback, NULL, NULL);
    gst_object_unref(srcpad);


    loop = g_main_loop_new(NULL, FALSE);

    gst_element_set_state(pipeline, GST_STATE_PLAYING);
  
    g_main_loop_run(loop);

    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(GST_OBJECT(pipeline));
    g_main_loop_unref(loop);

    rclcpp::spin(node);

    rclcpp::shutdown();

    return 0;
}