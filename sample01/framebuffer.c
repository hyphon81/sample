#include "defines.h"
#include "mailbox.h"
#include "framebuffer.h"
#include "lib.h"

FrameBufferDescription FrameBufferInfo =
  { 1024, 768, 1024, 768, 0, 16, 0, 0, 0, 0 };


FrameBufferDescription* init_frame_buffer(uint32 width,
                                              uint32 height,
                                              uint32 bitDepth)
{
  static FrameBufferDescription *fb = (FrameBufferDescription*)FLAME_BUFFER;
  uint32 r;
  
  if(width > 4096 || height > 4096 || bitDepth > 32){
    return 0;
  }

  *fb=FrameBufferInfo;

  fb->width = width;
  fb->height = height;
  fb->vWidth = width;
  fb->vHeight = height;
  fb->bitDepth = bitDepth;

  mailbox_write((uint32)fb, 1);

  r = mailbox_read(1);
  if(r == E_INVALID_VAL){
    puts("Error!! mailbox_read() E_INVALID_VAL!\r\n");
    return NULL;
  }
  if(r == E_COUNTUP_MAX){
    puts("Error!! mailbox_read() E_COUNTUP_MAX!\r\n");
    return NULL;
  }

  return fb;
}
