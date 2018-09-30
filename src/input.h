#pragma once

#include <stdint.h>
#include <stdio.h>
#include <complex.h>

#include "acquire.h"
#include "decode.h"
#include "defines.h"
#include "firdecim_q15.h"
#include "frame.h"
#include "output.h"
#include "sync.h"

typedef int (*input_snr_cb_t) (void *, float);

typedef struct input_t
{
    output_t *output;
    FILE *outfp;

    firdecim_q15 decim;
    cint16_t *buffer;
    double center;
    unsigned int avail, used, skip;

    fftwf_plan snr_fft;
    float complex snr_fft_in[64];
    float complex snr_fft_out[64];
    float snr_power[64];
    int snr_cnt;
    input_snr_cb_t snr_cb;
    void *snr_cb_arg;

    acquire_t acq;
    decode_t decode;
    frame_t frame;
    sync_t sync;
} input_t;

void input_init(input_t *st, output_t *output, double center, unsigned int program, FILE *outfp);
void input_cb(uint8_t *, uint32_t, void *);
void input_set_snr_callback(input_t *st, input_snr_cb_t cb, void *);
void input_set_skip(input_t *st, unsigned int skip);
void input_pdu_push(input_t *st, uint8_t *pdu, unsigned int len, unsigned int program);
void input_aas_push(input_t *st, uint8_t *psd, unsigned int len);
#ifdef USE_SOAPYSDR
void input_soapy_cb(float complex *buf, int len, input_t *st);
#endif
