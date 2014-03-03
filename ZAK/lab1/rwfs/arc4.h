#ifndef ARC4H_INCLUDED
#define ARC4H_INCLUDED

  typedef struct
  {
    unsigned int x;
    unsigned int y;
    unsigned char state[256];
  } ArcfourContext;
   
  void arcfour_init(ArcfourContext *ctx, const unsigned char *key, unsigned int key_len);
  unsigned int arcfour_byte(ArcfourContext *ctx);
  void arcfour_encrypt(ArcfourContext *ctx, unsigned char *dest, const unsigned char *src, unsigned int len);

#endif