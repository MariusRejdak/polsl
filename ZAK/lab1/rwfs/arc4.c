  /*        This code illustrates a sample implementation
   *                 of the Arcfour algorithm
   *         Copyright (c) April 29, 1997 Kalle Kaukonen.
   *                    All Rights Reserved.
   *
   * Redistribution and use in source and binary forms, with or
   * without modification, are permitted provided that this copyright
   * notice and disclaimer are retained.
   *
   * THIS SOFTWARE IS PROVIDED BY KALLE KAUKONEN AND CONTRIBUTORS ``AS
   * IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
   * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL KALLE
   * KAUKONEN OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
   * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
   * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
   * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
   */
  
   #include "arc4.h"
   
  /*int main(int argc, char **argv)
  {
	unsigned char dest[500];
	unsigned char mykey[] = {0x29, 0x04, 0x19, 0x72, 0xfb, 0x42,
							 0xba, 0x5f, 0xc7, 0x12, 0x77, 0x12,
							 0xf1, 0x38, 0x29, 0xc9};
	unsigned char src[]   = "Know thyself";
  
	ArcfourContext mycontext;
	   
	// Initialize the algoritm
	arcfour_init(&mycontext, mykey, 16);
  
	// Encrypt 13 bytes of the src string
  
	arcfour_encrypt(&mycontext, dest, src, 13);
	   
	// Now "dest" contains the encrypted string. Do whatever
	// you please with it...
		 
	return 0;
  }*/
			
  
  void arcfour_init(ArcfourContext *ctx, const unsigned char *key, 
					unsigned int key_len)
  {
	unsigned int t, u;
	unsigned int keyindex;
	unsigned int stateindex;
	unsigned char *state;
	unsigned int counter;
	   
	state = ctx->state;
	ctx->x = 0;
	ctx->y = 0;
	for (counter = 0; counter < 256; counter++)
		 state[counter] = counter;
	keyindex = 0;
	stateindex = 0;
	for (counter = 0; counter < 256; counter++)
	{
	  t = state[counter];
	  stateindex = (stateindex + key[keyindex] + t) & 0xff;
	  u = state[stateindex];
	  state[stateindex] = t;
	  state[counter] = u;
	  if (++keyindex >= key_len)
		keyindex = 0;
	}
  }
  
   
  unsigned int arcfour_byte(ArcfourContext *ctx)
  {
	unsigned int x;
	unsigned int y;
	unsigned int sx, sy;
	unsigned char *state;
	   
	state = ctx->state;
	x = (ctx->x + 1) & 0xff;
	sx = state[x];
	y = (sx + ctx->y) & 0xff;
	sy = state[y];
	ctx->x = x;
	ctx->y = y;
	state[y] = sx;
	state[x] = sy;
  
	return state[(sx + sy) & 0xff];
  }
  
	   
  void arcfour_encrypt(ArcfourContext *ctx, unsigned char *dest, 
					   const unsigned char *src, unsigned int len)
  {
	unsigned int i;
	for (i = 0; i < len; i++)
	  dest[i] = src[i] ^ arcfour_byte(ctx);
  }


