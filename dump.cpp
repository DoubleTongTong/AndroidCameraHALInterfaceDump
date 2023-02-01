#include "include/hardware/camera3.h"
#include <stdio.h>
#include <stdlib.h>

#define LOGSIZE 2048
#define MALLOCLOGSIZE 10240
#define PATHSIZE 512

void WriteToFile(const char* filePath, const char* str, int len)
{
    FILE* f = fopen(filePath, "w");
    if (f != NULL)
    {
        fwrite(str, len, 1, f);
        fclose(f);
    }
}

void PrintInitialize(const struct camera3_device* device,
    const camera3_callback_ops_t* callback_ops,
    int return_value)
{
    static int count = 0;
    char log[LOGSIZE], filePath[PATHSIZE];

    int len = snprintf(log, sizeof(log), "camera3_device=%p\n"
                                         "callback_ops=%p\n"
                                         "return_value=%d\n",
        device, callback_ops, return_value);

    snprintf(filePath, sizeof(filePath), "/data/vendor/camera/tlog/initialize/initialize_%d.txt", count++);
    WriteToFile(filePath, log, len);
}

void PrintConfigureStreams(const struct camera3_device* device,
    camera3_stream_configuration_t* stream_list,
    int return_value)
{
    static int count = 0;
    char* log = (char*)malloc(MALLOCLOGSIZE);
    char filePath[PATHSIZE];

    int len = snprintf(log, MALLOCLOGSIZE, "camera3_device=%p\n"
                                           "stream_list=%p\n"
                                           "stream_list->num_streams=%u\n"
                                           "stream_list->operation_mode=0x%x\n"
                                           "stream_list->streams=%p\n\n",
        device, stream_list, stream_list->num_streams, stream_list->operation_mode, stream_list->streams);
    for (int i = 0; i < stream_list->num_streams; i++)
    {
        camera3_stream_t* stream = stream_list->streams[i];
        len += snprintf(log + len, MALLOCLOGSIZE - len, "stream[%d]=%p\n"
                                                        "stream->stream_type=%d\n"
                                                        "stream->width=%u\n"
                                                        "stream->height=%u\n"
                                                        "stream->format=%d\n"
                                                        "stream->usage=%u\n"
                                                        "stream->max_buffers=%u\n"
                                                        "stream->priv=%p\n"
                                                        "stream->data_space=%u\n"
                                                        "stream->rotation=%d\n\n",
            i, stream,
            stream->stream_type, stream->width, stream->height, stream->format, stream->usage, stream->max_buffers, stream->priv, stream->data_space, stream->rotation);
    }

    len += snprintf(log + len, MALLOCLOGSIZE - len, "return_value=%d\n", return_value);

    snprintf(filePath, sizeof(filePath), "/data/vendor/camera/tlog/configure_streams/configure_streams_%x_%d.txt", stream_list->operation_mode, count++);
    WriteToFile(filePath, log, len);

    free(log);
}

void PrintProcessCaptureRequest(const struct camera3_device* device,
    camera3_capture_request_t* request,
    int return_value)
{
    static int count = 0;
    char log[LOGSIZE], filePath[PATHSIZE];

    int len = snprintf(log, sizeof(log), "camera3_device=%p\n"
                                         "request=%p\n"
                                         "request->frame_number=%u\n"
                                         "request->settings=%p\n"
                                         "request->input_buffer=%p\n"
                                         "request->num_output_buffers=%u\n"
                                         "request->output_buffers=%p\n\n",
        device, request, request->frame_number, request->settings, request->input_buffer, request->num_output_buffers, request->output_buffers);

    for (int i = 0; i < request->num_output_buffers; i++)
    {
        const camera3_stream_buffer_t* buffer = request->output_buffers + i;

        len += snprintf(log + len, sizeof(log) - len, "output_buffers[%d]=%p\n"
                                          "buffer->stream=%p\n"
                                          "buffer->buffer=%p\n"
                                          "buffer->status=%d\n"
                                          "buffer->acquire_fence=%d\n"
                                          "buffer->release_fence=%d\n\n",
            i, buffer,
            buffer->stream, buffer->buffer, buffer->status, buffer->acquire_fence, buffer->release_fence);
    }

    len += snprintf(log + len, sizeof(log) - len, "return_value=%d\n", return_value);

    snprintf(filePath, sizeof(filePath), "/data/vendor/camera/tlog/process_capture_request/process_capture_request_num%u_%d.txt", request->frame_number, count++);
    WriteToFile(filePath, log, len);
}

