#ifndef __FRAMEBUFFER_H_INCLUDE__
#define __FRAMEBUFFER_H_INCLUDE__

#define FLAME_BUFFER ((volatile uint32*)0x40040000)

typedef struct {
  uint32 width;
  uint32 height;
  uint32 vWidth;
  uint32 vHeight;
  uint32 pitch;
  uint32 bitDepth;
  uint32 x;
  uint32 y;
  void* pointer;
  uint32 size;
} FrameBufferDescription;

FrameBufferDescription* init_frame_buffer(uint32 width,
                                          uint32 height,
                                          uint32 bitDepth);

#endif