void PrintFlush(const struct camera3_device* device, int return_value)
{
    static int count = 0;
    char log[LOGSIZE], filePath[PATHSIZE];

    int len = snprintf(log, sizeof(log), "camera3_device=%p\n"
                                     "return_value=%d\n",
        device, return_value);

    snprintf(filePath, sizeof(filePath), "/data/vendor/camera/tlog/flush/flush_%d.txt", count++);
    WriteToFile(filePath, log, len);
}

void PrintProcessCaptureResult(const struct camera3_callback_ops* ops,
    const camera3_capture_result_t* result)
{
    static int count = 0;
    char log[LOGSIZE], filePath[PATHSIZE];

    int len = snprintf(log, sizeof(log), "camera3_callback_ops=%p\n"
                                         "result=%p\n"
                                         "result->frame_number=%u\n"
                                         "result->result=%p\n"
                                         "result->num_output_buffers=%u\n"
                                         "result->output_buffers=%p\n"
                                         "result->input_buffer=%p\n"
                                         "result->partial_result=%u\n\n",
        ops, result,
        result->frame_number, result->result, result->num_output_buffers, result->output_buffers, result->input_buffer, result->partial_result);

    for (int i = 0; i < result->num_output_buffers; i++)
    {
        const camera3_stream_buffer_t* buffer = result->output_buffers + i;

        len += snprintf(log + len, sizeof(log) - len, "output_buffers[%d]=%p\n"
            "buffer->stream=%p\n"
            "buffer->buffer=%p\n"
            "buffer->status=%d\n"
            "buffer->acquire_fence=%d\n"
            "buffer->release_fence=%d\n\n",
            i, buffer,
            buffer->stream, buffer->buffer, buffer->status, buffer->acquire_fence, buffer->release_fence);
    }

    snprintf(filePath, sizeof(filePath), "/data/vendor/camera/tlog/process_capture_result/process_capture_result_num%u_%d.txt", result->frame_number, count++);
    WriteToFile(filePath, log, len);
}

void PrintNotify(const struct camera3_callback_ops* ops,
    const camera3_notify_msg_t* msg)
{
    static int count = 0;
    char log[LOGSIZE], filePath[PATHSIZE];

    int len = snprintf(log, sizeof(log), "camera3_callback_ops=%p\n"
                                         "msg=%p\n"
                                         "msg->type=%d\n\n",
        ops, msg,
        msg->type);

    if (msg->type == CAMERA3_MSG_SHUTTER)
    {
        len += snprintf(log + len, sizeof(log) - len, "msg->message.shutter.frame_number=%u\n"
                                                      "msg->message.shutter.timestamp=%llu\n\n",
            msg->message.shutter.frame_number, msg->message.shutter.timestamp);
    }
    else if (msg->type == CAMERA3_MSG_ERROR)
    {
        len += snprintf(log + len, sizeof(log) - len, "msg->message.error.frame_number=%u\n"
                                                      "msg->message.error.error_stream=%p\n"
                                                      "msg->message.error.error_code=%d\n\n",
            msg->message.error.frame_number, msg->message.error.error_stream, msg->message.error.error_code);
    }

    snprintf(filePath, sizeof(filePath), "/data/vendor/camera/tlog/notify/notify_num%u_type%d_%d.txt", msg->message.shutter.frame_number, msg->type, count++);
    WriteToFile(filePath, log, len);
}